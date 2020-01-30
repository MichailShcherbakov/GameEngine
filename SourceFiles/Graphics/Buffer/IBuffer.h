#ifndef _I_BUFFER_H_
#define _I_BUFFER_H_

struct IBuffer
{
	virtual ~IBuffer() {}

	virtual bool Allocate(const size_t& size, void* data) = 0;
	virtual bool Free() = 0;
};

#endif