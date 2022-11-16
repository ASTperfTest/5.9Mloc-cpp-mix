#include "m_button.h"

#define BUTTONSETID				WM_USER+55
#define BUTTONDRAWINPARENT		WM_USER+56
#define BUTTONSETMARGINS		WM_USER+57
#define BUTTONSETSENDONDOWN     WM_USER+58

#define MBM_UPDATETRANSPARENTFLAG	WM_USER+52
#define MBM_SETICOLIBHANDLE			WM_USER+53
#define MBM_REFRESHICOLIBICON		WM_USER+54

#define SBF_ALIGN_TL_RIGHT   1
#define SBF_ALIGN_TL_HCENTER 2
#define SBF_ALIGN_TL_BOTTOM  4
#define SBF_ALIGN_TL_VCENTER 8

#define SBF_ALIGN_BR_RIGHT   16
#define SBF_ALIGN_BR_HCENTER 32
#define SBF_ALIGN_BR_BOTTOM  64
#define SBF_ALIGN_BR_VCENTER 128

#define SBF_CALL_ON_PRESS    256

// Params for draw flag at Repos function
#define SBRF_DO_NOT_DRAW      0
#define SBRF_DO_REDRAW_ALL    1
#define SBRF_DO_ALT_DRAW      2
#define SBRF_REDRAW           ( SBRF_DO_REDRAW_ALL|SBRF_DO_ALT_DRAW )

#define BUTTONNEEDREDRAW		1256