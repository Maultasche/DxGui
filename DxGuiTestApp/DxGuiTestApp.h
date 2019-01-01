/*DxGuiTestApp.h*/

#ifndef DXGUITESTAPP_H
#define DXGUITESTAPP_H

#define	IDW_BLUEWINDOW			100
#define  IDW_REDWINDOW			101
#define	IDW_YELLOWWINDOW		102
#define	IDW_GREENWINDOW		103

#define  IDL_TESTLABEL        110
#define  IDB_TESTBUTTON       111
#define  IDC_RED_TITLEBAR     112
#define  IDC_RED_RESIZE       113

#define  IDI_IMAGE            120
#define  IDE_IMAGENAME        121
#define  IDC_GREEN_TITLEBAR   122
#define  IDC_GREEN_RESIZE     123
#define  IDC_LOADBUTTON       124
#define  IDC_SB_VERTICAL      125
#define  IDC_SB_HORIZONTAL    126

#define  IDB_IMAGEBITMAP      100

class DxGuiTestApp : public DG::Application
{
public:
   void InitApp();
};

#endif