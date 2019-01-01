/*------------------------------------------------------------------------
File Name: DGMainWindow.h
Description: This file contains the DG::MainWindow class, which represents
   the main window in the GUI and is at the top of the window hierarchy.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class MainWindow : public Window
   {
   public:
      MainWindow(UINT winID, bool control);
      MainWindow(UINT winID, bool control, Rectangle dimensions);
      MainWindow(UINT winID, bool control, Point position, Point size);
      MainWindow(UINT winID, bool control, int xPos, int yPos,
         int width, int height);
      ~MainWindow();

      //Overridden Member Functions
      void FC SetPosition(Point& position);
      void FC SetPosition(int xPos, int yPos);

      void FC SetSize(Point& size);
      void FC SetSize(int width, int height);

      void FC SetDimensions(Area& dimensions);
      void FC SetDimensions(Point& position, Point& size);
      void FC SetDimensions(int x, int y, int width, int height);

      void FC HideWindow(void);
      void FC ShowWindow(void);

      //Overridden Message Handlers
      void FC OnWindowReposition(int xPos, int yPos);
      void FC OnWindowResize(int width, int height);
   };
}