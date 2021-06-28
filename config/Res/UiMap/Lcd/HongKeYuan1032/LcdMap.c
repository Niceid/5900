#include "LcdMap.h"


#if defined(_TEMP_WARM_DISPLAY_)
const uint16 fp_icon_map[]=
{
/*            A,                    B,                  C,                   D,                   E,                   F,                   G,                 H,                I,                   J,                 K,                    L,                M     */
	   L2I2(4,16),	L2I2(3,17),  L2I2(2,17),  L2I2(1,16),  L2I2(2,15),	L2I2(4,15),  L2I2(2,16),  LI_NULL,	L2I2(3,15),  L2I2(1,17), L2I2(3,16),  L2I2(1,15),  L2I2(4,17)
	  ,L2I2(4,19),	L2I2(3,20),  L2I2(2,20),  L2I2(1,19),  L2I2(2,18),	L2I2(4,18),  L2I2(2,19),  LI_NULL,	L2I2(3,18),  L2I2(1,20), L2I2(3,19),  L2I2(1,18),  L2I2(4,20)
	  ,L2I2(4,22),	L2I2(3,23),  L2I2(2,23),  L2I2(1,22),  L2I2(2,21),	L2I2(4,21),  L2I2(2,22),  LI_NULL,	L2I2(3,21),  L2I2(1,23), L2I2(3,22),  L2I2(1,21),  L2I2(4,23)
	  ,L2I2(4,27),	L2I2(3,26),  L2I2(2,26),  L2I2(1,27),  L2I2(2,24),	L2I2(4,24),  L2I2(2,27),  LI_NULL,	L2I2(3,24),  L2I2(1,26), L2I2(3,27),  L2I2(1,24),  L2I2(4,26)
};
#endif

#if defined(_HKY1032_SPECIAL_DISPLAY_)
const uint16 fp_char_map[]=
{
/*            A,                    B,                  C,                   D,                   E,                   F,                   G,                 H,                I,                   J,                 K,                    L,                M     */
	  LI_NULL, 	  LI_NULL,	   LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
	 ,LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	LI_NULL,	LI_NULL,	LI_NULL,	LI_NULL,	LI_NULL
	 ,LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	LI_NULL,	LI_NULL,	LI_NULL,	LI_NULL,	LI_NULL
	 ,LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	 LI_NULL,	  LI_NULL,	   LI_NULL, 	LI_NULL,	LI_NULL,	LI_NULL,	LI_NULL,	LI_NULL,	LI_NULL
	 ,L2I2(4,2),   L2I2(3,3),	L2I2(2,3),	 L2I2(1,2),   L2I2(2,1),   L2I2(4,1),	L2I2(2,2),	 LI_NULL,  L2I2(3,1),   L2I2(1,3),	L2I2(3,2),	 L2I2(1,1),   L2I2(4,3)
	 ,L2I2(4,5),   L2I2(3,6),	L2I2(2,6),	 L2I2(1,5),   L2I2(2,4),   L2I2(4,4),	L2I2(2,5),	 LI_NULL,  L2I2(3,4),   L2I2(1,6),	L2I2(3,5),	 L2I2(1,4),   L2I2(4,6)

     ,L2I2(4,9),   L2I2(3,10),	L2I2(2,10),  L2I2(1,9),   L2I2(2,8),   L2I2(4,8),	L2I2(2,9),   LI_NULL,  L2I2(3,8),   L2I2(1,10),	L2I2(3,9),   L2I2(1,8),   L2I2(4,10)
	 ,L2I2(4,13),  L2I2(3,14),	L2I2(2,14),  L2I2(1,13),  L2I2(2,12),  L2I2(4,12),	L2I2(2,13),  LI_NULL,  L2I2(3,12),  L2I2(1,14),	L2I2(3,13),  L2I2(1,12),  L2I2(4,14)

	 ,LI_NULL,     LI_NULL,	    LI_NULL,     LI_NULL,     LI_NULL,     LI_NULL,	    LI_NULL,     LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL
};

#else
const uint16 fp_char_map[]=
{
/*            A,                    B,                  C,                   D,                   E,                   F,                   G,                 H,                I,                   J,                 K,                    L,                M     */
	  L2I2(4,2),   L2I2(3,3),	L2I2(2,3),	 L2I2(1,2),   L2I2(2,1),   L2I2(4,1),	L2I2(2,2),	 LI_NULL,  L2I2(3,1),   L2I2(1,3),	L2I2(3,2),	 L2I2(1,1),   L2I2(4,3)
	 ,L2I2(4,5),   L2I2(3,6),	L2I2(2,6),	 L2I2(1,5),   L2I2(2,4),   L2I2(4,4),	L2I2(2,5),	 LI_NULL,  L2I2(3,4),   L2I2(1,6),	L2I2(3,5),	 L2I2(1,4),   L2I2(4,6)

     ,L2I2(4,9),   L2I2(3,10),	L2I2(2,10),  L2I2(1,9),   L2I2(2,8),   L2I2(4,8),	L2I2(2,9),   LI_NULL,  L2I2(3,8),   L2I2(1,10),	L2I2(3,9),   L2I2(1,8),   L2I2(4,10)
	 ,L2I2(4,13),  L2I2(3,14),	L2I2(2,14),  L2I2(1,13),  L2I2(2,12),  L2I2(4,12),	L2I2(2,13),  LI_NULL,  L2I2(3,12),  L2I2(1,14),	L2I2(3,13),  L2I2(1,12),  L2I2(4,14)

     ,L2I2(4,16),  L2I2(3,17),	L2I2(2,17),  L2I2(1,16),  L2I2(2,15),  L2I2(4,15),	L2I2(2,16),  LI_NULL,  L2I2(3,15),  L2I2(1,17),	L2I2(3,16),  L2I2(1,15),  L2I2(4,17)
	 ,L2I2(4,19),  L2I2(3,20),	L2I2(2,20),  L2I2(1,19),  L2I2(2,18),  L2I2(4,18),	L2I2(2,19),  LI_NULL,  L2I2(3,18),  L2I2(1,20),	L2I2(3,19),  L2I2(1,18),  L2I2(4,20)
	 ,L2I2(4,22),  L2I2(3,23),	L2I2(2,23),  L2I2(1,22),  L2I2(2,21),  L2I2(4,21),	L2I2(2,22),  LI_NULL,  L2I2(3,21),  L2I2(1,23),	L2I2(3,22),  L2I2(1,21),  L2I2(4,23)
	 ,L2I2(4,27),  L2I2(3,26),	L2I2(2,26),  L2I2(1,27),  L2I2(2,24),  L2I2(4,24),	L2I2(2,27),  LI_NULL,  L2I2(3,24),  L2I2(1,26),	L2I2(3,27),  L2I2(1,24),  L2I2(4,26)

	 ,LI_NULL,     LI_NULL,	    LI_NULL,     LI_NULL,     LI_NULL,     LI_NULL,	    LI_NULL,     LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL
};
#endif

#if defined(_HKY1032_SPECIAL_LOGO_)
const uint16 fp_char_map1[]=
{
/*            A,                    B,                  C,                   D,                   E,                   F,                   G,                 H,                I,                   J,                 K,                    L,                M     */
	  L2I2(4,2),   L2I2(3,3),	L2I2(2,3),	 L2I2(1,2),   L2I2(2,1),   L2I2(4,1),	L2I2(2,2),	 LI_NULL,  L2I2(3,1),   L2I2(1,3),	L2I2(3,2),	 L2I2(1,1),   L2I2(4,3)
	 ,L2I2(4,5),   L2I2(3,6),	L2I2(2,6),	 L2I2(1,5),   L2I2(2,4),   L2I2(4,4),	L2I2(2,5),	 LI_NULL,  L2I2(3,4),   L2I2(1,6),	L2I2(3,5),	 L2I2(1,4),   L2I2(4,6)

     ,L2I2(4,9),   L2I2(3,10),	L2I2(2,10),  L2I2(1,9),   L2I2(2,8),   L2I2(4,8),	L2I2(2,9),   LI_NULL,  L2I2(3,8),   L2I2(1,10),	L2I2(3,9),   L2I2(1,8),   L2I2(4,10)
	 ,L2I2(4,13),  L2I2(3,14),	L2I2(2,14),  L2I2(1,13),  L2I2(2,12),  L2I2(4,12),	L2I2(2,13),  LI_NULL,  L2I2(3,12),  L2I2(1,14),	L2I2(3,13),  L2I2(1,12),  L2I2(4,14)

     ,L2I2(4,16),  L2I2(3,17),	L2I2(2,17),  L2I2(1,16),  L2I2(2,15),  L2I2(4,15),	L2I2(2,16),  LI_NULL,  L2I2(3,15),  L2I2(1,17),	L2I2(3,16),  L2I2(1,15),  L2I2(4,17)
	 ,L2I2(4,19),  L2I2(3,20),	L2I2(2,20),  L2I2(1,19),  L2I2(2,18),  L2I2(4,18),	L2I2(2,19),  LI_NULL,  L2I2(3,18),  L2I2(1,20),	L2I2(3,19),  L2I2(1,18),  L2I2(4,20)
	 ,L2I2(4,22),  L2I2(3,23),	L2I2(2,23),  L2I2(1,22),  L2I2(2,21),  L2I2(4,21),	L2I2(2,22),  LI_NULL,  L2I2(3,21),  L2I2(1,23),	L2I2(3,22),  L2I2(1,21),  L2I2(4,23)
	 ,L2I2(4,27),  L2I2(3,26),	L2I2(2,26),  L2I2(1,27),  L2I2(2,24),  L2I2(4,24),	L2I2(2,27),  LI_NULL,  L2I2(3,24),  L2I2(1,26),	L2I2(3,27),  L2I2(1,24),  L2I2(4,26)

	 ,LI_NULL,     LI_NULL,	    LI_NULL,     LI_NULL,     LI_NULL,     LI_NULL,	    LI_NULL,     LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL
};
#endif

