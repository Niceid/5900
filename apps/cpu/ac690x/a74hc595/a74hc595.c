#include "includes.h"

#if defined(_SUPPORT_PANEL_74HC595)
void Latches_Delay(int time)
{
	int i;
	for(i = 0; i < time; i++)
	{
		asm volatile ("nop");
	}
}


#define LATCHES_Delay()				Latches_Delay(100)

static void latches_port_init(void)
{
	LATCH_CLK_INIT();	LATCH_CLK_OUT();  	   LATCH_CLK_LOW();//LATCH_CLK_HIGH();
	LATCH_DATA_INIT();	LATCH_DATA_OUT();	   LATCH_DATA_LOW(); //LATCH_DATA_HIGH();
	LATCH_SCLK_INIT();	LATCH_SCLK_OUT();	   LATCH_SCLK_LOW();//LATCH_SCLK_HIGH();
}


void latches_send_data(uint8 Data)
{
	uint8 Screen;   
	

	for(Screen = _B7_; Screen; Screen >>= 1)
	{
		LATCH_CLK_LOW();
		if(Data & Screen)
			LATCH_DATA_HIGH();
		else
			LATCH_DATA_LOW();
		
		LATCHES_Delay();
		LATCH_CLK_HIGH();
		LATCHES_Delay();
	}
	
	LATCH_SCLK_LOW();
	LATCHES_Delay();
	LATCH_SCLK_HIGH();
	LATCHES_Delay();


}
#endif

