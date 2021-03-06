#include "LcdMap.h"
//enter here

/*
  ----A----	     ----0----		
|\    |   / |	|\   |   / |		
F I   J  K  B	1 3  6  4  2		
|  \  | /   |	|  \ | /   |		
-G-  -|--  H-	 -5- | -7-		
|  /  | \   |	|  / | \   |		
E M   J  L  C	A 8  6  9  B		
|/    |   \ |	|/   |   \ |		
  ----D----	      ----C----
*/

#ifdef	TD_ADD_USER_LCD

const uint16 fp_char_map[]=
{
	/*1A*/124,	/*1B*/112,	/*1C*/114,	/*1D*/127,	/*1E*/122,	/*1F*/120,	/*1G*/126,	/*1H*/121,	/*1I*/125,	/*1J*/113,	/*1K*/126,	/*1L*/115,	/*1M*/125,	/*1N*/123,
	/*2A*/104,	/*2B*/108,	/*2C*/110,	/*2D*/107,	/*2E*/118,	/*2F*/116,	/*2G*/106,	/*2H*/117,	/*2I*/105,	/*2J*/109,	/*2K*/106,	/*2L*/111,	/*2M*/105,	/*2N*/119,
	/*3A*/100,	/*3B*/88,	/*3C*/90,	/*3D*/103,	/*3E*/98,	/*3F*/96,	/*3G*/102,	/*3H*/97,	/*3I*/101,	/*3J*/89,	/*3K*/102,	/*3L*/91,	/*3M*/101,	/*3N*/99,
	/*4A*/80,	/*4B*/84,	/*4C*/86,	/*4D*/83,	/*4E*/94,	/*4F*/92,	/*4G*/82,	/*4H*/93,	/*4I*/81,	/*4J*/85,	/*4K*/82,	/*4L*/87,	/*4M*/81,	/*4N*/95,
	/*5A*/76,	/*5B*/64,	/*5C*/66,	/*5D*/79,	/*5E*/74,	/*5F*/72,	/*5G*/78,	/*5H*/73,	/*5I*/77,	/*5J*/65,	/*5K*/78,	/*5L*/67,	/*5M*/77,	/*5N*/75,
	/*6A*/56,	/*6B*/60,	/*6C*/62,	/*6D*/59,	/*6E*/70,	/*6F*/68,	/*6G*/58,	/*6H*/69,	/*6I*/57,	/*6J*/61,	/*6K*/58,	/*6L*/63,	/*6M*/57,	/*6N*/71,
	/*7A*/52,	/*7B*/40,	/*7C*/42,	/*7D*/55,	/*7E*/50,	/*7F*/48,	/*7G*/54,	/*7H*/49,	/*7I*/53,	/*7J*/41,	/*7K*/54,	/*7L*/43,	/*7M*/53,	/*7N*/51,
	/*8A*/32,	/*8B*/36,	/*8C*/38,	/*8D*/35,	/*8E*/46,	/*8F*/44,	/*8G*/34,	/*8H*/45,	/*8I*/33,	/*8J*/37,	/*8K*/34,	/*8L*/39,	/*8M*/33,	/*8N*/47,
	/*9A*/24,	/*9B*/28,	/*9C*/25,	/*9D*/27,	/*9E*/30,	/*9F*/29,	/*9G*/26,	/*9H*/156,	/*9I*/156,	/*9J*/156,	/*9K*/156,	/*9L*/156,	/*9M*/156,	/*9N*/156
	
};

#else
const uint16 fp_char_map[]=
{
/*		A,		     B,		   C, 		D, 			 E,		     F,			G,		H,			I,			J,		    K,			 L, 		M  */
	 L2I(4,8),	L2I(3,10), L2I(1,10), L2I(1,9),	 L2I(1,7),  L2I(3,7),  L2I(2,7),  L2I(2,10),  L2I(3,8),	 L2I(2,8),  L2I(3,9),	 L2I(2,9),	L2I(1,8)
	,L2I(4,12), L2I(3,14), L2I(1,14), L2I(1,13), L2I(1,11), L2I(3,11), L2I(2,11), L2I(2,14),  L2I(3,12), L2I(2,12), L2I(3,13),	 L2I(2,13), L2I(1,12)
	,L2I(4,16), L2I(3,18), L2I(1,18), L2I(1,17), L2I(1,15), L2I(3,15), L2I(2,15), L2I(2,18),  L2I(3,16), L2I(2,16), L2I(3,17),	 L2I(2,17), L2I(1,16)
	,L2I(4,20), L2I(3,22), L2I(1,22), L2I(1,21), L2I(1,19), L2I(3,19), L2I(2,19), L2I(2,22),  L2I(3,20), L2I(2,20), L2I(3,21),	 L2I(2,21), L2I(1,20)	
	,L2I(4,24), L2I(3,26), L2I(1,26), L2I(1,25), L2I(1,23), L2I(3,23), L2I(2,23), L2I(2,26),  L2I(3,24), L2I(2,24), L2I(3,25),	 L2I(2,25), L2I(1,24)
	,L2I(4,28), L2I(3,30), L2I(1,30), L2I(1,29), L2I(1,27), L2I(3,27), L2I(2,27), L2I(2,30),  L2I(3,28), L2I(2,28), L2I(3,29),	 L2I(2,29), L2I(1,28)
	,L2I(4,32), L2I(3,34), L2I(1,34), L2I(1,33), L2I(1,31), L2I(3,31), L2I(2,31), L2I(2,34),  L2I(3,32), L2I(2,32), L2I(3,33),	 L2I(2,33), L2I(1,32)

	,L2I(3,35), LI_NULL,   L2I(4,35), L2I(4,34), LI_NULL,   L2I(2,35), L2I(1,35), L2I(1,35),    LI_NULL,    LI_NULL,   LI_NULL,		 LI_NULL, 	LI_NULL
	
	,L2I(4,22), L2I(4,10), L2I(4,14), L2I(4,17), L2I(4,18), L2I(4,19), L2I(4,13), L2I(4,13),	LI_NULL,	LI_NULL,   LI_NULL,  	 LI_NULL,   LI_NULL
};
#endif

