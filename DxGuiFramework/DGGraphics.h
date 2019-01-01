/*------------------------------------------------------------------------
File Name: DGGraphics.h
Description: This file contains the DG::Graphics class, which manages
   the DirectDraw surfaces and controls and what is drawn to the screen.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class Surface;

   class Graphics
   {
      friend HRESULT CALLBACK EnumDisplayModes(LPDDSURFACEDESC2 lpDDSurfaceDesc,  
         LPVOID lpContext);
      friend class Bitmap;

   public:
      Graphics();
      virtual ~Graphics();

      void SetResolution(Point res);
      void SetResolution(int hRes, int vRes);
      const Point& GetResolution(void) {return screenRes;}

      void SetWindowedState(UINT winState);
      UINT GetWindowedState(void) {return windowedState;}

      void SetColorDepth(UINT clrDepth);
      UINT GetColorDepth() {return colorDepth;}

      void SetBufferingMode(UINT bufferMode);
      UINT GetBufferingMode(void) {return bufferingMode;}

      void SetSurfacePersistence(bool persistence);
      bool GetSurfacePersistence(void) {return SSPersistence;}

      void SetClientRect(RECT& rect) {clientRect = rect;}

      void SetGraphicsMode(Point res, UINT winState, UINT clrDepth,
         UINT bufferMode = BT_SINGLE, bool persistence = false);

      void BeginFrame(void);
      void EndFrame(void);

      //Bitmap functions
      Bitmap* FC GetBitmap(UINT bitmapID);
      void LoadBitmap(UINT bitmapID, UINT priority);
      void LoadBitmap(UINT bitmapID, UINT priority, const char* fileName);
      void LoadBitmapDimensions(UINT bitmapID, UINT priority,
         Area& area);
      void LoadBitmapDimensions(UINT bitmapID, UINT priority,
         Area& area, const char* fileName);
      void RemoveBitmap(UINT bitmapID);
      void RemoveAllBitmaps(void);
      void DeleteBitmap(UINT bitmapID);
      void DeleteAllBitmaps(void);
      void EnableAutoClean(bool clean, bool maxBmp, bool maxMem);
      void CleanMaxBitmaps(void);
      void CleanMaxBitmapMemory(void);
      void CleanBitmap(UINT bitmapID);
      void CleanBitmapByPriority(UINT priority);
      void CleanAllBitmapsByPriority(UINT priority);
      void SetMaxBitmaps(UINT maxNumBmps);
      UINT GetMaxBitmaps(void) {return maxNumOfBitmaps;}
      void SetMaxBitmapMemory(UINT maxMem);
      UINT GetMaxBitmapMemory(void) {return maxAmountOfMemory;}
      UINT GetCurrentNumOfBitmaps(void) {return curNumOfBitmaps;}
      UINT GetCurrentBitmapMemory(void) {return curAmountOfMemory;}
      void SetTransparentColor(UINT bitmapID, Color& transparentColor);
      bool BitmapFileExists(const char* fileName);

      //Drawing Functions
      void FC LockSurface(void);
      void FC UnlockSurface(void);
      void FC FlipSurface(void);

      //Non-Blit Drawing Functions
      void FC SetPixel(int x, int y, Color& color);
      void FC DrawHorizontalLine(int x1, int x2, int y, Color& color);
      void FC DrawVerticalLine(int x, int y1, int y2, Color& color);
      void FC DrawLine(Point& p1, Point& p2, Color& color);
      void FC DrawRectangle(Rectangle& rect, Color& color);
      void FC DrawFilledRectangle(Rectangle& rect, Color& color);

      //Blit Drawing Functions
      void FC FillArea(Area& area, Color& color);
      void FC FillScreen(Color& color);
      void FC DrawBitmap(Point& location, UINT bitmapID);
      void FC DrawScaledBitmap(Area& area, UINT bitmapID);
      void FC DrawTransparentBitmap(Point& location, UINT bitmapID,
         Color& transparentColor);
      void FC DrawTransparentScaledBitmap(Area& area, UINT bitmapID,
         Color& transparentColor);
      void FC DrawTransparentBitmap(Point& location, UINT bitmapID);
      void FC DrawTransparentScaledBitmap(Area& area, UINT bitmapID);   

      //GDI Drawing Functions
      void FlipToGDISurface(void);
      void FC SetGDIFont(Font& font);
      void FC SetTextTransparency(int mode);
      void FC SetTextColor(Color& color);
      void FC SetTextBackgroundColor(Color& color);
      void FC DrawText(char* text, Rectangle& rect, UINT flags);

      //Clipping Functions
      void FC AddClippingArea(Rectangle& area);
      void FC RemoveClippingArea(void);
      void FC ClearClippingAreas(void);

      static Graphics* instance;

      //Functions to get and release a Surface
      Surface* FC GetSurface(Area& area);
      void FC ReleaseSurface(Surface* surface = NULL);
      Surface* GetCurrentSurface(void) {return currentSurface;}

      //DirectX-related functions
      void HandleDDrawError(UINT errorCode, HRESULT error, 
         char* fileName, UINT lineNumber);

   private:
      void FC RestoreAllSurfaces(void);
      void FC DestroyAllSurfaces(void);
      void FC ClearPrimarySurface(void);

      //Pointer to DirectDraw object
      LPDIRECTDRAW7 lpDD;

      //Hardware capabilites structure
      DDCAPS ddCaps;

      //The surface description structure that can be used throughout
      //the Graphics member functions.
      DDSURFACEDESC2 ddsd;

      //The primary and secondary surface objects
      LPDIRECTDRAWSURFACE7 lpDDSPrimary;
      LPDIRECTDRAWSURFACE7 lpDDSSecondary;
      LPDIRECTDRAWSURFACE7 lpDDSDrawingSurface;

      //MS Windows info
      HWND hWnd;

      //Mode info
      Point screenRes;

      UINT windowedState;
      UINT colorDepth;
      UINT bufferingMode;

      //Controls the persistence of the secondary surface.
      //When it is on, the secondary surface stays the same and is never
      //cleared by the framework. This makes flipping slower, as it flips by
      //blitting, but there is less redrawing involved, so it may save time
      //in the long run.
      bool SSPersistence;

      //When in windowed mode, we must know where the client area is so
      //that we can update the clipper and draw in the right place
      RECT clientRect;

      //The list of supported display modes
      DisplayModeList displayModeList;  

      //The list which stores and manages the bitmaps
      BitmapList bitmapList;

      bool autoClean;
      bool maxBitmaps;
      bool maxMemory;
      UINT maxNumOfBitmaps;
      UINT maxAmountOfMemory;
      UINT curNumOfBitmaps;
      UINT curAmountOfMemory;

      bool surfaceLocked;
      int bytesPerPixel;

      //Text information
      Font currentGDIFont;
      int textTransparencyMode;
      Color textColor;
      Color textBackgroundColor;
      
      //Clipper for the primary surface: usually used by the framework
      //in windowed mode.
      LPDIRECTDRAWCLIPPER lpPSClipper;
      //Clipper for the drawing surface: usually used by the application
      LPDIRECTDRAWCLIPPER lpDSClipper;

      //Whether there is clipping on the drawing surface
      bool clipping;
      //The current clipping area
      RGNDATA* clippingArea;
      Rectangle clippingRect;

      //The current Surface
      Surface* currentSurface;

      UCHAR* videoBuffer;
      LONG bufferPitch;
   };

   //The callback function used to enumerate the supported display modes.
   //Unfortunately it cannot be a member function of any class.
   HRESULT CALLBACK EnumDisplayModes(LPDDSURFACEDESC2 lpDDSurfaceDesc,  
      LPVOID lpContext);
}
