#define m1 1<<19
#define m2 1<<20
#define mois_pin 8 
int count_1=0;
void motor_init(void)
{
IODIR0|=m1|m2;
}
void motor_on(void)
{
IOSET0=m1;
IOCLR0=m2;
LCD_COMMAND(0x80);
LCD_STR("MOTOR ON ");
if(count_1==0)
{
gsm("MOTOR ON\r\n");
count_1=1;
}
}
void motor_off(void)
{
IOCLR0=m1|m2;
LCD_COMMAND(0x80);
LCD_STR("MOTOR OFF");
if(count_1==1)
{
gsm("MOTOR OFF\r\n");
count_1=0;
}
}


void mois_status(void)

{

if(((IOPIN0>>mois_pin)&1)==1)

	motor_on();


else

	motor_off();

	

}

