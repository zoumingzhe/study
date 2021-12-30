# filename='step'
filename=$1
gcc ${filename}.c -o ${filename} -lpthread -std=c99
./${filename}
