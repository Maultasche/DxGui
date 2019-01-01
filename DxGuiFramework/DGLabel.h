/*------------------------------------------------------------------------
File Name: DGLabel.h
Description: This file contains the DG::Label class, which is a control 
   that displays text.
Version:
   1.0.0    11.07.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

#define LABEL_DEFAULT_FONT_HEIGHT   16

namespace DG
{
   class Label : public Window
   {
   public:
      Label(UINT winID, Window* parentWin);
      Label(UINT winID, Window* parentWin, const int xPos, 
         const int yPos,  const int width, const int height,
         char* text = "", bool transparent = false, 
         Color foreColor = Color(255, 255, 255), 
         Color backColor = Color(0, 0, 0), 
         UINT flags = DT_SINGLELINE | DT_LEFT, 
         Font _font = Font("MS Sans Serif", LABEL_DEFAULT_FONT_HEIGHT),
         bool _parentNotify = true);

      Label(UINT winID, Window* parentWin, const Area& dimensions,
         char* text = "", bool transparent = false, 
         Color foreColor = Color(255, 255, 255), 
         Color backColor = Color(0, 0, 0), 
         UINT flags = DT_SINGLELINE | DT_LEFT, 
         Font _font = Font("MS Sans Serif", LABEL_DEFAULT_FONT_HEIGHT),
         bool _parentNotify = true);

      Label(UINT winID, Window* parentWin, const Point& position, 
         const Point& size, 
         char* text = "", bool transparent = false, 
         Color foreColor = Color(255, 255, 255), 
         Color backColor = Color(0, 0, 0), 
         UINT flags = DT_SINGLELINE | DT_LEFT, 
         Font _font = Font("MS Sans Serif", LABEL_DEFAULT_FONT_HEIGHT),
         bool _parentNotify = true);

      void SetTransparency(bool transparent)
      {transparentBackground = transparent;}
      bool GetTransparency(void) {return transparentBackground;}

      void SetParentNotify(bool notify) {parentNotify = notify;}
      bool GetParentNotify(void) {return parentNotify;}

      void SetForegroundColor(Color color) {foregroundColor = color;}
      Color GetForegroundColor(void) {return foregroundColor;}

      void SetBackgroundColor(Color color) {backgroundColor = color;}
      Color GetBackgroundColor(void) {return backgroundColor;}

      void SetFlags(UINT flags) {textFlags = flags;}
      UINT GetFlags(void) {return textFlags;}

      void SetFont(Font aFont) {font = aFont;}
      Font GetFont(void) {return font;}

      void SetText(const char* aText) {strcpy(text, aText);}
      char* const GetText(void) {return text;}

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

   private:
      bool transparentBackground;
      bool parentNotify;
      Color foregroundColor;
      Color backgroundColor;
      UINT textFlags;  //The same flags as in the DrawText() WinAPI function
      Font font;

      char text[1000];
   };
}
