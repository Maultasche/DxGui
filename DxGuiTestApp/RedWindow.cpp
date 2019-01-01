/*RedWindow.cpp*/

#include <DxGuiFramework.h>
#include <time.h>
#include "DxGuiTestApp.h"
#include "BlueWindow.h"
#include "RedWindow.h"

using namespace DG;

#define  TITLEBAR_HEIGHT      20
#define  LABEL_OFFSET_X       20
#define  LABEL_OFFSET_Y       10
#define  LABEL_HEIGHT         60

RedWindow::RedWindow() : Window(IDW_REDWINDOW, false)
{
   srand((unsigned)time(NULL));
}

RedWindow::RedWindow(int xPos, int yPos, int width, int height) :
   Window(IDW_REDWINDOW, false, xPos, yPos, width, height)
{

}

void FC RedWindow::OnCreate()
{
   Window::OnCreate();

   testLabel = new Label(IDL_TESTLABEL, this, Area(10, 0, 180, 60),
      "Click the button to change the background to a random color", true, 
      Color(0, 0, 0), Color(255, 0, 0),
      DT_CENTER | DT_WORDBREAK);
 
   testButton = new Button(IDB_TESTBUTTON, this, Area(75, 110, 140, 30),
      BT_PUSHBUTTON | BT_TEXTBUTTON, "Change Background");

   titleBar = new TitleBar(IDC_RED_TITLEBAR, this, 
      Area(0, 0, 300, TITLEBAR_HEIGHT), "Red Window",
      Font("MS Sans Serif", 16));

   resizeCtrl = new Resize(IDC_RED_RESIZE, this);

   AddChildWindow(testLabel);
   AddChildWindow(testButton);
   AddChildWindow(titleBar);
   AddChildWindow(resizeCtrl);

   AddMessageHandler(GM_BUTTON_CLICKED, 
      new MsgHandlerFunction<RedWindow>(this, OnButtonClicked));
}

void FC RedWindow::OnDrawWindow(WindowSurface* surface)
{
   surface->FillSurface(Color(255, 0, 0));
}

void FC RedWindow::OnWindowSized()
{
   Point size = GetSize();

   int labelWidth = size.x - (2 * LABEL_OFFSET_X);

   if(labelWidth < 0)
      labelWidth = 0;

   titleBar->SetDimensions(Area(0, 0, size.x, TITLEBAR_HEIGHT));
   testLabel->SetDimensions(Area(LABEL_OFFSET_X, TITLEBAR_HEIGHT + 
      LABEL_OFFSET_Y, labelWidth, LABEL_HEIGHT));

   Point buttonSize = testButton->GetSize();
   testButton->SetPosition((size.x / 2) - (buttonSize.x / 2), 
      ((size.y * 2) / 3) - (buttonSize.y / 2));

   Point resizeSize = resizeCtrl->GetSize();
   resizeCtrl->SetPosition(Point(size.x - resizeSize.x, 
      size.y - resizeSize.y));
}

void RedWindow::OnButtonClicked(Message* msg)
{
   if(msg->data3 == IDB_TESTBUTTON)
   {
      Color randomColor(rand() % 256, rand() % 256, rand() % 256);
      ((BlueWindow*)GetParent())->SetBackgroundColor(randomColor);
   }
}
