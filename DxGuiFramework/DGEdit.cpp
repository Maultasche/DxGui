/*------------------------------------------------------------------------
File Name: DGEdit.cpp
Description: This file contains the implementation of the DG::Edit class, 
   which is a control that allows the user to input text.
Version:
   1.0.0    17.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

/*Constructors*/

Edit::Edit(UINT winID, Window* parentWin) :
   Window(winID, true),
   cursorBlinkRate(DGEDIT_BLINK_RATE),
   cursorOn(false)
{
   SetWindowType(WT_EDIT);
   SetParent(parentWin);

   lastBlinkTime = ::GetTickCount();
}

Edit::Edit(UINT winID, Window* parentWin, const int xPos, 
               const int yPos,  const int width, const int height,
               char* _text, Color foreColor, Color backColor, 
               Font _font, bool _parentNotify) :
   Window(winID, true, xPos, yPos, width, height),
   cursorBlinkRate(DGEDIT_BLINK_RATE),
   cursorOn(false)
{
   SetWindowType(WT_EDIT);
   SetParent(parentWin);

   strcpy(text, _text);
   foregroundColor = foreColor;
   backgroundColor = backColor;
   font = _font;
   parentNotify = _parentNotify;

   lastBlinkTime = ::GetTickCount();
}

Edit::Edit(UINT winID, Window* parentWin, const Area& dimensions,
               char* _text, Color foreColor, Color backColor, 
               Font _font, bool _parentNotify) :
   Window(winID, true, dimensions),
   cursorBlinkRate(DGEDIT_BLINK_RATE),
   cursorOn(false)
{
   SetWindowType(WT_EDIT);
   SetParent(parentWin);

   strcpy(text, _text);
   foregroundColor = foreColor;
   backgroundColor = backColor;
   font = _font;
   parentNotify = _parentNotify;

   lastBlinkTime = ::GetTickCount();
}

Edit::Edit(UINT winID, Window* parentWin, const Point& position, 
               const Point& size, char* _text, Color foreColor, 
               Color backColor, Font _font, bool _parentNotify) :
   Window(winID, true, position, size),
   cursorBlinkRate(DGEDIT_BLINK_RATE),
   cursorOn(false)
{
   SetWindowType(WT_EDIT);
   SetParent(parentWin);

   strcpy(text, _text);
   foregroundColor = foreColor;
   backgroundColor = backColor;
   font = _font;
   parentNotify = _parentNotify;   

   lastBlinkTime = ::GetTickCount();
}

/*Overridden message-handling methods*/

void FC Edit::OnDrawWindow(WindowSurface* surface)
{
   //Draw the background
   surface->FillSurface(backgroundColor);

   Point size = GetSize();

   //Draw the text
   surface->SetTextColor(foregroundColor);
   surface->SetGDIFont(font);
   surface->DrawText(text, Rectangle(2, 0, size.x, size.y), 
      DT_LEFT | DT_SINGLELINE | DT_VCENTER);

   Rectangle calcRect(2, 0, size.x, size.y);

   //Calculate the area of the text
   surface->DrawText(text, calcRect, 
      DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);

   surface->LockSurface();

   //If we don't have focus, then don't draw the cursor
   if(GetGui()->GetWindowFocus() == GetWindowID())
   {
      //Draw the cursor when necessary
      DWORD currentTime = ::GetTickCount();
      if(currentTime - lastBlinkTime >= DGEDIT_BLINK_RATE)
      {
         lastBlinkTime = currentTime;

         if(cursorOn)
            cursorOn = false;
         else
            cursorOn = true;
      }

      if(cursorOn)
      {
         int fontHeight = font.GetHeight();
         int lineOffsetY = size.y - fontHeight; 

         //To prevent divide by 0 errors when the font is bigger than
         //the window
         if(lineOffsetY <= 0)
            lineOffsetY = fontHeight;
         else
            lineOffsetY = lineOffsetY / 2;

         surface->DrawVerticalLine(calcRect.right + 1, lineOffsetY, 
            lineOffsetY + fontHeight, foregroundColor);
      }
   }

   //Draw the border around the edit control
   surface->DrawRectangle(Rectangle(0, 0, size.x - 1, size.y - 1), 
      foregroundColor);

   surface->UnlockSurface();
}

void FC Edit::OnLButtonUp(int x, int y, BYTE* keyboardState)
{
   GetGui()->SetWindowFocus(GetWindowID());
}

void FC Edit::OnLButtonDblClk(int x, int y, BYTE* keyboardState)
{
   OnLButtonUp(x, y, keyboardState);
}

void FC Edit::OnCharacter(char character, PBYTE keyboardState)
{
   //Detect an Enter key
   if((character == '\n' || character == '\r') && parentNotify)
   {
      Message* msg = new Message(GM_EDIT_ENTER,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());
      GetParent()->SendMessage(msg);
      delete msg;
   }

   //If it is another character other than enter
   else
   {
      bool textChanged = false;
      unsigned int textLength = (unsigned int)strlen(text);

      //Detect a Backspace
      if(character == '\b')
      {
         if(textLength > 0)
         {
            text[textLength - 1] = '\0';
            textChanged = true;
         }
      }

      else
      {
         //We don't want go beyond the end of the array
         if(textLength < DGEDIT_DEFAULT_TEXT_LENGTH - 1)
         {
            text[textLength] = character;
            text[textLength + 1]  = '\0';
            textChanged = true;
         }

      }

      if(textChanged)
      {
         Message* msg = new Message(GM_EDIT_CHANGE,
            GetParent()->GetWindowID(), 0, 0, GetWindowID());
         GetParent()->SendMessage(msg);
         delete msg;
      }
   }
}