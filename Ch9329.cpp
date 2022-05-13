#include "Ch9329.h"

Ch9329::Ch9329()
{
	for(int i=0;i<8;i++) 
	{
		ctrkeys[i].press == 0;
		ctrkeys[i].code == 0;
	}
	for(int i=0;i<6;i++) 
	{
		keys[i].press == 0;
		keys[i].code == 0;
	}
	for(int i=0;i<3;i++) 
	{
		mousekeys[i].press == 0;
		mousekeys[i].code == 0;
	}
	
	
	ctrbyte = 0;
	ctrkeys[0].code = 0xE0;
	ctrkeys[1].code = 0xE1;
	ctrkeys[2].code = 0xE2;
	ctrkeys[3].code = 0xE3;
	
	ctrkeys[4].code = 0xE4;
	ctrkeys[5].code = 0xE5;
	ctrkeys[6].code = 0xE6;
	ctrkeys[7].code = 0xE7;
	
	mousekeys[0].code = 1;
	mousekeys[1].code = 2;
	mousekeys[2].code = 3;
	mousebyte = 0;
	mousexbyte = 0;
	mouseybyte = 0;
	mousewbyte = 0;
	
	cmd = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x87, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0F};

}

BUFFER Ch9329::keydown(char code)
{
	
	for(int i=0;i<8;i++) 
	{
		if(ctrkeys[i].code == code) 
		{
			ctrkeys[i].press = 1; 
			return getKeyData();
		} 
	}
	
	for(int i=0;i<6;i++) 
	{
		if(keys[i].press == 1 && keys[i].code == code) 
		{
			return getKeyData();
		} 
	}
	
	for(int i=0;i<6;i++) 
	{
		if(keys[i].press == 0) 
		{
			keys[i].press = 1; 
			keys[i].code = code;
			return getKeyData();
		} 
	}
	return getKeyData();
}
BUFFER Ch9329::keyup(char code)
{

	for(int i=0;i<8;i++) 
	{
		if(ctrkeys[i].code == code) 
		{
			ctrkeys[i].press = 0; 
			return getKeyData();
		} 
	}
	for(int i=0;i<6;i++) 
	{
		if(keys[i].code == code) 
		{
			keys[i].press = 0; 
			keys[i].code = 0;
			return getKeyData();
		} 
	}
	return getKeyData();
}


BUFFER Ch9329::mousedown(char code)
{
	if(code>=0 && code<3)
		mousekeys[code].press = 1;

	return getMouseData();
}

BUFFER Ch9329::mouseup(char code)
{
	if(code>=0 && code<3)
		mousekeys[code].press = 0;

	return getMouseData();
}

BUFFER Ch9329::mousemove(char x, char y)
{
	if(x > -128 && x< 128)
	{
		if(x<0)
		{
			mousexbyte = 0xFF + x + 1;
		}
		else
		{
			mousexbyte = x;	
		} 
	} 
	
	if(y > -128 && y< 128)
	{
		if(y<0)
		{
			mouseybyte = 0xFF + y + 1;
		}
		else
		{
			mouseybyte = y;	
		} 
	} 
	return getMouseData();
}

BUFFER Ch9329::mousewheel(char num)
{
	if(num > -128 && num< 128)
	{
		if(num<0)
		{
			mousewbyte = 0xFF + num + 1;
		}
		else
		{
			mousewbyte = num;	
		} 
	} 
	return getMouseData();
}

BUFFER Ch9329::getKeyData()
{
	UART_FMT fmt = {0x57,0xAB,0x00,0x02,0x08,{0x00,},0x00};
	//fmt.CMD = cmd.SEND_KB_GENERAL_DATA;
	//fmt.LEN = 8;
	char ctr = 0;
	for(int i=0;i<8;i++) 
	{
		if(ctrkeys[i].press == 1) 
		{
			ctr |= 1<<i;
		} 
	}
	ctrbyte = ctr; 
	fmt.DATA[0] = ctr;
	
	for(int i=0;i<6;i++) 
	{
		if(keys[i].press == 1) 
		{
			fmt.DATA[i+2] = keys[i].code;
		} 
	}
	
	setSum(&fmt);
	
	return getOutData(fmt);
	
}

BUFFER Ch9329::getMouseData()
{
	UART_FMT fmt = {0x57,0xAB,0x00,0x05,0x05,{0x00,},0x00};
	//fmt.CMD = cmd.SEND_MS_REL_DATA;
	//fmt.LEN = 5;
	char key = 0;
	for(int i=0;i<3;i++) 
	{
		if(mousekeys[i].press == 1) 
		{
			key |= 1<<i;
		} 
	}
	mousebyte = key; 
	fmt.DATA[0] = 0x01;
	fmt.DATA[1] = key;

	fmt.DATA[2] = mousexbyte;
	fmt.DATA[3] = mouseybyte;
	fmt.DATA[4] = mousewbyte;
	mousexbyte = 0;
	mouseybyte = 0;
	mousewbyte = 0;
	
	setSum(&fmt);
	
	return getOutData(fmt);
}

BUFFER Ch9329::getOutData(UART_FMT data)
{
	BUFFER buf;
	buf.len = data.LEN + 6;
	buf.data[0] = data.HEAD[0];
	buf.data[1] = data.HEAD[1];
	buf.data[2] = data.ADDR;
	buf.data[3] = data.CMD;
	buf.data[4] = data.LEN;
	for(int i=0;i<data.LEN;i++) 
	{
		buf.data[5 + i] = data.DATA[i];
	}
	buf.data[5 + data.LEN] = data.SUM;
	return buf;
}

char Ch9329::getBit(char byte, int position)
{
	return byte & (1 << position);
}

void Ch9329::setSum(UART_FMT* data)
{
	int t = data->HEAD[0] + data->HEAD[1] + data->ADDR + data->CMD + data->LEN;
	for(int i=0;i<data->LEN;i++)
	{
		t +=  data->DATA[i];
	} 
	data->SUM = t & 0xFF;
}

Ch9329::~Ch9329()
{
}
