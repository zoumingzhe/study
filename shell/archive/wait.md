wait
====

# 样例
```shell
function log
{
  echo -e "`date "+%Y-%m-%d %H:%M:%S"` $1"
}

function test_wait
{
    sleep $1 &
    sleep $2 &
    log "function waiting"
    wait
    log "function wait ok"
}

test_wait 1 3 &
test_wait 3 5 &
log "main waiting"
wait
log "main wait ok"
```

# 参考
 * []()
