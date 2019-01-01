/*------------------------------------------------------------------------
File Name: DGTitleBar.cpp
Description: This file contains the implementation of the DG::TitleBar 
   class, which is a control that holds the title of a window as well 
   allows the window to be moved.
Version:
   1.0.0    19.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

TitleBar::TitleBar(UINT winID, Window* parentWin, const int xPos, 
                       const int yPos,  const int width, const int height,
                       char* text, Font font) :
   Button(winID, parentWin, xPos, yPos, width, height, BT_PUSHBUTTON | 
      BT_TEXTBUTTON, text, font)
{
   SetWindowType(WT_TITLEBAR);
}

TitleBar::TitleBar(UINT winID, Window* parentWin, const Area& dimensions,
                       char* text, Font font) :
   Button(winID, parentWin, dimensions, BT_PUSHBUTTON | BT_TEXTBUTTON, 
      text, font)
{
   SetWindowType(WT_TITLEBAR);
}

TitleBar::TitleBar(UINT winID, Window* parentWin, const Point& position, 
                       const Point& size, char* text, Font font) :
   Button(winID, parentWin, position, size, BT_PUSHBUTTON | BT_TEXTBUTTON, 
      text, font)
{
   SetWindowType(WT_TITLEBAR);
}

/*Overridden methods*/

void FC TitleBar::OnCreate()
{
   Button::OnCreate();
}

void FC TitleBar::OnLButtonDown(int x, int y, BYTE* keyboardState)
{
   //If there was no pressure before the button goes down,
   //then we need to send a GM_TITLEBAR_DOWN message
   if(!GetPressure())
   {
      Message* msg = new Message(GM_TITLEBAR_DOWN, 
         GetParent()->GetWindowID(), 0, 0, GetWindowID());
      GetParent()->SendMessage(msg);
      delete msg;
   }

   Button::OnLButtonDown(x, y, keyboardState);

   //We need to keep track of the mouse movement in absolute coords
   //Relative coords will always be the same when it is moving
   previousPosition = RelToAbsCoords(x, y);
   GetGui()->SetMouseCapture(GetWindowID());
}

void FC TitleBar::OnLButtonUp(int x, int y, BYTE* keyboardState)
{
   Button::OnLButtonUp(x, y, keyboardState);

   GetGui()->ReleaseMouseCapture();
}

void FC TitleBar::OnMouseMove(int x, int y, BYTE* keyboardState)
{
   Button::OnMouseMove(x, y, keyboardState);

   if(GetPressure())
   {
      //Translate the current position of the mouse to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      Point windowMovement(absoluteCoords.x - previousPosition.x, 
         absoluteCoords.y - previousPosition.y);

      previousPosition = absoluteCoords;

      //Change the position of the parent relative to how the mouse
      //cursor moved
      Point windowPosition = GetParent()->GetPosition();
      windowPosition.Offset(windowMovement.x, windowMovement.y);
      GetParent()->SetPosition(windowPosition);
   }
}