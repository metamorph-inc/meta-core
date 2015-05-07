#include <Python.h>

static PyObject *crc(PyObject *self, PyObject *args)
{
	const void *data;
	const unsigned short cnt;
	
	if(!


unsigned short crc_update(unsigned short crc, unsigned char data)
{
	data ^= (crc & 0xff);
	data ^= data << 4;
	
	return ((((unsigned short) data << 8) | ((crc>>8 & 0xff)) ^ (unsigned char) (data >> 4) ^ ((unsigned short)data << 3));
}

unsigned short crc16(void* data, unsigned short cnt)
{
	unsigned short crc = 0xff;
	unsigned char *ptr = (unsigned char*)data;
	int i;
	for(i=0;i<cnt;i++)
	{
		crc=crc_update(crc,*ptr);
		ptr++;
	}
	return crc;
}


