#include <iostream>
#include "WCOM.h"
#include "Ch9329.h"
using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

WCOM com;
Ch9329 chip;

void MouseDown(char code);
void MouseUp(char code);
void MousePress(char code);
void MouseMove(char x, char y);

void printbuff(BUFFER buf)
{
	for(int i=0;i<buf.len;i++)
	{
		printf("%02X,",(char)buf.data[i]);
	}
	printf("\n\r");
	
}

//自动刷抖音 
void douying()
{
	while(true)
	{
		com.purge();
		MouseDown(MOUSE_LEFT);

		MouseMove(0, -100);
		MouseMove(0, -100);

		MouseUp(MOUSE_LEFT);
		Sleep(1000);
		MouseMove(0, 100);
		MouseMove(0, 100);
		
		Sleep(10000);
	
	} 
}

int main(int argc, char** argv) {
	
	//打开串口 
	com.open((char*)"COM4");
	
	cout<<"程序已启动"<<endl; 

	douying(); 


	BUFFER buf={0,{0,}};
	com.read(&buf);
	printbuff(buf);

	return 0;
}

void MouseDown(char code)
{
	com.write(chip.mousedown(code));
} 

void MouseUp(char code)
{
	com.write(chip.mouseup(code));
} 

void MousePress(char code)
{
	com.write(chip.mousedown(code));
	Sleep(1);
	com.write(chip.mouseup(code));
}

void MouseMove(char x, char y)
{
	com.write(chip.mousemove(x, y));
} 
