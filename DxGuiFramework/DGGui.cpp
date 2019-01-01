/*------------------------------------------------------------------------
File Name: DGGui.cpp
Description: This file contains the implementation of the DG::Gui class, 
   which represents the graphical user interface of the application 
   and manages windows and messages.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

/*------------------------------------------------------------------------
Function Name: Constructor
Parameters:
Description:
   This function constructs the DG::Gui object and creates a default main
   window.
------------------------------------------------------------------------*/

Gui::Gui() :
   mainWindow(NULL),
   mouseCaptureWinID(IDW_NONE),
   prevMouseCursorWinID(IDW_NONE)
{
   //The main window can't be constructed here; when the application is
   //created, the DG::Graphics object hasn't been created yet, so there
   //is no way to know the resolution of the screen

   windowList.SetGrowAmount(GUI_WINLIST_GROWTH);
   windowList.SetInitialValue(NULL);
   windowList.SetSize(GUI_INITIAL_WINLIST_SIZE);
}

/*Destructor*/
Gui::~Gui()
{
   //Destroy the main window, which will cause it do destroy all its
   //child windows.
   if(mainWindow != NULL)
      delete mainWindow;
}

/*------------------------------------------------------------------------
Function Name: SetMainWindow
Parameters:
   MainWindow mainWin : the pointer to the window to be made the main
      window
Description:
   This function receives a pointer to a window to be the main window
   and makes it the main window. If the window will be resized as
   necessary so that it covers the entire screen. The new main window
   will automatically be given the focuse. The previous main window will 
   be deleted. The pointer to the main window is assumed to be a valid 
   pointer.
------------------------------------------------------------------------*/

void Gui::SetMainWindow(MainWindow* mainWin)
{
   assert(mainWin != NULL);

   Point screenRes = dgGraphics->GetResolution();
   mainWin->SetDimensions(Point(0, 0), screenRes);
   
   //Make sure the old window is destroyed
   if(mainWindow != NULL)
   {
      mainWindow->Destroy();
      delete mainWindow;
   }

   mainWindow = mainWin;

   //Make sure the new window is created
   mainWindow->Create();

   SetWindowFocus(mainWindow->GetWindowID());
}

/*------------------------------------------------------------------------
Function Name: PostMessage
Parameters:
   Message msg : a pointer to the message to be put on the message
      queue.
Description:
   This function receives a pointer a message to be put on the message
   queue and puts that message at the end of the message queuek. 
   The pointer to the message is assumed to be a valid pointer.
------------------------------------------------------------------------*/

void Gui::PostMessage(Message* msg)
{
   assert(msg != NULL);

   messageQueue.Enqueue(msg, msg->messageType);
}

/*------------------------------------------------------------------------
Function Name: SendMessage
Parameters:
   Message msg : a pointer to the message to be sent
Description:
   This function receives a pointer a message and immediately sends that
   message. This function behaves asynchronously and does not wait until
   the usual message-dispatching phase to send the message. This
   function deletes the message when it is done with it.
------------------------------------------------------------------------*/

void Gui::SendMessage(Message* msg)
{
   assert(msg != NULL);

   mainWindow->SendMessage(msg);

   delete msg;
}

/*------------------------------------------------------------------------
Function Name: GenerateMessages
Parameters:
Description:
   This function generates GUI messages based on user input, such as key
   presses and mouse movement.
------------------------------------------------------------------------*/

void Gui::GenerateMessages(void)
{
   InputEvent* event = NULL;
   Message* message = NULL;
   int xPos = 0;
   int yPos = 0;
   UINT windowID = IDW_NONE;

   //Loop throught the input event queue and generate messages from
   //each input event.
   while(dgInput->EventsInQueue())
   {
      event = dgInput->RemoveEvent();
     
      switch(event->messageType)
      {
         case WM_KEYDOWN:
         case WM_SYSKEYDOWN:
            message = new Message(GM_KEYDOWN, focusedWindow, 0, 0,
               event->wParam, event->keyboardState);
            PostMessage(message);
            break;
         case WM_KEYUP:
         case WM_SYSKEYUP:
            message = new Message(GM_KEYUP, focusedWindow, 0, 0,
               event->wParam, event->keyboardState);
            PostMessage(message);
            break;
         case WM_CHAR:
            if(focusedWindow != IDW_NONE)
            {
               message = new Message(GM_CHARACTER, focusedWindow, 0, 0, 
                  LOBYTE(LOWORD(event->wParam)), event->keyboardState);
               PostMessage(message);
            }
            break;
         case WM_LBUTTONDOWN:
            //Get the absolute x and y coordinates
            xPos = LOWORD(event->lParam);
            yPos = HIWORD(event->lParam);

            //If the mouse is captured, send it to that window
            if(mouseCaptureWinID != IDW_NONE)
               windowID = mouseCaptureWinID;
            //Otherwise find the window at those coordinates
            else
               windowID = mainWindow->GetWinIDOfCoords(xPos, yPos);

            //Put the message on the queue
            message = new Message(GM_LBUTTONDOWN, windowID, xPos, yPos, 0, 
               event->keyboardState);
            PostMessage(message);
            break;
         case WM_LBUTTONUP:
            //Get the absolute x and y coordinates
            xPos = LOWORD(event->lParam);
            yPos = HIWORD(event->lParam);

            //If the mouse is captured, send it to that window
            if(mouseCaptureWinID != IDW_NONE)
               windowID = mouseCaptureWinID;
            //Otherwise find the window at those coordinates
            else
               windowID = mainWindow->GetWinIDOfCoords(xPos, yPos);

            //Put the message on the queue
            message = new Message(GM_LBUTTONUP, windowID, xPos, yPos, 0, 
               event->keyboardState);
            PostMessage(message);
            break;
         case WM_MBUTTONDOWN:
            //Get the absolute x and y coordinates
            xPos = LOWORD(event->lParam);
            yPos = HIWORD(event->lParam);

            //If the mouse is captured, send it to that window
            if(mouseCaptureWinID != IDW_NONE)
               windowID = mouseCaptureWinID;
            //Otherwise find the window at those coordinates
            else
               windowID = mainWindow->GetWinIDOfCoords(xPos, yPos);

            //Put the message on the queue
            message = new Message(GM_MBUTTONDOWN, windowID, xPos, yPos, 0, 
               event->keyboardState);
            PostMessage(message);
            break;
         case WM_MBUTTONUP:
            //Get the absolute x and y coordinates
            xPos = LOWORD(event->lParam);
            yPos = HIWORD(event->lParam);

            //If the mouse is captured, send it to that window
            if(mouseCaptureWinID != IDW_NONE)
               windowID = mouseCaptureWinID;
            //Otherwise find the window at those coordinates
            else
               windowID = mainWindow->GetWinIDOfCoords(xPos, yPos);

            //Put the message on the queue
            message = new Message(GM_MBUTTONUP, windowID, xPos, yPos, 0, 
               event->keyboardState);
            PostMessage(message);
            break;
         case WM_RBUTTONDOWN:
            //Get the absolute x and y coordinates
            xPos = LOWORD(event->lParam);
            yPos = HIWORD(event->lParam);

            //If the mouse is captured, send it to that window
            if(mouseCaptureWinID != IDW_NONE)
               windowID = mouseCaptureWinID;
            //Otherwise find the window at those coordinates
            else
               windowID = mainWindow->GetWinIDOfCoords(xPos, yPos);

            //Put the message on the queue
            message = new Message(GM_RBUTTONDOWN, windowID, xPos, yPos, 0, 
               event->keyboardState);
            PostMessage(message);
            break;
         case WM_RBUTTONUP:
            //Get the absolute x and y coordinates
            xPos = LOWORD(event->lParam);
            yPos = HIWORD(event->lParam);

            //If the mouse is captured, send it to that window
            if(mouseCaptureWinID != IDW_NONE)
               windowID = mouseCaptureWinID;
            //Otherwise find the window at those coordinates
            else
               windowID = mainWindow->GetWinIDOfCoords(xPos, yPos);

            //Put the message on the queue
            message = new Message(GM_RBUTTONUP, windowID, xPos, yPos, 0, 
               event->keyboardState);
            PostMessage(message);
            break;
         case WM_MOUSEMOVE:
            
            //Get the absolute x and y coordinates
            xPos = LOWORD(event->lParam);
            yPos = HIWORD(event->lParam);

            //If the mouse is captured, send it to that window
            if(mouseCaptureWinID != IDW_NONE)
               windowID = mouseCaptureWinID;
            //Otherwise find the window at those coordinates
            else
               windowID = mainWindow->GetWinIDOfCoords(xPos, yPos);

            //If necessary, generate GM_MOUSEENTER and GM_MOUSELEAVE 
            //messages
            if(windowID != prevMouseCursorWinID)
            {
               message = new Message(GM_MOUSELEAVE, prevMouseCursorWinID, 
                  prevXPos, prevYPos);
               PostMessage(message);
               message = new Message(GM_MOUSEENTER, windowID, xPos, yPos);
               PostMessage(message);
               prevMouseCursorWinID = windowID;
            }

            //Generate the GM_MOUSEMOVE message
            //Put the GM_MOUSEMOVE message on the queue
            message = new Message(GM_MOUSEMOVE, windowID, xPos, yPos);
            PostMessage(message);

            //Save where the mouse cursor was in case a GM_MOUSELEAVE message
            //needs to be generated next time
            prevXPos = xPos;
            prevYPos = yPos;
            break;
         case WM_TIMER:
            UINT timerID = event->wParam;
            std::map<UINT, UINT>::iterator it = timerTable.find(timerID);

            //If we can find an entry for this timer, then send the message
            if(it != timerTable.end())
            {
               UINT windowID = (*it).second;
               message = new Message(GM_TIMER, windowID, 0, 0, timerID);
               PostMessage(message);
            }

            //If we cannot find an entry for this timer, then destroy it
            else
               DestroyTimer(timerID);

            break;
      }
   }
}

/*------------------------------------------------------------------------
Function Name: DispatchMessages
Parameters:
Description:
   This function dispatches all the messages in the message queue until
   it becomes empty. The messages are removed from the message queue
   and sent to the window hierarchy.
------------------------------------------------------------------------*/

void Gui::DispatchMessages(void)
{
   Message* message = NULL;

   while(messageQueue.GetNumOfItems() > 0)
   {
      //Remove the next message from the message queue
      message = messageQueue.Dequeue();

#ifdef DGMESSAGELOG
      messageLog.LogMessage(message);
#endif

      mainWindow->SendMessage(message);

      //Destroy the message after it has been dispatched and handled
      delete message;
   }
}

/*------------------------------------------------------------------------
Function Name: GetWindow
Parameters:
   UINT windowID : the ID of a window 
Description:
   This function receives a window ID and returns a pointer to that 
   window.
------------------------------------------------------------------------*/

Window* Gui::GetWindow(UINT windowID)
{
   return mainWindow->GetWindow(windowID);
}

/*------------------------------------------------------------------------
Function Name: SetWindowFocus
Parameters:
   UINT windowID : the ID of a window 
Description:
   This function receives a window ID and sets the focus to that window.
------------------------------------------------------------------------*/

void Gui::SetWindowFocus(UINT windowID)
{
   //Inform the previously focused window that it is losing the focus
   if(focusedWindow != IDW_NONE)
      PostMessage(new Message(GM_LOSEFOCUS, focusedWindow, 0, 0, 
         windowID));

   //Inform the newly focused window that it is gaining the focus
   if(windowID != IDW_NONE)
      PostMessage(new Message(GM_FOCUS, windowID, 0, 0, 
         focusedWindow));

   focusedWindow = windowID;
   
}

/*------------------------------------------------------------------------
Function Name: GetWindowFocus
Parameters:
Description:
   This function returns the window ID of the window with the focus.
   If no window currently has the focus, IDW_NONE is returned.
------------------------------------------------------------------------*/

UINT Gui::GetWindowFocus()
{
   return focusedWindow;
}

/*------------------------------------------------------------------------
Function Name: RemoveWindowFocus
Parameters:
Description:
   This function removes the focus from the window that currently has
   it and sets the focus to IDW_NONE, meaning no window has the focus.
------------------------------------------------------------------------*/

void Gui::RemoveWindowFocus()
{
   //Inform the previously focused window that it is losing the focus
   if(focusedWindow != IDW_NONE)
      PostMessage(new Message(GM_LOSEFOCUS, focusedWindow, 0, 0, IDW_NONE));

   focusedWindow = IDW_NONE;
}

/*------------------------------------------------------------------------
Function Name: SetMouseCapture
Parameters:
Description:
   This function sets the window that is capturing the mouse. When a
   window captures the mouse, it receives all mouse messages, no matter
   where the mouse actually is.
------------------------------------------------------------------------*/

void Gui::SetMouseCapture(UINT windowID)
{
   mouseCaptureWinID = windowID;
}

/*------------------------------------------------------------------------
Function Name: ReleaseMouseCapture
Parameters:
Description:
   This function releases any mouse capture that is set. Afterward,
   the mouse messages only go to the window which is directly under the
   mouse cursor.
------------------------------------------------------------------------*/

void Gui::ReleaseMouseCapture()
{
   mouseCaptureWinID = IDW_NONE;
}

/*------------------------------------------------------------------------
Function Name: GetMouseCapture
Parameters:
Description:
   This function returns the ID of the window that has captured the mouse.
   If no window is currently capturing the mouse, IDW_NONE is returned.
------------------------------------------------------------------------*/

UINT Gui::GetMouseCapture()
{
   return mouseCaptureWinID;
}

/*------------------------------------------------------------------------
Function Name: CreateTimer
Description:
   This function creates a timer associated with timerID with an interval
   of interval milliseconds. The timer messages are send to windows
   with an ID of windowID.
Parameters:
   UINT timerID : A unique ID for a timer
   UINT windowID : The ID of a window
   UINT interval : The number of milliseconds between timer messages
------------------------------------------------------------------------*/

void Gui::CreateTimer(UINT timerID, UINT windowID, UINT interval)
{
   HWND hWnd = GetApp()->GetWindowsHandle();
   ::SetTimer(hWnd, timerID, interval, NULL);
   timerTable.insert(TimerEntry(timerID, windowID));
}

/*------------------------------------------------------------------------
Function Name: DestroyTimer
Description:
   This function destroy a timer associated with timerID.
Parameters:
   UINT timerID : A unique ID for a timer
------------------------------------------------------------------------*/

void Gui::DestroyTimer(UINT timerID)
{
   ::KillTimer(GetApp()->GetWindowsHandle(), timerID);
   timerTable.erase(timerID);
}

/*------------------------------------------------------------------------
Function Name: DrawGUI
Parameters:
Description:
   This function causes the windows of the GUI to draw themselves on the
   screen.
------------------------------------------------------------------------*/

void Gui::DrawGUI()
{
   mainWindow->DrawWindow();
}