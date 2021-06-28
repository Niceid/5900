#include "LcdMap.h"

#ifdef	TD_ADD_USER_LCD

const uint16 fp_char_map[]=
{
	///*1A*/111,	/*1B*/99,	/*1C*/97,	/*1D*/104,	/*1E*/105,	/*1F*/107,	/*1G*/109,	
	///*1H*/106,	/*1I*/110,	/*1J*/98,	/*1K*/109,	/*1L*/96,	/*1M*/110,	/*1N*/108,
	
	/*2A*/91,	/*2B*/95,	/*2C*/93,	/*2D*/100,	/*2E*/101,	/*2F*/103,	/*2G*/89,	
	/*2H*/102,	/*2I*/90,	/*2J*/94,	/*2K*/89,	/*2L*/92,	/*2M*/90,	/*2N*/88,
	
	/*3A*/87,	/*3B*/75,	/*3C*/73,	/*3D*/80,	/*3E*/81,	/*3F*/83,	/*3G*/85,	
	/*3H*/82,	/*3I*/86,	/*3J*/74,	/*3K*/85,	/*3L*/72,	/*3M*/86,	/*3N*/84,
	
	/*4A*/67,	/*4B*/71,	/*4C*/69,	/*4D*/76,	/*4E*/77,	/*4F*/79,	/*4G*/65,	
	/*4H*/78,	/*4I*/66,	/*4J*/70,	/*4K*/65,	/*4L*/68,	/*4M*/66,	/*4N*/64,

	/*1A*/63,	/*1B*/51,	/*1C*/49,	/*1D*/56,	/*1E*/57,	/*1F*/59,	/*1G*/61,	
	/*1H*/58,	/*1I*/62,	/*1J*/50,	/*1K*/61,	/*1L*/48,	/*1M*/62,	/*1N*/60,

	
	/*5A*/35,	/*5B*/34,	/*5C*/33,	/*5D*/32,	/*5E*/45,	/*5F*/47,	/*5G*/46,	
	/*5H*/156,	/*5I*/156,	/*5J*/156,	/*5K*/46,	/*5L*/156,	/*5M*/156,	/*5N*/156,
	
	/*6A*/7,	/*6B*/6,	/*6C*/5,	/*6D*/4,	/*6E*/37,	/*6F*/39,	/*6G*/38,	
	/*6H*/156,	/*6I*/156,	/*6J*/156,	/*6K*/38,	/*6L*/156,	/*6M*/156,	/*6N*/156,
	
	/*7A*/15,	/*7B*/14,	/*7C*/13,	/*7D*/12,	/*7E*/1,	/*7F*/3,	/*7G*/2,	
	/*7H*/156,	/*7I*/156,	/*7J*/156,	/*7K*/2,	/*7L*/156,	/*7M*/156,	/*7N*/156,
	
	/*8A*/23,	/*8B*/22,	/*8C*/21,	/*8D*/20,	/*8E*/9,	/*8F*/11,	/*8G*/10,	
	/*8H*/156,	/*8I*/156,	/*8J*/156,	/*8K*/10,	/*8L*/156,	/*8M*/156,	/*8N*/156,


	
	/*9A*/119,	/*9B*/118,	/*9C*/117,	/*9D*/116,	/*9E*/113,	/*9F*/115,	/*9G*/114,	
	/*9H*/156,	/*9I*/156,	/*9J*/156,	/*9K*/114,	/*9L*/156,	/*9M*/156,	/*9N*/156
};

#else

const uint16 fp_char_map[]=
{
	/*      A,          	B,          		   C,          		  D,          		  E,          		  F,          		  G,          		  H,          		  I,          		  J,          		  K,          L,          		  M       */
      L2I(COM_4,SEG_4),   L2I(COM_4,SEG_5),   L2I(COM_2,SEG_5),   L2I(COM_1,SEG_4),   L2I(COM_2,SEG_3),   L2I(COM_4,SEG_3),   L2I(COM_2,SEG_4),   L2I(COM_3,SEG_3),   L2I(COM_3,SEG_4),   L2I(COM_3,SEG_5),   LI_NULL,    L2I(COM_1,SEG_5),   L2I(COM_1,SEG_3)
	 ,L2I(COM_4,SEG_7),   L2I(COM_4,SEG_8),   L2I(COM_2,SEG_8),   L2I(COM_1,SEG_7),   L2I(COM_2,SEG_6),   L2I(COM_4,SEG_6),   L2I(COM_2,SEG_7),   L2I(COM_3,SEG_6),   L2I(COM_3,SEG_7),   L2I(COM_3,SEG_8),   LI_NULL,    L2I(COM_1,SEG_8),   L2I(COM_1,SEG_6)
	 ,L2I(COM_4,SEG_10),  L2I(COM_4,SEG_11),  L2I(COM_2,SEG_11),  L2I(COM_1,SEG_10),  L2I(COM_2,SEG_9),   L2I(COM_4,SEG_9),   L2I(COM_2,SEG_10),  L2I(COM_3,SEG_9),   L2I(COM_3,SEG_10),  L2I(COM_3,SEG_11),  LI_NULL,    L2I(COM_1,SEG_11),  L2I(COM_1,SEG_9)
	 ,L2I(COM_4,SEG_13),  L2I(COM_4,SEG_14),  L2I(COM_2,SEG_14),  L2I(COM_1,SEG_13),  L2I(COM_2,SEG_12),  L2I(COM_4,SEG_12),  L2I(COM_2,SEG_13),  L2I(COM_3,SEG_12),  L2I(COM_3,SEG_13),  L2I(COM_3,SEG_14),  LI_NULL,    L2I(COM_1,SEG_14),  L2I(COM_1,SEG_12)

     ,L2I(COM_4,SEG_17),  L2I(COM_4,SEG_18),  L2I(COM_2,SEG_18),  L2I(COM_1,SEG_17),  L2I(COM_2,SEG_16),  L2I(COM_4,SEG_16),  L2I(COM_2,SEG_17),  L2I(COM_3,SEG_16),  L2I(COM_3,SEG_17),  L2I(COM_3,SEG_18),  LI_NULL,    L2I(COM_1,SEG_18),  L2I(COM_1,SEG_16)
	 ,L2I(COM_4,SEG_20),  L2I(COM_4,SEG_21),  L2I(COM_2,SEG_21),  L2I(COM_1,SEG_20),  L2I(COM_2,SEG_19),  L2I(COM_4,SEG_19),  L2I(COM_2,SEG_20),  L2I(COM_3,SEG_19),  L2I(COM_3,SEG_20),  L2I(COM_3,SEG_21),  LI_NULL,    L2I(COM_1,SEG_21),  L2I(COM_1,SEG_19)
	 ,L2I(COM_4,SEG_23),  L2I(COM_4,SEG_24),  L2I(COM_2,SEG_24),  L2I(COM_1,SEG_23),  L2I(COM_2,SEG_22),  L2I(COM_4,SEG_22),  L2I(COM_2,SEG_23),  L2I(COM_3,SEG_22),  L2I(COM_3,SEG_23),  L2I(COM_3,SEG_24),  LI_NULL,    L2I(COM_1,SEG_24),  L2I(COM_1,SEG_22)	 
	 ,L2I(COM_4,SEG_26),  L2I(COM_4,SEG_27),  L2I(COM_2,SEG_27),  L2I(COM_1,SEG_26),  L2I(COM_2,SEG_25),  L2I(COM_4,SEG_25),  L2I(COM_2,SEG_26),  L2I(COM_3,SEG_25),  L2I(COM_3,SEG_26),  L2I(COM_3,SEG_27),  LI_NULL,    L2I(COM_1,SEG_27),  L2I(COM_1,SEG_25)

	 ,LI_NULL,            LI_NULL,            LI_NULL,            LI_NULL,            LI_NULL,            LI_NULL,            LI_NULL,            LI_NULL,    	  	  LI_NULL,    	  	  LI_NULL,    	  	  LI_NULL,    LI_NULL,    	  	  LI_NULL
};
#endif

