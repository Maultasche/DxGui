/*------------------------------------------------------------------------
File Name: DGSurface.cpp
Description: This file contains the implementation of the DG::Surface  
	class, which represents an area of the screen that can be drawn upon.
Version:
   1.0.0    24.03.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

/*Constructor*/
//rect.right and rect.bottom are the width and height of the surface.
Surface::Surface(Area& area)
{
#ifdef _DEBUG
   Point res = dgGraphics->GetResolution();
//   assert(area.left >= 0 && area.left < res.x && 
//      area.top >= 0 && area.top < res.y);
#endif

   screenArea = area;
   windowOrg = NULL;
}

/*Destructor*/
Surface::~Surface()
{
   if(windowOrg != NULL)
      delete windowOrg;
}

//These functions control the window origin setting, so that the surface
//can do offsets when clipping a window
void Surface::SetWindowOrg(Point origin)
{
   RemoveWindowOrg();
   windowOrg = new Point(origin);
}

void Surface::RemoveWindowOrg()
{
   if(windowOrg != NULL)
   {
      delete windowOrg;
      windowOrg = NULL;
   }
}

//Non-Blit Drawing Functions
void FC Surface::SetPixel(int x, int y, Color& color)
{
   //Clip to within the surface
   if(x >= 0 && x < screenArea.width && y >= 0 && y < screenArea.height)
   {
      //Clip to within the screen
      int drawX = x + screenArea.left;
      int drawY = y + screenArea.top;

      Point res = dgGraphics->GetResolution();
      if(drawX < res.x && drawY < res.y && drawX >= 0 && drawY >= 0)
         dgGraphics->SetPixel(drawX, drawY, color);
   }
}

void FC Surface::DrawHorizontalLine(int x1, int x2, int y, Color& color)
{
   //Clip to within the surface
   if((y < screenArea.height && y >= 0) && 
      (x1 < screenArea.width || x2 < screenArea.width) &&
      (x1 >= 0 || x2 >= 0))
   {
      //Make adjustments to keep the line within the surface
      if(x1 < 0)
         x1 = 0;
      else if(x1 >= screenArea.width)
         x1 = screenArea.width - 1;
      if(x2 < 0)
         x2 = 0;
      else if(x2 >= screenArea.width)
         x2 = screenArea.width - 1;

      //Clip to within the screen
      int drawX1 = x1 + screenArea.left;
      int drawX2 = x2 + screenArea.left;
      int drawY = y + screenArea.top;

      Point res = dgGraphics->GetResolution();

      //Check to see if at least part of the line is within the screen area
      if((drawY < res.y && drawY >= 0) && (drawX1 < res.x || drawX2 < res.x) &&
         (drawX1 >= 0 || drawX2 >= 0))
      {
         //Make adjustments to keep the line within the screen
         if(drawX1 < 0)
            drawX1 = 0;
         else if(drawX1 >= res.x)
            drawX1 = res.x - 1;
         if(drawX2 < 0)
            drawX2 = 0;
         else if(drawX2 >= res.x)
            drawX2 = res.x - 1;
        
         dgGraphics->DrawHorizontalLine(drawX1, drawX2, drawY, color);
      }
   }
}

void FC Surface::DrawVerticalLine(int x, int y1, int y2, Color& color)
{
   //Clip to within the surface
   if((x < screenArea.width && x >= 0) && 
      (y1 < screenArea.height || y2 < screenArea.height) &&
      (y1 >= 0 || y2 >= 0))
   {
      //Make adjustments to keep the line within the surface
      if(y1 < 0)
         y1 = 0;
      else if(y1 >= screenArea.height)
         y1 = screenArea.height - 1;
      if(y2 < 0)
         y2 = 0;
      else if(y2 >= screenArea.height)
         y2 = screenArea.height - 1;

      //Clip to within the screen
      int drawY1 = y1 + screenArea.top;
      int drawY2 = y2 + screenArea.top;
      int drawX = x + screenArea.left;

      Point res = dgGraphics->GetResolution();

      //Check to see if at least part of the line is within the screen area
      if((drawX < res.x && drawX >= 0) && (drawY1 < res.y || drawY2 < res.y) &&
         (drawY1 >= 0 || drawY2 >= 0))
      {
         //Make adjustments to keep the line within the screen
         if(drawY1 < 0)
            drawY1 = 0;
         else if(drawY1 >= res.y)
            drawY1 = res.y - 1;
         if(drawY2 < 0)
            drawY2 = 0;
         else if(drawY2 >= res.y)
            drawY2 = res.y - 1;
         
         dgGraphics->DrawVerticalLine(drawX, drawY1, drawY2, color);
      }
   }
}

void FC Surface::DrawLine(Point p1, Point p2, Color& color)
{
   //Clip to within the surface
   if((p1.x < screenArea.width || p2.x < screenArea.width) &&
      (p1.x >= 0 || p2.x >= 0) && 
      (p1.y < screenArea.height || p2.y < screenArea.height) &&
      (p1.y >= 0 || p2.y >= 0))
   {
      int ratio;
      int edge;

      //Make adjustments to keep the line within the surface
      //Clip on the right and bottom
      if(p1.x >= screenArea.width)
      {
         edge = screenArea.width - 1;   
         ratio = (((edge - p2.x) << 16) / (p1.x - p2.x));
         p1.x = edge;
         p1.y = p2.y + (((p1.y - p2.y) * ratio) >> 16);
      }
      if(p1.y >= screenArea.height)
      {
         edge = screenArea.height - 1;
         ratio = (((edge - p2.y) << 16) / (p1.y - p2.y));
         p1.y = edge;
         p1.x = p2.x + (((p1.x - p2.x) * ratio) >> 16);
      }
      if(p2.x >= screenArea.width)
      {
         edge = screenArea.width - 1;
         ratio = (((edge - p1.x) << 16) / (p2.x - p1.x));
         p2.x = edge;
         p2.y = p1.y + (((p2.y - p1.y) * ratio) >> 16);
      }
      if(p2.y >= screenArea.height)
      {
         edge = screenArea.height - 1;
         ratio = (((edge - p1.y) << 16) / (p2.y - p1.y));
         p2.y = edge;
         p2.x = p1.x + (((p2.x - p1.x) * ratio) >> 16);
      }
      //Clip on the left and top
      if(p1.x < 0)
      {
         ratio = (((-p2.x) << 16) / (p1.x - p2.x));
         p1.x = 0;
         p1.y = p2.y + (((p1.y - p2.y) * ratio) >> 16);
      }
      if(p1.y < 0)
      {
         ratio = (((-p2.y) << 16) / (p1.y - p2.y));
         p1.y = 0;
         p1.x = p2.x + (((p1.x - p2.x) * ratio) >> 16);
      }
      if(p2.x < 0)
      {
         ratio = (((-p1.x) << 16) / (p2.x - p1.x));
         p2.x = 0;
         p2.y = p1.y + (((p2.y - p1.y) * ratio) >> 16);
      }
      if(p2.y < 0)
      {
         ratio = (((-p1.y) << 16) / (p2.y - p1.y));
         p2.y = 0;
         p2.x = p1.x + (((p2.x - p1.x) * ratio) >> 16);
      }

      //Clip to within the screen
      int drawY1 = p1.y + screenArea.top;
      int drawY2 = p2.y + screenArea.top;
      int drawX1 = p1.x + screenArea.left;
      int drawX2 = p2.x + screenArea.left;

      Point res = dgGraphics->GetResolution();

      //Check to see if at least part of the line is within the screen area
      if((drawX1 < res.x || drawX2 < res.x) && (drawX1 >= 0 || drawX2 >= 0) && 
         (drawY1 < res.y || drawY2 < res.y) && (drawY1 >= 0 || drawY2 >= 0))
      {
         //Make adjustments to keep the line within the screen
         //Clip on the right and bottom
         if(drawX1 >= res.x)
         {
            edge = res.x - 1;   
            ratio = (((edge - drawX2) << 16) / (drawX1 - drawX2));
            drawX1 = edge;
            drawY1 = p2.y + (((drawY1 - drawY2) * ratio) >> 16);
         }
         if(drawY1 >= res.y)
         {  
            edge = res.y - 1;
            ratio = (((edge - drawY2) << 16) / (drawY1 - drawY2));
            drawY1 = edge;
            drawX1 = drawX2 + (((drawX1 - drawX2) * ratio) >> 16);
         }
         if(drawX2 >= res.x)
         {
            edge = res.x - 1;
            ratio = (((edge - drawX1) << 16) / (drawX2 - drawX1));
            drawX2 = edge;
            drawY2 = drawY1 + (((drawY2 - drawY1) * ratio) >> 16);
         }
         if(drawY2 >= res.y)
         {
            edge = res.y - 1;
            ratio = (((edge - drawY1) << 16) / (drawY2 - drawY1));
            drawY2 = edge;
            drawX2 = drawX1 + (((drawX2 - drawX1) * ratio) >> 16);
         }
         //Clip on the left and top
         if(drawX1 < 0)
         {
            ratio = (((-drawX2) << 16) / (drawX1 - drawX2));
            drawX1 = 0;
            drawY1 = drawY2 + (((drawY1 - drawY2) * ratio) >> 16);
         }
         if(drawY1 < 0)
         {
            ratio = (((-drawY2) << 16) / (drawY1 - drawY2));
            drawY1 = 0;
            drawX1 = drawX2 + (((drawX1 - drawX2) * ratio) >> 16);
         }
         if(drawX2 < 0)
         {
            ratio = (((-drawX1) << 16) / (drawX2 - drawX1));
            drawX2 = 0;
            drawY2 = drawY1 + (((drawY2 - drawY1) * ratio) >> 16);
         }
         if(drawY2 < 0)
         {
            ratio = (((-drawY1) << 16) / (drawY2 - drawY1));
            drawY2 = 0;
            drawX2 = drawX1 + (((drawX2 - drawX1) * ratio) >> 16);
         }

         dgGraphics->DrawLine(Point(drawX1, drawY1), Point(drawX2, drawY2), color);
      }
   }
}

void FC Surface::DrawRectangle(DG::Rectangle rect, Color& color)
{
   bool drawEntireRectangle = true;

   //Clip to within the surface
   if((rect.left < screenArea.width || rect.right < screenArea.width) &&
      (rect.left >= 0 || rect.right >= 0) && 
      (rect.top < screenArea.height || rect.bottom < screenArea.height) &&
      (rect.top >= 0 || rect.bottom >= 0))
   {
      //Determine if the entire rectangle if drawn within the surface area
      if(rect.left < 0 || rect.top < 0 || rect.right >= screenArea.width ||
         rect.bottom >= screenArea.height)
         drawEntireRectangle = false;

      //Clip to within the screen
      int drawX1 = rect.left + screenArea.left;
      int drawX2 = rect.right + screenArea.left;
      int drawY1 = rect.top + screenArea.top;
      int drawY2 = rect.bottom + screenArea.top;

      Point res = dgGraphics->GetResolution();

      //Check to see if at least part of the rectangle is within the screen area
      if((drawX1 < res.x || drawX2 < res.x) && (drawX1 >= 0 || drawX2 >= 0) && 
         (drawY1 < res.y || drawY2 < res.y) && (drawY1 >= 0 || drawY2 >= 0))
      {
         //Determine if the entire rectangle if drawn within the screen
         if(drawX1 < 0 || drawY1 < 0 || drawX2 >= res.x ||
            drawY2 >= res.y)
            drawEntireRectangle = false;

         //If the entire rectangle is to be drawn on the screen, then simply
         //draw it, as no clipping is necessary
         if(drawEntireRectangle)
            dgGraphics->DrawRectangle(Rectangle(drawX1, drawY1, 
               drawX2, drawY2), color);         
         //If part of the rectangle is not drawn, then draw it with lines,
         //which will clip themselves appropriately
         else
         {
            DrawVerticalLine(rect.left, rect.top, rect.bottom, color);
            DrawVerticalLine(rect.right, rect.top, rect.bottom, color);
            DrawHorizontalLine(rect.left, rect.right, rect.top, color);
            DrawHorizontalLine(rect.left, rect.right, rect.bottom, color);
         }
      }
   }
}

void FC Surface::DrawFilledRectangle(DG::Rectangle rect, Color& color)
{
   //Clip to within the surface
   if((rect.left < screenArea.width || rect.right < screenArea.width) &&
      (rect.left >= 0 || rect.right >= 0) && 
      (rect.top < screenArea.height || rect.bottom < screenArea.height) &&
      (rect.top >= 0 || rect.bottom >= 0))
   {
      //Make adjustments to keep the line within the surface
      if(rect.left < 0)
         rect.left = 0;
      else if(rect.right >= screenArea.width)
         rect.right = screenArea.width - 1;
      if(rect.top < 0)
         rect.top = 0;
      else if(rect.bottom >= screenArea.height)
         rect.bottom = screenArea.height - 1;

      //Clip to within the screen
      int drawY1 = rect.top + screenArea.top;
      int drawY2 = rect.bottom + screenArea.top;
      int drawX1 = rect.left + screenArea.left;
      int drawX2 = rect.right + screenArea.left;

      Point res = dgGraphics->GetResolution();

      //Check to see if at least part of the line is within the screen area
      if((drawX1 < res.x || drawX2 < res.x) && (drawX1 >= 0 || drawX2 >= 0) && 
         (drawY1 < res.y || drawY2 < res.y) && (drawY1 >= 0 || drawY2 >= 0))
      {
         //Make adjustments to keep the line within the screen
         if(drawX1 < 0)
            drawX1 = 0;
         else if(drawX2 >= res.x)
            drawX2 = res.x - 1;
         if(drawY1 < 0)
            drawY1 = 0;
         else if(drawY2 >= res.y)
            drawY2 = res.y - 1;
         
         dgGraphics->DrawFilledRectangle(Rectangle(drawX1, drawY1, 
            drawX2, drawY2), color);
      }
   }
}
