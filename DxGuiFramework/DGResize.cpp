/*------------------------------------------------------------------------
File Name: DGResize.cpp
Description: This file contains the implementation of the DG::Resize  
   class, which is a control that allows the user to resize a window.
Version:
   1.0.0    23.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

Resize::Resize(UINT winID, Window* parentWin) :
   Window(winID, true, 0, 0, RESIZE_CONTROL_SIZE_X,
      RESIZE_CONTROL_SIZE_Y),
   resizing(false)
{
   SetWindowType(WT_RESIZE);
   //SetParent(parentWin);

   //If the resize bitmap is not loaded, then load it
   Bitmap* bitmap = dgGraphics->GetBitmap(IDB_RESIZE);
   if(bitmap == NULL)
   {
      dgGraphics->LoadBitmap(IDB_RESIZE, 1, "Bitmaps/ResizeControl.bmp");
      bitmap = dgGraphics->GetBitmap(IDB_RESIZE);
      bitmap->SetTransparentColor(Color(255, 255, 255));
   }
}

void FC Resize::OnCreate()
{
   Point parentSize = GetParent()->GetSize();
   Point size = GetSize();

   SetPosition(parentSize.x - size.x, parentSize.y - size.y); 
}

void FC Resize::OnDrawWindow(WindowSurface* surface)
{
   surface->DrawBitmap(Point(0, 0), IDB_RESIZE);
   //surface->DrawTransparentBitmap(Point(0, 0), IDB_RESIZE,
   //   Color(255, 255, 255));
}

void FC Resize::OnLButtonDown(int x, int y, BYTE* keyboardState)
{
   //We need to keep track of the mouse movement in absolute coords
   //Relative coords will always be the same when it is moving
   previousPosition = RelToAbsCoords(x, y);
   GetGui()->SetMouseCapture(GetWindowID());

   resizing = true;
}

void FC Resize::OnLButtonUp(int x, int y, BYTE* keyboardState)
{
   GetGui()->ReleaseMouseCapture();

   resizing = false;
}

void FC Resize::OnMouseMove(int x, int y, BYTE* keyboardState)
{
   if(resizing)
   {
      //Translate the current position of the mouse to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      Point mouseMovement(absoluteCoords.x - previousPosition.x, 
         absoluteCoords.y - previousPosition.y);

      previousPosition = absoluteCoords;

      //Change the size of the parent relative to how the mouse
      //cursor moved
      Point parentWindowSize = GetParent()->GetSize();
      Point windowSize = GetSize();
      parentWindowSize.Offset(mouseMovement.x, mouseMovement.y);

      if(parentWindowSize.x < windowSize.x)
         parentWindowSize.x = windowSize.x;
      if(parentWindowSize.y < windowSize.y)
         parentWindowSize.y = windowSize.y;

      GetParent()->SetSize(parentWindowSize);
   }
}

