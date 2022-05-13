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
	char HEAD[2]; // ֡ͷ��ռ 2 ���ֽڣ��̶�Ϊ 0x57��0xAB
	char ADDR; // ��ַ�룺ռ 1 ���ֽڣ�Ĭ��Ϊ 0x00���ɽ��������ַ�������������оƬ��ַ���ó� 0x01---0xFE����ֻ�ܽ��ն�Ӧ��ַ����ַ��Ϊ 0xFF ���������0xFF Ϊ�㲥����оƬ���� Ҫ����Ӧ��
	char CMD; // �����룺ռ 1 ���ֽڣ���Χ�����豸�����֡����������Ч��ΧΪ��0x01---0x3F��CH9329 оƬ��������Ӧ���ʱ��������Ϊ��ԭ������ | 0x80��CH9329 оƬ�����쳣Ӧ���ʱ���� ����Ϊ��ԭ������ | 0xC0��
	char LEN; // �������ݳ��ȣ�ռ 1 ���ֽڣ���Ҫ���ڼ�¼�ð�ʵ�ʺ������ݵĳ��ȣ��������������ݲ��֣�������֡ͷ�ֽڡ���ַ�롢��������ۼӺ��ֽڣ� 
	char DATA[64]; // �������ݣ�ռ N ���ֽڣ�N ��Ч��ΧΪ 0---64��typedef 
	char SUM; // �ۼӺͣ�ռ 1 ���ֽڣ����㷽ʽΪ�� SUM = HEAD+ADDR+CMD+LEN+DATA
}UART_FMT;

typedef struct ch9329cmd{
	char GET_INFO; //��ȡоƬ�汾����Ϣ ͨ����������оƬ��ȡ�汾�š� USB ö��״̬�����̴�Сдָʾ�� ״̬����Ϣ 
	char SEND_KB_GENERAL_DATA; //���� USB ������ͨ���� ͨ����������оƬ������ͨ���� ���ݰ���ģ����ͨ�������»��ͷ� ����
	char SEND_KB_MEDIA_DATA; //���� USB ���̶�ý������ ͨ����������оƬ���Ͷ�ý��� �����ݰ���ģ���ý�尴�����»� �ͷŶ��� 
	char SEND_MS_ABS_DATA; //���� USB ����������� ͨ����������оƬ���;������ ���ݰ���ģ����������ض��� 
	char SEND_MS_REL_DATA; //���� USB ���������� ͨ����������оƬ���������� ���ݰ���ģ����������ض��� 
	char SEND_MY_HID_DATA; //���� USB �Զ��� HID �豸���� ͨ����������оƬ�����Զ��� HID ���豸���ݰ�
	char READ_MY_HID_DATA; //��ȡ USB �Զ��� HID �豸���� ͨ���������оƬ��ȡ�Զ��� HID ���豸���ݰ� ע��PC ����оƬ�´� 1 ���Զ��� HID ���ݰ�����оƬ�����Զ��� �����͸���Χ�����豸 
	char GET_PARA_CFG; //��ȡ�������� ͨ����������оƬ��ȡ��ǰ���� ������Ϣ
	char SET_PARA_CFG; //���ò������� ͨ����������оƬ���õ�ǰ���� ������Ϣ 
	char GET_USB_STRING; //��ȡ�ַ������������� ͨ����������оƬ��ȡ��ǰ��ʹ �õ� USB �ַ������������� 
	char SET_USB_STRING; //�����ַ������������� 
	char SET_DEFAULT_CFG; //�ָ�����Ĭ������ ͨ�������оƬ�Ĳ������ü� �ַ���������Ϣ�ָ�������Ĭ�� ���� 
	char RESET; //��λоƬ ͨ�����������оƬ��������� λ���� 
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
