#include "LcdMap.h"

#if defined(TD_ADD_USER_LCD)
const uint16 fp_char_map[]=
{
	/*1A*/119,	/*1B*/107,	/*1C*/105,	/*1D*/116,	/*1E*/113,	/*1F*/115,	/*1G*/117,
	/*1H*/114,	/*1I*/118,	/*1J*/106,	/*1K*/117,	/*1L*/104,	/*1M*/118,	/*1N*/112,
	
	/*2A*/99,	/*2B*/103,	/*2C*/101,	/*2D*/96,	/*2E*/109,	/*2F*/111,	/*2G*/97,
	/*2H*/110,	/*2I*/98,	/*2J*/102,	/*2K*/97,	/*2L*/100,	/*2M*/98,	/*2N*/108,
	
	/*3A*/95,	/*3B*/83,	/*3C*/81,	/*3D*/92,	/*3E*/89,	/*3F*/91,	/*3G*/93,
	/*3H*/90,	/*3I*/94,	/*3J*/82,	/*3K*/93,	/*3L*/80,	/*3M*/94,	/*3N*/88,
	
	/*4A*/75,	/*4B*/79,	/*4C*/77,	/*4D*/72,	/*4E*/85,	/*4F*/87,	/*4G*/73,
	/*4H*/86,	/*4I*/74,	/*4J*/78,	/*4K*/73,	/*4L*/76,	/*4M*/74,	/*4N*/84,
	
	/*5A*/63,	/*5B*/51,	/*5C*/49,	/*5D*/60,	/*5E*/57,	/*5F*/59,	/*5G*/61,
	/*5H*/58,	/*5I*/62,	/*5J*/50,	/*5K*/61,	/*5L*/48,	/*5M*/62,	/*5N*/56,
	
	/*6A*/43,	/*6B*/47,	/*6C*/45,	/*6D*/40,	/*6E*/53,	/*6F*/55,	/*6G*/41,
	/*6H*/54,	/*6I*/42,	/*6J*/46,	/*6K*/41,	/*6L*/44,	/*6M*/42,	/*6N*/52,
	
	/*7A*/7,	/*7B*/3,	/*7C*/1,	/*7D*/4,	/*7E*/37,	/*7F*/39,	/*7G*/5,
	/*7H*/38,	/*7I*/6,	/*7J*/2,	/*7K*/5,	/*7L*/0,	/*7M*/6,	/*7N*/36,
	
	/*8A*/11,	/*8B*/23,	/*8C*/21,	/*8D*/8,	/*8E*/13,	/*8F*/15,	/*8G*/9,
	/*8H*/14,	/*8I*/10,	/*8J*/22,	/*8K*/9,	/*8L*/20,	/*8M*/10,	/*8N*/12,
	
	/*9A*/31,	/*9B*/30,	/*9C*/29,	/*9D*/16,	/*9E*/17,	/*9F*/19,	/*9G*/18,
	/*9H*/156,	/*9I*/156,	/*9J*/156,	/*9K*/18,	/*9L*/156,	/*9M*/156,	/*9N*/156,
};

#else
const uint16 fp_char_map[]=
{
	    /*      A,          	               B,          		          C,          		          D,          		          E,          		           F,          		          G,          		           H,          		          I,          		         J,          		      K,                                   L,          		        M       */
      L2I(COM_1,SEG_4),   L2I(COM_1,SEG_5),   L2I(COM_3,SEG_5),   L2I(COM_4,SEG_4),   L2I(COM_3,SEG_3),   L2I(COM_1,SEG_3),   L2I(COM_3,SEG_4),   L2I(COM_2,SEG_4),   L2I(COM_2,SEG_3),   L2I(COM_4,SEG_5),   L2I(COM_2,SEG_4),    L2I(COM_4,SEG_3),   L2I(COM_2,SEG_5)
	 ,L2I(COM_1,SEG_7),   L2I(COM_1,SEG_8),   L2I(COM_3,SEG_8),   L2I(COM_4,SEG_7),   L2I(COM_3,SEG_6),   L2I(COM_1,SEG_6),   L2I(COM_3,SEG_7),   L2I(COM_2,SEG_7),   L2I(COM_2,SEG_6),   L2I(COM_4,SEG_8),   L2I(COM_2,SEG_7),    L2I(COM_4,SEG_6),   L2I(COM_2,SEG_8)
	 ,L2I(COM_1,SEG_10),  L2I(COM_1,SEG_11),  L2I(COM_3,SEG_11),  L2I(COM_4,SEG_10),  L2I(COM_3,SEG_9),   L2I(COM_1,SEG_9),   L2I(COM_3,SEG_10),  L2I(COM_2,SEG_10),  L2I(COM_2,SEG_9),   L2I(COM_4,SEG_11),  L2I(COM_2,SEG_10),   L2I(COM_4,SEG_9),   L2I(COM_2,SEG_11)
	 ,L2I(COM_1,SEG_13),  L2I(COM_1,SEG_14),  L2I(COM_3,SEG_14),  L2I(COM_4,SEG_13),  L2I(COM_3,SEG_12),  L2I(COM_1,SEG_12),  L2I(COM_3,SEG_13),  L2I(COM_2,SEG_13),  L2I(COM_2,SEG_12),  L2I(COM_4,SEG_14),  L2I(COM_2,SEG_13),   L2I(COM_4,SEG_12),  L2I(COM_2,SEG_14)

     ,L2I(COM_1,SEG_18),  L2I(COM_1,SEG_19),  L2I(COM_3,SEG_19),  L2I(COM_4,SEG_18),  L2I(COM_3,SEG_17),  L2I(COM_1,SEG_17),  L2I(COM_3,SEG_18),  L2I(COM_2,SEG_18),  L2I(COM_2,SEG_17),  L2I(COM_4,SEG_19),  L2I(COM_2,SEG_18),   L2I(COM_4,SEG_17),  L2I(COM_2,SEG_19)
	 ,L2I(COM_1,SEG_21),  L2I(COM_1,SEG_22),  L2I(COM_3,SEG_22),  L2I(COM_4,SEG_21),  L2I(COM_3,SEG_20),  L2I(COM_1,SEG_20),  L2I(COM_3,SEG_21),  L2I(COM_2,SEG_21),  L2I(COM_2,SEG_20),  L2I(COM_4,SEG_22),  L2I(COM_2,SEG_21),   L2I(COM_4,SEG_20),  L2I(COM_2,SEG_22)

	 ,L2I(COM_1,SEG_25),  L2I(COM_1,SEG_26),  L2I(COM_3,SEG_26),  L2I(COM_4,SEG_25),  L2I(COM_3,SEG_24),  L2I(COM_1,SEG_24),  L2I(COM_3,SEG_25),  L2I(COM_2,SEG_25),  L2I(COM_2,SEG_24),  L2I(COM_4,SEG_26),  L2I(COM_2,SEG_25),   L2I(COM_4,SEG_24),  L2I(COM_2,SEG_26)	 
	 ,L2I(COM_1,SEG_28),  L2I(COM_1,SEG_29),  L2I(COM_3,SEG_29),  L2I(COM_4,SEG_28),  L2I(COM_3,SEG_27),  L2I(COM_1,SEG_27),  L2I(COM_3,SEG_28),  L2I(COM_2,SEG_28),  L2I(COM_2,SEG_27),  L2I(COM_4,SEG_29),  L2I(COM_2,SEG_28),   L2I(COM_4,SEG_27),  L2I(COM_2,SEG_29)

     ,L2I(COM_1,SEG_31),  L2I(COM_2,SEG_31),  L2I(COM_3,SEG_31),  L2I(COM_4,SEG_30),  L2I(COM_3,SEG_30),  L2I(COM_1,SEG_30),  L2I(COM_2,SEG_30),  LI_NULL,    	  	  LI_NULL,    	  	  LI_NULL,    	  	  LI_NULL,    LI_NULL,    	  	  LI_NULL
};
#endif
