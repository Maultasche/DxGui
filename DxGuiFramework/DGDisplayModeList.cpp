/*------------------------------------------------------------------------
File Name: DGDisplayModeList.cpp
Description: This file contains the implementation of a derivation of 
   DG::LinkedList that is used to store display mode enumerations.
Version:
   1.0.0    17.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

/*------------------------------------------------------------------------
Function Name: DisplayModeSupported
Parameters:
   int width : the width of the display mode
   int height : the height of the display mode
   int colorDepth : the color depth(bit per pixel) of the display mode
Description:
   Receives a width, height, and color depth and returns whether the 
   display mode is in the list. If the display mode is in the list, it
   is supported by DirectDraw.
------------------------------------------------------------------------*/

bool DisplayModeList::DisplayModeSupported(int width, 
   int height, int colorDepth)
{
   //If the list is empty
   if(numOfItems == 0)
      return false;

   bool found = false;

   Node<DisplayMode>* nodePtr = first;
   for(int i = 0; i < numOfItems && !found; i++)
   {
      DisplayMode* displayMode = nodePtr->data;
      if(displayMode->width == width && displayMode->height == height &&
         displayMode->colorDepth == colorDepth)
         found = true;
      nodePtr = nodePtr->next;
   }

   return found;
}