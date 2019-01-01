/*BlueWindow.h*/

#pragma once

class BlueWindow : public DG::MainWindow
{
public:
   BlueWindow();
   ~BlueWindow();

   void SetBackgroundColor(DG::Color color) {backgroundColor = color;}
   DG::Color GetBackgroundColor(void) {return backgroundColor;}

protected:
   void FC OnCreate(void);
   void FC OnDrawWindow(DG::WindowSurface* surface);

private:
   bool windowFocused;

   DG::Color backgroundColor;
};
