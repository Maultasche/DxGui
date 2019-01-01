/*------------------------------------------------------------------------
File Name: DGWindowSurface.h
Description: This file contains the DG::WindowSurface class, which 
	represents the area of a window.
Version:
   1.0.0    12.08.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class WindowSurface
   {
   public:

      /*------------------------------------------------------------------------
      Function Name: Constructor
      Parameters:
         Surface* surface : the area on the screen to which the window is to 
            be clipped
         Point winOrigin: the surface coordinates of the upper-left corner 
            of the window
      Description:
         This function constructs the DG::WindowSurface object. If winOrigin is
         (0, 0), then the upper-left corner of the window is the same as the
         upper-left corner of the screen surface. If winOrigin coordinates are
         negative then, some of the top and left parts of the window will be
         clipped.
      ------------------------------------------------------------------------*/

      WindowSurface(Surface* surface, Point winOrigin)
      {
         screenSurface = surface;
         windowOrigin = winOrigin;
      }

      Point GetWindowOrigin(void) {return windowOrigin;}
      void SetWindowOrigin(Point origin) {windowOrigin = origin;}

      Surface* GetScreenSurface(void) {return screenSurface;}
      void SetScreenSurface(Surface* surface) {screenSurface = surface;}

      //Drawing Functions
      void FC LockSurface(void) {screenSurface->LockSurface();}
      void FC UnlockSurface(void) {screenSurface->UnlockSurface();}

      //Non-Blit Drawing Functions
      void FC SetPixel(int x, int y, Color& color)
      {
         screenSurface->SetPixel(x + windowOrigin.x, y + windowOrigin.y, color);
      }

      void FC DrawHorizontalLine(int x1, int x2, int y, Color& color)
      {
         screenSurface->DrawHorizontalLine(x1 + windowOrigin.x, x2 + windowOrigin.x,
         y + windowOrigin.y, color);
      }

      void FC DrawVerticalLine(int x, int y1, int y2, Color& color)
      {
         screenSurface->DrawVerticalLine(x + windowOrigin.x, y1 + windowOrigin.y,
         y2 + windowOrigin.y, color);
      }

      void FC DrawLine(Point p1, Point p2, Color& color)
      {
         p1.Offset(windowOrigin.x, windowOrigin.y);
         p2.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawLine(p1, p2, color);
      }

      void FC DrawRectangle(Rectangle rect, Color& color)
      {
         rect.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawRectangle(rect, color);
      }

      void FC DrawFilledRectangle(Rectangle rect, Color& color)
      {
         rect.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawRectangle(rect, color);
      }

      //Blit Drawing Functions
      void FC FillArea(Area area, Color& color)
      {
         area.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->FillArea(area, color);
      } 
      
      void FC FillSurface(Color& color) 
      {
         screenSurface->FillSurface(color);
      }

      void FC DrawBitmap(Point location, UINT bitmapID)
      {
         location.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawBitmap(location, bitmapID);
      }

      void FC DrawScaledBitmap(Area area, UINT bitmapID)
      {
         area.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawScaledBitmap(area, bitmapID);
      }

      void FC DrawTransparentBitmap(Point location, UINT bitmapID,
         Color& transparentColor)
      {
         location.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawTransparentBitmap(location, bitmapID, 
            transparentColor);
      }

      void FC DrawTransparentScaledBitmap(Area area, UINT bitmapID,
         Color& transparentColor)
      {
         area.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawTransparentScaledBitmap(area, bitmapID,
            transparentColor);
      }

      void FC DrawTransparentBitmap(Point location, UINT bitmapID)
      {
         location.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawTransparentBitmap(location, bitmapID);
      }

      void FC DrawTransparentScaledBitmap(Area area, UINT bitmapID)
      {
         area.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawTransparentScaledBitmap(area, bitmapID);
      }

      //GDI Drawing Functions
      void FC SetGDIFont(Font& font)
      {screenSurface->SetGDIFont(font);}

      void FC SetTextTransparency(int mode)
      {screenSurface->SetTextTransparency(mode);}

      void FC SetTextColor(Color& color)
      {screenSurface->SetTextColor(color);}

      void FC SetTextBackgroundColor(Color& color)
      {screenSurface->SetTextBackgroundColor(color);}

      void FC DrawText(char* text, Rectangle& rect, UINT flags)
      {
         rect.Offset(windowOrigin.x, windowOrigin.y);
         screenSurface->DrawText(text, rect, flags);

         //If there is DT_CALCRECT flag, translate the result 
         //of the method call.
         if(flags & DT_CALCRECT)
            rect.Offset(-windowOrigin.x, -windowOrigin.y);
      }

   protected:
      Surface* screenSurface;
      
      Point windowOrigin;
   };
}