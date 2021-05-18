/*
 * main.c
 *
 *  Created on: Sep 28, 2020
 *      Author: yehia
 */

#include "STD_TYPES.h"

#include "DIO_interface.h"
#include "PORT_interface.h"
#include "ADC_interface.h"

#include "LED_interface.h"
#include "CLCD_interface.h"

#include <util/delay.h>

#define LDR1	120	//Near  LDR: 175
#define LDR2	85	//Far LDR: 115

void main(void)
{
	u8 Local_u8AvailablePositions=3;
	u16 Local_u16Ldr1, Local_u16Ldr2;

	LED_t Local_LED_tRedLed = {LED_PORTA, LED_PIN2, LED_u8ACTIVE_HIGH};
	LED_t Local_LED_tGreenLed = {LED_PORTA, LED_PIN3, LED_u8ACTIVE_HIGH};

	PORT_voidInit();
	ADC_voidInit();

	CLCD_voidInit();

	while(1)
	{
		ADC_u8StartConversionSynch(0, &Local_u16Ldr1);
		ADC_u8StartConversionSynch(1, &Local_u16Ldr2);

		if(Local_u8AvailablePositions==0)
		{
			LED_voidLedOn(&Local_LED_tRedLed);
			LED_voidLedOff(&Local_LED_tGreenLed);
		}
		else
		{
			LED_voidLedOff(&Local_LED_tRedLed);
			LED_voidLedOn(&Local_LED_tGreenLed);
		}

		if(Local_u16Ldr2<LDR2)
		{
			Local_u8AvailablePositions--;
			while(1)
			{
				ADC_u8StartConversionSynch(0, &Local_u16Ldr1);
				ADC_u8StartConversionSynch(1, &Local_u16Ldr2);

				if((Local_u16Ldr2>LDR2) && (Local_u16Ldr1>LDR1))
					break;
			}
		}
		if(Local_u16Ldr1<LDR1)
		{
			Local_u8AvailablePositions++;
			while(1)
			{
				ADC_u8StartConversionSynch(0, &Local_u16Ldr1);
				ADC_u8StartConversionSynch(1, &Local_u16Ldr2);

				if((Local_u16Ldr2>LDR2) && (Local_u16Ldr1>LDR1))
					break;
			}
		}


		if(Local_u8AvailablePositions==255)
			Local_u8AvailablePositions=1;

		CLCD_voidGotoXY(0,0);
		CLCD_voidSendString("Available       Positions:");
		CLCD_voidSendNumber(Local_u8AvailablePositions);
		CLCD_voidGotoXY(11,1);
		CLCD_voidSendString("     ");
	}
}
