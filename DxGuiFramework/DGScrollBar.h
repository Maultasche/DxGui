/*------------------------------------------------------------------------
File Name: DGScrollBar.h
Description: This file contains the DG::ScrollBar class, which is a scroll
   bar control. 
Version:
   1.0.0    30.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

//The ID of the child windows, which are only visible inside the
//control
#define IDW_BN_UP             100
#define IDW_BN_DOWN           101
#define IDW_BN_TRACK          102

#define SB_HORIZONTAL         0
#define SB_VERTICAL           1

namespace DG
{
   class ScrollBar : public Window
   {
   public:
      ScrollBar(UINT winID, Window* parentWin, const int xPos, 
         const int yPos,  const int width, const int height,
         const int scrollMin, const int scrollMax, UINT scrollType = SB_VERTICAL);

      ScrollBar(UINT winID, Window* parentWin, const Area& dimensions,
         const int scrollMin, const int scrollMax, UINT scrollType = SB_VERTICAL);

      ScrollBar(UINT winID, Window* parentWin, const Point& position, 
         const Point& size, const int scrollMin, const int scrollMax, 
         UINT scrollType = SB_VERTICAL);

      void SetMinValue(const int min); 
      int GetMinValue(void) {return minValue;}

      void SetMaxValue(const int max);
      int GetMaxValue(void) {return maxValue;}

      void SetPosition(int scrollPosition);
      int GetPosition(void) {return position;}

      void SetLineValue(int value);
      int GetLineValue(void) {return lineValue;}

   protected:
      void FC OnCreate(void);
      void FC OnDrawWindow(WindowSurface* surface);
      void FC OnWindowSizing(void);
      void FC OnWindowSized(void);
      void FC OnTimer(UINT timerID);

      void OnButtonPressure(Message* msg);
      void OnButtonClicked(Message* msg);

      virtual void OnLineUp(void);
      virtual void OnLineDown(void);
      virtual void OnTrack(Message* msg);

   private:
      Button* upButton;
      Image* upImage;
      Button* downButton;
      Image* downImage;
      Button* trackingButton;

      UINT scrollType;

      int minValue;
      int maxValue;
      int position;

      int lineValue;
   };
}