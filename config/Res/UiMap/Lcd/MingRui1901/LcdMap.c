#include "LcdMap.h"

#if defined(_SUPPORT_TIME_DISPLAY_)
const uint16 fp_icon_map[]=
{
/*		A,			B,			   C,				D,				E,			F,			   G,				H,			I,			J,			K,			L,			  M  */
	 L2I(4,2),    L2I(3,2),	    L2I(2,2),	  L2I(1,2),	    L2I(2,1),	  L2I(4,1),		L2I(3,1),	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
	,L2I(4,4),    L2I(3,4),	    L2I(2,4),	  L2I(1,4),	    L2I(2,3),	  L2I(4,3),	    L2I(3,3),	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
	,L2I(4,6),    L2I(3,6),	    L2I(2,6),	  L2I(1,6),	    L2I(2,5),	  L2I(4,5),	    L2I(3,5),	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
	,L2I(4,8),    L2I(3,8),	    L2I(2,8),	  L2I(1,8),	    L2I(2,7),	  L2I(4,7),	    L2I(3,7),	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL

	,L2I(4,10),   L2I(3,10),	L2I(2,10),	  L2I(1,10),	L2I(2,9),	  L2I(4,9),	    L2I(3,9),	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
	,L2I(4,12),   L2I(3,12),	L2I(2,12),	  L2I(1,12),	L2I(2,11),	  L2I(4,11),	L2I(3,11),	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
    ,L2I(1,17),   L2I(1,13),	L2I(1,13),	  L2I(1,17),	L2I(1,17),	  L2I(1,17),	LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
	,L2I(4,14),   L2I(3,14),	L2I(2,14),	  L2I(1,14),	L2I(2,13),	  L2I(4,13),	L2I(3,13),	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL

    //,L2I(1,15),   L2I(1,15),	L2I(1,15),	  L2I(1,15),	L2I(1,15),	  L2I(1,15),	L2I(1,15),	  L2I(1,15),	  L2I(1,15),	  L2I(1,15),	  L2I(1,15),	  L2I(1,15),	  L2I(1,15)
    //,L2I(1,15),   L2I(1,15),	L2I(1,15),	  L2I(1,15),	L2I(1,15),	  L2I(1,15),	L2I(1,15),	  L2I(1,15),	  L2I(1,15),	  L2I(1,15),	  L2I(1,15),	  L2I(1,15),	  L2I(1,15)
	,L2I(4,16),   L2I(3,16),	L2I(2,16),	  L2I(1,16),	L2I(2,15),	  L2I(4,15),	L2I(3,15),	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
	,L2I(4,18),   L2I(3,18),	L2I(2,18),	  L2I(1,18),	L2I(2,17),	  L2I(4,17),	L2I(3,17),	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL

	,L2I(4,20),   L2I(3,20),	L2I(2,20),	  L2I(1,20),	L2I(1,19),	  L2I(4,19),	L2I(2,19),	  L2I(3,19),  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
	,L2I(4,22),   L2I(3,22),	L2I(2,22),	  L2I(1,11),	L2I(1,21),	  L2I(4,21),	L2I(2,21),	  L2I(3,21),  LI_NULL,	  L2I(1,22),  LI_NULL,	  LI_NULL,	  LI_NULL
	,L2I(4,24),   L2I(3,24),	L2I(2,24),	  L2I(1,24),	L2I(1,23),	  L2I(4,23),	L2I(2,23),	  L2I(3,23),  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL,	  LI_NULL
};
#endif

const uint16 fp_char_map[]=
{
/*		A,		        B,			   C,				D,				E,			F,				G,			    H,			I,			J,			K,			L,			  M  */
	 L2I(4,26),   L2I(3,27), 	L2I(2,27),	  L2I(1,26), 	L2I(2,25),	  L2I(3,25), 	L2I(2,26),	  L2I(3,26),   L2I(4,25),	 L2I(1,27),	  LI_NULL,    L2I(1,25),	L2I(4,27)
	,L2I(4,29),	  L2I(3,30),	L2I(2,30),	  L2I(1,29), 	L2I(2,28),	  L2I(3,28), 	L2I(2,29),	  L2I(3,29),   L2I(4,28),	 L2I(1,30),	  LI_NULL,    L2I(1,28),	L2I(4,30)
	 
	,L2I(4,32),   L2I(3,33),	L2I(2,33),	  L2I(1,32),	L2I(2,31),	  L2I(3,31),	L2I(2,32),	  L2I(3,32),   L2I(4,31),	 L2I(1,33),   LI_NULL,    L2I(1,31),	L2I(4,33)

	,L2I(4,35),   L2I(3,36),	L2I(2,36),	  L2I(1,35),	L2I(2,34),	  L2I(3,34),	L2I(2,35),	  L2I(3,35),   L2I(4,34),	 L2I(1,36),	  LI_NULL,    L2I(1,34),	L2I(4,36)
	,L2I(4,38),   L2I(3,39),	L2I(2,39),	  L2I(1,38),	L2I(2,37),	  L2I(3,37),	L2I(2,38),	  L2I(3,38),   L2I(4,37),	 L2I(1,39),	  LI_NULL,    L2I(1,37),	L2I(4,39)

	,L2I(4,41),   L2I(3,42),	L2I(2,42),	  L2I(1,41),	L2I(2,40),	  L2I(3,40),	L2I(2,41),	  L2I(3,41),   L2I(4,40),	 L2I(1,42),	  LI_NULL,    L2I(1,40),	L2I(4,42)
	
	,L2I(4,44),   L2I(3,44),	L2I(2,44),	  L2I(1,44),	L2I(2,43),	  L2I(4,43),	L2I(3,43),	  LI_NULL,      LI_NULL,	 LI_NULL,	  LI_NULL,    LI_NULL,	    LI_NULL
	,L2I(4,46),   L2I(3,46),	L2I(2,46),	  L2I(1,46),	L2I(2,45),	  L2I(4,45),	L2I(3,45),	  LI_NULL,      LI_NULL,	 LI_NULL,	  LI_NULL,    LI_NULL,	    LI_NULL
	,L2I(4,48),   L2I(3,48),	L2I(2,48),	  L2I(1,48),	L2I(2,47),	  L2I(4,47),	L2I(3,47),	  LI_NULL,      LI_NULL,	 LI_NULL,	  LI_NULL,    LI_NULL,	    LI_NULL
};


