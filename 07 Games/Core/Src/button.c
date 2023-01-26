/**
* @file button.c
*
* @brief button interact file
*
* @par
* COPYRIGHT NOTICE: 2022 BSW TECH
*/
#include"button.h"



uint8_t Button_Check(button BTN)
{
	if (!HAL_GPIO_ReadPin(BTN.PORT, BTN.PIN) && !BTN.StateFlag)
	{
		BTN.StateFlag = TRUE;
		return TRUE;
	}
	else if (HAL_GPIO_ReadPin(BTN.PORT, BTN.PIN) && BTN.StateFlag)
	{
		BTN.StateFlag = FALSE;
		return FALSE;
	}
	return FALSE;
}

/*** end of file ***/
