#include "WCOM.h"

WCOM::WCOM()
{
}

void WCOM::open(char* name)
{
	hComm = CreateFile( TEXT(name), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	this->setOptions(9600);
}

void WCOM::setOptions(int baudRate)
{
	DCB dcb;


	//GetComm(hComm, &dcb);

	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;

	SetCommState(hComm, &dcb);	
}

int WCOM::write(BUFFER buf)
{
	DWORD written_cnt; // 实际写入串口的字节数

	WriteFile( hComm, (void *)buf.data, buf.len, &written_cnt, NULL );
}

int WCOM::read(BUFFER *buf)
{
	DWORD toread_cnt = 70; // 要从串口读入的字节数
	DWORD read_cnt;        // 实际从串口读入的字节数

	ReadFile( hComm, (void *)buf->data, toread_cnt, &read_cnt, NULL );
	cout<<"len: "<<toread_cnt<<endl;
	buf->len = toread_cnt;
}

void WCOM::purge()
{
	PurgeComm( hComm, PURGE_RXCLEAR );
}
void WCOM::close(){
	CloseHandle(hComm);	
}

WCOM::~WCOM()
{
	this->close();
}
