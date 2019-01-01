/*------------------------------------------------------------------------
File Name: DGTitleBar.h
Description: This file contains the DG::TitleBar class, which is a control
   that holds the title of a window as well allows the window to be 
   moved.
Version:
   1.0.0    19.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

//The ID of the child button, which is only visible inside the
//title bar control
#define IDW_TB_CHILDWINDOW       100

#define DGTITLEBAR_DEFAULT_FONT_HEIGHT   16

namespace DG
{
   class TitleBar : public Button
   {
   public:
      TitleBar(UINT winID, Window* parentWin, const int xPos, 
         const int yPos,  const int width, const int height,
         char* text = "", 
         Font font = Font("MS Sans Serif", DGTITLEBAR_DEFAULT_FONT_HEIGHT));

      TitleBar(UINT winID, Window* parentWin, const Area& dimensions,
         char* text = "", 
         Font font = Font("MS Sans Serif", DGTITLEBAR_DEFAULT_FONT_HEIGHT));

      TitleBar(UINT winID, Window* parentWin, const Point& position, 
         const Point& size, char* text = "", 
         Font font = Font("MS Sans Serif", DGTITLEBAR_DEFAULT_FONT_HEIGHT));

   protected:
      void FC OnCreate(void);
      void FC OnLButtonDown(int x, int y, BYTE* keyboardState);
      void FC OnLButtonUp(int x, int y, BYTE* keyboardState);
      void FC OnMouseMove(int x, int y, BYTE* keyboardState);

      void OnChildButtonMessage(Message* msg);

   private:
      Point previousPosition;
   };
}
