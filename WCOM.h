#ifndef WCOM_H
#define WCOM_H
#include <windows.h>
#include "Ch9329.h"

class WCOM
{
	public:
		HANDLE hComm;
		WCOM();
		void open(char* name);
		void setOptions(int baudRate);
		int write(BUFFER buf);
		int read(BUFFER *buf);
		void purge();
		void close();
		~WCOM();
	protected:
};

#endif
