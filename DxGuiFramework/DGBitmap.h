/*------------------------------------------------------------------------
File Name: DGBitmap.h
Description: This file contains the DGGBitmap class, which represents
	a bitmap stored in memory. An object of this class is able to load
	as well as store bitmaps.
Version:
   1.0.0    25.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class Bitmap
   {
   public:
      Bitmap();
      Bitmap(UINT bitmapID, UINT bitmapPriority = 1);
      Bitmap(UINT bitmapID, UINT bitmapPriority, UINT resourceID);
      Bitmap(UINT bitmapID, UINT bitmapPriority, UINT resourceID,
         Area& dimensions);
      Bitmap(UINT bitmapID, UINT bitmapPriority, 
         const char* bitmapFileName);
      Bitmap(UINT bitmapID, UINT bitmapPriority, 
         const char* bitmapFileName, Area& dimensions);
      virtual ~Bitmap();

      void SetID(UINT bitmapID) {id = bitmapID;}
      UINT GetID(void) {return id;}
      void SetPriority(UINT bitmapPriority) {priority = bitmapPriority;}
      UINT GetPriority(void) {return priority;}
      bool IsResourceBitmap(void) {return resourceBitmap;}
      char* const GetFileName(void) {return fileName;}
      Point GetDimensions(void) {return Point(width, height);}
      bool IsLoaded(void) {return isLoaded;}
      int GetWidth(void) {return width;}
      int GetHeight(void) {return height;}

      LPDIRECTDRAWSURFACE7 GetDDSurface(void);
      void FC SetTransparentColor(Color& color);

      void FC LoadBitmap(UINT resourceID);
      void FC LoadBitmap(UINT resourceID, Area& bitmapDimensions);
      void FC LoadBitmap(const char* bitmapFileName);
      void FC LoadBitmap(const char* bitmapFileName, 
         Area& bitmapDimensions);
      void FC DestroyBitmap(void);
      void FC ReloadBitmap(void);
      void FC RestoreBitmap(void);

   private:
      void LoadBitmapFromFile(Area& rect, const char* bitmapFileName);
      Point FindBitmapSize(const char* bitmapFileName);

      UINT id;
      UINT priority;

      bool isLoaded;
      bool useDimensions;
      bool resourceBitmap;
      char fileName[128];

      //The area in the file that this was loaded from
      Area dimensions;

      int width;
      int height;
      UINT colorDepth;

      //The color to draw transparently: only is transparent when
      //a transparent blit function is called.
      Color transparentColor;

      //DirectDraw surface: stores the loaded bitmap
      LPDIRECTDRAWSURFACE7 lpDDSBitmap;

      UINT memoryUsage;
   };
}
