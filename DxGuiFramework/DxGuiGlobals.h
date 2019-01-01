/*------------------------------------------------------------------------
File Name: DxGuiGlobals.h
Description: This file contains all the global definitions and macros
   that are needed throughout the DirectX GUI framework.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

//GUI message definitions
#define  GM_NONE                    0        
#define  GM_FOCUS                   1
#define  GM_LOSEFOCUS               2
#define  GM_LBUTTONDOWN             3
#define  GM_LBUTTONUP               4
#define  GM_LBUTTONDBLCLK           5
#define  GM_MBUTTONDOWN             6
#define  GM_MBUTTONUP               7
#define  GM_MBUTTONDBLCLK           8
#define  GM_RBUTTONDOWN             9
#define  GM_RBUTTONUP               10
#define  GM_RBUTTONDBKCLK           11
#define  GM_MOUSEMOVE               12
#define  GM_MOUSEENTER              13
#define  GM_MOUSELEAVE              14
#define  GM_KEYDOWN                 15
#define  GM_KEYUP                   16
#define  GM_KEYPRESSED              16
#define  GM_MOVING                  17
#define  GM_MOVED                   18
#define  GM_REPOSITION              19
#define  GM_SIZING                  20
#define  GM_SIZED                   21
#define  GM_RESIZE                  22
#define  GM_SHOWWINDOW              23
#define  GM_HIDEWINDOW              24
#define  GM_DISPLAYCHANGE           25
#define  GM_CREATE                  26
#define  GM_DESTROY                 27
#define  GM_HIDECHILDWINDOW         28
#define  GM_SHOWCHILDWINDOW         29
#define  GM_CHARACTER               30
#define  GM_PARENTREPOSITION        31
#define  GM_TIMER                   40

//DGLabel messages
#define  GM_LABEL_LBUTTONDOWN       100
#define  GM_LABEL_LBUTTONUP         101
#define  GM_LABEL_LBUTTONDBLCLK     102
#define  GM_LABEL_MBUTTONDOWN       103
#define  GM_LABEL_MBUTTONUP         104
#define  GM_LABEL_MBUTTONDBLCLK     105
#define  GM_LABEL_RBUTTONDOWN       106
#define  GM_LABEL_RBUTTONUP         107
#define  GM_LABEL_RBUTTONDBLCLK     108
#define  GM_LABEL_MOUSEMOVE         109

//DGImage messages
#define  GM_IMAGE_LBUTTONDOWN       110
#define  GM_IMAGE_LBUTTONUP         111
#define  GM_IMAGE_LBUTTONDBLCLK     112
#define  GM_IMAGE_MBUTTONDOWN       113
#define  GM_IMAGE_MBUTTONUP         114
#define  GM_IMAGE_MBUTTONDBLCLK     115
#define  GM_IMAGE_RBUTTONDOWN       116
#define  GM_IMAGE_RBUTTONUP         117
#define  GM_IMAGE_RBUTTONDBLCLK     118
#define  GM_IMAGE_MOUSEMOVE         119

//DGButton messages
#define  GM_BUTTON_DOWN             120
#define  GM_BUTTON_PRESSURE         121
#define  GM_BUTTON_CLICKED          122

//DGEdit messages
#define  GM_EDIT_CHANGE             130
#define  GM_EDIT_ENTER              131

//DGTitleBar messages
#define  GM_TITLEBAR_DOWN           140

//DGResize messages

//DGScrollBar messages
#define  GM_SCROLLBAR_LINEUP        160
#define  GM_SCROLLBAR_LINEDOWN      161
#define  GM_SCROLLBAR_TRACK         162

//WindowType Definitions
#define  WT_GENERICWINDOW     0
#define  WT_MAINWINDOW        1

#define  WT_LABEL             10
#define  WT_BUTTON            11
#define  WT_IMAGE             12
#define  WT_TITLEBAR          13
#define  WT_RESIZE            14
#define  WT_EDIT              15
#define  WT_SCROLLBAR         16

//Window IDs (0-99 are reserved for the framework)
#define  IDW_NONE             0
#define  IDW_DEFAULTMAIN      1

//Timer IDs (0-99 are reserved for the framework)
#define  SB_UP_TIMER          0
#define  SB_DOWN_TIMER        1
         
//Data Type Definitions
namespace DG
{
   class Window;
   class Message;
   class MsgHandlerFunction;

   typedef std::map<UINT, MsgHandlerInterface*> MessageTable;
   typedef MessageTable::value_type MessageEntry;
}

//Buffering Definitions
#define  BT_SINGLE            1
#define  BT_DOUBLE            2
#define  BT_TRIPLE            3

//Windowed State Definitions
#define  WS_WINDOWED          1
#define  WS_FULLSCREEN        2

//Color Depth Definitions
#define  CD_8BIT              1
#define  CD_16BIT             2
#define  CD_24BIT             3
#define  CD_32BIT             4

//Bitmap ID Definitions
#define  IDB_NO_ID            0
#define  IDB_RESIZE           1
#define  IDB_UP_ARROW         2
#define  IDB_DOWN_ARROW       3
#define  IDB_LEFT_ARROW       4
#define  IDB_RIGHT_ARROW      5

//Exception Definitions
//Error Codes

#define  EC_UNKNOWN           0

#define  EC_DDINIT            1
#define  EC_DDSETGRAPHMODE    2
#define  EC_DDLOCKSURFACE     3
#define  EC_DDUNLOCKSURFACE   4
#define  EC_DDFLIPSURFACE     5
#define  EC_DDRESTORESURFACES 6
#define  EC_DDFILLSCREEN      7
#define  EC_DDCLEARPRIMARY    8
#define  EC_DDFILLAREA        9
#define  EC_DDDRAWBMP         10
#define  EC_DDCOLORKEY        11
#define  EC_DDTEXT            12
#define  EC_DDCLIPPING        13

#define  EC_BMBITMAPSIZE      1
#define  EC_BMBITMAPLOAD      2

#define  EC_CREATEFONT        1

//Error Types

#define  ET_UNKNOWN           0
#define  ET_DIRECTDRAW        1
#define  ET_BITMAP            2
#define  ET_FONT              3

//Name Substitution Defines
#define  FC   __fastcall
