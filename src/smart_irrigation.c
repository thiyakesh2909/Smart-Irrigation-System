#include<lpc21xx.h>
#include<stdio.h>

#define lcd_data 0xf<<20
#define RS 1<<17
#define RW 1<<18
#define E 1<<19

#define motor_1A 1<<24
#define motor_2A 1<<25
#define moisture_pin 1<<8

int count1=0,count2=0;
//Milli seconds delay
void delay_ms(int s)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<s);
	T0TCR=0X03;
	T0TCR=0x00;
}

//16X2 Alphanumeric LCD
void command(unsigned char cmd)
{
	IOCLR1=lcd_data;
	IOSET1=(cmd&0xf0)<<16;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;

	IOCLR1=lcd_data;
	IOSET1=(cmd&0x0f)<<20;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
}

void lcd_init(void)
{
	IODIR1|=lcd_data|RS|RW|E;
	command(0x01);
	command(0x02);
	command(0x0c);
	command(0x28);
	command(0x80);
}

void data(unsigned char ch)
{
	IOCLR1=lcd_data;
	IOSET1=(ch&0xf0)<<16;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;

	IOCLR1=lcd_data;
	IOSET1=(ch&0x0f)<<20;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_ms(2);
	IOCLR1=E;
}

void lcd_str(char *s)
{
  while(*s)
  data(*s++);
}

//GSM Interfacing through UART
void uart_init(void)
{
	PINSEL0=0x05;
	U0LCR=0X83;
	U0DLL=97;
	U0DLM=0;
	U0LCR=0X03;
}

void uart_tx(unsigned char ch)
{
	U0THR=ch;
	while(((U0LSR>>5)&1)==0);
}

void uart_str(char *str)
{
	while(*str)
	uart_tx(*str++);
}

void gsm(char *message)
{
	uart_init();
	uart_str("AT\r\n");
	delay_ms(500);
	uart_str("AT+CMGF=1\r\n"); //text mode
	delay_ms(500);

	uart_str("AT+CMGS=\"+918328379325\"\r\n"); //sms
	delay_ms(500);
	uart_str(message);
	uart_tx(0x1A);
	delay_ms(500);
}

//Motor interfacing
void motor_init(void)
{
	IODIR0|=motor_1A|motor_2A; //selected p0.24 and p0.25
}

void motor_on(void)
{
	IOSET0=motor_1A;
	IOCLR0=motor_2A;
	command(0x80);
	lcd_str("MOTOR ON ");
	if(count1<3)
	{
	gsm("MOTOR ON\r\n");
	count1++;
	}
	
}

void motor_off(void)
{
	IOCLR0=motor_1A|motor_2A;
	command(0x80);
	lcd_str("MOTOR OFF ");
	if(count2<3)
	{
	gsm("MOTOR OFF\r\n");
	count2++;
	}
	}

int moisture_status(void)
{
	if(((IOPIN0>>8)&1)==1)
		return 1;
	else
		return 0;
}
//Main logic
int main()
{
	int moisture_level;
	char buffer[5];
	lcd_init();
	uart_init();
	//moisture_init();
	motor_init();

	while(1)
	{
		moisture_level = moisture_status();
		command(0xc0);
		
		sprintf(buffer,"%d",moisture_level);
		lcd_str(buffer);
		
		if(moisture_level)
		motor_on(); //turn on motor when moisture is low
		else
		motor_off();
		
		delay_ms(500);
		command(0x01);
	}
}
