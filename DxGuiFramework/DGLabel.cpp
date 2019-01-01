/*------------------------------------------------------------------------
File Name: DGLabel.cpp
Description: This file contains the implementation of the DG::Label class, 
   which is a control that displays text.
Version:
   1.0.0    11.07.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

Label::Label(UINT winID, Window* parentWin) : 
   Window(winID, true)
{
   SetWindowType(WT_LABEL);
   SetParent(parentWin);
}

Label::Label(UINT winID, Window* parentWin, const int xPos, 
                 const int yPos, const int width, 
                 const int height, char* _text, bool transparent, 
                 Color foreColor, Color backColor, 
                 UINT flags, Font _font, bool _parentNotify) :
   Window(winID, true, xPos, yPos, width, height)
{
   SetWindowType(WT_LABEL);
   SetParent(parentWin);

   strcpy(text, _text);
   transparentBackground = transparent;
   foregroundColor = foreColor;
   backgroundColor = backColor;
   textFlags = flags;
   font = _font;
   parentNotify = _parentNotify;
}

Label::Label(UINT winID, Window* parentWin, 
                 const Area& dimensions,
                 char* _text, bool transparent, 
                 Color foreColor, Color backColor, 
                 UINT flags, Font _font, bool _parentNotify) :
   Window(winID, true, dimensions)
{
   SetWindowType(WT_LABEL);
   SetParent(parentWin);

   strcpy(text, _text);
   transparentBackground = transparent;
   foregroundColor = foreColor;
   backgroundColor = backColor;
   textFlags = flags;
   font = _font;
   parentNotify = _parentNotify;
}

Label::Label(UINT winID, Window* parentWin,
                 const Point& position, const Point& size, 
                 char* _text, bool transparent, 
                 Color foreColor, Color backColor, 
                 UINT flags, Font _font, bool _parentNotify) :
   Window(winID, true, position, size)
{
   SetWindowType(WT_LABEL);
   SetParent(parentWin);

   strcpy(text, _text);
   transparentBackground = transparent;
   foregroundColor = foreColor;
   backgroundColor = backColor;
   textFlags = flags;
   font = _font;
   parentNotify = _parentNotify;
}

void FC Label::OnDrawWindow(WindowSurface* surface)
{
   if(!transparentBackground)
      surface->FillSurface(backgroundColor);

   surface->SetTextColor(foregroundColor);
   surface->SetGDIFont(font);

   Point surfaceSize = GetSize();
   surface->DrawText(text, DG::Rectangle(0, 0, surfaceSize.x - 1, 
      surfaceSize.y - 1), textFlags);
}

void FC Label::OnLButtonDown(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_LBUTTONDOWN,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Label::OnLButtonUp(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_LBUTTONUP,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Label::OnLButtonDblClk(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_LBUTTONDBLCLK,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Label::OnMButtonDown(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_MBUTTONDOWN,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Label::OnMButtonUp(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_MBUTTONUP,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Label::OnMButtonDblClk(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_MBUTTONDBLCLK,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Label::OnRButtonDown(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_RBUTTONDOWN,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Label::OnRButtonUp(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_RBUTTONUP,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Label::OnRButtonDblClk(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_RBUTTONDBLCLK,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Label::OnMouseMove(int x, int y, BYTE* keyboardState)
{
   if(parentNotify)
   {
      Message* msg = new Message(GM_LABEL_MOUSEMOVE,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());

      //Translate the coords back to absolute coords
      Point absoluteCoords = RelToAbsCoords(x, y);
      msg->data1 = absoluteCoords.x;
      msg->data2 = absoluteCoords.y;

      GetParent()->SendMessage(msg);
      delete msg;
   }
}
