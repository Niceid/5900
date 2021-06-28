#include "LcdMap.h"


const uint16 fp_char_map[]=
{
	/*           A,          	             B,          		                C,          		        D,          		            E,          		           F,          		           G,          	      H,          		      I,          		          J,          		          K,                              L,          		            M       */
      L2I(COM_1,SEG_4),   L2I(COM_1,SEG_5),   L2I(COM_3,SEG_5),   L2I(COM_4,SEG_4),   L2I(COM_3,SEG_3),   L2I(COM_1,SEG_3),   L2I(COM_3,SEG_4),   LI_NULL,  L2I(COM_2,SEG_3),   L2I(COM_4,SEG_5),   L2I(COM_2,SEG_4),    L2I(COM_4,SEG_3),   L2I(COM_2,SEG_5)
	 ,L2I(COM_1,SEG_7),   L2I(COM_1,SEG_8),   L2I(COM_3,SEG_8),   L2I(COM_4,SEG_7),   L2I(COM_3,SEG_6),   L2I(COM_1,SEG_6),   L2I(COM_3,SEG_7),   LI_NULL,  L2I(COM_2,SEG_6),   L2I(COM_4,SEG_8),   L2I(COM_2,SEG_7),    L2I(COM_4,SEG_6),   L2I(COM_2,SEG_8)
	 ,L2I(COM_1,SEG_10),  L2I(COM_1,SEG_11),  L2I(COM_3,SEG_11),  L2I(COM_4,SEG_10),  L2I(COM_3,SEG_9),   L2I(COM_1,SEG_9),   L2I(COM_3,SEG_10),  LI_NULL,  L2I(COM_2,SEG_9),   L2I(COM_4,SEG_11),  L2I(COM_2,SEG_10),   L2I(COM_4,SEG_9),   L2I(COM_2,SEG_11)
	 ,L2I(COM_1,SEG_13),  L2I(COM_1,SEG_14),  L2I(COM_3,SEG_14),  L2I(COM_4,SEG_13),  L2I(COM_3,SEG_12),  L2I(COM_1,SEG_12),  L2I(COM_3,SEG_13),  LI_NULL,  L2I(COM_2,SEG_12),  L2I(COM_4,SEG_14),  L2I(COM_2,SEG_13),   L2I(COM_4,SEG_12),  L2I(COM_2,SEG_14)

     ,L2I(COM_1,SEG_18),  L2I(COM_1,SEG_19),  L2I(COM_3,SEG_19),  L2I(COM_4,SEG_18),  L2I(COM_3,SEG_17),  L2I(COM_1,SEG_17),  L2I(COM_3,SEG_18),  LI_NULL,  L2I(COM_2,SEG_17),  L2I(COM_4,SEG_19),  L2I(COM_2,SEG_18),   L2I(COM_4,SEG_17),  L2I(COM_2,SEG_19)
	 ,L2I(COM_1,SEG_21),  L2I(COM_1,SEG_22),  L2I(COM_3,SEG_22),  L2I(COM_4,SEG_21),  L2I(COM_3,SEG_20),  L2I(COM_1,SEG_20),  L2I(COM_3,SEG_21),  LI_NULL,  L2I(COM_2,SEG_20),  L2I(COM_4,SEG_22),  L2I(COM_2,SEG_21),   L2I(COM_4,SEG_20),  L2I(COM_2,SEG_22)
	 ,L2I(COM_1,SEG_25),  L2I(COM_1,SEG_26),  L2I(COM_3,SEG_26),  L2I(COM_4,SEG_25),  L2I(COM_3,SEG_24),  L2I(COM_1,SEG_24),  L2I(COM_3,SEG_25),  LI_NULL,  L2I(COM_2,SEG_24),  L2I(COM_4,SEG_26),  L2I(COM_2,SEG_25),   L2I(COM_4,SEG_24),  L2I(COM_2,SEG_26)	 
	 ,L2I(COM_1,SEG_28),  L2I(COM_1,SEG_29),  L2I(COM_3,SEG_29),  L2I(COM_4,SEG_28),  L2I(COM_3,SEG_27),  L2I(COM_1,SEG_27),  L2I(COM_3,SEG_28),  LI_NULL,  L2I(COM_2,SEG_27),  L2I(COM_4,SEG_29),  L2I(COM_2,SEG_28),   L2I(COM_4,SEG_27),  L2I(COM_2,SEG_29)

	 ,L2I(COM_1,SEG_31),  L2I(COM_2,SEG_31),  L2I(COM_3,SEG_31),  L2I(COM_4,SEG_30),  L2I(COM_3,SEG_30),  L2I(COM_1,SEG_30),  L2I(COM_2,SEG_30),  LI_NULL,    	  	  LI_NULL,    	  	  LI_NULL,    	  	  LI_NULL,    LI_NULL,    	  	  LI_NULL
};

