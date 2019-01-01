/*------------------------------------------------------------------------
File Name: DGSurface.h
Description: This file contains the DG::Surface class, which represents an 
	area of the screen that can be drawn upon.
Version:
   1.0.0    24.03.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   extern Graphics* dgGraphics;

   class Surface
   {
   public:
      Surface(Area& area);
      virtual ~Surface();

      Point GetSize(void) 
      {return Point(screenArea.width, screenArea.height);}

      void SetWindowOrg(Point origin);
      void RemoveWindowOrg(void);

      //Drawing Functions
      void FC LockSurface(void) {dgGraphics->LockSurface();}
      void FC UnlockSurface(void) {dgGraphics->UnlockSurface();}

      //Non-Blit Drawing Functions
      void FC SetPixel(int x, int y, Color& color);
      void FC DrawHorizontalLine(int x1, int x2, int y, Color& color);
      void FC DrawVerticalLine(int x, int y1, int y2, Color& color);
      void FC DrawLine(Point p1, Point p2, Color& color);
      void FC DrawRectangle(Rectangle rect, Color& color);
      void FC DrawFilledRectangle(Rectangle rect, Color& color);

      //Blit Drawing Functions
      void FC FillArea(Area area, Color& color)
      {
         area.Offset(screenArea.left, screenArea.top);
         dgGraphics->FillArea(area, color);
      } 
      
      void FC FillSurface(Color& color) 
      {dgGraphics->FillArea(screenArea, color);}

      void FC DrawBitmap(Point location, UINT bitmapID)
      {
         location.Offset(screenArea.left, screenArea.top);
         dgGraphics->DrawBitmap(location, bitmapID);
      }

      void FC DrawScaledBitmap(Area area, UINT bitmapID)
      {
         area.Offset(screenArea.left, screenArea.top);
         dgGraphics->DrawScaledBitmap(area, bitmapID);
      }

      void FC DrawTransparentBitmap(Point location, UINT bitmapID,
         Color& transparentColor)
      {
         location.Offset(screenArea.left, screenArea.top);
         dgGraphics->DrawTransparentBitmap(location, bitmapID, 
            transparentColor);
      }

      void FC DrawTransparentScaledBitmap(Area area, UINT bitmapID,
         Color& transparentColor)
      {
         area.Offset(screenArea.left, screenArea.top);
         dgGraphics->DrawTransparentScaledBitmap(area, bitmapID,
            transparentColor);
      }

      void FC DrawTransparentBitmap(Point location, UINT bitmapID)
      {
         location.Offset(screenArea.left, screenArea.top);
         dgGraphics->DrawTransparentBitmap(location, bitmapID);
      }

      void FC DrawTransparentScaledBitmap(Area area, UINT bitmapID)
      {
         area.Offset(screenArea.left, screenArea.top);
         dgGraphics->DrawTransparentScaledBitmap(area, bitmapID);
      }

      //GDI Drawing Functions
      void FC SetGDIFont(Font& font)
      {dgGraphics->SetGDIFont(font);}

      void FC SetTextTransparency(int mode)
      {dgGraphics->SetTextTransparency(mode);}

      void FC SetTextColor(Color& color)
      {dgGraphics->SetTextColor(color);}

      void FC SetTextBackgroundColor(Color& color)
      {dgGraphics->SetTextBackgroundColor(color);}

      void FC DrawText(char* text, Rectangle& rect, UINT flags)
      {
         rect.Offset(screenArea.left, screenArea.top);
         dgGraphics->DrawText(text, rect, flags);

         //If there is DT_CALCRECT flag, translate the result 
         //of the method call.
         if(flags & DT_CALCRECT)
            rect.Offset(-screenArea.left, -screenArea.top);
      }


      //The clipping functions for a DG::Surface will be implemented once
      //DG::Graphics is capable of doing multiple clipping areas.

      //Clipping Functions
      //void FC AddClippingArea(Rectangle& area);
      //void FC RemoveClippingArea(void);
      //void FC ClearClippingAreas(void);

   protected:
      //Stores the area on the screen that the surface represents,
      //surfaceArea.left and surfaceArea.top describe upper-left corner of 
      //the surface, surfaceArea.right and surfaceArea.bottom describe the
      //width and height of the surface
      Area screenArea;
      
      //Stores the origin in surface coordinates of the window that currently
      //being drawn. This is necessary to allow a child window to clip its edge
      //at the edge of its parent window. Otherwise the upper-left corner of the
      //window being drawn would always be at the upper-left corner of the screen
      //surface, which isn't the case when the upper-left portion of the window
      //being drawn is clipped.
      Point* windowOrg;
   };
}
