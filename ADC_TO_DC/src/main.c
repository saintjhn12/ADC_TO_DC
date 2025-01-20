/*
 * voltage_covert.c
 *
 * Created: 12/18/2024 11:51:56 AM
 * Author : user
 */ 

#include <avr/io.h>
#define F_CPU 12000000
#include <util/delay.h>
#include <stdio.h>
#define v_ref 5.0//reference voltage
#define adc_rsolution 1023.0//10bit adc
#define  SCALING_FACTOR ((10.0 + 5.1) / 5.1) //scaling factor based on resistors (R1,R2)
char result[10];
#define dataport PORTD
#define control_port PORTC
#define rs 0
#define en 1
void lcd_cmd(unsigned char cmd){
	control_port &=~(1<<rs);
	control_port |=(1<<en);
	dataport = cmd;
	control_port &=~(1<<en);
	_delay_ms(10);
}
void lcd_data(unsigned char data){
	control_port|=(1<<rs);
	control_port|=(1<<en);
	dataport=data;
	control_port&=~(1<<en);
}
void lcd_print(char *str){
	while(*str){
	lcd_data(*str++);
	}
}

void adc_init()
{
	ADMUX = (1<<REFS0); //set reference voltage to avcc
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//enabling adc and seting the adpsc(128)
}
uint16_t adc_read(uint8_t channel){
	ADMUX=(ADMUX&0xf0)|(channel & 0xf0);//selecting and masking channel bits
	ADCSRA |=(1<ADSC); //start conversion
	while(ADCSRA&(1<<ADSC));//wait for conversion to conplete
	return ADC;
}
float adc_to_volt( uint8_t adc_value){
	 return (adc_value * v_ref) / adc_rsolution * SCALING_FACTOR;//converting adc to voltage
}

int main(void)
{
	uint16_t adc_value;
	float voltage;
	
	adc_init();

	
    /* Replace with your application code */
    while (1) 
    {
		adc_value =adc_read(0);
		voltage = adc_to_volt(adc_value);
		sprintf(result, "%f", voltage);
		
		lcd_print(result);
    }
}

