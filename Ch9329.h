#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <iostream>
using namespace std;

#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_MID 2

typedef struct buffer{
	int len;
	char data[70];
}BUFFER;

typedef struct keyevent{
	char press;
	char code;
}KEYEVENT;



typedef struct uart_fmt{
	char HEAD[2]; // 帧头：占 2 个字节，固定为 0x57、0xAB
	char ADDR; // 地址码：占 1 个字节，默认为 0x00，可接收任意地址码的命令包，如果芯片地址设置成 0x01---0xFE，则只能接收对应地址码或地址码为 0xFF 的命令包。0xFF 为广播包，芯片不需 要进行应答；
	char CMD; // 命令码：占 1 个字节，外围串口设备发起的帧的命令码有效范围为：0x01---0x3F，CH9329 芯片发送正常应答包时的命令码为：原命令码 | 0x80；CH9329 芯片发送异常应答包时的命 令码为：原命令码 | 0xC0；
	char LEN; // 后续数据长度：占 1 个字节，主要用于记录该包实际后续数据的长度，仅包含后续数据部分，不包括帧头字节、地址码、命令码和累加和字节； 
	char DATA[64]; // 后续数据：占 N 个字节，N 有效范围为 0---64。typedef 
	char SUM; // 累加和：占 1 个字节，计算方式为： SUM = HEAD+ADDR+CMD+LEN+DATA
}UART_FMT;

typedef struct ch9329cmd{
	char GET_INFO; //获取芯片版本等信息 通过该命令向芯片获取版本号、 USB 枚举状态、键盘大小写指示灯 状态等信息 
	char SEND_KB_GENERAL_DATA; //发送 USB 键盘普通数据 通过该命令向芯片发送普通键盘 数据包，模拟普通按键按下或释放 动作
	char SEND_KB_MEDIA_DATA; //发送 USB 键盘多媒体数据 通过该命令向芯片发送多媒体键 盘数据包，模拟多媒体按键按下或 释放动作 
	char SEND_MS_ABS_DATA; //发送 USB 绝对鼠标数据 通过该命令向芯片发送绝对鼠标 数据包，模拟绝对鼠标相关动作 
	char SEND_MS_REL_DATA; //发送 USB 相对鼠标数据 通过该命令向芯片发送相对鼠标 数据包，模拟相对鼠标相关动作 
	char SEND_MY_HID_DATA; //发送 USB 自定义 HID 设备数据 通过该命令向芯片发送自定义 HID 类设备数据包
	char READ_MY_HID_DATA; //读取 USB 自定义 HID 设备数据 通过该命令从芯片读取自定义 HID 类设备数据包 注：PC 机向芯片下传 1 包自定义 HID 数据包后，由芯片串口自动打 包发送给外围串口设备 
	char GET_PARA_CFG; //获取参数配置 通过该命令向芯片获取当前参数 配置信息
	char SET_PARA_CFG; //设置参数配置 通过该命令向芯片设置当前参数 配置信息 
	char GET_USB_STRING; //获取字符串描述符配置 通过该命令向芯片获取当前所使 用的 USB 字符串描述符配置 
	char SET_USB_STRING; //设置字符串描述符配置 
	char SET_DEFAULT_CFG; //恢复出厂默认配置 通过该命令将芯片的参数配置及 字符串配置信息恢复到出厂默认 设置 
	char RESET; //复位芯片 通过该命令控制芯片进行软件复 位控制 
}CH9329CMD;
//ch9329cmd CH9329CMD = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x87, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0F};

class Ch9329
{
	public:
		Ch9329();
		BUFFER keydown(char code);
		BUFFER keyup(char code);
		BUFFER mousedown(char code);
		BUFFER mouseup(char code);
		BUFFER mousemove(char x, char y);
		BUFFER mousewheel(char num);
		~Ch9329();
	protected:
		char ctrbyte;
		char mousebyte;
		char mousexbyte;
		char mouseybyte;
		char mousewbyte;
		KEYEVENT ctrkeys[8];
		KEYEVENT keys[6];
		KEYEVENT mousekeys[3];
		struct ch9329cmd cmd;
		
		BUFFER getKeyData();
		BUFFER getMouseData();
		char getBit(char byte, int position);
		void setSum(UART_FMT* data);
		BUFFER getOutData(UART_FMT data);
};

#endif
