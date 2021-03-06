#include "LcdMap.h"

#ifdef	TD_ADD_USER_LCD
const uint16 fp_char_map[]=
{
	
	/*1A*/14,	/*1B*/15,	/*1C*/95,	/*1D*/94,	/*1E*/62,	/*1F*/46,	/*1G*/63,	
	/*1H*/30,	/*1I*/47,	/*1J*/31,	/*1K*/63,	/*1L*/79,	/*1M*/47,	/*1N*/78,
	
	/*2A*/0,	/*2B*/1,	/*2C*/81,	/*2D*/80,	/*2E*/48,	/*2F*/32,	/*2G*/49,	
	/*2H*/16,	/*2I*/33,	/*2J*/17,	/*2K*/49,	/*2L*/65,	/*2M*/33,	/*2N*/64,
	
	/*3A*/3,	/*3B*/4,	/*3C*/84,	/*3D*/83,	/*3E*/51,	/*3F*/35,	/*3G*/52,	
	/*3H*/19,	/*3I*/36,	/*3J*/20,	/*3K*/52,	/*3L*/68,	/*3M*/36,	/*3N*/67,
	
	/*4A*/5,	/*4B*/6,	/*4C*/86,	/*4D*/85,	/*4E*/53,	/*4F*/37,	/*4G*/54,	
	/*4H*/21,	/*4I*/38,	/*4J*/22,	/*4K*/54,	/*4L*/70,	/*4M*/38,	/*4N*/69,
	
	/*5A*/156,	/*5B*/156,	/*5C*/156,	/*5D*/156,	/*5E*/156,	/*5F*/156,	/*5G*/156,	
	/*5H*/156,	/*5I*/156,	/*5J*/156,	/*5K*/156,	/*5L*/156,	/*5M*/156,	/*5N*/156,
	
	/*6A*/156,	/*6B*/156,	/*6C*/156,	/*6D*/156,	/*6E*/156,	/*6F*/156,	/*6G*/156,	
	/*6H*/156,	/*6I*/156,	/*6J*/156,	/*6K*/156,	/*6L*/156,	/*6M*/156,	/*6N*/156,
	
	/*7A*/156,	/*7B*/156,	/*7C*/156,	/*7D*/156,	/*7E*/156,	/*7F*/156,	/*7G*/156,	
	/*7H*/156,	/*7I*/156,	/*7J*/156,	/*7K*/156,	/*7L*/156,	/*7M*/156,	/*7N*/156,
	
	/*8A*/156,	/*8B*/156,	/*8C*/156,	/*8D*/156,	/*8E*/156,	/*8F*/156,	/*8G*/156,	
	/*8H*/156,	/*8I*/156,	/*8J*/156,	/*8K*/156,	/*8L*/156,	/*8M*/156,	/*8N*/156,
	
	/*9A*/156,	/*9B*/156,	/*9C*/156,	/*9D*/156,	/*9E*/156,	/*9F*/156,	/*9G*/156,	
	/*9H*/156,	/*9I*/156,	/*9J*/156,	/*9K*/156,	/*9L*/156,	/*9M*/156,	/*9N*/156
};
#else
const uint16 fp_char_map[]=
{
/* 	       A,          B,            C,           D,          E,             F,          G,           H,           I,          J,          K,             L,           M  */
	  L2I2(6,2),   L2I2(6,3),   L2I2(1,3),   L2I2(1,2),   L2I2(3,2),   L2I2(4,2),   L2I2(3,3),   L2I2(4,3),   L2I2(5,2),   L2I2(2,3),   L2I2(4,3),   L2I2(2,2),   L2I2(5,3)
	 ,L2I2(6,4),   L2I2(6,5),   L2I2(1,5),   L2I2(1,4),   L2I2(3,4),   L2I2(4,4),   L2I2(3,5),   L2I2(4,5),   L2I2(5,4),   L2I2(2,5),   L2I2(4,5),   L2I2(2,4),   L2I2(5,5)
	 ,L2I2(6,7),   L2I2(6,8),   L2I2(1,8),   L2I2(1,7),   L2I2(3,7),   L2I2(4,7),   L2I2(3,8),   L2I2(4,8),   L2I2(5,7),   L2I2(2,8),   L2I2(4,8),   L2I2(2,7),   L2I2(5,8)
	 ,L2I2(6,9),   L2I2(6,10),  L2I2(1,10),  L2I2(1,9),   L2I2(3,9),   L2I2(4,9),   L2I2(3,10),  L2I2(4,10),  L2I2(5,9),   L2I2(2,10),  L2I2(4,10),  L2I2(2,9),   L2I2(5,10)
	 ,LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL

	 ,LI_NULL,	   LI_NULL,	    LI_NULL, 	 LI_NULL,	  LI_NULL,	   LI_NULL,	    LI_NULL, 	 LI_NULL,	  LI_NULL,	   LI_NULL,	   LI_NULL,    LI_NULL,	   LI_NULL
	 ,LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
	 ,LI_NULL,	   LI_NULL,	    LI_NULL, 	 LI_NULL,	  LI_NULL,	   LI_NULL,	    LI_NULL, 	 LI_NULL,	  LI_NULL,	   LI_NULL,	   LI_NULL,    LI_NULL,	   LI_NULL
	 ,LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
};
#endif

