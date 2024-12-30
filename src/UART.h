void uart_init(void)

{

PINSEL0|=0x05;

U0LCR=0x83;

U0DLL=97;

U0DLM=0;

U0LCR=0x03;

}


void uart_tx(unsigned char ch)

{

U0THR=ch;

while(((U0LSR>>5)&1)==0);

}


void uart_str(char* str)

{

while(*str)

uart_tx(*str++);

}


void gsm(char* sms)

{

uart_init();

uart_str("AT\r\n");

delay_ms(500);

uart_str("AT+CMGF=1\r\n");

delay_ms(500);

uart_str("AT+CMGS=\"+916382826750\"\r\n");

delay_ms(500);

uart_str(sms);

uart_tx(0x1A);

delay_ms(500);

}
