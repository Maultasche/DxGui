/*------------------------------------------------------------------------
File Name: DGScrollBar.h
Description: This file contains the implementation of the DG::ScrollBar 
   class, which is a scroll bar control. 
Version:
   1.0.0    30.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

#define DEFAULT_LINE_VALUE    1
#define SB_SCROLL_INTERVAL    50

/*Constructors*/
ScrollBar::ScrollBar(UINT winID, Window* parentWin, const int xPos, 
                         const int yPos,  const int width, const int height,
                         const int scrollMin, const int scrollMax, 
                         UINT scrType) :
   Window(winID, true, xPos, yPos, width, height),
   minValue(scrollMin),
   maxValue(scrollMax),
   position(scrollMin),
   lineValue(DEFAULT_LINE_VALUE),
   scrollType(scrType),
   upButton(NULL),
   downButton(NULL),
   trackingButton(NULL)
{
   assert(scrollMin <= scrollMax);
}


ScrollBar::ScrollBar(UINT winID, Window* parentWin, const Area& dimensions,
                         const int scrollMin, const int scrollMax, UINT scrType) :
   Window(winID, true, dimensions),
   minValue(scrollMin),
   maxValue(scrollMax),
   position(scrollMin),
   lineValue(DEFAULT_LINE_VALUE),
   scrollType(scrType),
   upButton(NULL),
   downButton(NULL),
   trackingButton(NULL)
{
   assert(scrollMin <= scrollMax);
}

ScrollBar::ScrollBar(UINT winID, Window* parentWin, const Point& position, 
                         const Point& size, const int scrollMin, const int scrollMax, 
                         UINT scrType) :
   Window(winID, true, position, size),
   minValue(scrollMin),
   maxValue(scrollMax),
   position(scrollMin),
   lineValue(DEFAULT_LINE_VALUE),
   scrollType(scrType),
   upButton(NULL),
   downButton(NULL),
   trackingButton(NULL)
{
   assert(scrollMin <= scrollMax);
}

void ScrollBar::SetMinValue(const int min) 
{
   assert(min <= maxValue);
   minValue = min;

   if(position < minValue)
      position = minValue;
}

void ScrollBar::SetMaxValue(const int max) 
{
   assert(max >= minValue);
   maxValue = max;

   if(position > maxValue)
      position = maxValue;
}

void ScrollBar::SetPosition(int scrollPosition) 
{   
   position = scrollPosition;

   if(position < minValue)
      position = minValue;
   else if(position > maxValue)
      position = maxValue;
}

void ScrollBar::SetLineValue(int value)
{
   assert(abs(lineValue) <= maxValue);

   lineValue = value;
}

void FC ScrollBar::OnCreate()
{
   Point size = GetSize();

   upButton = new Button(IDW_BN_UP, this, Area(0, 0, 16, 16),
      BT_PUSHBUTTON | BT_IMAGEBUTTON);

   downButton = new Button(IDW_BN_DOWN, this, Area(size.x - 16, 
      size.y - 16, 16, 16), BT_PUSHBUTTON | BT_IMAGEBUTTON);

   //If the up and down arrow bitmaps have not already been loaded, 
   //load them
   if(scrollType == SB_VERTICAL)
   {
      Bitmap* bitmap = dgGraphics->GetBitmap(IDB_UP_ARROW);
      if(bitmap == NULL)
         dgGraphics->LoadBitmap(IDB_UP_ARROW, 1, "Bitmaps\\Arrow_Up.bmp");

      bitmap = dgGraphics->GetBitmap(IDB_DOWN_ARROW);
      if(bitmap == NULL)
         dgGraphics->LoadBitmap(IDB_DOWN_ARROW, 1, "Bitmaps\\Arrow_Down.bmp");

      upButton->SetBitmapID(IDB_UP_ARROW);
      downButton->SetBitmapID(IDB_DOWN_ARROW);
   }
   
   //scrollType == SB_HORIZONTAL
   else
   {
      Bitmap* bitmap = dgGraphics->GetBitmap(IDB_LEFT_ARROW);
      if(bitmap == NULL)
         dgGraphics->LoadBitmap(IDB_LEFT_ARROW, 1, "Bitmaps\\Arrow_Left.bmp");

      bitmap = dgGraphics->GetBitmap(IDB_RIGHT_ARROW);
      if(bitmap == NULL)
         dgGraphics->LoadBitmap(IDB_RIGHT_ARROW, 1, "Bitmaps\\Arrow_Right.bmp");

      upButton->SetBitmapID(IDB_LEFT_ARROW);
      downButton->SetBitmapID(IDB_RIGHT_ARROW);
   }

   AddChildWindow(upButton);
   AddChildWindow(downButton);

   AddMessageHandler(GM_BUTTON_PRESSURE, 
      new MsgHandlerFunction<ScrollBar>(this, OnButtonPressure));

   AddMessageHandler(GM_BUTTON_CLICKED, 
      new MsgHandlerFunction<ScrollBar>(this, OnButtonClicked));
}

void FC ScrollBar::OnDrawWindow(WindowSurface* surface)
{
   surface->FillSurface(Color(180, 180, 180));
}

void FC ScrollBar::OnWindowSizing()
{
   OnWindowSized();
}

void FC ScrollBar::OnWindowSized()
{
   Point size = GetSize();
   downButton->SetPosition(size.x - 16, size.y - 16);
}

void FC ScrollBar::OnTimer(UINT timerID)
{
   if(timerID == SB_UP_TIMER)
   {
      OnLineUp();
      //OutputDebugString("LineUp Timer\n");
   }
   else if(timerID == SB_DOWN_TIMER)
   {
      OnLineDown();
      //OutputDebugString("LineDown Timer\n");
   }
}

void ScrollBar::OnButtonPressure(Message* msg)
{
   if(msg->data3 == IDW_BN_UP)
   {
      OnLineUp();

      GetGui()->CreateTimer(SB_UP_TIMER, GetWindowID(), SB_SCROLL_INTERVAL);


   }

   else if(msg->data3 == IDW_BN_DOWN)
   {
      OnLineDown();

      GetGui()->CreateTimer(SB_DOWN_TIMER, GetWindowID(), SB_SCROLL_INTERVAL);

   }

   //OutputDebugString("Button Down\n");
}

void ScrollBar::OnButtonClicked(Message* msg)
{
   if(msg->data3 == IDW_BN_UP)
   {
      GetGui()->DestroyTimer(SB_UP_TIMER);
   }

   else if(msg->data3 == IDW_BN_DOWN)
   {
      GetGui()->DestroyTimer(SB_DOWN_TIMER);
   }

   //OutputDebugString("Button Up\n");
}

void ScrollBar::OnLineUp()
{
   position -= lineValue;

   if(position < minValue)
      position = minValue;

   //Send a GM_SCROLLBAR_LINEUP message to parent
   Message* msg = new Message(Message(GM_SCROLLBAR_LINEUP,
      GetParent()->GetWindowID(), 0, 0, GetWindowID()));
   GetParent()->SendMessage(msg);
   delete msg;

}

void ScrollBar::OnLineDown()
{
   position += lineValue;

   if(position > maxValue)
      position = maxValue;

   //Send a GM_SCROLLBAR_LINEDOWN message to parent
   Message* msg = new Message(Message(GM_SCROLLBAR_LINEDOWN,
      GetParent()->GetWindowID(), 0, 0, GetWindowID()));
   GetParent()->SendMessage(msg);
   delete msg;

}

void ScrollBar::OnTrack(Message* msg)
{

}
