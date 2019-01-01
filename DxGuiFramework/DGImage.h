/*------------------------------------------------------------------------
File Name: DGImage.h
Description: This file contains the DG::Image class, which is a control 
   that displays a bitmap.
Version:
   1.0.0    09.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class Image : public Window
   {
   public:
      Image(UINT winID, Window* parentWin, const int xPos, 
         const int yPos,  const int width, const int height,
         UINT bitmapID, bool transparent = false, 
         Color transparentColor = Color(255, 0, 255), 
         bool _parentNotify = true);

      Image(UINT winID, Window* parentWin, const Area& dimensions,
         UINT bitmapID, bool transparent = false, 
         Color transparentColor = Color(255, 0, 255), 
         bool _parentNotify = true);

      Image(UINT winID, Window* parentWin, const Point& position, 
         const Point& size, UINT bitmapID, bool transparent = false, 
         Color transparentColor = Color(255, 0, 255), 
         bool _parentNotify = true);

      void LoadBitmapFile(const char* bitmapName, int priority = 1);
      char* const GetBitmapFileName(void) {return bitmapFileName;}

      void SetTransparency(bool transparent);
      bool GetTransparency(void) {return transparentBitmap;}

      void SetParentNotify(bool notify) {parentNotify = notify;}
      bool GetParentNotify(void) {return parentNotify;}

      void SetTransparentColor(Color color);
      Color GetTransparentColor(void) {return transparentColor;}

      void SetBitmapID(UINT id) {bitmapID = id;}
      UINT GetBitmapID(void) {return bitmapID;}

      void SetBitmapOrigin(Point coords);
      Point GetBitmapOrigin(void) {return bitmapOrigin;}

      Point GetBitmapSize(void);

   protected:
      void FC OnDrawWindow(WindowSurface* surface);
      void FC OnLButtonDown(int x, int y, BYTE* keyboardState);
      void FC OnLButtonUp(int x, int y, BYTE* keyboardState);
      void FC OnLButtonDblClk(int x, int y, BYTE* keyboardState);
      void FC OnMButtonDown(int x, int y, BYTE* keyboardState);
      void FC OnMButtonUp(int x, int y, BYTE* keyboardState);
      void FC OnMButtonDblClk(int x, int y, BYTE* keyboardState);
      void FC OnRButtonDown(int x, int y, BYTE* keyboardState);
      void FC OnRButtonUp(int x, int y, BYTE* keyboardState);
      void FC OnRButtonDblClk(int x, int y, BYTE* keyboardState);
      void FC OnMouseMove(int x, int y, BYTE* keyboardState);
      void FC OnWindowSized(void);

   private:
      bool transparentBitmap;
      bool parentNotify;

      Color transparentColor;
      UINT bitmapID;
      char bitmapFileName[256];

      Point bitmapOrigin;
   };
}
