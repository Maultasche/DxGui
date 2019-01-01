/*------------------------------------------------------------------------
File Name: DGResize.h
Description: This file contains the DG::Resize class, which is a control 
   that allows the user to resize a window.
Version:
   1.0.0    23.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

#define RESIZE_CONTROL_SIZE_X      16
#define RESIZE_CONTROL_SIZE_Y      16

namespace DG
{
   class Resize : public Window
   {
   public:
      Resize(UINT winID, Window* parentWin);

   protected:
      void FC OnCreate();
      void FC OnDrawWindow(WindowSurface* surface);
      void FC OnLButtonDown(int x, int y, BYTE* keyboardState);
      void FC OnLButtonUp(int x, int y, BYTE* keyboardState);
      void FC OnMouseMove(int x, int y, BYTE* keyboardState);

   private:
      Point previousPosition;
      bool resizing;
   };
}
