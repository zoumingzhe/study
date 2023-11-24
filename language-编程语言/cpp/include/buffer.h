#ifndef __BUFFER_H__
#define __BUFFER_H__

extern void write_buffer(const char* buff, const int size);

#endif

#ifndef __BUFFER_SLICE_H__
#define __BUFFER_SLICE_H__

extern void write_buffer_slice(const char* oldbuf, const char* newbuf, const int size);

#endif
