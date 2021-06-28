#include "LcdMap.h"


const uint16 fp_char_map[]=
{
/*      A,                    B,                  C,                   D,                   E,                   F,                   G,                 H,                I,                    J,                   K,                 L,                 M                    */
	  L2I2(1,5),   L2I2(1,6),	L2I2(3,6),	 L2I2(4,5),   L2I2(3,4),   L2I2(1,4),	L2I2(3,5),	 L2I2(2,4),   L2I2(2,5),   L2I2(2,6),	L2I2(3,5),	 L2I2(4,6),   L2I2(4,4)
	 ,L2I2(1,8),   L2I2(1,9),	L2I2(3,9),	 L2I2(4,8),   L2I2(3,7),   L2I2(1,7),	L2I2(3,8),	 L2I2(2,7),   L2I2(2,8),   L2I2(2,9),	L2I2(3,8),	 L2I2(4,9),   L2I2(4,7)
	 ,L2I2(1,11),  L2I2(1,12),	L2I2(3,12),  L2I2(4,11),  L2I2(3,10),  L2I2(1,10),	L2I2(3,11),  L2I2(2,10),  L2I2(2,11),  L2I2(2,12),	L2I2(3,11),  L2I2(4,12),  L2I2(4,10)
	 ,L2I2(1,14),  L2I2(1,15),	L2I2(3,15),  L2I2(4,14),  L2I2(3,13),  L2I2(1,13),	L2I2(3,14),  L2I2(2,13),  L2I2(2,14),  L2I2(2,15),	L2I2(3,14),  L2I2(4,15),  L2I2(4,13)
	 ,L2I2(1,19),  L2I2(1,20),	L2I2(3,20),  L2I2(4,19),  L2I2(3,18),  L2I2(1,18),	L2I2(3,19),  L2I2(2,18),  L2I2(2,19),  L2I2(2,20),	L2I2(3,19),  L2I2(4,20),  L2I2(4,18)
	 ,L2I2(1,22),  L2I2(1,23),	L2I2(3,23),  L2I2(4,22),  L2I2(3,21),  L2I2(1,21),	L2I2(3,22),  L2I2(2,21),  L2I2(2,22),  L2I2(2,23),	L2I2(3,22),  L2I2(4,23),  L2I2(2,21)
	 ,L2I2(1,25),  L2I2(1,26),	L2I2(3,26),  L2I2(4,25),  L2I2(3,24),  L2I2(1,24),	L2I2(3,25),  L2I2(2,24),  L2I2(2,25),  L2I2(2,26),	L2I2(3,25),  L2I2(4,26),  L2I2(2,24)
	 ,L2I2(1,28),  L2I2(1,29),	L2I2(3,29),  L2I2(4,28),  L2I2(3,27),  L2I2(1,27),	L2I2(3,28),  L2I2(2,27),  L2I2(2,28),  L2I2(2,29),	L2I2(3,28),  L2I2(4,29),  L2I2(2,27)
	 
	 ,L2I2(1,31),  L2I2(2,31),	L2I2(3,31),  L2I2(4,30),  L2I2(3,30),  L2I2(1,30),	L2I2(2,30),  LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL,	 LI_NULL
};


