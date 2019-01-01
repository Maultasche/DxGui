/*------------------------------------------------------------------------
File Name: DGEdit.h
Description: This file contains the DG::Edit class, which is a control 
   that allows the user to input text.
Version:
   1.0.0    16.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

#define DGEDIT_DEFAULT_FONT_HEIGHT   16
#define DGEDIT_DEFAULT_TEXT_LENGTH   1000

#define DGEDIT_BLINK_RATE            500

namespace DG
{
   class Edit : public Window
   {
   public:
      Edit(UINT winID, Window* parentWin);
      Edit(UINT winID, Window* parentWin, const int xPos, 
         const int yPos,  const int width, const int height,
         char* text = "",
         Color foreColor = Color(200, 200, 200), 
         Color backColor = Color(0, 0, 0), 
         Font _font = Font("MS Sans Serif", DGEDIT_DEFAULT_FONT_HEIGHT),
         bool _parentNotify = true);

      Edit(UINT winID, Window* parentWin, const Area& dimensions,
         char* text = "", 
         Color foreColor = Color(200, 200, 200), 
         Color backColor = Color(0, 0, 0), 
         Font _font = Font("MS Sans Serif", DGEDIT_DEFAULT_FONT_HEIGHT),
         bool _parentNotify = true);

      Edit(UINT winID, Window* parentWin, const Point& position, 
         const Point& size, char* text = "", 
         Color foreColor = Color(200, 200, 200), 
         Color backColor = Color(0, 0, 0), 
         Font _font = Font("MS Sans Serif", DGEDIT_DEFAULT_FONT_HEIGHT),
         bool _parentNotify = true);

      void SetParentNotify(bool notify) {parentNotify = notify;}
      bool GetParentNotify(void) {return parentNotify;}

      void SetForegroundColor(Color color) {foregroundColor = color;}
      Color GetForegroundColor(void) {return foregroundColor;}

      void SetBackgroundColor(Color color) {backgroundColor = color;}
      Color GetBackgroundColor(void) {return backgroundColor;}

      void SetFont(Font aFont) {font = aFont;}
      Font GetFont(void) {return font;}

      void SetText(const char* aText) {strcpy(text, aText);}
      char* const GetText(void) {return text;}

   protected:
      void FC OnDrawWindow(WindowSurface* surface);
      void FC OnLButtonUp(int x, int y, BYTE* keyboardState);
      void FC OnLButtonDblClk(int x, int y, BYTE* keyboardState);
      void FC OnCharacter(char character, PBYTE keyboardState);

   private:
      bool parentNotify;
      Color foregroundColor;
      Color backgroundColor;
      Font font;
      
      int cursorBlinkRate;
      DWORD lastBlinkTime;
      bool cursorOn;

      char text[DGEDIT_DEFAULT_TEXT_LENGTH];
   };
}
