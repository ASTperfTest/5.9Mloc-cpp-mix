#ifndef M_MATHMODULE_H_
#define M_MATHMODULE_H_
//---------------------------------------------------


/*
           **************************
           *     2                  *
        ** *    x  + 2 Pi
          **  *************  +  R
           *     Sin(wt)
           *

                   Math-Module
                 **************

      Miranda Plugin by Stephan Kassemeyer


   MathModule API  -  (c) Stephan Kassemeyer
                  8 May, 2004

*/


// ---------

/*
   Miranda Service-functions defined by MathModule
   call with the
   int (*CallService)(const char * servicename,WPARAM,LPARAM)
   that you get from miranda when Miranda calls the
   Load(PLUGINLINK * link)
   of your PlugIn-dll
   the CallService function then is:
   link->CallServiceSync(Servicename,wparam,lparam)
*/

// ---------

#define MATH_RTF_REPLACE_FORMULAE "Math/RtfReplaceFormulae"
// replace all formulas in a RichEdit with bitmaps.
// wParam = 0
// lParam = *TMathRichedit Info
// return: TRUE if replacement succeeded, FALSE if not (disable by user?).
typedef struct
{
    HWND hwndRichEditControl;  // handle of richedit.
    CHARRANGE* sel;    // NULL: replace all.
    int disableredraw;
}TMathRicheditInfo;
// WARNING:   !!!
// Strange things happen if you use this function twice on the same CHARRANGE:
// if Math-startDelimiter == Math-endDelimiter, there is the following problem:
// it might be that someone forgot an endDelimiter, this results in a lonesome startdelimiter.
// if you try to MATH_REPLACE_FORMULAE the second time, startDelimiters and endDelimiters are mixed up.
// The same problem occours if we have empty formulae, because two succeding delimiters are
// replaced with a single delimiter.


#define MATH_GET_STARTDELIMITER "Math/GetStartDelimiter"
// returns the delimiter that marks the beginning of a formula
// wparam=0
// lparam=0
// result=*char Delimiter
// !!! the result-buffer must be deleted with MTH_FREE_MATH_BUFFER

#define MATH_GETENDDELIMITER "Math/GetEndDelimiter"
// returns the delimiter that marks the end of a formula
// wparam=0
// lparam=0
// result=*char Delimiter
// !!! the result-buffer must be deleted with MTH_FREE_MATH_BUFFER

#define MTH_FREE_MATH_BUFFER "Math/FreeRTFBitmapText"
// deletes any buffer that MathModule has created.
// wparam=0
// lparam=(*char) buffer
// result=0

#define MATH_SETBKGCOLOR "Math/SetBackGroundColor"
// changes the background color of the next formula to be rendered.
// wparam=0
// lparam=(COLORREF) color
// result=0

#define MATH_SET_PARAMS "Math/SetParams"
// sets a parameter (only integer values) encoded in  wparam
// wparam=paramcode
// lparam=parametervalue
// paramcodes:
#define MATH_PARAM_BKGCOLOR 0    // (COLORREF) std-rgb-color or TRANSPARENT_Color
#define MATH_PARAM_FONTCOLOR 1   // (COLORREF) std-rgb-color
#define RESIZE_HWND 2            // (HWND) preview window resizes RESIZE_HWND when it is being resized.
#define ToolboxEdit_HWND 3       // (HWND) If this hwnd (of an edit-box) is set, MathModule can insert Formula-elements from the Math-Toolbox.
// you can make the BKGCOLOR Transparent (default) by using this color:
#define TRANSPARENT_Color 0xffffffff -1  // this is default

#define MTH_GETBITMAP "Math/GetBitmap"
//returns Bitmap that represents the formula given in lparam (string-pointer)
//this formula has NO Delimiters.
//wparam=0
//lparam=(*char)Formula
//result=(HBITMAP) bitmap
//!!! the bitmap must be deleted with DeleteObject(hobject)

//example:
//HBITMAP Bmp=(HBITMAP)CallService(MTH_GETBITMAP,0, (LPARAM)formula);

#define MTH_GET_RTF_BITMAPTEXT "Math/GetRTFBitmapText"
// returns rich-text stream that includes bitmaps from text given in lparam
// text included between MATH_GET_STARTDELIMITER and MATH_GETENDDELIMITER
// hereby is replaced with a rtf-bitmap-stream that represents the corresponding formula
// wparam=0
// lparam=*char text
// result=*char rtfstream
// !!! the result-buffer must be deleted with MTH_FREE_RTF_BITMAPTEXT

#define MTH_FREE_RTF_BITMAPTEXT "Math/FreeRTFBitmapText"
// deletes the buffer that MTH_GET_RTF_BITMAPTEXT has created.
// wparam=0
// lparam=(*char) buffer
// result=0


// **************************************************************
// The following is still SRMM - specific.
// I plan to modify it, so that other PlugIns can take advantage of e.g. preview-window....

#define MTH_SHOW "Math/Show"
// shows the preview-window
// wparam=0
// lparam=0
// result=0

#define MTH_HIDE "Math/Hide"
// hides the preview-window
// wparam=0
// lparam=0
// result=0

#define MTH_RESIZE "Math/Resize"
// sets the size of the preview-window
// wparam=0
// lparam=(*TMathWindowInfo)
// result=0
typedef struct
{
    int top;
	int left;
	int right;
	int bottom;
}	TMathWindowInfo;

#define MTH_SETFORMULA "Math/SetFormula"
// sets the text that the preview-window should parse to display formulas found inside
// wparam=0
// lparam=(*char) text
// result=0

#define MTH_Set_ToolboxEditHwnd "Math/SetTBhwnd"
// If this hwnd (of an edit-box) is set, MathModule can insert Formula-elements from the Math-Toolbox.
// wparam=0
// lparam=handle

#define MTH_Set_Srmm_HWND "Math/SetSrmmHWND" //�bergibt fenster-Handle des aktuellen Message-Dialogs
// If MathModule knows the handle of a SRMM-based window, following features exist:
// - preview window resizes Math-Srmm when it is being resized.
// wparam=0
// lparam=handle
// result=0

#define MTH_GET_PREVIEW_HEIGHT "Math/getPreviewHeight"
// returns the height of the whole preview-window (including system-menu-bar)
// consider this when maximizing a window to that preview-window is hooked on top or bottom
// it returns the height no matter whether preview-window is visible or not
// wparam=0
// lparam=0
// result=(int) height

#define MTH_GET_PREVIEW_SHOWN "Math/getPreviewShown"
// returns 1 if preview window is visible
// returns 0 if preview window is invisible
// result=(int) shown

#define MTH_SUBSTITUTE_DELIMITER "Math/SubstituteDelimiter"
// replaces Substitute given lparam-structure with internal Math-Delimiter
// wparam=0
// lparam=(TMathSubstInfo) substInfo
// result=0
typedef struct
{
	HWND EditHandle;
	char* Substitute;
}	TMathSubstInfo;

//---------------------------------------------------
#endif
//#ifndef M_MATHMODULE_H_

