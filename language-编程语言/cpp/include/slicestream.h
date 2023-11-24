#ifndef __SLICESTREAM_H__
#define __SLICESTREAM_H__

#include "stdtypedef.h"
#include "stdfile.h"

typedef struct sinfo {
	u32     compress;
	u32     encrypt;
	u64     size;
}sinfo;

typedef struct shead {
	union
	{
		u8  mode;
		struct
		{
			u8  compress : 1; // 是否压缩
			u8  encrypt  : 1; // 是否加密
			u8  reserve3 : 1;
			u8  reserve4 : 1;
			u8  reserve5 : 1;
			u8  crc : 1;
			u8  reserve7 : 1;
			u8  reserve8 : 1;
		};
	};
	u32     dlen;   // 数据长度
	u32     slen;   // 切片长度
	u32     plen;   // 前片长度
}shead;

typedef struct sbuff {
	i32     size;
	i32     blen;
	i32     boff;
	i32     fpos;
	char*   buff;
}sbuff;

class slicestream {
private:
	sfile   file;
	sinfo   info;
	sbuff   buff[8];
	i32     buff_num;
	i32     buff_use;
public:
	slicestream(u32 compress, u32 encrypt);
	~slicestream();
	i32     seek();
	i32     read();
	i32     write();
	i32     flush();
};

#endif
