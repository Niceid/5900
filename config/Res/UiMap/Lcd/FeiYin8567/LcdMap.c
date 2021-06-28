#include "LcdMap.h"

#if defined(_ENABLE_REALTIME_DIAPLAY_)
const uint16 fp_icon_map[]=
{
/*		A,			B,			C,			D,			     E,			F,			    G */
	 L2I(7,1),   L2I(5,1),	L2I(2,1),	  L2I(1,1),	   L2I(3,1),	  L2I(6,1),	    L2I(4,1)
	,L2I(7,2),	 L2I(5,2),  L2I(2,2),	  L2I(1,2),	   L2I(3,2), 	  L2I(6,2),	    L2I(4,2)
	,L2I(7,3),	 L2I(5,3),  L2I(2,3),	  L2I(1,3),	   L2I(3,3), 	  L2I(6,3),	    L2I(4,3)
	,L2I(7,4),	 L2I(5,4),  L2I(2,4),	  L2I(1,4),	   L2I(3,4), 	  L2I(6,4),	    L2I(4,4)
};
#endif

const uint16 fp_char_map[]=
{
/* 	        A,                B,                 C,                 D,                 E,                   F,                 G,                 H,                  I,                 J,                 K,                  L,               M  ,                 N*/
	 L2I(7,6),   L2I(4,6),   L2I(1,6),   L2I(1,5),   L2I(4,5),   L2I(7,5),   L2I(5,5),   L2I(6,6),   L2I(6,5),   L2I(5,6),   L2I(2,5),   L2I(2,6),   L2I(3,5),    L2I(3,6)
	,L2I(7,8),   L2I(4,8),   L2I(1,8),   L2I(1,7),   L2I(4,7),   L2I(7,7),   L2I(5,7),   L2I(6,8),   L2I(6,7),   L2I(5,8),   L2I(2,7),   L2I(2,8),   L2I(3,7),    L2I(3,8)
	,L2I(7,10),  L2I(4,10),  L2I(1,10),  L2I(1,9),   L2I(4,9),   L2I(7,9),   L2I(5,9),   L2I(6,10),  L2I(6,9),   L2I(5,10),  L2I(2,9),   L2I(2,10),  L2I(3,9),    L2I(3,10)
	,L2I(7,12),  L2I(4,12),  L2I(1,12),  L2I(1,11),  L2I(4,11),  L2I(7,11),  L2I(5,11),  L2I(6,12),  L2I(6,11),  L2I(5,12),  L2I(2,11),  L2I(2,12),  L2I(3,11),   L2I(3,12)

	,L2I(7,14),  L2I(4,14),  L2I(1,14),  L2I(1,13),  L2I(4,13),  L2I(7,13),  L2I(5,13),  L2I(6,14),  L2I(6,13),  L2I(5,14),  L2I(2,13),  L2I(2,14),  L2I(3,13),   L2I(3,14)
	,L2I(7,16),  L2I(4,16),  L2I(1,16),  L2I(1,15),  L2I(4,15),  L2I(7,15),  L2I(5,15),  L2I(6,16),  L2I(6,15),  L2I(5,16),  L2I(2,15),  L2I(2,16),  L2I(3,15),   L2I(3,16)
	,L2I(7,18),  L2I(4,18),  L2I(1,18),  L2I(1,17),  L2I(4,17),  L2I(7,17),  L2I(5,17),  L2I(6,18),  L2I(6,17),  L2I(5,18),  L2I(2,17),  L2I(2,18),  L2I(3,17),   L2I(3,18)
	,L2I(7,20),  L2I(4,20),  L2I(1,20),  L2I(1,19),  L2I(4,19),  L2I(7,19),  L2I(5,19),  L2I(6,20),  L2I(6,19),  L2I(5,20),  L2I(2,19),  L2I(2,20),  L2I(3,19),   L2I(3,20)

	,L2I(7,22),  L2I(4,22),  L2I(1,22),  L2I(1,21),  L2I(4,21),  L2I(7,21),  L2I(5,21),  L2I(6,22),  L2I(6,21),  L2I(5,22),  L2I(2,21),  L2I(2,22),  L2I(3,21),   L2I(3,22)
	,L2I(7,24),  L2I(4,24),  L2I(1,24),  L2I(1,23),  L2I(4,23),  L2I(7,23),  L2I(5,23),  L2I(6,24),  L2I(6,23),  L2I(5,24),  L2I(2,23),  L2I(2,24),  L2I(3,23),   L2I(3,24)

    ,L2I(7,26),  L2I(4,26),  L2I(1,26),  L2I(1,25),  L2I(4,25),  L2I(7,25),  L2I(5,25),  L2I(6,26),  L2I(6,25),  L2I(5,26),  L2I(2,25),  L2I(2,26),  L2I(3,25),   L2I(3,26)
	,L2I(7,28),  L2I(4,28),  L2I(1,28),  L2I(1,27),  L2I(4,27),  L2I(7,27),  L2I(5,27),  L2I(6,28),  L2I(6,27),  L2I(5,28),  L2I(2,27),  L2I(2,28),  L2I(3,27),   L2I(3,28)
	,L2I(7,30),  L2I(4,30),  L2I(1,30),  L2I(1,29),  L2I(4,29),  L2I(7,29),  L2I(5,29),  L2I(6,30),  L2I(6,29),  L2I(5,30),  L2I(2,29),  L2I(2,30),  L2I(3,29),   L2I(3,30)

	,L2I(8,31),  L2I(8,32),  L2I(6,32),  L2I(7,31),  L2I(6,31),  L2I(5,31),  L2I(5,32),  LI_NULL,    LI_NULL,    LI_NULL,    LI_NULL,    LI_NULL,    LI_NULL,   LI_NULL
};

