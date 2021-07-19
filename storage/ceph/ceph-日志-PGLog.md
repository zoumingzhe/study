Ceph 日志 - PGLog
=================

## pg_log_entry_t
./src/osd/osd_types.h:pg_log_entry_t
```C++
/**
 * pg_log_entry_t - single entry/event in pg log
 *
 */
struct pg_log_entry_t {
  enum {
    MODIFY = 1,   // some unspecified modification (but not *all* modifications)
    CLONE = 2,    // cloned object from head
    DELETE = 3,   // deleted object
    //BACKLOG = 4,  // event invented by generate_backlog [obsolete]
    LOST_REVERT = 5, // lost new version, revert to an older version.
    LOST_DELETE = 6, // lost new version, revert to no object (deleted).
    LOST_MARK = 7,   // lost new version, now EIO
    PROMOTE = 8,     // promoted object from another tier
    CLEAN = 9,       // mark an object clean
    ERROR = 10,      // write that returned an error
  };
  static const char *get_op_name(int op) {
    switch (op) {
    case MODIFY:
      return "modify";
    case PROMOTE:
      return "promote";
    case CLONE:
      return "clone";
    case DELETE:
      return "delete";
    case LOST_REVERT:
      return "l_revert";
    case LOST_DELETE:
      return "l_delete";
    case LOST_MARK:
      return "l_mark";
    case CLEAN:
      return "clean";
    case ERROR:
      return "error";
    default:
      return "unknown";
    }
  }
  const char *get_op_name() const {
    return get_op_name(op);
  }

  // describes state for a locally-rollbackable entry
  ObjectModDesc mod_desc;
  ceph::buffer::list snaps;   // only for clone entries
  hobject_t  soid;
  osd_reqid_t reqid;  // caller+tid to uniquely identify request
  mempool::osd_pglog::vector<std::pair<osd_reqid_t, version_t> > extra_reqids;

  /// map extra_reqids by index to error return code (if any)
  mempool::osd_pglog::map<uint32_t, int> extra_reqid_return_codes;

  eversion_t version, prior_version, reverting_to;
  version_t user_version; // the user version for this entry
  utime_t     mtime;  // this is the _user_ mtime, mind you
  int32_t return_code; // only stored for ERRORs for dup detection

  std::vector<pg_log_op_return_item_t> op_returns;

  __s32      op;
  bool invalid_hash; // only when decoding sobject_t based entries
  bool invalid_pool; // only when decoding pool-less hobject based entries
  ObjectCleanRegions clean_regions;

  pg_log_entry_t()
   : user_version(0), return_code(0), op(0),
     invalid_hash(false), invalid_pool(false) {
    snaps.reassign_to_mempool(mempool::mempool_osd_pglog);
  }
  pg_log_entry_t(int _op, const hobject_t& _soid,
                const eversion_t& v, const eversion_t& pv,
                version_t uv,
                const osd_reqid_t& rid, const utime_t& mt,
                int return_code)
   : soid(_soid), reqid(rid), version(v), prior_version(pv), user_version(uv),
     mtime(mt), return_code(return_code), op(_op),
     invalid_hash(false), invalid_pool(false) {
    snaps.reassign_to_mempool(mempool::mempool_osd_pglog);
  }
      
  bool is_clone() const { return op == CLONE; }
  bool is_modify() const { return op == MODIFY; }
  bool is_promote() const { return op == PROMOTE; }
  bool is_clean() const { return op == CLEAN; }
  bool is_lost_revert() const { return op == LOST_REVERT; }
  bool is_lost_delete() const { return op == LOST_DELETE; }
  bool is_lost_mark() const { return op == LOST_MARK; }
  bool is_error() const { return op == ERROR; }

  bool is_update() const {
    return
      is_clone() || is_modify() || is_promote() || is_clean() ||
      is_lost_revert() || is_lost_mark();
  }
  bool is_delete() const {
    return op == DELETE || op == LOST_DELETE;
  }

  bool can_rollback() const {
    return mod_desc.can_rollback();
  }

  void mark_unrollbackable() {
    mod_desc.mark_unrollbackable();
  }

  bool requires_kraken() const {
    return mod_desc.requires_kraken();
  }

  // Errors are only used for dup detection, whereas
  // the index by objects is used by recovery, copy_get,
  // and other facilities that don't expect or need to
  // be aware of error entries.
  bool object_is_indexed() const {
    return !is_error();
  }

  bool reqid_is_indexed() const {
    return reqid != osd_reqid_t() &&
      (op == MODIFY || op == DELETE || op == ERROR);
  }

  void set_op_returns(const std::vector<OSDOp>& ops) {
    op_returns.resize(ops.size());
    for (unsigned i = 0; i < ops.size(); ++i) {
      op_returns[i].rval = ops[i].rval;
      op_returns[i].bl = ops[i].outdata;
    }
  }

  std::string get_key_name() const;
  void encode_with_checksum(ceph::buffer::list& bl) const;
  void decode_with_checksum(ceph::buffer::list::const_iterator& p);

  void encode(ceph::buffer::list &bl) const;
  void decode(ceph::buffer::list::const_iterator &bl);
  void dump(ceph::Formatter *f) const;
  static void generate_test_instances(std::list<pg_log_entry_t*>& o);
};
```

## ObjectModDesc
./src/include/encoding.h
```C++
/**
 * start encoding block
 *
 * @param v current (code) version of the encoding
 * @param compat oldest code version that can decode it
 * @param bl bufferlist to encode to
 *
 */
#define ENCODE_START(v, compat, bl)			     \
  __u8 struct_v = v;                                         \
  __u8 struct_compat = compat;		                     \
  ceph_le32 struct_len;				             \
  auto filler = (bl).append_hole(sizeof(struct_v) +	     \
    sizeof(struct_compat) + sizeof(struct_len));	     \
  const auto starting_bl_len = (bl).length();		     \
  using ::ceph::encode;					     \
  do {

/**
 * finish encoding block
 *
 * @param bl bufferlist we were encoding to
 * @param new_struct_compat struct-compat value to use
 */
#define ENCODE_FINISH_NEW_COMPAT(bl, new_struct_compat)      \
  } while (false);                                           \
  if (new_struct_compat) {                                   \
    struct_compat = new_struct_compat;                       \
  }                                                          \
  struct_len = (bl).length() - starting_bl_len;              \
  filler.copy_in(sizeof(struct_v), (char *)&struct_v);       \
  filler.copy_in(sizeof(struct_compat),			     \
    (char *)&struct_compat);				     \
  filler.copy_in(sizeof(struct_len), (char *)&struct_len);

#define ENCODE_FINISH(bl) ENCODE_FINISH_NEW_COMPAT(bl, 0)
```

./src/osd/osd_types.h:ObjectModDesc
```C++
class ObjectModDesc {
  bool can_local_rollback;
  bool rollback_info_completed;

  // version required to decode, reflected in encode/decode version
  __u8 max_required_version = 1;
public:
  class Visitor {
  public:
    virtual void append(uint64_t old_offset) {}
    virtual void setattrs(std::map<std::string, std::optional<ceph::buffer::list>> &attrs) {}
    virtual void rmobject(version_t old_version) {}
    /**
     * Used to support the unfound_lost_delete log event: if the stashed
     * version exists, we unstash it, otherwise, we do nothing.  This way
     * each replica rolls back to whatever state it had prior to the attempt
     * at mark unfound lost delete
     */
    virtual void try_rmobject(version_t old_version) {
      rmobject(old_version);
    }
    virtual void create() {}
    virtual void update_snaps(const std::set<snapid_t> &old_snaps) {}
    virtual void rollback_extents(
      version_t gen,
      const std::vector<std::pair<uint64_t, uint64_t> > &extents) {}
    virtual ~Visitor() {}
  };
  void visit(Visitor *visitor) const;
  mutable ceph::buffer::list bl;
  enum ModID {
    APPEND = 1,
    SETATTRS = 2,
    DELETE = 3,
    CREATE = 4,
    UPDATE_SNAPS = 5,
    TRY_DELETE = 6,
    ROLLBACK_EXTENTS = 7
  };
  ObjectModDesc() : can_local_rollback(true), rollback_info_completed(false) {
    bl.reassign_to_mempool(mempool::mempool_osd_pglog);
  }
  void claim(ObjectModDesc &other) {
    bl = std::move(other.bl);
    can_local_rollback = other.can_local_rollback;
    rollback_info_completed = other.rollback_info_completed;
  }
  void claim_append(ObjectModDesc &other) {
    if (!can_local_rollback || rollback_info_completed)
      return;
    if (!other.can_local_rollback) {
      mark_unrollbackable();
      return;
    }
    bl.claim_append(other.bl);
    rollback_info_completed = other.rollback_info_completed;
  }
  void swap(ObjectModDesc &other) {
    bl.swap(other.bl);

    using std::swap;
    swap(other.can_local_rollback, can_local_rollback);
    swap(other.rollback_info_completed, rollback_info_completed);
    swap(other.max_required_version, max_required_version);
  }
  void append_id(ModID id) {
    using ceph::encode;
    uint8_t _id(id);
    encode(_id, bl);
  }
  void append(uint64_t old_size) {
    if (!can_local_rollback || rollback_info_completed)
      return;
    ENCODE_START(1, 1, bl);
    append_id(APPEND);
    encode(old_size, bl);
    ENCODE_FINISH(bl);
  }
  void setattrs(std::map<std::string, std::optional<ceph::buffer::list>> &old_attrs) {
    if (!can_local_rollback || rollback_info_completed)
      return;
    ENCODE_START(1, 1, bl);
    append_id(SETATTRS);
    encode(old_attrs, bl);
    ENCODE_FINISH(bl);
  }
  bool rmobject(version_t deletion_version) {
    if (!can_local_rollback || rollback_info_completed)
      return false;
    ENCODE_START(1, 1, bl);
    append_id(DELETE);
    encode(deletion_version, bl);
    ENCODE_FINISH(bl);
    rollback_info_completed = true;
    return true;
  }
  bool try_rmobject(version_t deletion_version) {
    if (!can_local_rollback || rollback_info_completed)
      return false;
    ENCODE_START(1, 1, bl);
    append_id(TRY_DELETE);
    encode(deletion_version, bl);
    ENCODE_FINISH(bl);
    rollback_info_completed = true;
    return true;
  }
  void create() {
    if (!can_local_rollback || rollback_info_completed)
      return;
    rollback_info_completed = true;
    ENCODE_START(1, 1, bl);
    append_id(CREATE);
    ENCODE_FINISH(bl);
  }
  void update_snaps(const std::set<snapid_t> &old_snaps) {
    if (!can_local_rollback || rollback_info_completed)
      return;
    ENCODE_START(1, 1, bl);
    append_id(UPDATE_SNAPS);
    encode(old_snaps, bl);
    ENCODE_FINISH(bl);
  }
  void rollback_extents(
    version_t gen, const std::vector<std::pair<uint64_t, uint64_t> > &extents) {
    ceph_assert(can_local_rollback);
    ceph_assert(!rollback_info_completed);
    if (max_required_version < 2)
      max_required_version = 2;
    ENCODE_START(2, 2, bl);
    append_id(ROLLBACK_EXTENTS);
    encode(gen, bl);
    encode(extents, bl);
    ENCODE_FINISH(bl);
  }

  // cannot be rolled back
  void mark_unrollbackable() {
    can_local_rollback = false;
    bl.clear();
  }
  bool can_rollback() const {
    return can_local_rollback;
  }
  bool empty() const {
    return can_local_rollback && (bl.length() == 0);
  }

  bool requires_kraken() const {
    return max_required_version >= 2;
  }

  /**
   * Create fresh copy of bl bytes to avoid keeping large buffers around
   * in the case that bl contains ptrs which point into a much larger
   * message buffer
   */
  void trim_bl() const {
    if (bl.length() > 0)
      bl.rebuild();
  }
  void encode(ceph::buffer::list &bl) const;
  void decode(ceph::buffer::list::const_iterator &bl);
  void dump(ceph::Formatter *f) const;
  static void generate_test_instances(std::list<ObjectModDesc*>& o);
};
```

# 参考
 * [LOG BASED PG](https://docs.ceph.com/en/latest/dev/osd_internals/log_based_pg/)