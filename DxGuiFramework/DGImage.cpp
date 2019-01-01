/*------------------------------------------------------------------------
File Name: DGImage.cpp
Description: This file contains the implementation of the DG::Image class, 
   which is a control that displays a bitmap.
Version:
   1.0.0    09.09.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

Image::Image(UINT winID, Window* parentWin, const int xPos, 
                 const int yPos, const int width, 
                 const int height, UINT _bitmapID, bool transparent, 
                 Color transparentColor, bool _parentNotify) :
   Window(winID, true, xPos, yPos, width, height)
{
   SetWindowType(WT_IMAGE);
   SetParent(parentWin);

   bitmapID = _bitmapID;
   transparentBitmap = transparent;
   transparentColor = transparentColor;
   parentNotify = _parentNotify;

   bitmapOrigin = Point(0, 0);
   strcpy(bitmapFileName, "");
}

Image::Image(UINT winID, Window* parentWin, 
                 const Area& dimensions,
                 UINT _bitmapID, bool transparent, 
                 Color transparentColor, bool _parentNotify) :
   Window(winID, true, dimensions)
{
   SetWindowType(WT_IMAGE);
   SetParent(parentWin);

   bitmapID = _bitmapID;
   transparentBitmap = transparent;
   transparentColor = transparentColor;
   parentNotify = _parentNotify;

   bitmapOrigin = Point(0, 0);
   strcpy(bitmapFileName, "");
}

Image::Image(UINT winID, Window* parentWin,
                 const Point& position, const Point& size, 
                 UINT _bitmapID, bool transparent, 
                 Color transparentColor, bool _parentNotify) :
   Window(winID, true, position, size)
{
   SetWindowType(WT_IMAGE);
   SetParent(parentWin);

   bitmapID = _bitmapID;
   transparentBitmap = transparent;
   transparentColor = transparentColor;
   parentNotify = _parentNotify;

   bitmapOrigin = Point(0, 0);
   strcpy(bitmapFileName, "");
}

/*------------------------------------------------------------------------
Function Name: LoadBitmapFile
Description:
   This function loads the bitmap contained in a bitmap file into the
   bitmap list. The bitmap is associated with the bitmap ID of this image
   control. If there already exists a bitmap in the GUI bitmap list with
   bitmapID, then that bitmap will be unloaded.
   The image control will display the bitmap if it was loaded
   successfully.
Parameters: const char* bitmapName - the name of a bitmap file from 
               which a bitmap is to be loaded.
            int priority - the priority of the bitmap to be loaded
Preconditions: bitmapName points to a null-terminated character array
------------------------------------------------------------------------*/

void Image::LoadBitmapFile(const char* bitmapName, int priority)
{
   //If there is a bitmap with this ID already in the bitmap list, 
   //then unload it
   dgGraphics->DeleteBitmap(bitmapID);

   dgGraphics->LoadBitmap(bitmapID, priority, bitmapName);
   strcpy(bitmapFileName, bitmapName);

   if(transparentBitmap)
   {
      Bitmap* bitmap = dgGraphics->GetBitmap(bitmapID);
      assert(bitmap != NULL);
      bitmap->SetTransparentColor(transparentColor);
   }

   //Set the origin of the bitmap depending on whether it is smaller or
   //larger than the image control
/*   Point bitmapSize = GetBitmapSize();
   Point windowSize = GetSize();

   if(bitmapSize.x < windowSize.x)
      bitmapOrigin.x = (windowSize.x - bitmapSize.x) / 2;
   else
      bitmapOrigin.x = 0;

   if(bitmapSize.y < windowSize.y)
      bitmapOrigin.y = (windowSize.y - bitmapSize.y) / 2;
   else
      bitmapOrigin.y = 0;*/

   OnWindowSized();
}

/*------------------------------------------------------------------------
Function Name: SetTransparency
Description:
   This function sets whether the bitmap displayed in this control will
   be drawn with a transparent color
Parameters: bool transparent - true if the transparent color is to be
               used in drawing the bitmap, false otherwise
------------------------------------------------------------------------*/

void Image::SetTransparency(bool transparent)
{
   transparentBitmap = transparent;

   if(transparentBitmap)
   {
      Bitmap* bitmap = dgGraphics->GetBitmap(bitmapID);
      if(bitmap != NULL)
         bitmap->SetTransparentColor(transparentColor);
   }   
}

/*------------------------------------------------------------------------
Function Name: SetTransparentColor
Description:
   This function sets which color in the bitmap displayed by this control
   will be transparent. A transparent color is a color which is not drawn
   on the screen.
Parameters: Color color - the color in the bitmap to be made 
               transparent
------------------------------------------------------------------------*/

void Image::SetTransparentColor(Color color)
{
   transparentColor = color;

   if(transparentBitmap)
   {
      Bitmap* bitmap = dgGraphics->GetBitmap(bitmapID);
      if(bitmap != NULL)
         bitmap->SetTransparentColor(transparentColor);
   }
}

/*------------------------------------------------------------------------
Function Name: SetBitmapOrigin
Description:
   This function changes where the upper-left corner of the bitmap is 
   displayed in the window
Parameters: Point coords - the window coordinates of where the 
               upper-left corner of the bitmap will be drawn
------------------------------------------------------------------------*/

void Image::SetBitmapOrigin(Point coords)
{
   bitmapOrigin = coords;
}

/*------------------------------------------------------------------------
Function Name: SetBitmapSize
Description:
   This function returns the size of the bitmap that is being displayed.
Returns: the size of the bitmap being displayed by this control.
   If no bitmap is currently being displayed, then (0, 0) will be 
   returned.
------------------------------------------------------------------------*/

Point Image::GetBitmapSize()
{
   Point bitmapSize(0, 0);

   Bitmap* bitmap = dgGraphics->GetBitmap(bitmapID);
   if(bitmap != NULL)
      bitmapSize = Point(bitmap->GetWidth(), bitmap->GetHeight());

   return bitmapSize;
}

void FC Image::OnDrawWindow(WindowSurface* surface)
{
   Bitmap* bitmap = dgGraphics->GetBitmap(bitmapID);

   if(bitmap != NULL)
   {
      if(transparentBitmap)
         surface->DrawTransparentBitmap(bitmapOrigin, bitmapID);
      else
         surface->DrawBitmap(bitmapOrigin, bitmapID);
   }
   else if(!transparentBitmap)
      surface->FillSurface(Color(128, 128, 128));
}

void FC Image::OnLButtonDown(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_LBUTTONDOWN,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnLButtonUp(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_LBUTTONUP,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnLButtonDblClk(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_LBUTTONDBLCLK,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnMButtonDown(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_MBUTTONDOWN,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnMButtonUp(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_MBUTTONUP,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnMButtonDblClk(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_MBUTTONDBLCLK,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnRButtonDown(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_RBUTTONDOWN,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnRButtonUp(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_RBUTTONUP,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnRButtonDblClk(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_RBUTTONDBLCLK,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnMouseMove(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_IMAGE_MOUSEMOVE,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Image::OnWindowSized()
{
   //Set the origin of the bitmap depending on whether it is smaller or
   //larger than the image control
   Point bitmapSize = GetBitmapSize();
   Point windowSize = GetSize();

   if(bitmapSize.x < windowSize.x)
      bitmapOrigin.x = (windowSize.x - bitmapSize.x) / 2;
   else
      bitmapOrigin.x = bitmapOrigin.x;

   if(bitmapSize.y < windowSize.y)
      bitmapOrigin.y = (windowSize.y - bitmapSize.y) / 2;
   else
      bitmapOrigin.y = bitmapOrigin.y;
}
