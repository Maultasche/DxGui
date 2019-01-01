/*------------------------------------------------------------------------
File Name: DGMainWindow.cpp
Description: This file contains the implementation of the DG::MainWindow 
   class, which represents the main window in the GUI and is at the top 
   of the window hierarchy.
Version:
   1.0.0    25.04.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/


#include "DxGuiFramework.h"

using namespace DG;

/*------------------------------------------------------------------------
Function Name: Constructor
Parameters:
   UINT winID : the ID of the window to be constructed
   bool control : whether the window is a control or not
Description:
   This function constructs the MainWindow object.
------------------------------------------------------------------------*/

MainWindow::MainWindow(UINT winID, bool control) :
   Window(winID, control)
{
   SetWindowType(WT_MAINWINDOW);
}

/*------------------------------------------------------------------------
Function Name: Constructor
Parameters:
   UINT winID : the ID of the window to be constructed
   bool control : whether the window is a control or not
   Area dimensions : the dimensions of the window to be constructed
Description:
   This function constructs the MainWindow object.
------------------------------------------------------------------------*/

MainWindow::MainWindow(UINT winID, bool control, 
                       DG::Rectangle dimensions) :
   Window(winID, control, dimensions)
{
   SetWindowType(WT_MAINWINDOW);
}

/*------------------------------------------------------------------------
Function Name: Constructor
Parameters:
   UINT winID : the ID of the window to be constructed
   bool control : whether the window is a control or not
   Point position : the position of the window to be constructed
   Point size : the size of the window to be constructed
Description:
   This function constructs the MainWindow object.
------------------------------------------------------------------------*/

MainWindow::MainWindow(UINT winID, bool control, 
                           Point position, Point size) :
   Window(winID, control, position, size)
{
   SetWindowType(WT_MAINWINDOW);
}

/*------------------------------------------------------------------------
Function Name: Constructor
Parameters:
   UINT winID : the ID of the window to be constructed
   bool control : whether the window is a control or not
   int xPos : the x-coordinate of the position of the window
   int yPos : the y-coordinate of the position of the window
   int width : the width of the window
   int height : the height of the window
Description:
   This function constructs the MainWindow object.
------------------------------------------------------------------------*/

MainWindow::MainWindow(UINT winID, bool control, int xPos, int yPos,
                           int width, int height) :
   Window(winID, control, xPos, yPos, width, height)
{
   SetWindowType(WT_MAINWINDOW);
}

/*Destructor*/
MainWindow::~MainWindow()
{

}

//Overridden Member Functions

/*------------------------------------------------------------------------
Function Name: SetPosition
Parameters:
   const Point& position : a Point object describing a new window
      position
Description:
   This overridden function simply sets the window to position (0, 0).
------------------------------------------------------------------------*/

void FC MainWindow::SetPosition(Point& position)
{
   Window::SetPosition(0, 0);
}

/*------------------------------------------------------------------------
Function Name: SetPosition
Parameters:
   int xPos : the x-coordinate of the new window position
   int yPos : the y-coordinate of the new window position
Description:
   This overridden function simply sets the window to position (0, 0).
------------------------------------------------------------------------*/

void FC MainWindow::SetPosition(int xPos, int yPos)
{
   Window::SetPosition(0, 0);
}

/*------------------------------------------------------------------------
Function Name: SetSize
Parameters:
   const Point& size : a DG::Point object describing a new window
      size
Description:
   This overridden function resizes the window to the size of the screen
------------------------------------------------------------------------*/

void FC MainWindow::SetSize(Point& size)
{
   Point res = dgGraphics->GetResolution();
   Window::SetSize(res);
}

/*------------------------------------------------------------------------
Function Name: SetSize
Parameters:
   int width : the new width of the window
   int height : the new height of the window
Description:
   This overridden function resizes the window to the size of the screen
------------------------------------------------------------------------*/

void FC MainWindow::SetSize(int width, int height)
{
   Point res = dgGraphics->GetResolution();
   Window::SetSize(res);
}

/*------------------------------------------------------------------------
Function Name: SetDimensions
Parameters:
   const Area& dimensions : a DG::Area object describing a new window
      position and size
Description:
   This overridden function sets the window position to (0, 0) and the 
   window size to be the same as that of the screen.
------------------------------------------------------------------------*/

void FC MainWindow::SetDimensions(Area& dimensions)
{
   Point res = dgGraphics->GetResolution();
   Window::SetDimensions(0, 0, res.x, res.y);
}

/*------------------------------------------------------------------------
Function Name: SetDimensions
Parameters:
   const Point& position : a DG::Point object describing a new window
      position
   const Point& size : a DG::Point object describing a new window
      size
Description:
   This overridden function sets the window position to (0, 0) and the 
   window size to be the same as that of the screen.
------------------------------------------------------------------------*/

void FC MainWindow::SetDimensions(Point& position, Point& size)
{
   Point res = dgGraphics->GetResolution();
   Window::SetDimensions(0, 0, res.x, res.y);
}

/*------------------------------------------------------------------------
Function Name: SetDimensions
Parameters:
   int x : the x-coordinate of the new window position
   int y : the y-coordinate fo the new window position
   width : the new window width
   height : the new window height
Description:
   This overridden function sets the window position to (0, 0) and the 
   window size to be the same as that of the screen.
------------------------------------------------------------------------*/

void FC MainWindow::SetDimensions(int x, int y, int width, int height)
{
   Point res = dgGraphics->GetResolution();
   Window::SetDimensions(0, 0, res.x, res.y);
}

/*------------------------------------------------------------------------
Function Name: HideWindow
Parameters:
Description:
   This overridden function does nothing
------------------------------------------------------------------------*/

void FC MainWindow::HideWindow(void)
{
   //Do Nothing
}

/*------------------------------------------------------------------------
Function Name: ShowWindow
Parameters:
Description:
   This overridden function does nothing
------------------------------------------------------------------------*/

void FC MainWindow::ShowWindow(void)
{
   //Do Nothing
}

//Overridden Message Handlers

/*------------------------------------------------------------------------
Function Name: OnWindowResize
Parameters:
   int xPos - the x-coordinate of the new window position
   int yPos - the y-coordinate of the new window position
Description:
   This overridden function does nothing
------------------------------------------------------------------------*/

void FC MainWindow::OnWindowReposition(int xPos, int yPos)
{
   //Do Nothing
}

/*------------------------------------------------------------------------
Function Name: OnWindowResize
Parameters:
   int width - the new width of the window
   int height - the new height of the window
Description:
   This overridden function does nothing
------------------------------------------------------------------------*/

void FC MainWindow::OnWindowResize(int width, int height)
{
   //Do Nothing
}
