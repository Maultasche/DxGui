/*------------------------------------------------------------------------
File Name: DGApplication.h
Description: This file contains the DGApplication class, which represents
   the application and is the driving force behind it.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

//Declare Windows Message Function
LRESULT CALLBACK WindowsMessageFunction(HWND hWnd, 
   UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace DG
{
   class Application
   {
      //These Functions need to access the DG::Application object
      friend int APIENTRY ::WinMain(HINSTANCE hInstance, 
         HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
      friend LRESULT CALLBACK ::WindowsMessageFunction(HWND hWnd, 
         UINT uMsg, WPARAM wParam, LPARAM lParam);

   public:
      Application();
      virtual ~Application();

      virtual void PreInitApp(void);
      virtual void InitApp(void);
      virtual void DestroyApp(void);
      
      void TerminateApp(void);

      void SetFrameRate(UINT framesPerSecond)
      {frameRate = framesPerSecond;}

      UINT GetFrameRate(void) {return frameRate;}
      Gui* const GetGUI(void) {return &gui;}
      HINSTANCE GetWindowsInstance(void) {return hInstance;}
      HWND GetWindowsHandle(void) {return hWnd;}

      Gui* GetGui(void) {return &gui;}

      bool IsAppActive(void) {return appActive;};

      static Application* instance;

   protected:
      virtual void RunApp(void);

      virtual void PreGUIMessage(void);
      virtual void PreGUIDraw(void);
      virtual void PostGUIDraw(void);

      WNDCLASSEX windowClass;
      WindowSettings windowSettings;

   private:
      virtual void HandleWindowsMessages(void);
      LRESULT CALLBACK WindowsMessageFunction(HWND hWnd, UINT uMsg,
         WPARAM wParam, LPARAM lParam);

      //Number of frames executed each second
      UINT frameRate;
      
      //The pointer to the GUI object
      Gui gui;

      //The flag which tells the application when to terminate
      bool terminateApp;

      //The flag which tells whether the application is active
      bool appActive;

      //The MS Windows application instance
      HINSTANCE hInstance;

      //The MS Windows handle to the application
      HWND hWnd;
   };
}
