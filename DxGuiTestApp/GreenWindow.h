/*GreenWindow.h*/

#pragma once

class GreenWindow : public DG::Window
{
public:
   GreenWindow();
   GreenWindow(int xPos, int yPos, int width, int height);

protected:
   void FC OnCreate(void);
   void FC OnDrawWindow(DG::WindowSurface* surface);
   void FC OnMouseMove(int x, int y, BYTE* keyboardState);
   void FC OnWindowSized(void);

   void OnLoadBitmap(DG::Message* msg);
   void OnSBLineUp(DG::Message* msg);
   void OnSBLineDown(DG::Message* msg);

private:
   DG::Point previousPosition;

   bool windowFocused;
   bool windowMoving;

   DG::Image* imageCtrl;
   DG::Edit* inputCtrl;
   DG::TitleBar* titleBar;
   DG::Resize* resizeCtrl;
   DG::Button* loadButton;
   DG::ScrollBar* horizontalBar;
   DG::ScrollBar* verticalBar;
};
