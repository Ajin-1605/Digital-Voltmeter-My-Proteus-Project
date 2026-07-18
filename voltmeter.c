#include <reg51.h>

#define adc P1
#define lcd P2

sbit ADC_RD   = P3^7;
sbit ADC_WR   = P3^6;
sbit ADC_INTR = P3^5;

sbit RS = P3^0;
sbit RW = P3^1;
sbit EN = P3^2;

void delay(unsigned int ms)
{
    unsigned int i,j;
    for(i=0;i<ms;i++)
        for(j=0;j<1275;j++);
}

void lcd_cmd(unsigned char cmd)
{
    lcd = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    delay(2);
    EN = 0;
}

void lcd_data(unsigned char dat)
{
    lcd = dat;
    RS = 1;
    RW = 0;
    EN = 1;
    delay(2);
    EN = 0;
}

void lcd_string(char *str)
{
    while(*str)
        lcd_data(*str++);
}

void lcd_init()
{
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
}

unsigned char read_adc()
{
    ADC_WR = 0;
    delay(1);
    ADC_WR = 1;

    while(ADC_INTR==1);

    ADC_RD = 0;
    delay(1);

    return adc;
}

void display_voltage(unsigned int val)
{
    unsigned int voltage;

    voltage = (val * 500) / 255;

    lcd_cmd(0xC0);

    lcd_data((voltage/100)+'0');
    lcd_data('.');
    lcd_data(((voltage%100)/10)+'0');
    lcd_data((voltage%10)+'0');

    lcd_data('V');
}

void main()
{
    unsigned char adc_value;

    lcd_init();
    lcd_string("Digital Voltmeter");

    while(1)
    {
        adc_value = read_adc();
        display_voltage(adc_value);
        delay(200);
    }
}