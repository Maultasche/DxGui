/*------------------------------------------------------------------------
File Name: DxGuiFramework.cpp
Description: This file is the principal framework file. It defines the
   global variables and functions as well as the WinMain function 
   required for all MS Windows program.s
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/


#include "DxGuiFramework.h"

//Global Object Pointer Definitions
DG::Graphics* DG::dgGraphics = NULL;
DG::Input* DG::dgInput = NULL;

//Static variables need to be initialized to prevent linker errors
DG::Application* DG::Application::instance = NULL;
DG::Graphics* DG::Graphics::instance = NULL;

//Global Function Definitions
DG::Application* DG::GetApp(void) {return DG::Application::instance;}
DG::Graphics* DG::GetGraphics(void) {return DG::Graphics::instance;}
DG::Gui* DG::GetGui(void) {return DG::GetApp()->GetGui();}

//Declare Windows Message Function
LRESULT CALLBACK WindowsMessageFunction(HWND hWnd, 
   UINT uMsg, WPARAM wParam, LPARAM lParam);

//General Exception-Handling Function
void HandleException(DG::Exception* exception);

//The WinMain function is required for any Windows program.
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine, int nCmdShow)
{
   OutputDebugString("WinMain\n");

   assert(DG::GetApp() != NULL);

   try
   {
      DG::Application* application = DG::GetApp();

      //Call PreInitApp() to get MS Window settings
      application->PreInitApp();
   
      //Get the Windows class from the application object
      WNDCLASSEX winClass = application->windowClass;

      //The message function must be this one.
      winClass.lpfnWndProc = WindowsMessageFunction;
      winClass.hInstance = hInstance;

      //Register the Windows class
      if(!RegisterClassEx(&winClass))
         return 0;

      DG::WindowSettings winSettings = application->windowSettings;

      if(!(application->hWnd = CreateWindow(winClass.lpszClassName,
         winSettings.windowName, winSettings.dwStyle,
         winSettings.xPos, winSettings.yPos, 
         winSettings.nWidth, winSettings.nHeight, 
         NULL, winSettings.hMenu, hInstance, NULL)))
         return 0;

      //Allocate the global DG::Graphics and DG::Input objects
      //This must be done *after* the window is created
      DG::dgGraphics = new DG::Graphics();
      DG::dgInput = new DG::Input();

      //Now that the DG::Graphics object has been created, set the 
      //main window of the GUI
      //Construct the main window
      DG::Point screenRes = DG::dgGraphics->GetResolution();
      DG::MainWindow* mainWindow = new DG::MainWindow(IDW_DEFAULTMAIN, false, 
         DG::Point(0, 0), screenRes);

      DG::GetGui()->SetMainWindow(mainWindow);
      
      //Set the focus to the default main window
      DG::GetGui()->SetWindowFocus(IDW_DEFAULTMAIN);

      application->InitApp();

      application->RunApp();
   }

   catch(DG::Exception* exception)
   {
      HandleException(exception);
      delete exception;
   }

   if(DG::dgGraphics != NULL)
      delete DG::dgGraphics;
   if(DG::dgInput != NULL)
      delete DG::dgInput;

   return 0;
}

LRESULT CALLBACK WindowsMessageFunction(HWND hWnd, 
   UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   return DG::GetApp()->WindowsMessageFunction(hWnd, 
      uMsg, wParam, lParam);
}

void HandleException(DG::Exception* exception)
{
   char outputText[1024];

   sprintf(outputText, "%s\nFile: %s, Line: %i", exception->errorMessage,
      exception->fileName, exception->lineNumber);

   switch(exception->errorType)
   {
      //It's a DirectDraw error
      case ET_DIRECTDRAW:
      {
         switch(exception->errorCode)
         {
            case EC_DDINIT:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Initialization Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;

            case EC_DDSETGRAPHMODE:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw SetGraphicsMode Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            
            case EC_DDLOCKSURFACE:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Surface Locking Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;

            case EC_DDUNLOCKSURFACE:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Surface Unlocking Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            case EC_DDFLIPSURFACE:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Surface Flipping Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            case EC_DDRESTORESURFACES:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Surface Restore Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            case EC_DDFILLSCREEN:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Screen Fill Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            case EC_DDCLEARPRIMARY:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Primary Surface Clearing Error Has "\
                  "Occurred", MB_OK | MB_ICONERROR);
               break;
            case EC_DDFILLAREA:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Area Filling Error Has "\
                  "Occurred", MB_OK | MB_ICONERROR);
               break;
            case EC_DDDRAWBMP:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Bitmap Drawing Error Has "\
                  "Occurred", MB_OK | MB_ICONERROR);
               break;
            case EC_DDCOLORKEY:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Color Key Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            case EC_DDTEXT:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Text Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            case EC_DDCLIPPING:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A DirectDraw Clipper Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;

            default: 
            {
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "An Unknown DirectDraw Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            }
         }
         break;
      }

      //It's a Bitmap error
      case ET_BITMAP:
      {
         switch(exception->errorCode)
         {
            case EC_BMBITMAPSIZE:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A Bitmap File Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            case EC_BMBITMAPLOAD:
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "A Bitmap Loading Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            default: 
               MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
                  "An Unknown Bitmap Error Has Occurred", 
                  MB_OK | MB_ICONERROR);
               break;
            
         }
         break;
      }

      //The type of error is unknown
      default:
      {
         MessageBox(DG::GetApp()->GetWindowsHandle(), outputText, 
            "An Unknown Error Has Occurred", MB_OK | MB_ICONERROR);
      }
   }
}