/*------------------------------------------------------------------------
File Name: DGWindow.cpp
Description: This file contains the implementations of the DG::Window  
	class, which represent a basic window in the GUI.
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
   UINT winID : the ID of the window to be constructed
   bool control : whether the window is a control or not
Description:
   This function constructs the DG::Window object.
------------------------------------------------------------------------*/

Window::Window(UINT winID, bool control)
{
   windowPosition.SetPoint(0, 0);
   windowSize.SetPoint(0, 0);

   windowID = winID;
   windowType = WT_GENERICWINDOW;
   isControl = control;
   
   isCreated = false;
   isDestroyed = false;

   windowShowing = false;
   drawChildWindows = true;
   parentClipping = true;

   parentWindow = NULL;

   CalculateAbsCoords();

   windowList.SetDestroy(true);
   controlList.SetDestroy(true);
   hiddenWindowList.SetDestroy(true);
}

/*------------------------------------------------------------------------
Function Name: Constructor
Parameters:
   UINT winID : the ID of the window to be constructed
   bool control : whether the window is a control or not
   Area dimensions : the dimensions of the window to be constructed
Description:
   This function constructs the DG::Window object.
------------------------------------------------------------------------*/

Window::Window(UINT winID, bool control, const Area& dimensions)
{
   windowPosition.SetPoint(dimensions.left, dimensions.top);
   windowSize.SetPoint(dimensions.width, dimensions.height);

   windowID = winID;
   windowType = WT_GENERICWINDOW;
   isControl = control;

   isCreated = false;
   isDestroyed = false;

   windowShowing = true;
   drawChildWindows = true;
   parentClipping = true;

   parentWindow = NULL;

   CalculateAbsCoords();

   windowList.SetDestroy(true);
   controlList.SetDestroy(true);
   hiddenWindowList.SetDestroy(true);
}

/*------------------------------------------------------------------------
Function Name: Constructor
Parameters:
   UINT winID : the ID of the window to be constructed
   bool control : whether the window is a control or not
   Point position : the position of the window to be constructed
   Point size : the size of the window to be constructed
Description:
   This function constructs the DG::Window object.
------------------------------------------------------------------------*/

Window::Window(UINT winID, bool control, const Point& position, 
                   const Point& size)
{
   windowPosition.SetPoint(position.x, position.y);
   windowSize.SetPoint(size.x, size.y);

   windowID = winID;
   windowType = WT_GENERICWINDOW;
   isControl = control;

   isCreated = false;
   isDestroyed = false;

   windowShowing = true;
   drawChildWindows = true;
   parentClipping = true;

   parentWindow = NULL;

   CalculateAbsCoords();

   windowList.SetDestroy(true);
   controlList.SetDestroy(true);
   hiddenWindowList.SetDestroy(true);
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
   This function constructs the DG::Window object. The window position
   is the position relative to the upper-left corner of the parent 
   window.
------------------------------------------------------------------------*/

Window::Window(UINT winID, bool control, int xPos, int yPos,
                   int width, int height)
{
   windowPosition.SetPoint(xPos, yPos);
   windowSize.SetPoint(width, height);

   windowID = winID;
   windowType = WT_GENERICWINDOW;
   isControl = control;

   isCreated = false;
   isDestroyed = false;

   windowShowing = true;
   drawChildWindows = true;
   parentClipping = true;

   parentWindow = NULL;

   CalculateAbsCoords();

   windowList.SetDestroy(true);
   controlList.SetDestroy(true);
   hiddenWindowList.SetDestroy(true);
}

/*Destructor*/
Window::~Window()
{
   //Destroy the child windows
   windowList.DeleteAll();
   controlList.DeleteAll();
   hiddenWindowList.DeleteAll();

   //Destroy all the message handler functors
   MessageTable::iterator iter = messageTable.begin();

   while(iter != messageTable.end())
   {
      //Get the message handler function from the map
      MsgHandlerInterface* function = (*iter).second;
      
      delete function;

      iter++;
   }
}

/*------------------------------------------------------------------------
Function Name: SetPosition
Parameters:
   const Point& position : a DG::Point object describing a new window
      position
Description:
   This function sets the window to a new position.
------------------------------------------------------------------------*/

void FC Window::SetPosition(const Point& position)
{
   windowPosition = position;

   //Recalculate the absolute coordinates of this window
   CalculateAbsCoords();

   //Tell the child windows that the parent's position has changed so
   //that they can recalculate their absolute position
   ListIterator<Window> iterator = controlList.Begin();
   while(!iterator.EndOfList())
   {
      iterator.GetData()->BroadcastMessage(new Message(GM_PARENTREPOSITION,
         iterator.GetData()->GetWindowID()));
      iterator++;
   }

   iterator = windowList.Begin();
   while(!iterator.EndOfList())
   {
      iterator.GetData()->BroadcastMessage(new Message(GM_PARENTREPOSITION,
         iterator.GetData()->GetWindowID()));
      iterator++;
   }

   iterator = hiddenWindowList.Begin();
   while(!iterator.EndOfList())
   {
      iterator.GetData()->BroadcastMessage(new Message(GM_PARENTREPOSITION,
         iterator.GetData()->GetWindowID()));
      iterator++;
   }

   OnWindowMoved();
}

/*------------------------------------------------------------------------
Function Name: SetPosition
Parameters:
   int xPos : the x-coordinate of the new window position
   int yPos : the y-coordinate of the new window position
Description:
   This function sets the window to a new position.
------------------------------------------------------------------------*/

void FC Window::SetPosition(int xPos, int yPos)
{
   windowPosition.SetPoint(xPos, yPos);

   CalculateAbsCoords();

   //Tell the child windows that the parent's position has changed so
   //that they can recalculate their absolute position
   ListIterator<Window> iterator = controlList.Begin();
   while(!iterator.EndOfList())
   {
      iterator.GetData()->BroadcastMessage(new Message(GM_PARENTREPOSITION,
         iterator.GetData()->GetWindowID()));
      iterator++;
   }

   iterator = windowList.Begin();
   while(!iterator.EndOfList())
   {
      iterator.GetData()->BroadcastMessage(new Message(GM_PARENTREPOSITION,
         iterator.GetData()->GetWindowID()));
      iterator++;
   }

   iterator = hiddenWindowList.Begin();
   while(!iterator.EndOfList())
   {
      iterator.GetData()->BroadcastMessage(new Message(GM_PARENTREPOSITION,
         iterator.GetData()->GetWindowID()));
      iterator++;
   }

   OnWindowMoved();
}  

/*------------------------------------------------------------------------
Function Name: SetSize
Parameters:
   const Point& size : a DG::Point object describing a new window
      size
Description:
   This function resizes the window. It is assumed that the new width
   and height are greater than or equal to 0.
------------------------------------------------------------------------*/

void FC Window::SetSize(const Point& size)
{
   assert(size.y >= 0 && size.y >= 0);
   windowSize = size;
   OnWindowSized();
}

/*------------------------------------------------------------------------
Function Name: SetSize
Parameters:
   int width : the new width of the window
   int height : the new height of the window
Description:
   This function resizes the window. It is assumed that the new width
   and height are greater than or equal to 0.
------------------------------------------------------------------------*/

void FC Window::SetSize(int width, int height)
{
   assert(width >= 0 && height >= 0);
   windowSize.SetPoint(width, height);
   OnWindowSized();
}

/*------------------------------------------------------------------------
Function Name: SetDimensions
Parameters:
   const Area& dimensions : a DG::Area object describing a new window
      position and size
Description:
   This function sets the dimensions of the window. dimensions.left and
   dimensions.top describe the position of the window, dimensions.width
   and dimensions.height describe the width and height of the window.
   It is assumed that the specified width and height >= 0
------------------------------------------------------------------------*/

void FC Window::SetDimensions(const Area& dimensions)
{
   assert(dimensions.width >= 0 && dimensions.height >= 0);
   SetPosition(dimensions.left, dimensions.top);
   SetSize(dimensions.width, dimensions.height);
}

/*------------------------------------------------------------------------
Function Name: SetDimensions
Parameters:
   const Point& position : a DG::Point object describing a new window
      position
   const Point& size : a DG::Point object describing a new window
      size
Description:
   This function sets the dimensions of the window. position describes
   the new position of the window, size.y and size.y describe the new 
   width and height of the window.
   It is assumed that the specified width and height >= 0
------------------------------------------------------------------------*/

void FC Window::SetDimensions(const Point& position, 
                              const Point& size)
{
   assert(size.x >= 0 && size.y >= 0);
   SetPosition(position.x, position.y);
   SetSize(size.x, size.y);
}

/*------------------------------------------------------------------------
Function Name: SetDimensions
Parameters:
   int x : the x-coordinate of the new window position
   int y : the y-coordinate fo the new window position
   width : the new window width
   height : the new window height
Description:
   This function sets the dimensions of the window. 
   It is assumed that the specified width and height >= 0
------------------------------------------------------------------------*/

void FC Window::SetDimensions(int x, int y, int width, int height)
{
   assert(width >= 0 && height >= 0);
   SetPosition(x, y);
   SetSize(width, height);
}

/*------------------------------------------------------------------------
Function Name: HideWindow
Parameters:
Description:
   This function hides the window by setting its windowShowing flag to
   false and telling its parent to put it in the hidden window list.
------------------------------------------------------------------------*/

void FC Window::HideWindow()
{
   windowShowing = false;

   if(parentWindow != NULL)
   {
      GetGui()->PostMessage(new Message(GM_HIDECHILDWINDOW, 
         parentWindow->GetWindowID(), 0, 0, windowID));
   }
}

/*------------------------------------------------------------------------
Function Name: HideWindow
Parameters:
Description:
   This function shows the window by setting its windowShowing flag to
   true and telling its parent to put it in the shown window list.
------------------------------------------------------------------------*/

void FC Window::ShowWindow()
{
   windowShowing = true;

   if(parentWindow != NULL)
   {
      GetGui()->PostMessage(new Message(GM_SHOWCHILDWINDOW, 
         parentWindow->GetWindowID(), 0, 0, windowID));
   }
}

/*------------------------------------------------------------------------
Function Name: DrawWindow
Parameters:
Description:
   This function tells the window to draw itself on the screen. It does
   this by getting a surface from dgGraphics and sending it to the 
   OnDrawWindow function.
------------------------------------------------------------------------*/

void FC Window::DrawWindow()
{
   Area windowArea(absWindowPosition, windowSize);
   Area clippedArea = windowArea;

   //If parent clipping is enabled, we need to clip the window to the 
   //boundaries of its parent
   if(parentClipping && parentWindow != NULL)
   {
      Area parentArea = parentWindow->GetAbsDimensions();
      Rectangle parentRect(parentArea.left, parentArea.top,
         parentArea.Right(), parentArea.Bottom());

      //Check to see if this window will even be drawn, if not, we
      //can exit this function
      if((clippedArea.left > parentRect.right && 
         clippedArea.Right() > parentRect.right) ||
         (clippedArea.left < parentRect.left &&
         clippedArea.Right() < parentRect.left) ||
         (clippedArea.top > parentRect.bottom &&
         clippedArea.Bottom() > parentRect.bottom) ||
         (clippedArea.top < parentRect.top &&
         clippedArea.Bottom() < parentRect.top))
         return;

      //Clip the left and top of this window

      //When we clip the top or left, the screen area must decrease in
      //width or height, since part of the window is now hidden
      int numOfClippedPixels;

      if(clippedArea.left < parentRect.left)
      {
         numOfClippedPixels = parentRect.left - clippedArea.left;
         clippedArea.left = parentRect.left;
         clippedArea.width -= numOfClippedPixels;
      }

      if(clippedArea.top < parentRect.top)
      {
         numOfClippedPixels = parentRect.top - clippedArea.top;
         clippedArea.top = parentRect.top;
         clippedArea.height -= numOfClippedPixels;
      }

      //Clip the bottom and right of this window

      if(clippedArea.Right() > parentRect.right)
         clippedArea.width = (parentRect.right - clippedArea.left) + 1;
      if(clippedArea.Bottom() > parentRect.bottom)
         clippedArea.height = (parentRect.bottom - clippedArea.top) + 1;
   }

   //Get the drawing surface for this window
   //The window position is in relative coords, so we need to get the
   //absolute coords of its upper-left corner
   Surface* surface = dgGraphics->GetSurface(clippedArea);

   //If the surface is NULL, someone else has not released their surface
   assert(surface != NULL);

   //Calculate the window origin in surface coordinates
   Point windowOrigin(absWindowPosition.x - clippedArea.left, 
      absWindowPosition.y - clippedArea.top);
  
   WindowSurface* windowSurface = new WindowSurface(surface, windowOrigin);

   //Send the surface to be drawn upon
   OnDrawWindow(windowSurface);

   delete windowSurface;

   //Release the drawing surface
   dgGraphics->ReleaseSurface(surface);

   //If we aren't supposed to draw any child windows, then don't
   if(drawChildWindows)
   {
      //Iterate through the child windows and draw them in the 
      //correct z-order

      //Draw the controls first, since they must be under the windows
      ListIterator<Window> iterator = controlList.Begin();
      while(!iterator.EndOfList())
      {
         iterator.GetData()->DrawWindow();
         iterator++;
      }

      iterator = windowList.Begin();
      while(!iterator.EndOfList())
      {
         iterator.GetData()->DrawWindow();
         iterator++;
      }
   }
}

//Child Window Functions

/*------------------------------------------------------------------------
Function Name: AddChildWindow
Parameters:
   Window* childWindow : the child window to be added to this window
Description:
   This function receives a pointer to a window and makes it this 
   window's child by putting it in a child window list. The window
   will automatically delete any child windows when it is destroyed.
------------------------------------------------------------------------*/

void FC Window::AddChildWindow(Window* childWindow)
{
   assert(childWindow != NULL);

   if(childWindow->IsWindowShowing())
   {
      if(childWindow->IsControl())
         controlList.Append(childWindow, childWindow->GetWindowID());
      else
         windowList.Append(childWindow, childWindow->GetWindowID());
   }
   else
   {
      hiddenWindowList.Append(childWindow, childWindow->GetWindowID());
   }

   childWindow->SetParent(this);

   childWindow->Create();

   //Send the newly created window a size message, so that it can 
   //resize as necessary
   Message* msg = new Message(GM_SIZED, childWindow->GetWindowID());
   childWindow->SendMessage(msg);
   delete msg;
}

/*------------------------------------------------------------------------
Function Name: GetWindow
Parameters:
   UINT winID : the ID of a window
Description:
   This function receives a window ID and searches down the hierarchy
   for a window with that ID. If a window with the specified ID is found,
   a pointer to that window is returned. If no window with the specified
   ID is found, NULL is returned. There better not be any circular 
   dependencies in the window hierarchy otherwise this function will
   never finish.
------------------------------------------------------------------------*/

Window* FC Window::GetWindow(UINT winID)
{
   //If this window is the one we are looking for, return a pointer to it.
   if(windowID == winID)
      return this;

   //If this window is not the one we are looking for or if it is a control,
   //then the window with winID is not to be found here
   else if(isControl || (windowList.GetNumOfItems() == 0 && 
      controlList.GetNumOfItems() == 0 &&
      hiddenWindowList.GetNumOfItems() == 0))
      return NULL;

   else
   {
      Window* winPtr = NULL;
      winPtr = windowList.GetItemById(winID);
      if(winPtr == NULL)
      {
         winPtr = controlList.GetItemById(winID);
         if(winPtr == NULL)
            winPtr = hiddenWindowList.GetItemById(winID);
      }

      return winPtr;
   }
}

/*------------------------------------------------------------------------
Function Name: DestroyWindow
Parameters:
   UINT winID : the ID of a window
Description:
   This function receives a window ID and searches down the hierarchy
   for a window with that ID. If a window with the specified ID is found,
   it is deleted any references to it are removed from its parent window.
   The function returns true if the window was found and deleted and
   false if the window with the specified ID could not be found.
   There better not be any circular dependencies in the window hierarchy 
   otherwise this function will never finish.
------------------------------------------------------------------------*/

bool FC Window::DestroyWindow(UINT winID)
{
   //If this window is the one we are looking for, tell its parent to
   //delete it
   if(windowID == winID)
   {
      if(parentWindow == NULL)
      {
         Destroy();
         delete this;
      }
      else
         parentWindow->DestroyWindow(winID);

      return true;
   }

   else
   {
      bool found = false;
      Window* window;

      window = windowList.RemoveById(winID);
      if(window != NULL)
      {
         Destroy();
         delete window;
         found = true;
      }
      
      if(!found)
      {
         window = controlList.RemoveById(winID);
         if(window != NULL)
         {
            Destroy();
            delete window;
            found = true;
         }

         if(!found)
         {
            window = hiddenWindowList.RemoveById(winID);
            if(window != NULL)
            {
               Destroy();
               delete window;
               found = true;
            }
         }
      }

      return found;
   }
}

/*------------------------------------------------------------------------
Function Name: SetTopMostChildWindow
Parameters:
   UINT winID : the ID of a window
Description:
   This function receives a window ID and puts the child window 
   corresponding to that ID on the top of the z-ordered list.
   If the window corresponding to winID cannot be found or if it is in
   the hidden windows list, nothing will happen.
------------------------------------------------------------------------*/

void FC Window::SetTopMostChildWindow(UINT winID)
{
   Window* window;

   //Look for the window in the shown window list
   window = windowList.RemoveById(winID);

   //If the window was found, put it at the top of the z-order
   if(window != NULL)
      windowList.Append(window, window->GetWindowID());
   //If the window was not found, look for it among the controls
   else
   {
      window = controlList.RemoveById(winID);
      if(window != NULL)
         controlList.Append(window, window->GetWindowID());
   }
}

//Message Functions

/*------------------------------------------------------------------------
Function Name: SendMessage
Parameters:
   Message* msg : a pointer to the message to be sent
Description:
   This function receives a pointer to a message that is to be sent.
   The message filters down through the window hierarchy so that it gets
   to its destination window. If the destination window is not within
   the hierarchy under this window, false is returned indicating that 
   the message did not reach its destination window. If the message did
   reach its destination window, true is returned.
------------------------------------------------------------------------*/

bool FC Window::SendMessage(Message* msg)
{
   //If this window is the intended recipient of the message, then handle
   //the message.
   if(windowID == msg->destination)
   {
      //If this window is a control and the message is a mouse message,
      //pass the message on to the child window that it applies to
      if(isControl && msg->messageType >= GM_LBUTTONDOWN &&
         msg->messageType <= GM_MOUSELEAVE)
      {
         //Find the window ID of any child window with the mouse coordinates
         UINT winID = GetChildWinIDOfCoords(msg->data1, msg->data2);

         //If there is no child window, then this window will handle the message
         if(winID == IDW_NONE)
            HandleMessage(msg);

         //If an appropriate child window is found, send it the message
         else
         {
            msg->destination = winID;

            //This will not cause infinite recursion, because the destination
            //window ID has changed
            SendChildMessage(msg); 
         }
      }
      else
         HandleMessage(msg);

      return true;
   }

   //If this window is a control, but is not the intended recipient of
   //the message, then return false. Any child windows of a control 
   //aren't visible to the messaging system,
   else if(isControl)
      return false;

   //If not a control, pass the message down through the child windows
   else
   {
      bool windowFound = false;

      ListIterator<Window> iterator = windowList.Begin();
      while(!iterator.EndOfList() && !windowFound)
      {
         windowFound = iterator.GetData()->SendMessage(msg);
         iterator++;
      }

      iterator = controlList.Begin();
      while(!iterator.EndOfList() && !windowFound)
      {
         windowFound = iterator.GetData()->SendMessage(msg);
         iterator++;
      }

      iterator = hiddenWindowList.Begin();
      while(!iterator.EndOfList() && !windowFound)
      {
         windowFound = iterator.GetData()->SendMessage(msg);
         iterator++;
      }

      return windowFound;
   }
}

/*------------------------------------------------------------------------
Function Name: SendChildMessage
Parameters:
   Message* msg : a pointer to the message to be sent
Description:
   This function receives a pointer to a message that is to be sent.
   The message filters down through the window hierarchy so that it gets
   to its destination window. If the destination window is not within
   the hierarchy under this window, false is returned indicating that 
   the message did not reach its destination window. If the message did
   reach its destination window, true is returned. The difference 
   between this method and SendMessage is that this window does
   not a chance to look at the message; the message is send only to the
   children of this window. This is useful to avoid messages being 
   blocked if this window is a control.
------------------------------------------------------------------------*/

bool FC Window::SendChildMessage(Message* msg)
{
   bool windowFound = false;

   ListIterator<Window> iterator = windowList.Begin();
   while(!iterator.EndOfList() && !windowFound)
   {
      windowFound = iterator.GetData()->SendMessage(msg);
      iterator++;
   }

   iterator = controlList.Begin();
   while(!iterator.EndOfList() && !windowFound)
   {
      windowFound = iterator.GetData()->SendMessage(msg);
      iterator++;
   }

   iterator = hiddenWindowList.Begin();
   while(!iterator.EndOfList() && !windowFound)
   {
      windowFound = iterator.GetData()->SendMessage(msg);
      iterator++;
   }

   return windowFound;
}

/*------------------------------------------------------------------------
Function Name: BroadcastMessage
Parameters:
   Message* msg : a pointer to the message to be sent
Description:
   This function receives a pointer to a message that is to be sent.
   The message filters down through the window hierarchy and gets handled
   by every window in the hierarchy. This is useful for messages that
   have no specific recipient and need to be broadcast throughout the
   window hierarchy (display resolution change messages, for example).
   Any message is handled by this window first and then it is propagated
   onward to the children.
------------------------------------------------------------------------*/

void FC Window::BroadcastMessage(Message* msg)
{
   HandleMessage(msg);

   ListIterator<Window> iterator = windowList.Begin();
   while(!iterator.EndOfList())
   {
      iterator.GetData()->BroadcastMessage(msg);
      iterator++;
   }

   iterator = controlList.Begin();
   while(!iterator.EndOfList())
   {
      iterator.GetData()->BroadcastMessage(msg);
      iterator++;
   }

   iterator = hiddenWindowList.Begin();
   while(!iterator.EndOfList())
   {
      iterator.GetData()->BroadcastMessage(msg);
      iterator++;
   }

}

/*------------------------------------------------------------------------
Function Name: AddMessageHandler
Parameters:
   UINT messageType : a message type
   MsgHandlerFunction function : the function which will handle that
      type of message
Description:
   This function receives a message type and a function pointer and maps
   the message type to the function by storing the two parameters in
   the message table. Any further messages of the specified type received
   by this window will be handled by the specified function. If the 
   message type already exists in the table, this function will not do
   anything. The message handler entry must be removed before another
   message handler can be added for that message.
------------------------------------------------------------------------*/

void FC Window::AddMessageHandler(UINT messageType, MsgHandlerInterface* function)
{
   if(messageTable.count(messageType) == 0)
      messageTable.insert(MessageEntry(messageType, function));
}

/*------------------------------------------------------------------------
Function Name: RemoveMessageHandler
Parameters:
   UINT messageType : a message type
Description:
   This function receives a message type and removes any message handler
   associated with the message type from message table. If no message
   handler is associated with that message type when this function is
   called, nothing will happen.
------------------------------------------------------------------------*/

void FC Window::RemoveMessageHandler(UINT messageType)
{
   messageTable.erase(messageType);
}

/*------------------------------------------------------------------------
Function Name: HandleMessage
Parameters:
   Message msg : the message to be handled
Description:
   This function receives a pointer to a message to be handled and handles
   that message by calling the message handler associated with that 
   message type.
------------------------------------------------------------------------*/

void FC Window::HandleMessage(Message* msg)
{
   Point point;

   switch(msg->messageType)
   {
      case GM_NONE:
         //We actually shouldn't be getting a message of this type
         break;
      case GM_FOCUS:
         OnSetFocus(msg->data3);
         break;
      case GM_LOSEFOCUS:
         OnLoseFocus(msg->data3);
         break;
      case GM_LBUTTONDOWN:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnLButtonDown(point.x, point.y, msg->keyboardState);
         break;
      case GM_LBUTTONUP:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnLButtonUp(point.x, point.y, msg->keyboardState);
         break;
      case GM_LBUTTONDBLCLK:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnLButtonDblClk(point.x, point.y, msg->keyboardState);
         break;
      case GM_MBUTTONDOWN:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnMButtonDown(point.x, point.y, msg->keyboardState);
         break;
      case GM_MBUTTONUP:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnMButtonUp(point.x, point.y, msg->keyboardState);
         break;
      case GM_MBUTTONDBLCLK:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnMButtonDblClk(point.x, point.y, msg->keyboardState);
         break;
      case GM_RBUTTONDOWN:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnRButtonDown(point.x, point.y, msg->keyboardState);
         break;
      case GM_RBUTTONUP:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnRButtonUp(point.x, point.y, msg->keyboardState);
         break;
      case GM_RBUTTONDBKCLK:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnRButtonDblClk(point.x, point.y, msg->keyboardState);
         break;
      case GM_MOUSEMOVE:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnMouseMove(point.x, point.y, msg->keyboardState);
         break;
      case GM_MOUSEENTER:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnMouseEnter(point.x, point.y, msg->keyboardState);
         break;
      case GM_MOUSELEAVE:
         point = AbsToRelCoords(msg->data1, msg->data2);
         OnMouseLeave(point.x, point.y, msg->keyboardState);
         break;
      case GM_KEYDOWN:
         OnKeyDown((BYTE)msg->data3, msg->keyboardState);
         break;
      case GM_KEYUP:
         OnKeyDown((BYTE)msg->data3, msg->keyboardState);
         break;
      case GM_CHARACTER:
         OnCharacter((char)msg->data3, msg->keyboardState);
         break;
      case GM_MOVING:
         OnWindowMoving();
         break;
      case GM_MOVED:
         OnWindowMoved();
         break;
      case GM_REPOSITION:
         OnWindowReposition(msg->data1, msg->data2);
         break;
      case GM_SIZING:
         OnWindowSizing();
         break;
      case GM_SIZED:
         OnWindowSized();
         break;
      case GM_RESIZE:
         OnWindowResize(msg->data1, msg->data2);
         break;
      case GM_SHOWWINDOW:
         OnShowWindow();
         break;
      case GM_HIDEWINDOW:
         OnHideWindow();
         break;
      case GM_DISPLAYCHANGE:
      {
         ResolutionMessage* resMsg = (ResolutionMessage*)msg;
         OnDisplayChange(resMsg->newXResPercentage, resMsg->newYResPercentage,
            resMsg->data3);
         break;
      }
      case GM_CREATE:
         OnCreate();
         break;
      case GM_DESTROY:
         OnDestroy();
         break;
      case GM_HIDECHILDWINDOW:
         OnShowChildWindow(msg->data3);
         break;
      case GM_SHOWCHILDWINDOW:
         OnHideChildWindow(msg->data3);
         break;
      case GM_PARENTREPOSITION:
         OnParentReposition();
         break;
      case GM_TIMER:
         OnTimer(msg->data3);
         break;
      default:
         //If the message was not a basic framework message, then look
         //for it in the message table
         MessageTable::iterator it = messageTable.find(msg->messageType);
         if(it != messageTable.end())
         {
            //Get the message handler function from the map
            MsgHandlerInterface* function = (*it).second;

            //Call the message handler function, passing it the message
            function->Call(msg);
         }
   }
}

/*------------------------------------------------------------------------
Function Name: Create
Parameters:
Description:
   This function is called to allow the window to call the OnCreate()
   function. Messaging doesn't work because the window will not be
   part of the window hierarchy when it is constructed. This window
   *must* be within the GUI window hierarchy when this function is 
   called.
------------------------------------------------------------------------*/

void FC Window::Create()
{
   if(!isCreated)
   {
      OnCreate();
      isCreated = true;
   }
}

/*------------------------------------------------------------------------
Function Name: Destroy
Parameters:
Description:
   This function is called to allow the window to call the OnDestroy()
   function. Messaging doesn't work because the window probably won't
   exist by the time the GM_DESTROY message is dispatched. This window
   must *still* be within the GUI window hierarchy when this function
   is called.
------------------------------------------------------------------------*/

void FC Window::Destroy()
{
   if(!isDestroyed)
   {
      OnDestroy();
      isDestroyed = true;
   }
}


//Default Message Handlers

/*------------------------------------------------------------------------
Function Name: OnSetFocus
Parameters:
   winID : the ID of the window that lost the focus
Description:
   This function is called when a GM_FOCUS message is received, meaning
   that the window has gained the focus.
------------------------------------------------------------------------*/

void FC Window::OnSetFocus(UINT winID)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnLoseFocus
Parameters:
   winID : the ID of the window that gained the focus
Description:
   This function is called when a GM_LOSEFOCUS message is received, meaning
   that the window has lost the focus.
------------------------------------------------------------------------*/

void FC Window::OnLoseFocus(UINT winID)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnLButtonDown
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_LBUTTONDOWN message is received, 
   meaning that the left mouse button was pushed down within the window.
------------------------------------------------------------------------*/

void FC Window::OnLButtonDown(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnLButtonUp
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_LBUTTONUP message is received, 
   meaning that the left mouse button was released within the window.
------------------------------------------------------------------------*/

void FC Window::OnLButtonUp(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnLButtonDblClk
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_LBUTTONDBLCLK message is received, 
   meaning that the left mouse button was double clicked within 
   the window.
------------------------------------------------------------------------*/

void FC Window::OnLButtonDblClk(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnMButtonDown
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_MBUTTONDOWN message is received, 
   meaning that the middle mouse button was pushed down within 
   the window.
------------------------------------------------------------------------*/

void FC Window::OnMButtonDown(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnMButtonUp
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_MBUTTONUP message is received, 
   meaning that the middle mouse button was released within the window.
------------------------------------------------------------------------*/

void FC Window::OnMButtonUp(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnMButtonDblClk
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_MBUTTONDBLCLK message is received, 
   meaning that the middle mouse button was double clicked within 
   the window.
------------------------------------------------------------------------*/

void FC Window::OnMButtonDblClk(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnRButtonDown
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_RBUTTONDOWN message is received, 
   meaning that the right mouse button was pushed down within the window.
------------------------------------------------------------------------*/

void FC Window::OnRButtonDown(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnRButtonUp
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_RBUTTONUP message is received, 
   meaning that the right mouse button was released within the window.
------------------------------------------------------------------------*/

void FC Window::OnRButtonUp(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnRButtonDblClk
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_RBUTTONDBLCLK message is received, 
   meaning that the right mouse button was double clicked within 
   the window.
------------------------------------------------------------------------*/

void FC Window::OnRButtonDblClk(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnMouseMove
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_MOUSEMOVE message is received, 
   meaning that the mouse cursor was moved withing the window
------------------------------------------------------------------------*/

void FC Window::OnMouseMove(int x, int y, BYTE* keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnMouseEnter
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_MOUSEENTER message is received, 
   meaning that the mouse cursor has entered the window
------------------------------------------------------------------------*/

void FC Window::OnMouseEnter(int x, int y, BYTE* keyboardState)
{
   //Do Nothing
}

/*------------------------------------------------------------------------
Function Name: OnMouseLeave
Parameters:
   int x - the relative x-coordinate of the mouse cursor
   int y - the relative y-coordinate of the mouse cursor
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_MOUSELEAVE message is received, 
   meaning that the mouse cursor has left the window
------------------------------------------------------------------------*/

void FC Window::OnMouseLeave(int x, int y, BYTE* keyboardState)
{
   //Do Nothing
}

/*------------------------------------------------------------------------
Function Name: OnKeyDown
Parameters:
   BYTE keyCode - the virtual key code corresponding to the key being
      pressed.
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_KEYDOWN message is received, 
   meaning that a key has been pressed when the window has the focus.
------------------------------------------------------------------------*/

void FC Window::OnKeyDown(BYTE keyCode, PBYTE keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnKeyPressed
Parameters:
   BYTE keyCode - the virtual key code corresponding to the key being
      released.
   BYTE* keyboardState - a pointer to a 256-byte array describing the
      state of the keys and the mouse buttons
Description:
   This function is called when a GM_KEYUP or a GM_KEYPRESSED message 
   is received, meaning that a key has been released when the window 
   has the focus. Often when wanting to know if a key has been pressed,
   it is useful to do something just after it is released.
------------------------------------------------------------------------*/

void FC Window::OnKeyPressed(BYTE keyCode, PBYTE keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnCharacter
Parameters:
   char character - the character that was generated by the keyboard
Description:
   This function is called when a GM_CHARACTER message is received, 
   which is usually the result of keys pressed on the keyboard. The 
   character is a normal ASCII character.
------------------------------------------------------------------------*/

void FC Window::OnCharacter(char character, PBYTE keyboardState)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnWindowMoving
Parameters:
Description:
   This function is called when a GM_MOVING message is received, 
   meaning that the window is currently being moved. This message is not
   generated automatically by the framework, but has to be sent by a 
   control.
------------------------------------------------------------------------*/

void FC Window::OnWindowMoving()
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnWindowMoved
Parameters:
Description:
   This function is called when a GM_MOVED message is received, 
   meaning that the window has been moved and that the moving operation
   has been completed. This message is not generated automatically by 
   the framework, but has to be sent by a control.
------------------------------------------------------------------------*/

void FC Window::OnWindowMoved()
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnWindowReposition
Parameters:
   int xPos - the x-coordinate of the new window position
   int yPos - the y-coordinate of the new window position
Description:
   This function is called when a GM_REPOSITION message is received, 
   meaning that the window is being commanded to move to the specified
   coordinates. This message is not generated automatically by 
   the framework, but has to be sent by a control.
------------------------------------------------------------------------*/

void FC Window::OnWindowReposition(int xPos, int yPos)
{
   SetPosition(xPos, yPos);
}

/*------------------------------------------------------------------------
Function Name: OnWindowSizing
Parameters:
Description:
   This function is called when a GM_SIZING message is received, 
   meaning that the window is currently being resized. This message is 
   not generated automatically by the framework, but has to be sent by a 
   control.
------------------------------------------------------------------------*/

void FC Window::OnWindowSizing()
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnWindowSized
Parameters:
Description:
   This function is called when a GM_SIZED message is received, 
   meaning that the window has been resized and that the resizing 
   operation has been completed. This message is not generated 
   automatically by the framework, but has to be sent by a control.
------------------------------------------------------------------------*/

void FC Window::OnWindowSized()
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnWindowResize
Parameters:
   int width - the new width of the window
   int height - the new height of the window
Description:
   This function is called when a GM_RESIZE message is received, 
   meaning that the window is being commanded to resize to the specified
   width and height. This message is not generated automatically by 
   the framework, but has to be sent by a control.
------------------------------------------------------------------------*/

void FC Window::OnWindowResize(int width, int height)
{
   SetSize(width, height);
}

/*------------------------------------------------------------------------
Function Name: OnShowWindow
Parameters:
Description:
   This function is called when a GM_SHOWWINDOW message is received, 
   meaning that the window is being commanded to show itself.
------------------------------------------------------------------------*/

void FC Window::OnShowWindow()
{
   ShowWindow();
}

/*------------------------------------------------------------------------
Function Name: OnShowWindow
Parameters:
Description:
   This function is called when a GM_HIDEWINDOW message is received, 
   meaning that the window is being commanded to hide itself.
------------------------------------------------------------------------*/

void FC Window::OnHideWindow()
{
   HideWindow();
}

/*------------------------------------------------------------------------
Function Name: OnDisplayChange
Parameters:
   float xRelChange - the percentage of the resolution width change 
   float yRelChange - the percentage of the resolution height change
   UINT colorDepth - the color depth of the new graphics mode
Description:
   This function is called when a GM_DISPLAYCHANGE message is received, 
   meaning that the resolution and/or color depth has change. The default
   behavior of this function is to change to size and positon of the 
   window so that it matches the resolution change.
------------------------------------------------------------------------*/

void FC Window::OnDisplayChange(float xRelChange, float yRelChange,
                                  UINT colorDepth)
{
   SetPosition((int)(windowPosition.x * xRelChange), 
      (int)(windowPosition.y * yRelChange));
   SetSize((int)(windowSize.x * xRelChange), 
      (int)(windowSize.y * yRelChange));
}

/*------------------------------------------------------------------------
Function Name: OnCreate
Parameters:
Description:
   This function is called when the window is being created. At this 
   point the constructor has already been called and the window is
   assumed to be within the GUI window hierarcy.
------------------------------------------------------------------------*/

void Window::OnCreate()
{
   //Set a title bar message handler
   AddMessageHandler(GM_TITLEBAR_DOWN, 
      new MsgHandlerFunction<Window>(this, OnTitleBarDown));
}

/*------------------------------------------------------------------------
Function Name: OnDestroy
Parameters:
Description:
   This function is called when the window is being destroyed. At this 
   point, the destructor has not been called yet and the window is 
   assumed to be within the GUI window hierarchy.
------------------------------------------------------------------------*/

void FC Window::OnDestroy()
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnShowChildWindow
Parameters:
Description:
   This function is called when a GM_SHOWCHILDWINDOW message is received, 
   meaning that the window should show one of its child windows. This is
   necessary in the showing of windows, as they rely on which window list
   they are in. When showing a child, the child is automatically brought
   to the top of the z-order, which is at the list
------------------------------------------------------------------------*/

void FC Window::OnShowChildWindow(UINT winID)
{
   //We need to look in all lists, since the user might just want to
   //bring the window to the top of the z-order

   Window* window = windowList.GetItemById(winID);

   if(window != NULL)
   {
      windowList.RemoveById(winID);
      if(window->IsControl())
         controlList.Append(window);
      else
         windowList.Append(window);
   }

   window = controlList.GetItemById(winID);

   if(window != NULL)
   {
      controlList.RemoveById(winID);
      if(window->IsControl())
         controlList.Append(window);
      else
         windowList.Append(window);
   }

   window = hiddenWindowList.GetItemById(winID);

   if(window != NULL)
   {
      hiddenWindowList.RemoveById(winID);
      if(window->IsControl())
         controlList.Append(window);
      else
         windowList.Append(window);
   }
}

/*------------------------------------------------------------------------
Function Name: OnHideChildWindow
Parameters:
   UINT winID - the ID of the child window that should be hidden
Description:
   This function is called when a GM_HIDECHILDWINDOW message is received, 
   meaning that the window should hide one of its child windows. This is
   necessary in the hiding of windows, as they rely on which window list
   they are in.
------------------------------------------------------------------------*/

void FC Window::OnHideChildWindow(UINT winID)
{
   //We only need to look in the control and shown window lists
   Window* window = windowList.RemoveById(winID);

   if(window != NULL)
      hiddenWindowList.Insert(window);

   else
   {
      Window* window = controlList.RemoveById(winID);

      if(window != NULL)
         hiddenWindowList.Insert(window);
   }
}

/*------------------------------------------------------------------------
Function Name: OnHideChildWindow
Parameters:
Description:
   This function is called when a GM_PARENTREPOSITION message is received, 
   meaning that the parent window has changed its position. The absolute
   position of this window must then be recalculated.
------------------------------------------------------------------------*/

void FC Window::OnParentReposition()
{
   CalculateAbsCoords();
}

/*------------------------------------------------------------------------
Function Name: OnTimer
Description:
   This function is called when a GM_TIMER message is received, 
   meaning that there is an active timer sending messages to this window.
Parameters:
   UINT timerID: the ID of the timer sending the message
------------------------------------------------------------------------*/

void FC Window::OnTimer(UINT timerID)
{
   //Do nothing
}

/*------------------------------------------------------------------------
Function Name: OnDrawWindow
Parameters:
   Surface* surface : the surface upon which to draw
   Point windowOrigin : the surface coordinates of the upper-left
      corner of this window
Description:
   This function is called when the window is instructed to draw itself.
   The given surface must not be destroyed during the function. It will
   be destroyed by the framework. The window origin must be used, since
   the surface represents a physical area on the screen and not the 
   entire surface of the window. The upper-left corner of the surface
   is not necessarily the upper-left corner of the window.
------------------------------------------------------------------------*/

void FC Window::OnDrawWindow(WindowSurface* surface)
{

}

//Control Message Handlers

/*------------------------------------------------------------------------
Function Name: OnTitleBarDown
Parameters:
   Message* msg : a pointer to a GM_TITLEBAR_DOWN message 
Description:
   This function is called when the title bar for the window is pushed.
   If this window has a parent, this window will be brought to the top
   of the parent's z-order list.
------------------------------------------------------------------------*/

void Window::OnTitleBarDown(Message* msg)
{
   Window* parent = GetParent();
   if(parent != NULL)
      parent->SetTopMostChildWindow(GetWindowID());
}

//Window Coordinate Functions

/*------------------------------------------------------------------------
Function Name: AreCoordsInWindow
Parameters:
   MainWindow mainWin : a pair of absolute coordinates
Description:
   This function receives a pair of absolute coordinates and returns
   true if those coordinates are within the window, false otherwise.
------------------------------------------------------------------------*/

bool FC Window::AreCoordsInWindow(int x, int y)
{
   if(x >= absWindowPosition.x && 
      x < absWindowPosition.x + windowSize.x && 
      y >= absWindowPosition.y && 
      y < absWindowPosition.y + windowSize.y)
      return true;
   else
      return false;
}

/*------------------------------------------------------------------------
Function Name: GetWinIDOfCoords
Parameters:
   int x : the x-value of a pair of absolute coordinates
   int y : the y-value of a pair of absolute coordinates
Description:
   This function receives a pair of absolute coordinates and returns the
   ID of the topmost window that contains that point on the screen.
   If no window in the window hierarchy contains that point, IDW_NONE is
   returned.
------------------------------------------------------------------------*/

UINT FC Window::GetWinIDOfCoords(int x, int y)
{
   if(!AreCoordsInWindow(x, y))
      return IDW_NONE;

   //If the coordinates are in this window and it is a control,
   //then ignore all child windows and return the ID of this window
   if(isControl)
      return windowID;

   //If not a control, look at the child windows
   UINT winID = GetChildWinIDOfCoords(x, y);

   //If none of the child windows has those coordinates, then this
   //window is the one that is within those coordinates
   if(winID == IDW_NONE)
      winID = windowID;

   return winID;
}

/*------------------------------------------------------------------------
Function Name: GetChildWinIDOfCoords
Parameters:
   int x : the x-value of a pair of absolute coordinates
   int y : the y-value of a pair of absolute coordinates
Description:
   This method receives a pair of absolute coordinates and returns the
   ID of the topmost *child* window that contains that point on the 
   screen. Note that unlike GetWinIDOfCoords, this method does not check
   this window, but is only interested in child windows
   If no child window contains the point, IDW_NONE is returned.
------------------------------------------------------------------------*/

UINT FC Window::GetChildWinIDOfCoords(int x, int y)
{
   UINT winID = IDW_NONE;
   Window* window = NULL;

   //NOTE: We don't stop in these while loops when we find a matching
   //window, because windows are ordered with the topmost at the end
   //of the list. The topmost matching window is the one we are 
   //interested in. We should actually be iterating backward through
   //the list, but DG::LinkedList doesn't support that at the moment.
   //We should eventually convert to a standard STL double-linked list

   //NOTE: The changes to these loops (which are due to the fact we
   //can't yet iterate backward in the list) have made the GUI much
   //slower. Fixing the list iteration will speed things up significantly

   //Loop through the shown windows
   ListIterator<Window> iterator = windowList.Begin();
   while(!iterator.EndOfList())
   {
      window = iterator.GetData();
      int id = window->GetWinIDOfCoords(x, y);
      if(id != IDW_NONE)
         winID = id;

      iterator++;
   }

   //If we've found it, return it
   if(winID != IDW_NONE)
      return winID;

   //Loop through the controls
   iterator = controlList.Begin();
   while(!iterator.EndOfList())
   {
      window = iterator.GetData();
      int id = window->GetWinIDOfCoords(x, y);
      if(id != IDW_NONE)
         winID = id;

      iterator++;
   }

   return winID;
}

/*------------------------------------------------------------------------
Function Name: AbsToRelCoords
Parameters:
   int x : the x-value of a pair of absolute coordinates
   int y : the y-value of a pair of absolute coordinates
Description:
   This function receives a pair of absolute coordinates and returns a
   pair of coordinates relative to the upper-left corner of the window.
------------------------------------------------------------------------*/

Point FC Window::AbsToRelCoords(int x, int y)
{
   return Point(x - absWindowPosition.x, y - absWindowPosition.y);
}

/*------------------------------------------------------------------------
Function Name: GetWinIDOfCoords
Parameters:
   int x : the x-value of a pair of relative coordinates
   int y : the y-value of a pair of relative coordinates
Description:
   This function receives a pair of coordinates relative to the 
   upper-left corner of the window and returns a
   pair of absolute coordinates.
------------------------------------------------------------------------*/

Point FC Window::RelToAbsCoords(int x, int y)
{
   return Point(x + absWindowPosition.x, y + absWindowPosition.y);
}

/*------------------------------------------------------------------------
Function Name: CalculateAbsCoords
Parameters:
Description:
   This function calculates the absolute position of the window based
   on the window's current position, which is its position relative to
   the upper-left corner of its parent window
------------------------------------------------------------------------*/

void Window::CalculateAbsCoords()
{
   //If there is a parent, get the absolute coords from it
   if(parentWindow != NULL)
      absWindowPosition = parentWindow->RelToAbsCoords(windowPosition.x,
         windowPosition.y);

   //Otherwise, the relative window position is relative to the screen,
   //so the relative coords are the same as absolute coords
   else
      absWindowPosition = windowPosition;
}