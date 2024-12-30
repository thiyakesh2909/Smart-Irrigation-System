#define LCD_D 0xf<<14

#define RS 1<<12

#define E 1<<13

void delay_ms(unsigned int sec)

{

T0PR=15000-1;

T0TCR=0x01;

while(T0TC< sec);

T0TCR=0x03;

T0TCR=0X00;

}

void LCD_COMMAND(unsigned char cmd)

{

IOCLR0=LCD_D;

IOSET0=(cmd&0xf0)<<10;

IOCLR0=RS;

IOSET0=E;

delay_ms(2);

IOCLR0=E;




IOCLR0=LCD_D;

IOSET0=(cmd&0x0f)<<14;

IOCLR0=RS;

IOSET0=E;

delay_ms(2);

IOCLR0=E;

}


void LCD_DATA(unsigned char d)

{  

IOCLR0=LCD_D;

IOSET0=(d&0xf0)<<10;

IOSET0=RS;

IOSET0=E;

delay_ms(2);

IOCLR0=E;




IOCLR0=LCD_D;

IOSET0=(d&0x0f)<<14;

IOSET0=RS;

IOSET0=E;

delay_ms(2);

IOCLR0=E;

}




void LCD_INIT(void)

{

IODIR0|=LCD_D|RS|E;

LCD_COMMAND(0x01);

LCD_COMMAND(0x02);

LCD_COMMAND(0x0c);

LCD_COMMAND(0x28);

LCD_COMMAND(0x80);

}




void LCD_STR(char*s)

{

while(*s)

LCD_DATA(*s++);

}


