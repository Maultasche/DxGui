/*------------------------------------------------------------------------
File Name: DGGui.h
Description: This file contains the DG::Gui class, which represents the
   graphical user interface of the application and manages windows and
   messages.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

//The beginning number of window pointer indices in the GUI
//window list
#define  GUI_INITIAL_WINLIST_SIZE   200

//The amount by which the window list should grow when growth
//is necessary
#define  GUI_WINLIST_GROWTH         50

namespace DG
{
   class Gui
   {
   public:
      Gui();
      virtual ~Gui();

      void SetMainWindow(MainWindow* mainWin);
      MainWindow* GetMainWindow(void) {return mainWindow;}

      void PostMessage(Message* msg);
      void SendMessage(Message* msg);
      void GenerateMessages(void);
      void DispatchMessages(void);

      Window* GetWindow(UINT windowID);

      void SetWindowFocus(UINT windowID);
      UINT GetWindowFocus(void);
      void RemoveWindowFocus(void);

      void SetMouseCapture(UINT windowID);
      void ReleaseMouseCapture(void);
      UINT GetMouseCapture(void);

      void CreateTimer(UINT timerID, UINT windowID, UINT interval);
      void DestroyTimer(UINT timerID);

      void DrawGUI(void);

   private:
      //The main windows for the GUI
      MainWindow* mainWindow;

      //The message queue
      Queue<Message> messageQueue;

      //The collection of window pointers that the GUI manages
      DynamicArray<Window*> windowList;

      //Indicates which window currently has the focus
      UINT focusedWindow;

      //The window that has the mouse capture (always gets mouse messages)
      UINT mouseCaptureWinID;

      //The ID of the window the mouse cursor was previously in
      UINT prevMouseCursorWinID;

      //Stores where the mouse cursor was the last time a GM_MOUSEMOVE
      //message was generated, this is needed for the GM_MOUSELEAVE message
      int prevXPos;
      int prevYPos;

      std::map<UINT, UINT> timerTable;
      typedef std::map<UINT, UINT>::value_type TimerEntry;

   #ifdef DGMESSAGELOG
      MessageLog messageLog;
   #endif
   };
}
