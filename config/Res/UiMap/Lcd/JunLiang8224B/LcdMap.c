#include "LcdMap.h"


const uint16 fp_char_map[]=
{
/*		A,			  B,			    C,		 D,			   E,		          F,			   G,		         H,			   I,			    J,			  K,			L,			M  */
	  L2I(2,3),   L2I(2,5),   L2I(1,5),   L2I(3,3),   L2I(1,1),   L2I(2,1),   L2I(1,2),   L2I(2,2),   L2I(1,3),   L2I(2,4),   L2I(1,4),   L2I(3,4),   L2I(3,2)
	 ,L2I(2,8),   L2I(2,10),  L2I(1,10),  L2I(3,8),   L2I(1,6),   L2I(2,6),   L2I(1,7),   L2I(2,7),   L2I(1,8),   L2I(2,9),   L2I(1,9),   L2I(3,9),   L2I(3,7)
	 ,L2I(2,13),  L2I(2,15),  L2I(1,15),  L2I(3,13),  L2I(1,11),  L2I(2,11),  L2I(1,12),  L2I(2,12),  L2I(1,13),  L2I(2,14),  L2I(1,14),  L2I(3,14),  L2I(3,12)
	 ,L2I(2,18),  L2I(2,20),  L2I(1,20),  L2I(3,18),  L2I(1,16),  L2I(2,16),  L2I(1,17),  L2I(2,17),  L2I(1,18),  L2I(2,19),  L2I(1,19),  L2I(3,19),  L2I(3,17)
	 ,L2I(2,23),  L2I(2,25),  L2I(1,25),  L2I(3,23),  L2I(1,21),  L2I(2,21),  L2I(1,22),  L2I(2,22),  L2I(1,23),  L2I(2,24),  L2I(1,24),  L2I(3,24),  L2I(3,22)
	 ,L2I(2,28),  L2I(2,30),  L2I(1,30),  L2I(3,28),  L2I(1,26),  L2I(2,26),  L2I(1,27),  L2I(2,27),  L2I(1,28),  L2I(2,29),  L2I(1,29),  L2I(3,29),  L2I(3,27)
	 ,L2I(2,33),  L2I(2,35),  L2I(1,35),  L2I(3,33),  L2I(1,31),  L2I(2,31),  L2I(1,32),  L2I(2,32),  L2I(1,33),  L2I(2,34),  L2I(1,34),  L2I(3,34),  L2I(3,32)
	 ,L2I(2,38),  L2I(2,40),  L2I(1,40),  L2I(3,38),  L2I(1,36),  L2I(2,36),  L2I(1,37),  L2I(2,37),  L2I(1,38),  L2I(2,39),  L2I(1,39),  L2I(3,39),  L2I(3,37)
	
	 ,L2I(3,6),   L2I(3,11),  L2I(3,21),  L2I(3,35),  L2I(3,36),  L2I(3,40),  L2I(3,16),  LI_NULL,    LI_NULL,	  LI_NULL,	  LI_NULL,	 LI_NULL,	 LI_NULL
};



