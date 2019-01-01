/*------------------------------------------------------------------------
File Name: DGMessage.h
Description: This file contains the DG::Message class, which represents
   a generic GUI message.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class Message
   {
   public:
      //msgType is the type of message, dest is the ID of the window the
      //message is intended for, d1, d2, d3 are message data, kbState
      //must be a 256 byte array describing the keyboard state or it can
      //be NULL
      Message(UINT msgType, UINT dest, int d1 = 0, 
         int d2 = 0, UINT d3 = 0, BYTE* kbState = NULL)
      {
         messageType = msgType;
         destination = dest;
         data1 = d1;
         data2 = d2;
         data3 = d3;
         if(kbState != NULL)
         {
            keyboardState = new BYTE[256];
            memcpy(keyboardState, kbState, 256);
         }
         else
            keyboardState = NULL;
      }

      virtual ~Message()
      {
         if(keyboardState != NULL)
            delete[] keyboardState;
      }

      UINT messageType;
      UINT destination;
      int data1;
      int data2;
      UINT data3;
      BYTE* keyboardState;
   };

   class ResolutionMessage : public Message
   {
   public:
      ResolutionMessage(UINT msgType, UINT dest, float newX, float newY,
         UINT colorDepth) : Message(msgType, dest)
      {
         newXResPercentage = newX;
         newYResPercentage = newY;
      }

      float newXResPercentage;
      float newYResPercentage;
   };
}

/*The following are the different messages and the data that they need.
The GM_ identifiers are the values that are put in the messageType field.

GM_NONE - This actually a value which describes a lack of a message.
   This value should never be set in a actual message.

Data required: N/A

GM_FOCUS - This message is received by a window that has gained the focus

Data required:
data3 - the ID of the window that previously had the focus

GM_LOSEFOCUS - This message is received by a window that has lost the
   focus.

Data required:
data3 - ID of the window that gained the focus

GM_LBUTTONDOWN - This message is received when the left mouse button
   has been pressed.

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen
the keyboard state

GM_LBUTTONUP - This message is received when the left mouse button
   has been released.

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen
the keyboard state

GM_LBUTTONDBLCLK - This message is received when the left mouse button
   has been double clicked.

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen
the keyboard state

GM_MBUTTONDOWN - This message is received when the middle mouse button
   has been pressed.

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen
the keyboard state

GM_MBUTTONUP - This message is received when the middle mouse button
   has been released.

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen
the keyboard state

GM_MBUTTONDBLCLK - This message is received when the middle mouse button
   has been double clicked.

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen
the keyboard state

GM_RBUTTONDOWN - This message is received when the right mouse button
   has been pressed.

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen
the keyboard state

GM_RBUTTONUP - This message is received when the right mouse button
   has been released.

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen
the keyboard state

GM_RBUTTONDBKCLK - This message is received when the right mouse button
   has been double clicked.

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen
the keyboard state

GM_MOUSEMOVE - This message is received when the mouse cursor moves
   within a window

Data required:
data1 - the absolute x coordinate of the mouse cursor on the screen
data2 - the absolute y coordinate of the mouse cursor on the screen

GM_MOUSEENTER - This message is received when the mouse cursor enters
   the area of a window

Data required:
data1 - the absolute x coordinate of the mouse on the screen
data2 - the absolute y coordinate of the mouse on the screen

GM_MOUSELEAVE - This message is received when the mouse cursor enters
   the area of a window

Data required:
data1 - the absolute x coordinate of the mouse on the screen
data2 - the absolute y coordinate of the mouse on the screen

GM_KEYDOWN - This message is received when a key is pressed

Data required:
data3 - the virtual key code
the keyboard state

GM_KEYUP - This message is received when a key is released

Data required:
data3 - the virtual key code
the keyboard state

GM_KEYPRESSED - This message is received when a key is pressed
   and released.

Data required:
data3 - the virtual key code

GM_CHARACTER - This message is received when a character is generated
   through one or more keystrokes.

Data required:
data3 - the ASCII character code
the keyboard state

GM_MOVING - This message is received when a window is being moved.

Data required:
none

GM_MOVED - This message is received when a window has finished
   being moved.

Data required:
none

GM_REPOSITION - This message is received as a command to reposition
   the window

Data required:
data1 - x-coordinate of the new position
data2 - y-coordinate of the new position

GM_SIZING - This message is received when a window is being resized.

Data required:
none

GM_SIZED - This message is received when a window has finished
   being resized.

Data required:
none

GM_RESIZE - This message is received as a command to resize
   the window

Data required:
data1 - new width of the window
data2 - new height of the window

GM_SHOWWINDOW - This message is received as a command for the window
   to make itself visible

Data required:
none

GM_HIDEWINDOW - This message is received as a command for the window
   to make itself invisible

Data required:
none

GM_DISPLAYCHANGE - This message is received by a window when the 
   display changes to give it a chance to resize itself
   *This message type uses the DGResolutionMessage class*

Data required:
newXResPercentage - percentage of the old screen width represented by 
   the new screen width
newYResPercentage - percentage of the old screen height represented by 
   the new screen height
data3 - the new color depth

GM_CREATE - This message is received by the window after it has been
   constructed and set up to give it a chance to do some initialization

Data required:
none

GM_DESTROY - This message is received by the window just before it is
   to be destroyed.

Data required:
none

GM_HIDECHILDWINDOW - This message is received by a parent window so 
   that it can move its child window into the hidden window list

Data required:
data3 - the ID of the child window to be hidden

GM_SHOWCHILDWINDOW - This message is received by a parent window so 
   that it can move its child window into the shown window list

Data required:
data3 - the ID of the child window to be shown

GM_PARENTREPOSITION - This message is received by a window when its
   parent is repositioned, so that the window can recalculate its
   absolute position

Data required:
none

GM_TIMER - This message is received by a window then a timer exists
   that is sending timer messages to the window after a constant 
   interval

Data required:
data3 - the ID of the timer

-----Control Messages-----

-DGLabel-
GM_LABEL_LBUTTONDOWN,
GM_LABEL_LBUTTONUP,
GM_LABEL_LBUTTONDBLCLK,
GM_LABEL_MBUTTONDOWN,
GM_LABEL_MBUTTONUP,
GM_LABEL_MBUTTONDBLCLK,
GM_LABEL_RBUTTONDOWN,
GM_LABEL_RBUTTONUP,
GM_LABEL_RBUTTONDBKCLK

These messages are all received by a parent window when a child label
control is notifying the parent window of mouse messages

Data required:
data3 - the ID of the label that is sending the message

-DGLabel-
GM_IMAGE_LBUTTONDOWN,
GM_IMAGE_LBUTTONUP,
GM_IMAGE_LBUTTONDBLCLK,
GM_IMAGE_MBUTTONDOWN,
GM_IMAGE_MBUTTONUP,
GM_IMAGE_MBUTTONDBLCLK,
GM_IMAGE_RBUTTONDOWN,
GM_IMAGE_RBUTTONUP,
GM_IMAGE_RBUTTONDBKCLK

These messages are all received by a parent window when a child image
control is notifying the parent window of mouse messages

Data required:
data3 - the ID of the label that is sending the message

-DGButton-
GM_BUTTON_PRESSURE - This message is received by a parent window when
   a button is being put under pressure, which is when the left mouse 
   button is down when the mouse cursor is over the button control.

Data required:
data3 - the ID of the button that is sending the message

GM_BUTTON_CLICKED - This message is received by a parent window when
   the left mouse button has been pressed and released over the button

Data required:
data3 - the ID of the button that is sending the message

-DGEdit-
DG_EDIT_CHANGED - This message is received by a parent window when the
   text inside the edit box changes due to user input

DG_EDIT_ENTER - This message is received by a parent window when the 
   edit box has focus and user presses the enter key

Data required:
data3 - the ID of the edit box that is sending the message
*/
