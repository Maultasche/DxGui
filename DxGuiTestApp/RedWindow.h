/*RedWindow.h*/

#pragma once

class RedWindow : public DG::Window
{
public:
   RedWindow();
   RedWindow(int xPos, int yPos, int width, int height);

protected:
   void FC OnCreate(void);
   void FC OnDrawWindow(DG::WindowSurface* surface);
   void FC OnWindowSized(void);

   //Non-overridden message handling methods
   void OnButtonClicked(DG::Message* msg);

private:
   DG::Point previousPosition;

   bool windowFocused;
   bool windowMoving;

   DG::Label* testLabel;
   DG::Button* testButton;
   DG::TitleBar* titleBar;
   DG::Resize* resizeCtrl;
};
