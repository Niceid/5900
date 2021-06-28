#ifndef __KEY_DRV_VR_H__
#define __KEY_DRV_VR_H__

#if defined(_SUPPORT_VR_)

typedef struct{
	uint8	ad_val;
	uint8 	Vrs;
	int8 	Value;
#if defined(_TY2200A_SPECIAL_VR_)
	uint8	vr;
#endif
}key_vr_t;

extern void key_vr_init(void);
extern void key_vr_value(void);

#endif

#if defined(_SUPPORT_VR2_)
typedef struct{
	uint8	ad_val;
	uint8 	Vrs;
	int8 	Value;
}key_vr2_t;

extern void key_vr2_init(void);
extern void key_vr2_value(void);

#endif

#endif /* __KEY_DRV_VR_H__ */

