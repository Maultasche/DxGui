/*------------------------------------------------------------------------
File Name: DxGuiFramework.h
Description: This file is the principal include file, which includes
   everything needed in the framework. To use the framework, simply
   include this file and link the library.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

//#pragma warning(disable:4786)

#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <fstream>
#include <map>

#include "DGDebugLog.h"
#include "DGSimpleClasses.h"
#include "DxGuiGlobals.h"
#include "DGException.h"
#include "DGLinkedList.h"
#include "DGQueue.h"
#include "DGDynamicArray.h"
#include "DGColor.h"
#include "DGDisplayModeList.h"
#include "DGBitmap.h"
#include "DGBitmapList.h"
#include "DGFont.h"
#include "DGGraphics.h"
#include "DGSurface.h"
#include "DGWindowSurface.h"
#include "DGInput.h"
#include "DGMessage.h"
#include "DGMessageLog.h"
#include "DGWindow.h"
#include "DGMainWindow.h"
#include "DGGUI.h"
#include "DGApplication.h"

#include "DGLabel.h"
#include "DGImage.h"
#include "DGButton.h"
#include "DGEdit.h"
#include "DGTitleBar.h"
#include "DGResize.h"
#include "DGScrollBar.h"

namespace DG
{
   //Global objects in the framework are declared here, 
   extern DG::Graphics* dgGraphics;
   extern DG::Input* dgInput;

   //Global Functions
   extern DG::Application* GetApp(void);
   extern DG::Graphics* GetGraphics(void);
   extern DG::Gui* GetGui(void);
}
