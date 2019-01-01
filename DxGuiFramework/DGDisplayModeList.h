/*------------------------------------------------------------------------
File Name: DGDisplayModeList.h
Description: This file contains a derivation of DG::LinkedList that is 
	used to store display mode enumerations.
Version:
   1.0.0    17.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class DisplayMode
   {
   public:
      int width;
      int height;
      int colorDepth;

      DisplayMode(int w, int h, int clrDepth)
      {
         width = w;
         height = h;
         colorDepth = clrDepth;
      }

   };

   class DisplayModeList : public LinkedList<DisplayMode>
   {
   public:
      //Constructor: destroyItems specifies whether the data 
      //in the list should be deleted when the list is destroyed
      DisplayModeList(bool destroyItems) : 
         LinkedList<DisplayMode>(destroyItems) {}
      DisplayModeList() : LinkedList<DisplayMode>() {}

      //Destructor
      ~DisplayModeList() {}

      bool DisplayModeSupported(int width, int height, int colorDepth);
   };
}
