#include "LcdMap.h"

#if defined(_ENABLE_REALTIME_DIAPLAY_)
const uint16 fp_icon_map[]=
{
/*		  A,			  B,			      C,			D,			     E,			   F,			    G  */
	 L2I2(4,8),    L2I2(4,9),	L2I2(1,9),	  L2I2(1,8),	L2I2(1,7),	  L2I2(4,7),	L2I2(2,8)
	,L2I2(4,11),   L2I2(4,12),	L2I2(1,12),	  L2I2(1,11),	L2I2(1,10),	  L2I2(4,10),	L2I2(2,11)

	,L2I2(4,14),   L2I2(4,15),	L2I2(1,15),	  L2I2(1,14),	L2I2(1,13),	  L2I2(4,13),	L2I2(2,14)
	,L2I2(4,17),   L2I2(4,18),	L2I2(1,18),	  L2I2(1,17),	L2I2(1,16),	  L2I2(4,16),	L2I2(2,17)
};
#endif

const uint16 fp_char_map[]=
{
/*		A,		        B,			     C,		        D,				E,			F,				G,			    H,			I,		    J,			K,			   L,			     M  */
	 L2I2(4,2),	   L2I2(4,3), 	L2I2(1,3),	  L2I2(1,2), 	L2I2(1,1),	  L2I2(4,1), 	L2I2(2,2),	  L2I2(3,1),    L2I2(3,2),	L2I2(3,3),	 L2I2(2,3),    L2I2(2,1), 	LI_NULL
	,L2I2(4,5),	   L2I2(4,6),	L2I2(1,6),	  L2I2(1,5), 	L2I2(1,4),	  L2I2(4,4), 	L2I2(2,5),	  L2I2(3,4),    L2I2(3,5),	L2I2(3,6),	 L2I2(2,6),    L2I2(2,4),   LI_NULL
	 
	,L2I2(4,38),   L2I2(4,37),	L2I2(1,37),	  L2I2(1,38),	L2I2(1,39),	  L2I2(4,39),	L2I2(2,38),	  L2I2(3,39),   L2I2(3,38),	L2I2(3,37),  L2I2(2,37),   L2I2(2,39),	LI_NULL

	,L2I2(4,35),   L2I2(4,34),	L2I2(1,34),	  L2I2(1,35),	L2I2(1,36),	  L2I2(4,36),	L2I2(2,35),	  L2I2(3,36),   L2I2(3,35),	L2I2(3,34),	 L2I2(2,34),   L2I2(2,36),	LI_NULL
	,L2I2(4,32),   L2I2(4,31),	L2I2(1,31),	  L2I2(1,32),	L2I2(1,33),	  L2I2(4,33),	L2I2(2,32),	  L2I2(3,33),   L2I2(3,32),	L2I2(3,31),	 L2I2(2,31),   L2I2(2,33),	LI_NULL
	,L2I2(4,29),   L2I2(4,28),	L2I2(1,28),	  L2I2(1,29),	L2I2(1,30),	  L2I2(4,30),	L2I2(2,29),	  L2I2(3,30),   L2I2(3,29),	L2I2(3,28),	 L2I2(2,28),   L2I2(2,30),	LI_NULL
	,L2I2(4,26),   L2I2(4,25),	L2I2(1,25),	  L2I2(1,26),	L2I2(1,27),	  L2I2(4,27),	L2I2(2,26),	  L2I2(3,27),   L2I2(3,26),	L2I2(3,25),	 L2I2(2,25),   L2I2(2,27),	LI_NULL

	,L2I2(4,23),   L2I2(4,22),	L2I2(1,22),	  L2I2(1,23),	L2I2(1,24),	  L2I2(4,24),	L2I2(2,23),	  L2I2(3,24),   L2I2(3,23),	L2I2(3,22),	 L2I2(2,22),   L2I2(2,24),	LI_NULL
	,L2I2(4,20),   L2I2(3,20),	L2I2(2,20),	  L2I2(1,20),	L2I2(2,21),	  L2I2(4,21),	L2I2(3,21),	  LI_NULL,     LI_NULL,	    LI_NULL,	 LI_NULL,     LI_NULL,	    LI_NULL
};

