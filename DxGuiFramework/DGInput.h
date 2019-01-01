/*------------------------------------------------------------------------
File Name: DGInput.h
Description: This file contains the DG::Input class, which collects and
   stores input events, so that they can be passed on to the GUI, which
   converts them into input messages.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class InputEvent
   {
   public:
      InputEvent()
      {
         messageType = 0;
         wParam = 0;
         lParam = 0;
      }

      InputEvent(UINT message, UINT w_param, LONG l_param, BYTE* keyStateArray)
      {
         messageType = message;
         wParam = w_param;
         lParam = l_param;
         memcpy(&keyboardState, keyStateArray, 256);
      }

      UINT messageType;
      UINT wParam;
      LONG lParam;
      BYTE keyboardState[256];
   };


   class Input
   {
   public:
      Input();
      virtual ~Input();

      void FC CreateEvent(UINT msg, WPARAM wparam, LPARAM lparam,
         BYTE* keyboardState);
      InputEvent* FC GetEvent(void);
      InputEvent* FC RemoveEvent(void);

      void FC ClearEventQueue(void) {inputQueue.DeleteAll();}
      bool FC EventsInQueue(void) 
      {return (inputQueue.GetNumOfItems() > 0);}

   private:
      Queue<InputEvent> inputQueue;
   };
}