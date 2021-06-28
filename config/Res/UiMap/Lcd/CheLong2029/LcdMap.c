#include "LcdMap.h"

#ifdef	TD_ADD_USER_LCD
const uint16 fp_char_map[]=
{
	
	/*1A*/86,	/*1B*/85,	/*1C*/53,	/*1D*/38,	/*1E*/55,	/*1F*/87,	/*1G*/54,	
	/*1H*/71,	/*1I*/70,	/*1J*/69,	/*1K*/54,	/*1L*/37,	/*1M*/70,	/*1N*/39,
	
	/*2A*/83,	/*2B*/82,	/*2C*/50,	/*2D*/35,	/*2E*/52,	/*2F*/84,	/*2G*/51,	
	/*2H*/68,	/*2I*/67,	/*2J*/66,	/*2K*/51,	/*2L*/34,	/*2M*/67,	/*2N*/36,
	
	/*3A*/80,	/*3B*/95,	/*3C*/63,	/*3D*/32,	/*3E*/49,	/*3F*/81,	/*3G*/48,	
	/*3H*/65,	/*3I*/64,	/*3J*/79,	/*3K*/48,	/*3L*/47,	/*3M*/64,	/*3N*/33,
	
	/*4A*/93,	/*4B*/92,	/*4C*/60,	/*4D*/45,	/*4E*/62,	/*4F*/94,	/*4G*/61,	
	/*4H*/78,	/*4I*/77,	/*4J*/76,	/*4K*/61,	/*4L*/44,	/*4M*/77,	/*4N*/46,
	
	/*5A*/90,	/*5B*/89,	/*5C*/57,	/*5D*/42,	/*5E*/59,	/*5F*/91,	/*5G*/58,	
	/*5H*/75,	/*5I*/74,	/*5J*/73,	/*5K*/58,	/*5L*/41,	/*5M*/74,	/*5N*/43,
	
	/*6A*/25,	/*6B*/12,	/*6C*/11,	/*6D*/10,	/*6E*/26,	/*6F*/28,	/*6G*/27,	
	/*6H*/156,	/*6I*/156,	/*6J*/156,	/*6K*/27,	/*6L*/156,	/*6M*/156,	/*6N*/156,
	
	/*7A*/24,	/*7B*/156,	/*7C*/24,	/*7D*/24,	/*7E*/156,	/*7F*/24,	/*7G*/24,	
	/*7H*/156,	/*7I*/156,	/*7J*/156,	/*7K*/24,	/*7L*/156,	/*7M*/156,	/*7N*/156,
	
	/*8A*/156,	/*8B*/156,	/*8C*/156,	/*8D*/156,	/*8E*/156,	/*8F*/156,	/*8G*/156,	
	/*8H*/156,	/*8I*/156,	/*8J*/156,	/*8K*/156,	/*8L*/156,	/*8M*/156,	/*8N*/156,
	
	/*9A*/8,	/*9B*/7,	/*9C*/6,	/*9D*/5,	/*9E*/21,	/*9F*/23,	/*9G*/22,	
	/*9H*/156,	/*9I*/156,	/*9J*/156,	/*9K*/22,	/*9L*/156,	/*9M*/156,	/*9N*/156
};
#else
const uint16 fp_char_map[]=
{
/* 	       A,                    B,                     C,                 D,                   E,                 F,               G,                 H,                     I,                   J,               K,                L,                  M  */
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

