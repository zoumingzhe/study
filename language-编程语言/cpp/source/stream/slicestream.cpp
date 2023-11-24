#include "slicestream.h"

slicestream::slicestream(u32 compress, u32 encrypt)
{
	file.open(".\\test", "r");
	info.compress = compress;
	info.encrypt = encrypt;
}

slicestream::~slicestream()
{
}

i32 slicestream::seek()
{
	return -1;
}

i32 slicestream::read()
{
	return -1;
}

i32 slicestream::write()
{
	return -1;
}

i32 slicestream::flush()
{
	return -1;
}

