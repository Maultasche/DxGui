/*------------------------------------------------------------------------
File Name: DGInput.cpp
Description: This file contains the implementation of the DG::Input class, 
   which collects and stores input events, so that they can be passed on 
   to the GUI, which converts them into input messages.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

/*Default Constructor*/
Input::Input()
{
}

/*Destructor*/
Input::~Input()
{
}

/*------------------------------------------------------------------------
Function Name: CreateEvent()
Parameters:
   UINT msg : the MS Windows message id
   WPARAM wparam : the wParam parameter passed to the message handler
   LPARAM lparam : the lParam parameter passed to the message handler
   BYTE* keyboardState : an array which describes the keyboard state
      at the time this message was generated, it must be a 256 byte array
Description:
   This function is called when MS Windows input messages are received
   and put them in an InputEvent queue for later handling in the GUI.
------------------------------------------------------------------------*/

void FC Input::CreateEvent(UINT msg, WPARAM wparam, LPARAM lparam,
   BYTE* keyboardState)
{
   inputQueue.Enqueue(new InputEvent(msg, (UINT)wparam, (LONG)lparam, 
      keyboardState));
}

/*------------------------------------------------------------------------
Function Name: GetEvent()
Parameters:
Description:
   This function returns the first event on the InputEvent queue, but
   leaves that event at the front of the queue.
------------------------------------------------------------------------*/

InputEvent* FC Input::GetEvent()
{
   return inputQueue.GetFirstItem();
}

/*------------------------------------------------------------------------
Function Name: GetEvent()
Parameters:
Description:
   This function returns the first event on the InputEvent queue, and
   removes that event from the queue.
------------------------------------------------------------------------*/

InputEvent* FC Input::RemoveEvent()
{
   return inputQueue.Dequeue();
}