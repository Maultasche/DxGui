/*BlueWindow.cpp*/

#include <DxGuiFramework.h>
#include "DxGuiTestApp.h"
#include "RedWindow.h"
#include "BlueWindow.h"
#include "GreenWindow.h"

using namespace DG;

BlueWindow::BlueWindow() : 
   MainWindow(IDW_BLUEWINDOW, false),
   backgroundColor(Color(0, 0, 255))
{
   windowFocused = false;
}

BlueWindow::~BlueWindow()
{

}

void FC BlueWindow::OnCreate()
{
   //Create the child windows
   AddChildWindow(new RedWindow(100, 100, 200, 200));
   AddChildWindow(new GreenWindow(150, 150, 300, 300));
}

void FC BlueWindow::OnDrawWindow(WindowSurface* surface)
{
   surface->FillSurface(backgroundColor);
}
