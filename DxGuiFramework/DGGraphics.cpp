/*------------------------------------------------------------------------
File Name: DGGraphics.cpp
Description: This file contains the DG::Graphics class implementation, 
   which manages the DirectDraw surfaces and controls and what is drawn 
   to the screen.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

//The callback function used to enumerate the supported display modes.
//Unfortunately it cannot be a member function of any class.
HRESULT CALLBACK EnumDisplayModes(LPDDSURFACEDESC2 lpDDSurfaceDesc,  
   LPVOID lpContext);

/*Default Constructor*/
Graphics::Graphics()
{
   if(instance != NULL)
      return;
   
   instance = this;

   hWnd = GetApp()->GetWindowsHandle();
   lpDDSPrimary = NULL;
   lpDDSSecondary = NULL;
   lpDDSDrawingSurface = NULL;
   lpPSClipper = NULL;
   lpDSClipper = NULL;

   surfaceLocked = false;
   SSPersistence = false;

   bitmapList.SetDestroy(true);

   autoClean = false;
   maxBitmaps = false;
   maxMemory = false;
   maxNumOfBitmaps = 100;
   maxAmountOfMemory = 10000000;
   curNumOfBitmaps = 0;
   curAmountOfMemory = 0;

   textTransparencyMode = TRANSPARENT;
   textColor = Color(255, 255, 255);
   textBackgroundColor = Color(0, 0, 0);

   clipping = false;
   clippingArea = (RGNDATA*)new char[sizeof(RGNDATAHEADER) + sizeof(RECT)];
   clippingArea->rdh.dwSize = sizeof(RGNDATAHEADER);
   clippingArea->rdh.iType = RDH_RECTANGLES;
   clippingArea->rdh.nCount = 1;
   clippingArea->rdh.nRgnSize = sizeof(RECT);
   clippingArea->rdh.rcBound.left = 0;
   clippingArea->rdh.rcBound.top = 0;
   clippingArea->rdh.rcBound.right = 0;
   clippingArea->rdh.rcBound.bottom = 0;
   
   RECT clipRect = {0, 0, 0, 0};
   memcpy(clippingArea->Buffer, &clipRect, sizeof(RECT));

   currentSurface = NULL;

   //Initialize DirectDraw object
   HRESULT result = DirectDrawCreateEx(NULL, (void**)&lpDD, 
      IID_IDirectDraw7, NULL);
   
   if(result != DD_OK)
      HandleDDrawError(EC_DDINIT, result, __FILE__, __LINE__);

   //Get the supported display modes
   result = lpDD->EnumDisplayModes(0, NULL, NULL, 
      (LPDDENUMMODESCALLBACK2)EnumDisplayModes);

   if(result != DD_OK)
      HandleDDrawError(EC_DDINIT, result, __FILE__, __LINE__);

   //Get the hardware capabilities
   ddCaps.dwSize = sizeof(DDCAPS);

   result = lpDD->GetCaps(&ddCaps, NULL);

   if(result != DD_OK)
      HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);
}

/*Destructor*/
Graphics::~Graphics()
{
   if(clippingArea != NULL)
      delete[] clippingArea;
      
   if(currentSurface != NULL)
      delete currentSurface;

   //Release DirectDraw object
   if(lpDD != NULL)
   {
      lpDD->Release();
      lpDD = NULL;
   }

   DestroyAllSurfaces();
}

/*------------------------------------------------------------------------
Function Name: SetResolution
Parameters:
   Point res : the screen resolution to which the application will 
      be set.
Description:
   This function receives a DG::Point object specifying a resolution 
   width and height and changes the screen resolution.
------------------------------------------------------------------------*/

void Graphics::SetResolution(Point res)
{
   SetGraphicsMode(res, windowedState, colorDepth, bufferingMode,
      SSPersistence);
}

/*------------------------------------------------------------------------
Function Name: SetResolution
Parameters:
   int hRes : the new horizontal screen resolution
   int hves : the new vertical screen resolution
Description:
   This function receives two integers specifying a resolution width
   and height and changes the screen resolution.
------------------------------------------------------------------------*/

void Graphics::SetResolution(int hRes, int vRes)
{
   SetGraphicsMode(Point(hRes, vRes), windowedState, colorDepth, 
      bufferingMode, SSPersistence);
}

/*------------------------------------------------------------------------
Function Name: SetWindowedState
Parameters:
   UINT winState : the mode to which the application will be set, can
      be WS_FULLSCREEN or WS_WINDOWED
Description:
   This function switches the application between full-screen and 
   windowed mode.
------------------------------------------------------------------------*/

void Graphics::SetWindowedState(UINT winState)
{
   SetGraphicsMode(screenRes, winState, colorDepth, bufferingMode,
      SSPersistence);
}

/*------------------------------------------------------------------------
Function Name: SetColorDepth
Parameters:
   UINT clrDepth : the color depth to which the application will be set.
      Valid color depth flags are CD_16BIT, CD_24BIT, and CD_32BIT.
Description:
   This function sets the color depth of the application.
------------------------------------------------------------------------*/

void Graphics::SetColorDepth(UINT clrDepth)
{
   SetGraphicsMode(screenRes, windowedState, clrDepth, bufferingMode,
      SSPersistence);
}

/*------------------------------------------------------------------------
Function Name: SetBufferingMode
Parameters:
   UINT bufferMode : the buffer mode to which the application will be set.
      Valid buffer mode flags are BT_SINGLE, BT_DOUBLE, and BT_TRIPLE.
Description:
   This function sets the color depth of the application.
------------------------------------------------------------------------*/

void Graphics::SetBufferingMode(UINT bufferMode)
{
   SetGraphicsMode(screenRes, windowedState, colorDepth, bufferMode,
      SSPersistence);
}

/*------------------------------------------------------------------------
Function Name: SetColorDepth
Parameters:
   bool persistence : whether the drawing surface is to remain the same
      from frame to frame.
Description:
   This function sets the persistence of the drawing surface. When 
   persistence is disabled, page flipping occurs, and in the next frame,
   it is unknown what is currently in the buffer. With persistence, the
   drawing surface is blitted to the primary surface and one always
   knows what is currently on the drawing surface. Persistence makes the
   showing of the drawing surface slower, but time may be saved in the
   long run if one doesn't have to redrawn the application graphical
   output on the drawing surface for every frame.
------------------------------------------------------------------------*/

void Graphics::SetSurfacePersistence(bool persistence)
{
   SetGraphicsMode(screenRes, windowedState, colorDepth, bufferingMode,
      persistence);
}

/*------------------------------------------------------------------------
Function Name: SetGraphicsMode
Parameters:
   Point res : the screen resolution to which the application will 
      be set.
   UINT winState : the mode to which the application will be set, can
      be WS_FULLSCREEN or WS_WINDOWED
   UINT clrDepth : the color depth to which the application will be set.
      Valid color depth flags are CD_16BIT, CD_24BIT, and CD_32BIT.
   UINT bufferMode : the buffer mode to which the application will be set.
      Valid buffer mode flags are BT_SINGLE, BT_DOUBLE, and BT_TRIPLE.
   bool persistence : whether the drawing surface is to remain the same
      from frame to frame.
Description:
   This function sets the overall graphics mode for the application.
------------------------------------------------------------------------*/

void Graphics::SetGraphicsMode(Point res, UINT winState, UINT clrDepth,
      UINT bufferMode, bool persistence)
{
   //Change things so that when an exception is thrown, no member variables
   //are permanently altered.

   HRESULT result;

   DestroyAllSurfaces();

   //If we want windowed mode
   if(winState == WS_WINDOWED)
   {
      HWND desktopWindow;
      int desktopClrDepth;

      //Set the cooperative level.
      result = lpDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);

      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

      //If we are switching to windowed mode, we need to question the desktop
      //about its color depth

      desktopWindow = GetDesktopWindow();
      HDC desktopDC = GetDC(desktopWindow);
      desktopClrDepth = GetDeviceCaps(desktopDC, BITSPIXEL);
      ReleaseDC(desktopWindow, desktopDC);
      
      //Check to see if MS Windows is at a sufficient color depth
      if(desktopClrDepth != 16 && desktopClrDepth != 24 && 
         desktopClrDepth != 32)
      {
         throw new Exception("Only 16-, 24-, and 32-bit color depths are "\
            "supported. Windows needs to be set to 16-bit or true color.", 
            EC_DDSETGRAPHMODE, ET_DIRECTDRAW, __FILE__, __LINE__);
      }

      //Change the window so that it has a title and borders and is centered
      //on the screen
      SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
      
      //Calculate the size of the window when the client area represents the
      //application resolution
      int xWindowSize = res.x + (GetSystemMetrics(SM_CXSIZEFRAME) * 2);
      int yWindowSize = res.y + (GetSystemMetrics(SM_CYSIZEFRAME) * 2) +
         GetSystemMetrics(SM_CYCAPTION);

      //If the size is the same as the Windows resolution, the window will
      //automatically be full-screen.

      SetWindowPos(hWnd, 0, 
         (GetSystemMetrics(SM_CXSCREEN) - res.x) / 2,
         (GetSystemMetrics(SM_CYSCREEN) - res.y) / 2,
         xWindowSize, yWindowSize, SWP_SHOWWINDOW);

      //Prepare to create the primary surface
      memset(&ddsd, 0, sizeof(ddsd));
      ddsd.dwSize = sizeof(ddsd); 

      ddsd.dwFlags = DDSD_CAPS;
      ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE; 
 
      //Create the primary surface
      result = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL);

      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

      //Set up a clipper on the primary surface so that we don't draw all over
      //the rest of the screen.
      result = lpDD->CreateClipper(0, &lpPSClipper, NULL);
   
      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

      result = lpPSClipper->SetHWnd(0, hWnd);

      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

      result = lpDDSPrimary->SetClipper(lpPSClipper);

      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

      //lpPSClipper->Release();

      //Create the windowed secondary surface, which is actually an offscreen
      //surface.  This takes place no matter what the buffering mode is.
      //Triple buffering has no effect in windowed mode, and single buffering
      //require complicated complications to draw in a window.
      ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; 
      ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
      ddsd.dwHeight = res.y; 
      ddsd.dwWidth = res.x; 
      result = lpDD->CreateSurface(&ddsd, &lpDDSSecondary, NULL); 

      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);
      
      lpDDSDrawingSurface = lpDDSSecondary;

      //Create the drawing surface clipper object
      result = lpDD->CreateClipper(0, &lpDSClipper, NULL);
   
      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

      ClearPrimarySurface();

      colorDepth = desktopClrDepth;

      switch(desktopClrDepth)
      {
         case 16:
            colorDepth = CD_16BIT;
            bytesPerPixel = 2;
            break;
         case 24:
            colorDepth = CD_24BIT;
            bytesPerPixel = 3;
            break;
         case 32:
            colorDepth = CD_32BIT;
            bytesPerPixel = 4;
            break;
      }
   }

   //We want full-screen mode
   else
   {
      //First check to see if the desired mode is supported
      if(clrDepth != CD_16BIT && clrDepth != CD_24BIT && clrDepth != CD_32BIT)
      {
         throw new Exception("Only 16-, 24-, and 32-bit color depths are "\
            "supported.", EC_DDSETGRAPHMODE, ET_DIRECTDRAW, __FILE__, __LINE__);
      }

      SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

      result = lpDD->SetCooperativeLevel(hWnd, DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE |
         DDSCL_FULLSCREEN);

      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

      //We need to change the color depth flag to a number
      int clrBitNum = 0;

      switch(clrDepth)
      {
         case CD_16BIT:
            clrBitNum = 16;
            bytesPerPixel = 2;
            break;
         //Usually video cards will support only 32-bit modes, but a minority
         //support only 24-bit. The colors are the same in both modes, so we
         //can choose which one is best.
         case CD_24BIT:
         case CD_32BIT:
            if(displayModeList.DisplayModeSupported(res.x, res.y, 24))
            {
               clrBitNum = 24;
               bytesPerPixel = 3;
            }
            else
            {
               clrBitNum = 32;
               bytesPerPixel = 4;
            }
         break;
         default: break;
      }

      //Set the display mode
      result = lpDD->SetDisplayMode(res.x, res.y, clrBitNum, 0, 0);

      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

      //Prepare to create the primary surface
      memset(&ddsd, 0, sizeof(ddsd));
      ddsd.dwSize = sizeof(ddsd); 
         
      if(bufferMode == BT_SINGLE || persistence == true)
      {
            OutputDebugString("Persistence == true\n");
            ddsd.dwFlags = DDSD_CAPS;
            ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE; 
      }
      
      else if(bufferMode == BT_DOUBLE)
      {
         ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT; 
         ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | 
         DDSCAPS_FLIP | DDSCAPS_COMPLEX; 
         ddsd.dwBackBufferCount = 1;
      }
      
      else if(bufferMode == BT_TRIPLE)
      {
         ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT; 
         ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | 
            DDSCAPS_FLIP | DDSCAPS_COMPLEX; 
         ddsd.dwBackBufferCount = 2;
      }

      else
         throw new Exception("The buffer mode flag is not valid", 
            EC_DDSETGRAPHMODE, ET_DIRECTDRAW, __FILE__, __LINE__);

      //Create the primary surface
      result = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL);

      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);
      
      if(bufferMode != BT_SINGLE)
      {
         if(persistence)
         {
            ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; 
            ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
            ddsd.dwHeight = res.y; 
            ddsd.dwWidth = res.x; 
            result = lpDD->CreateSurface(&ddsd, &lpDDSSecondary, NULL); 
         }

         else
         {
            DDSCAPS2 ddsCaps;
            memset(&ddsCaps, 0, sizeof(ddsCaps));
            ddsCaps.dwCaps = DDSCAPS_BACKBUFFER; 
            result = lpDDSPrimary->GetAttachedSurface(&ddsCaps, &lpDDSSecondary); 
         }

         if(result != DD_OK)
            HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

         lpDDSDrawingSurface = lpDDSSecondary;
      }

      else
         lpDDSDrawingSurface = lpDDSPrimary;   

      //Create the drawing surface clipper object
      result = lpDD->CreateClipper(0, &lpDSClipper, NULL);
   
      if(result != DD_OK)
         HandleDDrawError(EC_DDSETGRAPHMODE, result, __FILE__, __LINE__);

      colorDepth = clrDepth;
   }

   //Get the pixel format information and create the lookup tables, so that
   //we can construct pixels quickly
   DDPIXELFORMAT pixelFormat;
   memset(&pixelFormat, 0, sizeof(DDPIXELFORMAT));
   pixelFormat.dwSize = sizeof(DDPIXELFORMAT);

   lpDDSPrimary->GetPixelFormat(&pixelFormat);

   //Create the color lookup tables for the different pixel formats
   //Note that 16-bit and 15-bit pixel formats are taken into account
   //during the creation of the lookup tables
   Color::CreateLookupTables(pixelFormat, colorDepth);

   bufferingMode = bufferMode;

   screenRes.x = res.x;
   screenRes.y = res.y;

   windowedState = winState;
 
   if(bufferingMode == BT_SINGLE)
      SSPersistence = false;
   else
      SSPersistence = persistence;

   if(clipping)
      AddClippingArea(clippingRect);
}

//Bitmap functions

/*------------------------------------------------------------------------
Function Name: GetBitmap
Parameters:
   UINT bitmapID : the ID of the bitmap to be retrieved
Description:
   This function receives a bitmap ID and returns a pointer to the
   bitmap object matching that ID. If no bitmap with the given ID exists,
   NULL is returned.
------------------------------------------------------------------------*/

Bitmap* FC Graphics::GetBitmap(UINT bitmapID)
{
   return bitmapList.GetItemById(int(bitmapID));
}

/*------------------------------------------------------------------------
Function Name: LoadBitmap
Parameters:
   UINT bitmapID : the ID of the bitmap to be loaded
   UINT priority : the priority to be assigned to the loaded bitmap
Description:
   This function receives a bitmap ID and a priority. The bitmap with
   the given ID is loaded from the resource file and assigned the given
   priority.
------------------------------------------------------------------------*/

void Graphics::LoadBitmap(UINT bitmapID, UINT priority)
{
   //This function will be completed after resource files have been
   //implemented.
}

/*------------------------------------------------------------------------
Function Name: LoadBitmap
Parameters:
   UINT bitmapID : the ID of the bitmap to be loaded
   UINT priority : the priority to be assigned to the loaded bitmap
   UINT fileName : the name of the file that the bitmap will be loaded
      from.
Description:
   This function receives a bitmap ID, a priority, and a file name.  
   The bitmap contained within the file is loaded and assigned the ID
   and priority.
------------------------------------------------------------------------*/

void Graphics::LoadBitmap(UINT bitmapID, UINT priority, 
                            const char* fileName)
{
   Bitmap* bitmap = new Bitmap(bitmapID, priority, fileName);
   bitmapList.Append(bitmap, bitmapID);
   if(autoClean)
   {
      if(maxBitmaps)
         CleanMaxBitmaps();

      if(maxMemory)
         CleanMaxBitmapMemory();
   }
}

/*------------------------------------------------------------------------
Function Name: LoadBitmapDimensions
Parameters:
   UINT bitmapID : the ID of the bitmap to be loaded
   UINT priority : the priority to be assigned to the loaded bitmap
   Area& area : the rectangular area of the bitmap that is to be
      loaded into memory.
Description:
   This function receives a bitmap ID, a priority, and a DG::Area 
   object. The area specified by the DG::Area object is loaded from
   the bitmap with the given ID. The bitmap is loaded from the resource 
   file and assigned the given priority.
------------------------------------------------------------------------*/

void Graphics::LoadBitmapDimensions(UINT bitmapID, UINT priority,
      Area& area)
{
   //This function will be completed after resource files have been
   //implemented.
}

/*------------------------------------------------------------------------
Function Name: LoadBitmapDimensions
Parameters:
   UINT bitmapID : the ID of the bitmap to be loaded
   UINT priority : the priority to be assigned to the loaded bitmap
   UINT fileName : the name of the file that the bitmap will be loaded
      from.
   Area& area : the rectangular area of the bitmap that is to be
      loaded into memory.
Description:
   This function receives a bitmap ID, a priority, a DG::Area object
   describing a region of the bitmap, and a file name.  
   The specified area of the bitmap contained within the given file is 
   loaded and assigned the ID and priority.
------------------------------------------------------------------------*/

void Graphics::LoadBitmapDimensions(UINT bitmapID, UINT priority,
      Area& area, const char* fileName)
{
   Bitmap* bitmap = new Bitmap(bitmapID, priority, fileName, area);
   bitmapList.Append(bitmap, bitmapID);
   if(autoClean)
   {
      if(maxBitmaps)
         CleanMaxBitmaps();

      if(maxMemory)
         CleanMaxBitmapMemory();
   }
}

/*------------------------------------------------------------------------
Function Name: RemoveBitmap
Parameters:
   UINT bitmapID : the ID of the bitmap to be removed from memory
Description:
   This function receives a bitmap ID and removes the specified bitmap
   from memory. The bitmap, however, still remains in the bitmap list
   and can automatically be reloaded if necessary.
------------------------------------------------------------------------*/

void Graphics::RemoveBitmap(UINT bitmapID)
{
   bitmapList.GetItemById(bitmapID)->DestroyBitmap();
}

/*------------------------------------------------------------------------
Function Name: RemoveAllBitmaps
Parameters:
Description:
   This function removes all bitmaps in the bitmap list from memory.
   All the bitmaps, however, still remain in the bitmap list and can
   automatically be reloaded if necessary.
------------------------------------------------------------------------*/

void Graphics::RemoveAllBitmaps()
{
   bitmapList.RemoveAllBitmaps();
}

/*------------------------------------------------------------------------
Function Name: DeleteBitmap
Parameters:
   UINT bitmapID : the ID of the bitmap to be deleted
Description:
   This function receives a bitmap ID and deletes the bitmap completely.
   The bitmap is no longer in the bitmap list and has to specifically
   be reloaded by the program before it can be used again.
------------------------------------------------------------------------*/

void Graphics::DeleteBitmap(UINT bitmapID)
{
   bitmapList.DeleteById(bitmapID);
}

/*------------------------------------------------------------------------
Function Name: DeleteAllBitmaps
Parameters:
Description:
   This function deletes all the bitmaps in the bitmap list completely.
   The bitmaps are no longer in the bitmap list and have to specifically
   be reloaded by the program before they can be used again.
------------------------------------------------------------------------*/

void Graphics::DeleteAllBitmaps()
{
   bitmapList.DeleteAll();
}

/*------------------------------------------------------------------------
Function Name: EnableAutoClean
Parameters:
   bool clean : true if autoclean is to be enabled, false if it is to be
      disabled
   bool maxBmp : whether autoclean is to clean by the number of bitmaps
      true - enabled, false - disabled
   bool maxMem : whether autoclean is to clean by the amount of memory
      taken up by bitmaps, true - enabled, false - disabled
Description:
   This function enables and disables autoclean and controls how the 
   cleaning will be done.
------------------------------------------------------------------------*/

void Graphics::EnableAutoClean(bool clean, bool maxBmp, bool maxMem)
{
   autoClean = clean;
   maxBitmaps = maxBmp;
   maxMemory = maxMem;
}

/*------------------------------------------------------------------------
Function Name: CleanMaxBitmaps
Parameters:
Description:
   This function removes the lowest priority bitmap possible from memory
   until the number of bitmaps in the list is under the max. bitmap limit.
------------------------------------------------------------------------*/

void Graphics::CleanMaxBitmaps(void)
{
   while(maxNumOfBitmaps < bitmapList.GetNumOfLoadedBitmaps())
   {
      bitmapList.RemoveFirstLowestPriorityBitmap();   
   }
}

/*------------------------------------------------------------------------
Function Name: CleanMaxBitmaps
Parameters:
Description:
   This function removes the lowest priority bitmap possible from memory
   until the amount of memory used by the bitmaps in the list is under 
   the max. memory limit.
------------------------------------------------------------------------*/

void Graphics::CleanMaxBitmapMemory(void)
{
   while(maxAmountOfMemory < bitmapList.GetTotalMemory())
   {
      bitmapList.RemoveFirstLowestPriorityBitmap();   
   }
}

/*------------------------------------------------------------------------
Function Name: CleanBitmap
Parameters:
   UINT bitmapID : The ID of the bitmap to be "cleaned" (removed from
      memory)
Description:
   This function removes the bitmap with the specified ID from memory.
------------------------------------------------------------------------*/

void Graphics::CleanBitmap(UINT bitmapID)
{
   Bitmap* bitmap = bitmapList.GetItemById(bitmapID);
   if(bitmap != NULL)
      bitmap->DestroyBitmap();
}

/*------------------------------------------------------------------------
Function Name: CleanBitmapByPriority
Parameters:
   UINT priority : The priority of the bitmap to be removed from memory
Description:
   This function receives a priority and removes the first bitmap of that
   priority it finds in the list from memory.
------------------------------------------------------------------------*/

void Graphics::CleanBitmapByPriority(UINT priority)
{
   bitmapList.RemoveFirstBitmapByPriority(priority);
}

/*------------------------------------------------------------------------
Function Name: CleanAllBitmapsByPriority
Parameters:
   UINT priority : The priority of the bitmaps to be removed from memory
Description:
   This function receives a priority and removes all the bitmaps of that
   priority it finds in the list from memory.
------------------------------------------------------------------------*/

void Graphics::CleanAllBitmapsByPriority(UINT priority)
{
   bitmapList.RemoveAllBitmapsByPriority(priority);
}

/*------------------------------------------------------------------------
Function Name: SetMaxBitmaps
Parameters:
   UINT maxNumBmps : The maximum number of bitmaps allowed to be loaded
      at one time.
Description:
   This function sets the maximum number of bitmaps allowed to be in 
   memory at one time, which is used when cleaning by number of bitmaps.
------------------------------------------------------------------------*/

void Graphics::SetMaxBitmaps(UINT maxNumBmps)
{
   maxNumOfBitmaps = maxNumBmps;
}

/*------------------------------------------------------------------------
Function Name: SetMaxBitmapMemory
Parameters:
   UINT maxMem : The maximum amount of memory that is allowed to be
      occupied by bitmaps
Description:
   This function sets the maximum amount of memory allowed to be occupied
   by loaded bitmaps, which is used when cleaning by memory.
------------------------------------------------------------------------*/

void Graphics::SetMaxBitmapMemory(UINT maxMem)
{
   maxAmountOfMemory = maxMem;
}

/*------------------------------------------------------------------------
Function Name: SetTransparentColor
Parameters:
   UINT bitmapID : the ID of the bitmap to which the transparent color is
      to be set
   Color& transparentColor : the transparent color to be set
Description:
   This function sets the transparent color of the bitmap, so that it
   does not have to be specified each time when drawing it transparently.
   This color will only be transparent when drawing the bitmap with one
   of the transparent bitmap drawing functions.
------------------------------------------------------------------------*/

void Graphics::SetTransparentColor(UINT bitmapID, Color& transparentColor)
{
   bitmapList.GetItemById(bitmapID)->SetTransparentColor(transparentColor);
}

/*------------------------------------------------------------------------
Function Name: BitmapFileExists
Parameters:
   const char* fileName : the name of a bitmap file
Returns: 
   true if the bitmap file exists, false if the bitmap file does not exist
Description:
   This function receives the name of a bitmap file and checks to see
   if the file exists.
------------------------------------------------------------------------*/

bool Graphics::BitmapFileExists(const char* fileName)
{
   bool fileExists = false;

   FILE* bitmapFile = fopen(fileName, "r");

   if(bitmapFile != NULL)
   {
      fileExists = true;
      fclose(bitmapFile);
   }

   return fileExists;
}

/*------------------------------------------------------------------------
Function Name: BeginFrame
Parameters:
Description:
   This function must be called at the beginning of every frame before
   anything is drawn. It does whatever is necessary to prepare for 
   drawing for the current frame.
------------------------------------------------------------------------*/

void Graphics::BeginFrame()
{
   if(bufferingMode != BT_SINGLE && SSPersistence == false)
   {
      DDBLTFX bltFx;
      bltFx.dwSize = sizeof(bltFx);
      bltFx.dwFillColor = 0;
      lpDDSSecondary->Blt(NULL, NULL, NULL, 
         DDBLT_COLORFILL | DDBLT_WAIT, &bltFx);
   }
}

/*------------------------------------------------------------------------
Function Name: EndFrame
Parameters:
Description:
   This function must be called at the end of every frame after
   everything is drawn. It does whatever is necessary at the end of the
   frame, such as displaying the drawing surface. If this function is
   not called at the end of the frame, the drawing surface will not
   be displayed.
------------------------------------------------------------------------*/

void Graphics::EndFrame()
{
   FlipSurface();
}

//Drawing Functions

/*------------------------------------------------------------------------
Function Name: LockSurface
Parameters:
Description:
   This function locks the surface and must be called before any of the
   non-blit functions are used. When the surface is locked, no blit 
   functions may be used. If it is not called when using non-blit 
   functions, the program will fail with an error.
------------------------------------------------------------------------*/

void FC Graphics::LockSurface()
{
   assert(surfaceLocked == false);

   HRESULT result;

   memset(&ddsd, 0, sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);
   
   result = lpDDSDrawingSurface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR |
      DDLOCK_WAIT, NULL);

   switch(result)
   {
      case DDERR_SURFACELOST:
         RestoreAllSurfaces();
         LockSurface();
         break;
      case DD_OK:
         videoBuffer = (UCHAR*)ddsd.lpSurface;
         bufferPitch = ddsd.lPitch;
         surfaceLocked = true;
         break;
      default:
         lpDDSDrawingSurface->Unlock(NULL);
         HandleDDrawError(EC_DDLOCKSURFACE, result, __FILE__, __LINE__);
         break;
   }
   
#ifdef _DEBUG
   lpDDSDrawingSurface->Unlock(NULL);
#endif
}

/*------------------------------------------------------------------------
Function Name: UnlockSurface
Parameters:
Description:
   This function unlocks the surface and must be called after the
   non-blit functions are used. Always unlock the surface before the end
   of the frame if it was locked during that frame. If the surface is 
   not unlocked, the entire system will be unresponsive, as Windows will 
   be waiting for the surface to be unlocked.
------------------------------------------------------------------------*/

void FC Graphics::UnlockSurface(void)
{
   assert(surfaceLocked == true);

#ifndef _DEBUG
   HRESULT result;

   result = lpDDSDrawingSurface->Unlock(NULL);

   if(result != DD_OK)
      HandleDDrawError(EC_DDLOCKSURFACE, result, __FILE__, __LINE__);
#endif

   videoBuffer = NULL;
   surfaceLocked = false;
}

/*------------------------------------------------------------------------
Function Name: FlipSurface
Parameters:
Description:
   This function displays the drawing surface on the screen, otherwise
   the drawing surface remains invisible. This function is automatically
   called by the framework at the end of each frame, so it is not
   necessary for the framework user to call it. If the user does decide
   to call it, the drawing surface should be unlocked.
------------------------------------------------------------------------*/

void FC Graphics::FlipSurface(void)
{
   HRESULT result;

   if(IsIconic(hWnd))
      return;

   if(surfaceLocked)
      UnlockSurface();

   //Check to see if the DirectDraw surfaces were lost
   if(lpDDSPrimary->IsLost() == DDERR_SURFACELOST)
      RestoreAllSurfaces();

   //If we are in windowed mode
   if(windowedState == WS_WINDOWED)
   {
      result = lpDDSPrimary->Blt(&clientRect, lpDDSSecondary, NULL, 
         DDBLT_WAIT, 0);
   }

   //If we are in full-screen single buffer mode, no flipping required
   else if(bufferingMode == BT_SINGLE)
   {
      return;
   }

   //If we are in full-screen mode with a persistent secondary surface
   else if(SSPersistence)
   {
      result = lpDDSPrimary->Blt(NULL, lpDDSSecondary, NULL, DDBLT_WAIT, 0);
   }

   //If we are in full-screen mode with normal flipping
   else
   {
      result = lpDDSPrimary->Flip(NULL, DDFLIP_WAIT);
   }

   if(result != DD_OK)
      HandleDDrawError(EC_DDFLIPSURFACE, result, __FILE__, __LINE__);

}

//Non-Blit Drawing Functions

/*------------------------------------------------------------------------
Function Name: SetPixel
Parameters:
   int x : the x coordinate of the pixel location
   int y : the y coordinate of the pixel location
   Color& color : the color that the pixel is to be set to
Description:
   This function sets a pixel at the specified location to the specified
   color. The specified location must be on the screen and the surface
   must be locked or the function will fail.
------------------------------------------------------------------------*/

void FC Graphics::SetPixel(int x, int y, Color& color)
{
   assert(surfaceLocked == true);
   assert(x >= 0 && x < screenRes.x && y >= 0 && y < screenRes.y);

   switch(colorDepth)
   {
      case CD_16BIT:
         ((USHORT*)videoBuffer)[x + y * (bufferPitch >> 1)] = color.To16Bit();
         break;
      case CD_24BIT:
      {
         UINT colorValue = color.To24Bit();
         
         int bufferPosition = (x * 3) + (y * bufferPitch);

         //Copy the 3 bytes into video memory
			((UCHAR*)videoBuffer)[bufferPosition++] = ((UCHAR*)&colorValue)[0];
			((UCHAR*)videoBuffer)[bufferPosition++] = ((UCHAR*)&colorValue)[1];
			((UCHAR*)videoBuffer)[bufferPosition++] = ((UCHAR*)&colorValue)[2];
      }
         break;
      case CD_32BIT:
         ((UINT*)videoBuffer)[x + y * (bufferPitch >> 2)] = color.To32Bit();
         break;
   }
}

/*------------------------------------------------------------------------
Function Name: DrawHorizontalLine
Parameters:
   int x1 : the x coordinate of the beginning of the line
   int x2 : the x coordinate of the end of the line
   int y : the y coordinate of the entire horizontal line
   Color& color : the color that the line is to be set to
Description:
   This function draws a horizontal line between 2 x-coordinates at
   the specified y-coordinate. The coordinates must be valid coordinates
   on the screen and the surface must be locked when calling this 
   function or the function will fail.
------------------------------------------------------------------------*/

void FC Graphics::DrawHorizontalLine(int x1, int x2, int y, Color& color)
{
   assert(surfaceLocked == true);
   assert(x1 >= 0 && x1 < screenRes.x && x2 >= 0 && x2 < screenRes.x &&
      y >= 0 && y < screenRes.y);

   int beginX, endX;
   if(x1 < x2)
   {
      beginX = x1;
      endX = x2;
   }
   else
   {
      beginX = x2;
      endX = x1;
   }

   int bufferPosition;
   switch(colorDepth)
   {
      case CD_16BIT:
         bufferPosition = beginX + y * (bufferPitch >> 1);
         break;
      case CD_24BIT:
         bufferPosition = (beginX * 3) + (y * bufferPitch);
         break;
      case CD_32BIT:
         bufferPosition = beginX + y * (bufferPitch >> 2);
         break;
   }

   for(int x = beginX; x <= endX; x++)
   {
      switch(colorDepth)
      {
         case CD_16BIT:
            ((USHORT*)videoBuffer)[bufferPosition] = color.To16Bit();
            bufferPosition++;
            break;
         case CD_24BIT:
         {
            UINT colorValue = color.To24Bit();
     
            //Due to the way an int is stored, index 0 in colorValue
            //would access the *least* significant byte and not the
            //most significant byte as would assume
			   ((UCHAR*)videoBuffer)[bufferPosition++] = ((UCHAR*)&colorValue)[0];
			   ((UCHAR*)videoBuffer)[bufferPosition++] = ((UCHAR*)&colorValue)[1];
			   ((UCHAR*)videoBuffer)[bufferPosition++] = ((UCHAR*)&colorValue)[2];
         }
            break;
         case CD_32BIT:
            ((UINT*)videoBuffer)[bufferPosition] = color.To32Bit();
            bufferPosition++;
            break;
      }      
   }
}

/*------------------------------------------------------------------------
Function Name: DrawVerticalLine
Parameters:
   int x : the x coordinate of the entire vertical line
   int y1 : the y coordinate of the beginning of the line
   int y2 : the y coordinate of the end of the line
   Color& color : the color that the line is to be set to
Description:
   This function draws a vertical line between 2 y-coordinates at
   the specified x-coordinate. The coordinates must be valid coordinates
   on the screen and the surface must be locked when calling this 
   function or the function will fail.
------------------------------------------------------------------------*/

void FC Graphics::DrawVerticalLine(int x, int y1, int y2, Color& color)
{
   assert(surfaceLocked == true);
   assert(x >= 0 && x < screenRes.x && y1 >= 0 && y1 < screenRes.y &&
      y2 >= 0 && y2 < screenRes.y);

   int beginY, endY;
   if(y1 < y2)
   {
      beginY = y1;
      endY = y2;
   }
   else
   {
      beginY = y2;
      endY = y1;
   }

   int bufferPosition;
   switch(colorDepth)
   {
      case CD_16BIT:
         bufferPosition = x + beginY * (bufferPitch >> 1);
         break;
      case CD_24BIT:
         bufferPosition = (x * 3) + (beginY * bufferPitch);
         break;
      case CD_32BIT:
         bufferPosition = x + beginY * (bufferPitch >> 2);
         break;
   }

   for(int y = beginY; y <= endY; y++)
   {
      switch(colorDepth)
      {
         case CD_16BIT:
            ((USHORT*)videoBuffer)[bufferPosition] = color.To16Bit();
            bufferPosition = bufferPosition + (bufferPitch >> 1);
            break;
         case CD_24BIT:
         {
            UINT colorValue = color.To24Bit();
     
			   ((UCHAR*)videoBuffer)[bufferPosition] = ((UCHAR*)&colorValue)[0];
			   ((UCHAR*)videoBuffer)[bufferPosition + 1] = ((UCHAR*)&colorValue)[1];
			   ((UCHAR*)videoBuffer)[bufferPosition + 2] = ((UCHAR*)&colorValue)[2];
         }
            bufferPosition = bufferPosition + bufferPitch;
            break;
         case CD_32BIT:
            ((UINT*)videoBuffer)[bufferPosition] = color.To32Bit();
            bufferPosition = bufferPosition + (bufferPitch >> 2);
            break;
      }      
   }
}

/*------------------------------------------------------------------------
Function Name: DrawLine
Parameters:
   Point p1: the coordinate describing the beginning of the line
   Point p2: the coordinate describing the end of the line
   Color& color : the color that the line is to be set to
Description:
   This function draws a line between 2 points on the screen. Each point
   needs to be a valid point on the screen and the surface needs to be
   locked when calling this function or the function will fail.
------------------------------------------------------------------------*/

void FC Graphics::DrawLine(Point& p1, Point& p2, Color& color)
{
   assert(surfaceLocked == true);
   assert(p1.x >= 0 && p1.x < screenRes.x && p1.y >= 0 && p1.y < screenRes.y);
   assert(p2.x >= 0 && p2.x < screenRes.x && p2.y >= 0 && p2.y < screenRes.y);

   //Difference along the x axis
   int dx = p2.x - p1.x;

   //Difference along the y axis
   int dy = p2.y - p1.y;
   
   //Increment to get to the next pixel in the x direction
   int xInc;
   
   //Increment to get to the next pixel in the y direction
   int yInc;

   //The error factor which tells us when to move to the next pixel line/column
   int errorFactor = 0;

   //The position in the buffer
   int bufferPosition;

   switch(colorDepth)
   {
      case CD_16BIT:
         bufferPosition = p1.x + p1.y * (bufferPitch >> 1);
         break;
      case CD_24BIT:
         bufferPosition = (p1.x * 3) + (p1.y * bufferPitch);
         break;
      case CD_32BIT:
         bufferPosition = p1.x + p1.y * (bufferPitch >> 2);
         break;
   }

   //Now we need to calculate in which direction we need to go when
   //drawing the line

   //Line is moving to the right
   if(dx >= 0)
   {
      switch(colorDepth)
      {
         case CD_16BIT:
         case CD_32BIT:
            xInc = 1;
            break;
         case CD_24BIT:
            xInc = 3;
            break;         
      }
   }
   //Line is moving to the left
   else
   {
      switch(colorDepth)
      {
         case CD_16BIT:
         case CD_32BIT:
            xInc = -1;
            break;
         case CD_24BIT:
            xInc = -3;
            break;         
      }
      dx = -dx;
   }

   //Line is moving down
   if(dy >= 0)
   {
      switch(colorDepth)
      {
         case CD_16BIT:
            yInc = bufferPitch >> 1;
            break;
         case CD_24BIT:
            yInc = bufferPitch;
            break;         
         case CD_32BIT:
            yInc = bufferPitch >> 2;
      }
   }

   //Line is moving up
   else
   {
      switch(colorDepth)
      {
         case CD_16BIT:
            yInc = -(bufferPitch >> 1);
            break;
         case CD_24BIT:
            yInc = -bufferPitch;
            break;         
         case CD_32BIT:
            yInc = -(bufferPitch >> 2);
      }
      dy = -dy;
   }


   //Now we draw the line
   if(dx > dy)
   {
      for(int i = 0; i <= dx; i++)
      {
         //Set the pixel
         switch(colorDepth)
         {  
            case CD_16BIT:
               ((USHORT*)videoBuffer)[bufferPosition] = color.To16Bit();
               break;
            case CD_24BIT:
            {
               UINT colorValue = color.To24Bit();

			      ((UCHAR*)videoBuffer)[bufferPosition] = ((UCHAR*)&colorValue)[0];
			      ((UCHAR*)videoBuffer)[bufferPosition + 1] = ((UCHAR*)&colorValue)[1];
			      ((UCHAR*)videoBuffer)[bufferPosition + 2] = ((UCHAR*)&colorValue)[2];
            }
               break;
            case CD_32BIT:
               ((UINT*)videoBuffer)[bufferPosition] = color.To32Bit();
               break;   
         }

         //Adjust the error factor
         errorFactor = errorFactor + dy;

         //Test to see if error has overflowed
         if(errorFactor > dx)
         {
            errorFactor = errorFactor - dx;

            //Move to next line
            bufferPosition = bufferPosition + yInc;
      	}

         //Move to the next pixel
         bufferPosition = bufferPosition + xInc;
      }
   }

   else
   {
      for (int i = 0; i <= dy; i++)
      {
         //Set the pixel
         switch(colorDepth)
         {  
            case CD_16BIT:
               ((USHORT*)videoBuffer)[bufferPosition] = color.To16Bit();
               break;
            case CD_24BIT:
            {
               UINT colorValue = color.To24Bit();

			      ((UCHAR*)videoBuffer)[bufferPosition] = ((UCHAR*)&colorValue)[0];
			      ((UCHAR*)videoBuffer)[bufferPosition + 1] = ((UCHAR*)&colorValue)[1];
			      ((UCHAR*)videoBuffer)[bufferPosition + 2] = ((UCHAR*)&colorValue)[2];
            }
               break;
            case CD_32BIT:
               ((UINT*)videoBuffer)[bufferPosition] = color.To32Bit();
               break;   
         }

         //Adjust the error factor
         errorFactor = errorFactor + dx;

         //Test to see if error overflowed
         if(errorFactor > 0)
         {
            errorFactor = errorFactor - dy;

            //Move to next line
            bufferPosition = bufferPosition + xInc;

         }

         //Move to the next pixel
         bufferPosition = bufferPosition + yInc;
      }
   }
}

/*------------------------------------------------------------------------
Function Name: DrawRectangle
Parameters:
   DG::Rectangle rect: a rectangle describing the upper-left and lower-
      right corners of the rectangle to be drawn
   Color& color : the color that the rectangle should be
Description:
   This function draws a non-filled rectangle with 1 pixel-wide borders. 
   The entire area of the rectangle needs to be within the screen and 
   the surface needs to be locked when calling this function
   or the function will fail.
------------------------------------------------------------------------*/

void FC Graphics::DrawRectangle(DG::Rectangle& rect, Color& color)
{
   assert(surfaceLocked == true);
   assert(rect.left >= 0 && rect.left < screenRes.x && rect.top >= 0 && 
      rect.top < screenRes.y && rect.right >= 0 && rect.right < screenRes.x && 
      rect.bottom >= 0 && rect.bottom < screenRes.y);

   int beginX, endX, beginY, endY;
   USHORT color16Bit = 0;
   UINT color32Bit = 0;

   if(rect.left <= rect.right)
   {
      beginX = rect.left;
      endX = rect.right;
   }
   else
   {
      beginX = rect.right;
      endX = rect.left;
   }

   if(rect.top <= rect.bottom)
   {
      beginY = rect.top;
      endY = rect.bottom;
   }
   else
   {
      beginY = rect.bottom;
      endY = rect.top;
   }

   int bufferPositionFirstLine;
   int bufferPositionSecondLine;

   //First we draw the top and bottom lines
   switch(colorDepth)
   {
      case CD_16BIT:
         bufferPositionFirstLine = beginX + beginY * (bufferPitch >> 1);
         bufferPositionSecondLine = beginX + endY * (bufferPitch >> 1);
         color16Bit = color.To16Bit();
         break;
      case CD_24BIT:
         bufferPositionFirstLine = (beginX * 3) + (beginY * bufferPitch);
         bufferPositionSecondLine = (beginX * 3) + (endY * bufferPitch);
         break;
      case CD_32BIT:
         bufferPositionFirstLine = beginX + beginY * (bufferPitch >> 2);
         bufferPositionSecondLine = beginX + endY * (bufferPitch >> 2);
         color32Bit = color.To32Bit();
         break;
   }

   for(int x = beginX; x <= endX; x++)
   {
      switch(colorDepth)
      {
         case CD_16BIT:
            ((USHORT*)videoBuffer)[bufferPositionFirstLine] = color16Bit;
            ((USHORT*)videoBuffer)[bufferPositionSecondLine] = color16Bit;
            bufferPositionFirstLine++;
            bufferPositionSecondLine++;
            break;
         case CD_24BIT:
         {
            UINT colorValue = color.To24Bit();
			   ((UCHAR*)videoBuffer)[bufferPositionFirstLine++] = ((UCHAR*)&colorValue)[0];
			   ((UCHAR*)videoBuffer)[bufferPositionFirstLine++] = ((UCHAR*)&colorValue)[1];
			   ((UCHAR*)videoBuffer)[bufferPositionFirstLine++] = ((UCHAR*)&colorValue)[2];

			   ((UCHAR*)videoBuffer)[bufferPositionSecondLine++] = ((UCHAR*)&colorValue)[0];
			   ((UCHAR*)videoBuffer)[bufferPositionSecondLine++] = ((UCHAR*)&colorValue)[1];
			   ((UCHAR*)videoBuffer)[bufferPositionSecondLine++] = ((UCHAR*)&colorValue)[2];
         }
            break;
         case CD_32BIT:
            ((UINT*)videoBuffer)[bufferPositionFirstLine] = color32Bit;
            ((UINT*)videoBuffer)[bufferPositionSecondLine] = color32Bit;
            bufferPositionFirstLine++;
            bufferPositionSecondLine++;
            break;
      }      
   }  
   
   //Now we draw the left and right lines
   //The corner pixels have already been drawn
   beginY++;
   endY--;

   switch(colorDepth)
   {
      case CD_16BIT:
         bufferPositionFirstLine = beginX + beginY * (bufferPitch >> 1);
         bufferPositionSecondLine = endX + beginY * (bufferPitch >> 1);
         color16Bit = color.To16Bit();
         break;
      case CD_24BIT:
         bufferPositionFirstLine = (beginX * 3) + (beginY * bufferPitch);
         bufferPositionSecondLine = (endX * 3) + (beginY * bufferPitch);
         break;
      case CD_32BIT:
         bufferPositionFirstLine = beginX + beginY * (bufferPitch >> 2);
         bufferPositionSecondLine = endX + beginY * (bufferPitch >> 2);
         color32Bit = color.To32Bit();
         break;
   }

   for(int y = beginY; y <= endY; y++)
   {
      switch(colorDepth)
      {
         case CD_16BIT:
            ((USHORT*)videoBuffer)[bufferPositionFirstLine] = color16Bit;
            ((USHORT*)videoBuffer)[bufferPositionSecondLine] = color16Bit;
            bufferPositionFirstLine = bufferPositionFirstLine + (bufferPitch >> 1);
            bufferPositionSecondLine = bufferPositionSecondLine + (bufferPitch >> 1);
            break;
         case CD_24BIT:
         {
            UINT colorValue = color.To24Bit();
			   ((UCHAR*)videoBuffer)[bufferPositionFirstLine] = ((UCHAR*)&colorValue)[0];
			   ((UCHAR*)videoBuffer)[bufferPositionFirstLine + 1] = ((UCHAR*)&colorValue)[1];
			   ((UCHAR*)videoBuffer)[bufferPositionFirstLine + 2] = ((UCHAR*)&colorValue)[2];

			   ((UCHAR*)videoBuffer)[bufferPositionSecondLine] = ((UCHAR*)&colorValue)[0];
			   ((UCHAR*)videoBuffer)[bufferPositionSecondLine + 1] = ((UCHAR*)&colorValue)[1];
			   ((UCHAR*)videoBuffer)[bufferPositionSecondLine + 2] = ((UCHAR*)&colorValue)[2];
         }
            bufferPositionFirstLine = bufferPositionFirstLine + bufferPitch;
            bufferPositionSecondLine = bufferPositionSecondLine + bufferPitch;
            break;
         case CD_32BIT:
            ((UINT*)videoBuffer)[bufferPositionFirstLine] = color32Bit;
            ((UINT*)videoBuffer)[bufferPositionSecondLine] = color32Bit;
            bufferPositionFirstLine = bufferPositionFirstLine + (bufferPitch >> 2);
            bufferPositionSecondLine = bufferPositionSecondLine + (bufferPitch >> 2);
            break;
      }      
   } 
}

/*------------------------------------------------------------------------
Function Name: DrawFilledRectangle
Parameters:
   DG::Rectangle rect: a rectangle describing the upper-left and lower-
      right corners of the rectangle to be drawn
   Color& color : the color that the rectangle should be
Description:
   This function draws a filled rectangle, completely filling the
   specified rectangle with the specified color. 
   The entire area of the rectangle needs to be within the screen and 
   the surface needs to be locked when calling this function
   or the function will fail.
------------------------------------------------------------------------*/

void FC Graphics::DrawFilledRectangle(DG::Rectangle& rect, Color& color)
{
   assert(surfaceLocked == true);
   assert(rect.left >= 0 && rect.left < screenRes.x && rect.top >= 0 && 
      rect.top < screenRes.y && rect.right >= 0 && rect.right < screenRes.x && 
      rect.bottom >= 0 && rect.bottom < screenRes.y);

   int beginX, endX, beginY, endY;
   USHORT color16Bit = 0;
   UINT color32Bit = 0;

   if(rect.left <= rect.right)
   {
      beginX = rect.left;
      endX = rect.right;
   }
   else
   {
      beginX = rect.right;
      endX = rect.left;
   }

   if(rect.top <= rect.bottom)
   {
      beginY = rect.top;
      endY = rect.bottom;
   }
   else
   {
      beginY = rect.bottom;
      endY = rect.top;
   }

   int bufferPosition;

   //First we draw the top and bottom lines
   switch(colorDepth)
   {
      case CD_16BIT:
         bufferPosition = beginX + beginY * (bufferPitch >> 1);
         color16Bit = color.To16Bit();
         break;
      case CD_24BIT:
         bufferPosition = (beginX * 3) + (beginY * bufferPitch);
         break;
      case CD_32BIT:
         bufferPosition = beginX + beginY * (bufferPitch >> 2);
         color32Bit = color.To32Bit();
         break;
   }

   for(int y = beginY; y <= endY; y++)
   {
      //Move the buffer position to the current line
      switch(colorDepth)
      {
         case CD_16BIT:
            bufferPosition = beginX + y * (bufferPitch >> 1);
            break;
         case CD_24BIT:
            bufferPosition = (beginX * 3) + (y * bufferPitch);
            break;
         case CD_32BIT:
            bufferPosition = beginX + y * (bufferPitch >> 2);
            break;
      }        

      //Draw the line
      for(int x = beginX; x <= endX; x++)
      {
         switch(colorDepth)
         {
            case CD_16BIT:
               ((USHORT*)videoBuffer)[bufferPosition] = color16Bit;
               bufferPosition++;
               break;
            case CD_24BIT:
            {
               UINT colorValue = color.To24Bit();
			      ((UCHAR*)videoBuffer)[bufferPosition++] = ((UCHAR*)&colorValue)[0];
			      ((UCHAR*)videoBuffer)[bufferPosition++] = ((UCHAR*)&colorValue)[1];
			      ((UCHAR*)videoBuffer)[bufferPosition++] = ((UCHAR*)&colorValue)[2];
            }
               break;
            case CD_32BIT:
               ((UINT*)videoBuffer)[bufferPosition] = color.To32Bit();
               bufferPosition++;
               break;
         }  
      }
   }
}

//Blit Drawing Functions

/*------------------------------------------------------------------------
Function Name: FillArea
Parameters:
   DG::Rectangle rect: a rectangle describing the upper-left and lower-
      right corners of the area to be filled
   Color& color : the color that the area should be filled with
Description:
   This function fills the area specified by the rectangle with the
   specified color. The function uses the DirectDraw blitter to
   accomplish this, so the surface must not be locked when this function
   is called. The entire area of the rectangle needs to be within the 
   screen. With the blitter comes one limitation: rect.top != bottom and
   rect.left != rect.right.  This means that areas of 1 pixel width or
   height are not allowed, but anything bigger than that is.
------------------------------------------------------------------------*/

void FC Graphics::FillArea(Area& area, Color& color)
{
   assert(surfaceLocked == false);
//  assert(area.left >= 0 && area.left < screenRes.x && area.Right() >= 0 &&
//      area.Right() < screenRes.x && area.top >= 0 && area.top < screenRes.y &&
//      area.Bottom() >= 0 && area.Bottom() < screenRes.y);
   assert(area.width != 0 && area.height != 0);

   HRESULT result;

   DDBLTFX bltFx;
   memset(&bltFx, 0, sizeof(bltFx));
   bltFx.dwSize = sizeof(bltFx);
   
   switch(colorDepth)
   {
      case CD_16BIT:
         bltFx.dwFillColor = color.To16Bit();
         break;
      case CD_24BIT:
         bltFx.dwFillColor = color.To24Bit();
         break;
      case CD_32BIT:
         bltFx.dwFillColor = color.To32Bit();
         break;
   }

   RECT areaRect = area.ToRECT();

   //If the bottom is less than the top or the left is less than
   //the right, they need to be switched
   int tempVariable;

   if(areaRect.right < areaRect.left)
   {
      tempVariable = areaRect.right;
      areaRect.right = areaRect.left;
      areaRect.left = tempVariable;
   }

   if(areaRect.bottom < areaRect.top)
   {
      tempVariable = areaRect.bottom;
      areaRect.bottom = areaRect.top;
      areaRect.top = tempVariable;
   }

   //The right and bottom need to be increased by 1, since they are exclusive
   areaRect.right++;
   areaRect.bottom++;

   result = lpDDSDrawingSurface->Blt(&areaRect, NULL, NULL, 
      DDBLT_COLORFILL | DDBLT_WAIT, &bltFx);

   if(result != DD_OK)
      HandleDDrawError(EC_DDFILLAREA, result, __FILE__, __LINE__);
}

/*------------------------------------------------------------------------
Function Name: FillScreen
Parameters:
   Color& color : the color that the screen should be filled with
Description:
   This function fills the entire screen with the specified color. 
   The function uses the DirectDraw blitter to accomplish this, so the 
   surface must not be locked when this function is called. 
------------------------------------------------------------------------*/

void FC Graphics::FillScreen(Color& color)
{
   assert(surfaceLocked == false);

   HRESULT result;

   DDBLTFX bltFx;
   memset(&bltFx, 0, sizeof(bltFx));
   bltFx.dwSize = sizeof(bltFx);
   
   switch(colorDepth)
   {
      case CD_16BIT:
         bltFx.dwFillColor = color.To16Bit();
         break;
      case CD_24BIT:
         bltFx.dwFillColor = color.To24Bit();
         break;
      case CD_32BIT:
         bltFx.dwFillColor = color.To32Bit();
         break;
   }

   result = lpDDSDrawingSurface->Blt(NULL, NULL, NULL, 
      DDBLT_COLORFILL | DDBLT_WAIT, &bltFx);

   if(result != DD_OK)
      HandleDDrawError(EC_DDFILLSCREEN, result, __FILE__, __LINE__);
}

/*------------------------------------------------------------------------
Function Name: DrawBitmap
Parameters:
   Point& location : the location at which to draw the bitmap, this
      is the coordinate where the upper-left corner of the bitmap will be.
   UINT bitmapID : the ID of the bitmap to be drawn
Description:
   This function draws the bitmap with the specified ID on the screen.
   If there is no clipping, the entire bitmap must fit on the screen.
------------------------------------------------------------------------*/

void FC Graphics::DrawBitmap(Point& location, UINT bitmapID)
{
   assert(surfaceLocked == false);

   Bitmap* bitmap = bitmapList.GetItemById(bitmapID);

#ifdef _DEBUG
   if(!clipping)
   {
      assert(location.x >= 0 && location.x < screenRes.x && 
         location.y >= 0 && location.y < screenRes.y);
      assert((location.x + bitmap->GetWidth()) - 1 < screenRes.x &&
         (location.y + bitmap->GetHeight()) - 1 < screenRes.y);
   }
#endif

   HRESULT result;

   RECT destRect = {location.x, location.y, 
      location.x + bitmap->GetWidth(),
      location.y + bitmap->GetHeight()};

   result = lpDDSDrawingSurface->Blt(&destRect, bitmap->GetDDSurface(), 
      NULL, DDBLT_WAIT, NULL);

   switch(result)
   {
      case DDERR_SURFACELOST:
         RestoreAllSurfaces();
         DrawBitmap(location, bitmapID);
         break;
      case DD_OK:
         break;
      default:
         HandleDDrawError(EC_DDDRAWBMP, result, __FILE__, __LINE__);
         break;
   }
}

/*------------------------------------------------------------------------
Function Name: DrawScaledBitmap
Parameters:
   Area& area: the area in which to draw the bitmap
   UINT bitmapID : the ID of the bitmap to be drawn
Description:
   This function draws the bitmap with the specified ID on the screen
   within the area specified. The bitmap will be automatically scaled to
   fit within the specified area. If no clipping region is defined, the 
   area must be entirely within the screen.
------------------------------------------------------------------------*/

void FC Graphics::DrawScaledBitmap(Area& area, UINT bitmapID)
{
   assert(surfaceLocked == false);

   Bitmap* bitmap = bitmapList.GetItemById(bitmapID);

#ifdef _DEBUG
   if(!clipping)
   {
      assert(area.left >= 0 && area.left < screenRes.x && 
         area.top >= 0 && area.top < screenRes.y);
      assert(area.Right() < screenRes.x &&
         area.Bottom() < screenRes.y);
   }
#endif

   HRESULT result;

   RECT destRect = {area.left, area.top, 
      area.Right() + 1, area.Bottom() + 1};

   result = lpDDSDrawingSurface->Blt(&destRect, bitmap->GetDDSurface(), 
      NULL, DDBLT_WAIT, NULL);

   switch(result)
   {
      case DDERR_SURFACELOST:
         RestoreAllSurfaces();
         DrawScaledBitmap(area, bitmapID);
         break;
      case DD_OK:
         break;
      default:
         HandleDDrawError(EC_DDDRAWBMP, result, __FILE__, __LINE__);
         break;
   }
}

/*------------------------------------------------------------------------
Function Name: DrawTransparentBitmap
Parameters:
   Point& location : the location at which to draw the bitmap, this
      is the coordinate where the upper-left corner of the bitmap will be.
   UINT bitmapID : the ID of the bitmap to be drawn
   Color& transparentColor : the color in the bitmap to be drawn 
      transparently
Description:
   This function draws the bitmap with the specified ID on the screen.
   Any pixel of the specified transparent color in the bitmap will not
   be drawn on the screen. If there is no clipping, the entire bitmap 
   must fit on the screen.
------------------------------------------------------------------------*/

void FC Graphics::DrawTransparentBitmap(Point& location, UINT bitmapID,
   Color& transparentColor)
{
   assert(surfaceLocked == false);

   Bitmap* bitmap = bitmapList.GetItemById(bitmapID);

#ifdef _DEBUG
   if(!clipping)
   {
      assert(location.x >= 0 && location.x < screenRes.x && 
         location.y >= 0 && location.y < screenRes.y);
      assert((location.x + bitmap->GetWidth()) - 1 < screenRes.x &&
         (location.y + bitmap->GetHeight()) - 1 < screenRes.y);
   }
#endif

   HRESULT result;

   RECT destRect = {location.x, location.y, 
      location.x + bitmap->GetWidth(),
      location.y + bitmap->GetHeight()};

   DDCOLORKEY colorKey;
   switch(colorDepth)
   {
      case CD_16BIT:
         colorKey.dwColorSpaceHighValue = transparentColor.To16Bit();
         colorKey.dwColorSpaceLowValue = colorKey.dwColorSpaceHighValue;
         break;
      default:
         colorKey.dwColorSpaceHighValue = transparentColor.To32Bit();
         colorKey.dwColorSpaceLowValue = colorKey.dwColorSpaceHighValue;
         break;
   }

   DDBLTFX bltFx;
   memset(&bltFx, 0, sizeof(bltFx));
   bltFx.dwSize = sizeof(bltFx);
   bltFx.ddckSrcColorkey = colorKey;

   result = lpDDSDrawingSurface->Blt(&destRect, bitmap->GetDDSurface(), 
      NULL, DDBLT_WAIT | DDBLT_KEYSRCOVERRIDE, &bltFx);

   switch(result)
   {
      case DDERR_SURFACELOST:
         RestoreAllSurfaces();
         DrawTransparentBitmap(location, bitmapID, transparentColor);
         break;
      case DD_OK:
         break;
      default:
         HandleDDrawError(EC_DDDRAWBMP, result, __FILE__, __LINE__);
         break;
   }
}

/*------------------------------------------------------------------------
Function Name: DrawTransparentScaledBitmap
Parameters:
   Area& area: the area in which to draw the bitmap
   UINT bitmapID : the ID of the bitmap to be drawn
   Color& transparentColor : the color in the bitmap to be drawn 
      transparently
Description:
   This function draws the bitmap with the specified ID on the screen
   within the area specified. Any pixel of the transparent color in the
   bitmap will not be drawn on the screen. The bitmap will be 
   automatically scaled to fit within the specified area. If no clipping 
   region is defined, the area must be entirely within the screen.
------------------------------------------------------------------------*/

void FC Graphics::DrawTransparentScaledBitmap(Area& area, UINT bitmapID,
   Color& transparentColor)
{
   assert(surfaceLocked == false);

   Bitmap* bitmap = bitmapList.GetItemById(bitmapID);

#ifdef _DEBUG
   if(!clipping)
   {
      assert(area.left >= 0 && area.left < screenRes.x && 
         area.top >= 0 && area.top < screenRes.y);
      assert(area.Right() < screenRes.x &&
         area.Bottom() < screenRes.y);
   }
#endif

   HRESULT result;

   RECT destRect = {area.left, area.top, 
      area.Right() + 1, area.Bottom() + 1};

   DDCOLORKEY colorKey;
   switch(colorDepth)
   {
      case CD_16BIT:
         colorKey.dwColorSpaceHighValue = transparentColor.To16Bit();
         colorKey.dwColorSpaceLowValue = colorKey.dwColorSpaceHighValue;
         break;
      default:
         colorKey.dwColorSpaceHighValue = transparentColor.To32Bit();
         colorKey.dwColorSpaceLowValue = colorKey.dwColorSpaceHighValue;
         break;
   }

   DDBLTFX bltFx;
   memset(&bltFx, 0, sizeof(bltFx));
   bltFx.dwSize = sizeof(bltFx);
   bltFx.ddckSrcColorkey = colorKey;

   result = lpDDSDrawingSurface->Blt(&destRect, bitmap->GetDDSurface(), 
      NULL, DDBLT_WAIT | DDBLT_KEYSRCOVERRIDE, &bltFx);

   switch(result)
   {
      case DDERR_SURFACELOST:
         RestoreAllSurfaces();
         DrawTransparentScaledBitmap(area, bitmapID, transparentColor);
         break;
      case DD_OK:
         break;
      default:
         HandleDDrawError(EC_DDDRAWBMP, result, __FILE__, __LINE__);
         break;
   }
}

/*------------------------------------------------------------------------
Function Name: DrawTransparentBitmap
Parameters:
   Point& location : the location at which to draw the bitmap, this
      is the coordinate where the upper-left corner of the bitmap will be.
   UINT bitmapID : the ID of the bitmap to be drawn
Description:
   This function draws the bitmap with the specified ID on the screen.
   The transparent color used is the transparent color already assigned 
   to the bitmap. Any pixel of the specified transparent color in the 
   bitmap will not be drawn on the screen. If there is no clipping, the 
   entire bitmap must fit on the screen.
------------------------------------------------------------------------*/

void FC Graphics::DrawTransparentBitmap(Point& location, UINT bitmapID)
{
   assert(surfaceLocked == false);

   Bitmap* bitmap = bitmapList.GetItemById(bitmapID);

#ifdef _DEBUG
   if(!clipping)
   {
   assert(location.x >= 0 && location.x < screenRes.x && 
      location.y >= 0 && location.y < screenRes.y);
   assert((location.x + bitmap->GetWidth()) - 1 < screenRes.x &&
      (location.y + bitmap->GetHeight()) - 1 < screenRes.y);
   }
#endif

   HRESULT result;

   RECT destRect = {location.x, location.y, 
      (location.x + bitmap->GetWidth()) - 1,
      (location.y + bitmap->GetHeight()) - 1};

   result = lpDDSDrawingSurface->Blt(&destRect, bitmap->GetDDSurface(), 
      NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

   switch(result)
   {
      case DDERR_SURFACELOST:
         RestoreAllSurfaces();
         DrawTransparentBitmap(location, bitmapID);
         break;
      case DD_OK:
         break;
      default:
         HandleDDrawError(EC_DDDRAWBMP, result, __FILE__, __LINE__);
         break;
   }
}

/*------------------------------------------------------------------------
Function Name: DrawTransparentScaledBitmap
Parameters:
   Area& area: the area in which to draw the bitmap
   UINT bitmapID : the ID of the bitmap to be drawn
Description:
   This function draws the bitmap with the specified ID on the screen
   within the area specified. The transparent color used is the
   transparent color already assigned to the bitmap. Any pixel of the 
   transparent color in the bitmap will not be drawn on the screen. 
   The bitmap will be automatically scaled to fit within the specified 
   area. If no clipping region is defined, the area must be entirely 
   within the screen.
------------------------------------------------------------------------*/

void FC Graphics::DrawTransparentScaledBitmap(Area& area, UINT bitmapID)
{
   assert(surfaceLocked == false);

   Bitmap* bitmap = bitmapList.GetItemById(bitmapID);

#ifdef _DEBUG
   if(!clipping)
   {
      assert(area.left >= 0 && area.left < screenRes.x && 
         area.top >= 0 && area.top < screenRes.y);
      assert(area.Right() < screenRes.x &&
         area.Bottom() < screenRes.y);
   }
#endif

   HRESULT result;

   RECT destRect = {area.left, area.top, 
      area.Right(), area.Bottom()};

   result = lpDDSDrawingSurface->Blt(&destRect, bitmap->GetDDSurface(), 
      NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

   switch(result)
   {
      case DDERR_SURFACELOST:
         RestoreAllSurfaces();
         DrawTransparentScaledBitmap(area, bitmapID);
         break;
      case DD_OK:
         break;
      default:
         HandleDDrawError(EC_DDDRAWBMP, result, __FILE__, __LINE__);
         break;
   }
}


//GDI Drawing Functions

/*------------------------------------------------------------------------
Function Name: FlipToGDISurface()
Parameters:
Description:
   This function needs to be called whenever a MS Windows dialog is 
   displayed, so that it always will appear on the screen. If this 
   function is not called, the dialog box will occasionally not be
   displayed, as it is sitting in a back buffer somewhere.
------------------------------------------------------------------------*/

void Graphics::FlipToGDISurface()
{
   lpDD->FlipToGDISurface();
}

/*------------------------------------------------------------------------
Function Name: SetGDIFont()
Parameters:
   Font& font : the font to be used when drawing text
Description:
   This function sets the GDI font that the framework uses to draw text
   on the screen.
------------------------------------------------------------------------*/

void FC Graphics::SetGDIFont(Font& font)
{
   currentGDIFont = font;
}

/*------------------------------------------------------------------------
Function Name: SetTextTransparency()
Parameters:
   int mode : the transparency mode to be used when drawing text, valid
      flags are OPAQUE and TRANSPARENT
Description:
   This function whether the background box around the text should be
   drawn opaquely or transparently.
------------------------------------------------------------------------*/

void FC Graphics::SetTextTransparency(int mode)
{
   assert(mode == OPAQUE || mode == TRANSPARENT);

   textTransparencyMode = mode;
}

/*------------------------------------------------------------------------
Function Name: SetTextColor()
Parameters:
   Color& color : the color to be used when drawing the actual text
Description:
   This function sets the color that the framework uses to draw the
   actual letters of any text drawn on the screen
------------------------------------------------------------------------*/

void FC Graphics::SetTextColor(Color& color)
{
   textColor = color;
}

/*------------------------------------------------------------------------
Function Name: SetTextBackgroundColor()
Parameters:
   Color& color : the color to be used when drawing the background
      behind the text
Description:
   This function sets the color that the framework uses to draw the
   background any text drawn on the screen
------------------------------------------------------------------------*/

void FC Graphics::SetTextBackgroundColor(Color& color)
{
   textBackgroundColor = color;
}

/*------------------------------------------------------------------------
Function Name: DrawText()
Parameters:
   char* text : an array of characters containing the string to be drawn
   DG::Rectangle& rect : the bounding rectangle on the screen
   UINT flags : these are the same flags used in the Win32 API DrawText()
      function; there are a lot of them, so see the Win32 API Reference
      for more information
Description:
   This function draws the specified text within the bounding rectangle
   according to the flags. This is a wrapper around the Win32 API
   DrawText() function and behaves about the same.
------------------------------------------------------------------------*/

void FC Graphics::DrawText(char* text, DG::Rectangle& rect, UINT flags)
{
   HDC hDC;
   HRESULT result;

   result = lpDDSDrawingSurface->GetDC(&hDC);

   HRGN clippingRegion;
   if(clipping)
   {
      clippingRegion = CreateRectRgnIndirect(&clippingRect.ToRECT());
      SelectClipRgn(hDC, clippingRegion);
   }

   ::SetTextColor(hDC, textColor.ToCOLORREF());
   ::SetBkColor(hDC, textBackgroundColor.ToCOLORREF());
   ::SetBkMode(hDC, textTransparencyMode);

   HFONT font = currentGDIFont.GetFontHandle();
   HFONT hOldFont = (HFONT)SelectObject(hDC, font);

   switch(result)
   {
      case DDERR_SURFACELOST:
         if(clipping)
            DeleteObject((HGDIOBJ)(HRGN)clippingRegion);
         RestoreAllSurfaces();
         DrawText(text, rect, flags);
         break;
      case DD_OK:
         break;
      default:
         if(clipping)
            DeleteObject((HGDIOBJ)(HRGN)clippingRegion);
         HandleDDrawError(EC_DDTEXT, result, __FILE__, __LINE__);
         break;
   }

   RECT rectStruct = rect.ToRECT();

   ::DrawText(hDC, text, -1, &rectStruct, flags | DT_EXPANDTABS |
      DT_NOPREFIX);

   //If one of the flags is DT_CALCRECT, then we need to store the RECT
   //struct that came back from DrawText in rect
   if(flags & DT_CALCRECT)
      rect.SetRectangle(rectStruct.left, rectStruct.top, rectStruct.right,
         rectStruct.bottom);

   font = (HFONT)SelectObject(hDC, hOldFont);

   result = lpDDSDrawingSurface->ReleaseDC(hDC);

   if(clipping)
      DeleteObject((HGDIOBJ)(HRGN)clippingRegion);
}

//Clipping Functions
/*------------------------------------------------------------------------
Function Name: AddClippingArea()
Parameters:
   DG::Rectangle& rect : the clipping area to be added
Description:
   This function adds the clipping area to the clip list on the drawing
   surface. At the moment only one clipping area can be in the clip list,
   so the given clipping area will replace the previous clipping area.
------------------------------------------------------------------------*/

void FC Graphics::AddClippingArea(DG::Rectangle& rect)
{
//   assert(rect.left >= 0 && rect.left < screenRes.x &&
//      rect.right >= 0 && rect.right < screenRes.x &&
//      rect.top >= 0 && rect.top < screenRes.y &&
//      rect.bottom >= 0 && rect.bottom < screenRes.y);

   int tempVariable;
   if(rect.left > rect.right)
   {
      tempVariable = rect.left;
      rect.left = rect.right;
      rect.right = tempVariable;
   }

   if(rect.top > rect.bottom)
   {
      tempVariable = rect.top;
      rect.top = rect.bottom;
      rect.bottom = tempVariable;
   }

   //Store the clipping rect in a DG::Rectangle object so we can
   //easily use it later
   clippingRect = rect;

   RECT areaRect = rect.ToRECT();

   //The right and bottom appear to be exclusive with a clipping 
   //rectangle, so we must increment them by 1
   areaRect.right++;
   areaRect.bottom++;

   //Add the clipping list to the DirectDraw clipper object
   memcpy(clippingArea->Buffer, &areaRect, sizeof(RECT));
   clippingArea->rdh.rcBound = areaRect;
   lpDSClipper->SetClipList(clippingArea, 0);

   HRESULT result;
   result = lpDDSDrawingSurface->SetClipper(lpDSClipper);

   if(result != DD_OK)
      HandleDDrawError(EC_DDCLIPPING, result, __FILE__, __LINE__);      

   clipping = true;
}

/*------------------------------------------------------------------------
Function Name: RemoveClippingArea()
Parameters:
Description:
   This function removes the clipping area from the drawing surface.
   After the function is called, there will be no clipping areas on the
   drawing surface.
------------------------------------------------------------------------*/

void FC Graphics::RemoveClippingArea()
{
   //We now aren't clipping anymore and need to remove the clipper object
   //from the surface
   clipping = false;
   lpDDSDrawingSurface->SetClipper(NULL);
}

/*------------------------------------------------------------------------
Function Name: ClearClippingAreas()
Parameters:
Description:
   This function removes all the clipping area from the drawing surface.
   The function is the same as RemoveClippingArea for now, but this will
   change once multiple clipping areas are implemented.
------------------------------------------------------------------------*/

void FC Graphics::ClearClippingAreas()
{
   RemoveClippingArea();
}

//Functions to get and release a DG::Surface

/*------------------------------------------------------------------------
Function Name: GetSurface()
Parameters:
   Area& area : the area on the screen to represent as a surface
Description:
   This function receives an area describing an area of the screen
   for which a DG::Surface object is to be created. Creating a surface 
   will set a clipper, removing any previous clipper. As little as possible 
   should be done to the DG::Graphics object until the surface is released. 
   The surface must be released after the caller is done drawing upon it.
------------------------------------------------------------------------*/

Surface* FC Graphics::GetSurface(Area& area)
{
   if(currentSurface != NULL)
      return NULL;
   
   currentSurface = new Surface(area);
   AddClippingArea(DG::Rectangle(area.left, area.top, area.Right(), 
      area.Bottom()));
   return currentSurface;
}

/*------------------------------------------------------------------------
Function Name: ReleaseSurface()
Parameters:
   Surface* surface : the pointer to the surface to be released 
Description:
   This function releases the current surface. In case multiple surfaces
   are ever implemented, there is a DG::Surface* parameter. At the moment,
   however, this pointer has no effect on the function.
------------------------------------------------------------------------*/

void FC Graphics::ReleaseSurface(Surface* surface)
{
   if(currentSurface == NULL)
      return;

   delete currentSurface;
   currentSurface = NULL;
   RemoveClippingArea();
}

//Private Functions

/*------------------------------------------------------------------------
Function Name: RestoreAllSurfaces
Parameters:
Description:
   This function is called whenever a DirectDraw function returns
   DDERR_SURFACELOST, which means that the surfaces have been lost.
   When this function is called, the primary and secondary surfaces are
   restored. In addition, all the surfaces of the bitmaps that are 
   currently loaded are restored and the bitmaps are reloaded.
------------------------------------------------------------------------*/

void FC Graphics::RestoreAllSurfaces()
{
   HRESULT result;

   //Try to restore the primary surface
   result = lpDDSPrimary->Restore();

   //Check to see if the display mode has been changed
   if(result == DDERR_WRONGMODE)
   {
      SetGraphicsMode(screenRes, windowedState, colorDepth, 
         bufferingMode, SSPersistence);
      
      result = lpDDSPrimary->Restore();
   }

   if(result != DD_OK)
      HandleDDrawError(EC_DDRESTORESURFACES, result, __FILE__, __LINE__);

   //Reload the bitmaps
   bitmapList.RestoreBitmaps();
}

/*------------------------------------------------------------------------
Function Name: DestroyAllSurfaces()
Parameters:
Description:
   This function destroys all DirectDraw surfaces.
------------------------------------------------------------------------*/

void FC Graphics::DestroyAllSurfaces()
{
   //Get rid of the old surfaces
   if(lpDDSPrimary != NULL)
   {
      lpDDSPrimary->Release();
      lpDDSPrimary = NULL;
   }

   //Don't release the secondary surface when it is part of
   //a flipping chain.
   if(lpDDSSecondary != NULL && SSPersistence == true)
   {
      lpDDSSecondary->Release();
      lpDDSSecondary = NULL;
   }

   else
      lpDDSSecondary = NULL;

   //Get rid of old clip lists
   if(lpPSClipper != NULL)
   {
      lpPSClipper->Release();
      lpPSClipper = NULL;
   }

   if(lpDSClipper != NULL)
   {
      lpDSClipper->Release();
      lpDSClipper = NULL;
   }
}

/*------------------------------------------------------------------------
Function Name: ClearPrimarySurface()
Parameters:
Description:
   This function clears the primary surface by filling it with black.
   It is assumed that the surface is unlocked when calling this function.
------------------------------------------------------------------------*/

void FC Graphics::ClearPrimarySurface()
{
   assert(surfaceLocked == false);

   HRESULT result;

   DDBLTFX bltFx;
   bltFx.dwSize = sizeof(bltFx);
   bltFx.dwFillColor = 0; 

   result = lpDDSDrawingSurface->Blt(NULL, NULL, NULL, 
      DDBLT_COLORFILL | DDBLT_WAIT, &bltFx);

   if(result != DD_OK)
      HandleDDrawError(EC_DDCLEARPRIMARY, result, __FILE__, __LINE__);
}

/*------------------------------------------------------------------------
Function Name: HandleDDrawError()
Parameters:
   UINT errorCode : the error code
   HRESULT : the DirectDraw error returned from a DirectDraw function
   char* fileName : the file the error occurred in
   UINT lineNumber : the line number at which this function was called
Description:
   When a DirectDraw function returns an HRESULT that specifies an error,
   this function can be called to handle that error. This function throws
   and exception which gets caught by the framework. The framework will
   display the error in a message box and then terminate the application.
------------------------------------------------------------------------*/

void Graphics::HandleDDrawError(UINT errorCode, HRESULT error, 
   char* fileName, UINT lineNumber)
{
   if(error == DD_OK)
      return;

   char errorText[512];

   switch(error)
   {
      case DDERR_ALREADYINITIALIZED:
         strcpy(errorText, "DirectDraw object has already been initialized.");
         break;
      case DDERR_BLTFASTCANTCLIP:
         strcpy(errorText, "BltFast() cannot be used with a surface "\
            "that has a DirectDrawClipper object attached.");
         break;
      case DDERR_CANNOTATTACHSURFACE: 
         strcpy(errorText, "The surface cannot be attached.");
         break;
      case DDERR_CANNOTDETACHSURFACE: 
         strcpy(errorText, "The surface cannot be detached.");
         break;
      case DDERR_CANTCREATEDC: 
         strcpy(errorText, "The Windows device context could not be "\
            "created.");
         break;
      case DDERR_CANTDUPLICATE:
         strcpy(errorText, "This surface cannot be duplicated.");
         break;
      case DDERR_CANTLOCKSURFACE:
         strcpy(errorText, "The surface cannot be locked.");
         break;
      case DDERR_CANTPAGELOCK: 
         strcpy(errorText, "Page lock failed.");
         break;
      case DDERR_CANTPAGEUNLOCK:
         strcpy(errorText, "Page unlock failed.");
         break;
      case DDERR_CLIPPERISUSINGHWND:
         strcpy(errorText, "The clip list could not be added, as the "\
            "DirectDrawClipper object is already monitoring a window "\
            "handle.");
         break;
      case DDERR_COLORKEYNOTSET: 
         strcpy(errorText, "No source color key has been set");
         break;
      case DDERR_CURRENTLYNOTAVAIL: 
         strcpy(errorText, "No support is currently available for this "\
            "operation");
         break;
      case DDERR_DDSCAPSCOMPLEXREQUIRED: 
         strcpy(errorText, "This surface needs to be a complex surface");
         break;
      case DDERR_DCALREADYCREATED:
         strcpy(errorText, "A device context has already been created for "\
            "this surface");
         break;
      case DDERR_DEVICEDOESNTOWNSURFACE:
         strcpy(errorText, "This surface is owned by another DirectDraw device "\
            "and cannot be used");
         break;
      case DDERR_DIRECTDRAWALREADYCREATED:
         strcpy(errorText, "A DirectDraw object representing this driver has "\
            "already been created.");
         break;
      case DDERR_EXCEPTION:
         strcpy(errorText, "An exception was encountered while performing "\
            "the requested operation.");
         break;
     case DDERR_EXCLUSIVEMODEALREADYSET:
         strcpy(errorText, "Exclusive mode has already been set.");
         break;
      case DDERR_EXPIRED:
         strcpy(errorText, "The data has expired and is invalid.");
         break;
      case DDERR_GENERIC: 
         strcpy(errorText, "An undefined error condition has occurred.");
         break;
      case DDERR_HEIGHTALIGN: 
         strcpy(errorText, "The height of the provided rectangle is not a "\
            "multiple of the required alignment.");
         break;
      case DDERR_HWNDALREADYSET:
         strcpy(errorText, "The cooperative-level window handle has already been "\
            "set.");
         break;
      case DDERR_HWNDSUBCLASSED:
         strcpy(errorText, "DirectDraw cannot restore because the DirectDraw "\
            "cooperative-level window handle has been subclassed.");
         break;
      case DDERR_IMPLICITLYCREATED:
         strcpy(errorText, "Surface cannot be restored because it was implicitly "\
            "created");
         break;
      case DDERR_INCOMPATIBLEPRIMARY:
         strcpy(errorText, "The primary surface creation request does not match "\
            "the existing primary surface.");
         break;
      case DDERR_INVALIDCAPS: 
         strcpy(errorText, "One or more of the capability bits passed to the "\
            "callback function are incorrect.");
         break;
      case DDERR_INVALIDCLIPLIST:
         strcpy(errorText, "DirectDraw does not support the provided clip list.");
         break;
      case DDERR_INVALIDDIRECTDRAWGUID: 
         strcpy(errorText, "The provided GUID is an invalid DirectDraw GUID");
         break;
      case DDERR_INVALIDMODE:
         strcpy(errorText, "DirectDraw does not support the requested mode.");
         break;
      case DDERR_INVALIDOBJECT:
         strcpy(errorText, "DirectDraw received a pointer to an invalid object.");
         break;
      case DDERR_INVALIDPARAMS: 
         strcpy(errorText, "One or more parameters passed to this method are "\
            "incorrect.");
         break;
      case DDERR_INVALIDPIXELFORMAT: 
         strcpy(errorText, "The specified pixel format was invalid.");
         break;
      case DDERR_INVALIDPOSITION: 
         strcpy(errorText, "The position of the overlay on the destination is "\
            "no longer legal.");
         break;
      case DDERR_INVALIDRECT: 
         strcpy(errorText, "The provided rectangle was invalid.");
         break;
      case DDERR_INVALIDSTREAM: 
         strcpy(errorText, "The specified stream contains invalid data.");
         break;
      case DDERR_INVALIDSURFACETYPE:
         strcpy(errorText, "The surface was of the wrong type.");
         break;
      case DDERR_LOCKEDSURFACES:
         strcpy(errorText, "This operation failed due to a locked surface.");
         break;
      case DDERR_MOREDATA:
         strcpy(errorText, "There is more data available than the specified buffer "\
            "can hold.");
         break;
      case DDERR_NO3D:
         strcpy(errorText, "No 3-D hardware or emulation is present.");
         break;
      case DDERR_NOALPHAHW: 
         strcpy(errorText, "No alpha-acceleration hardware is present or available.");
         break;
      case DDERR_NOBLTHW: 
         strcpy(errorText, "No blitter hardware is present.");
         break;
      case DDERR_NOCLIPLIST: 
         strcpy(errorText, "No clip list is available.");
         break;
      case DDERR_NOCLIPPERATTACHED: 
         strcpy(errorText, "No DirectDrawClipper is attached to the surface.");
         break;
      case DDERR_NOCOLORCONVHW: 
         strcpy(errorText, "No color-conversion hardware is present or available.");
         break;
      case DDERR_NOCOLORKEY: 
         strcpy(errorText, "The surface does not currently have a color key.");
         break;
      case DDERR_NOCOLORKEYHW: 
         strcpy(errorText, "There is no hardware support for the destination color "\
            "key.");
         break;
      case DDERR_NOCOOPERATIVELEVELSET: 
         strcpy(errorText, "No cooperative level had been set.");
         break;
      case DDERR_NODC: 
         strcpy(errorText, "No device context exists for this surface.");
         break;
      case DDERR_NODDROPSHW: 
         strcpy(errorText, "No DirectDraw raster-operation (ROP) hardware is"\
            "available.");
         break;
      case DDERR_NODIRECTDRAWHW: 
         strcpy(errorText, "Hardware-only DirectDraw creation not possible.");
         break;
      case DDERR_NODIRECTDRAWSUPPORT: 
         strcpy(errorText, "DirectDraw support is not possible with the current"\
            "display driver.");
         break;
      case DDERR_NODRIVERSUPPORT: 
         strcpy(errorText, "Testing cannot proceed due to no display driver support.");
         break;
      case DDERR_NOEMULATION: 
         strcpy(errorText, "Software emulation not available.");
         break;
      case DDERR_NOEXCLUSIVEMODE: 
         strcpy(errorText, "This operation requires the application to have "\
            "exclusive mode.");
         break;
      case DDERR_NOFLIPHW: 
         strcpy(errorText, "Flipping visible surfaces not supported.");
         break;
      case DDERR_NOFOCUSWINDOW:
         strcpy(errorText, "Cannot create or set a device window without having "\
            "first set the focus window.");
         break;         
      case DDERR_NOGDI: 
         strcpy(errorText, "No GDI is present.");
         break;
      case DDERR_NOHWND:
         strcpy(errorText, "No cooperative-level window handle.");
         break;
      case DDERR_NOMIPMAPHW: 
         strcpy(errorText, "No mipmap-capable texture mapping hardware is present "\
            "or available.");
         break;
      case DDERR_NOMIRRORHW: 
         strcpy(errorText, "No mirroring hardware is present of available.");
         break;
      case DDERR_NOMONITORINFORMATION: 
         strcpy(errorText, "No monitor information exists.");
         break;
      case DDERR_NONONLOCALVIDMEM:
         strcpy(errorText, "No nonlocal video memory is available.");
         break;
      case DDERR_NOOPTIMIZEHW: 
         strcpy(errorText, "Optimized surfaces not supported.");
         break;
      case DDERR_NOOVERLAYDEST: 
         strcpy(errorText, "Overlay is not a destination.");
         break;
      case DDERR_NOOVERLAYHW: 
         strcpy(errorText, "No overlay hardware present or available.");
         break;
      case DDERR_NOPALETTEATTACHED: 
         strcpy(errorText, "No palette object is attached to the surface.");
         break;
      case DDERR_NOPALETTEHW: 
         strcpy(errorText, "No hardware support for 16- or 256-color palettes.");
         break;
      case DDERR_NORASTEROPHW: 
         strcpy(errorText, "No appropriate raster-operation hardware is present "\
            " or available.");
         break;
      case DDERR_NOROTATIONHW: 
         strcpy(errorText, "No rotation hardware is present or available.");
         break;
      case DDERR_NOSTEREOHARDWARE: 
         strcpy(errorText, "No stereo hardware is present or available.");
         break;
      case DDERR_NOSTRETCHHW: 
         strcpy(errorText, "No hardware support for stretching.");
         break;
      case DDERR_NOSURFACELEFT: 
         strcpy(errorText, "No hardware that supports stereo surfaces.");
         break;
      case DDERR_NOT4BITCOLOR:
         strcpy(errorText, "This operation requires a 4-bit color palette.");
         break;
      case DDERR_NOT4BITCOLORINDEX: 
         strcpy(errorText, "This operation requires a 4-bit color *index* palette.");
         break;
      case DDERR_NOT8BITCOLOR: 
         strcpy(errorText, "This operation requires an 8-bit color palette.");
         break;
      case DDERR_NOTAOVERLAYSURFACE: 
         strcpy(errorText, "Not an overlay surface.");
         break;
      case DDERR_NOTEXTUREHW: 
         strcpy(errorText, "No texture-mapping hardware is present or available.");
         break;
      case DDERR_NOTFLIPPABLE: 
         strcpy(errorText, "This surface cannot be flipped.");
         break;
      case DDERR_NOTFOUND: 
         strcpy(errorText, "The requested item was not found.");
         break;
      case DDERR_NOTINITIALIZED:
         strcpy(errorText, "DirectDraw object has not been initialized.");
         break;
      case DDERR_NOTLOADED: 
         strcpy(errorText, "Surface has not been allocated any memory.");
         break;
      case DDERR_NOTLOCKED: 
         strcpy(errorText, "Surface was not locked.");
         break;
      case DDERR_NOTPAGELOCKED: 
         strcpy(errorText, "Surface was not page-locked.");
         break;
      case DDERR_NOTPALETTIZED: 
         strcpy(errorText, "Surface is not palette-based.");
         break;
      case DDERR_NOVSYNCHW: 
         strcpy(errorText, "There is no hardware support for vertical blank or "\
            "synchronized operations.");
         break;
      case DDERR_NOZBUFFERHW: 
         strcpy(errorText, "There is no z-buffer hardware support.");
         break;
      case DDERR_NOZOVERLAYHW:
         strcpy(errorText, "Overlay surfaces cannot be z-layered: no hardware "\
            "support.");
         break;
      case DDERR_OUTOFCAPS: 
         strcpy(errorText, "The hardware needed for this operation has already "\
            "been allocated.");
         break;
      case DDERR_OUTOFMEMORY: 
         strcpy(errorText, "There is not enough memory to perform this operation.");
         break;
      case DDERR_OUTOFVIDEOMEMORY: 
         strcpy(errorText, "There is not enough *display* memory to perform this "\
            "operation.");
         break;
      case DDERR_OVERLAPPINGRECTS: 
         strcpy(errorText, "Source and destination rectangles are on the same "\
            "surface and overlap each other.");
         break;
      case DDERR_OVERLAYCANTCLIP: 
         strcpy(errorText, "The hardware does not support clipped overlays.");
         break;
      case DDERR_OVERLAYCOLORKEYONLYONEACTIVE: 
         strcpy(errorText, "Only one color key can be active on an overlay.");
         break;
      case DDERR_OVERLAYNOTVISIBLE :
         strcpy(errorText, "The specified overlay is hidden.");
         break;
      case DDERR_PALETTEBUSY: 
         strcpy(errorText, "The palette is busy.");
         break;
      case DDERR_PRIMARYSURFACEALREADYEXISTS: 
         strcpy(errorText, "A primary surface already exists.");
         break;
      case DDERR_REGIONTOOSMALL: 
         strcpy(errorText, "The specified clip region is too small.");
         break;
      case DDERR_SURFACEALREADYATTACHED: 
         strcpy(errorText, "The surface was already attached.");
         break;
      case DDERR_SURFACEALREADYDEPENDENT: 
         strcpy(errorText, "The surface was already dependent.");
         break;
      case DDERR_SURFACEBUSY: 
         strcpy(errorText, "The surface is busy.");
         break;
      case DDERR_SURFACEISOBSCURED: 
         strcpy(errorText, "Access to surface denied: surface is obscured.");
         break;
      case DDERR_SURFACELOST: 
         strcpy(errorText, "The surface has been lost: it must be restored.");
         break;
      case DDERR_SURFACENOTATTACHED: 
         strcpy(errorText, "The requested surface is not attached.");
         break;
      case DDERR_TOOBIGHEIGHT: 
         strcpy(errorText, "The requested height is too large.");
         break;
      case DDERR_TOOBIGSIZE:
         strcpy(errorText, "The requested size is too large.");
         break;
      case DDERR_TOOBIGWIDTH: 
         strcpy(errorText, "The requested width is too large.");
         break;
      case DDERR_UNSUPPORTED: 
         strcpy(errorText, "This operation is not supported.");
         break;
      case DDERR_UNSUPPORTEDFORMAT: 
         strcpy(errorText, "The pixel format is not supported by DirectDraw.");
         break;
      case DDERR_UNSUPPORTEDMASK: 
         strcpy(errorText, "The bitmask in the pixel format is not supported by "\
            "DirectDraw.");
         break;
      case DDERR_UNSUPPORTEDMODE: 
         strcpy(errorText, "The display is currently in an unsupported mode.");
         break;
      case DDERR_VERTICALBLANKINPROGRESS: 
         strcpy(errorText, "A vertical blank is in progress.");
         break;
      case DDERR_VIDEONOTACTIVE: 
         strcpy(errorText, "The video port is not active.");
         break;
      case DDERR_WASSTILLDRAWING: 
         strcpy(errorText, "The operation was still drawing.");
         break;
      case DDERR_WRONGMODE: 
         strcpy(errorText, "This surface cannot be restored: it was created in "\
            "a different mode.");
         break;
      case DDERR_XALIGN: 
         strcpy(errorText, "The rectangle was not horizontally aligned on a "\
            "required boundary.");
         break;
      default:
         strcpy(errorText, "Unknown Error");
         break;
   }

   throw(new Exception(errorText, errorCode, ET_DIRECTDRAW, fileName,
      lineNumber));
}

HRESULT CALLBACK DG::EnumDisplayModes(LPDDSURFACEDESC2 lpDDSurfaceDesc,  
      LPVOID lpContext)
{
   GetGraphics()->displayModeList.Insert(new DisplayMode(lpDDSurfaceDesc->dwWidth,
      lpDDSurfaceDesc->dwHeight, lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount));

   return DDENUMRET_OK;
}
