/*------------------------------------------------------------------------
File Name: DGColor.cpp
Description: This file contains the implementation of the DG::Color class, 
   which is used to store a standard pixel format. DG::Color can convert 
   its standard color format into pixel formats matching the current pixel
   format and color depth.
Version:
   1.0.0    20.06.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

//Initialize the static variables
USHORT* Color::red16BitTable = NULL;
USHORT* Color::green16BitTable = NULL;
USHORT* Color::blue16BitTable = NULL;

UINT* Color::red24BitTable = NULL;
UINT* Color::green24BitTable = NULL;
UINT* Color::blue24BitTable = NULL;

UINT* Color::red32BitTable = NULL;
UINT* Color::green32BitTable = NULL;
UINT* Color::blue32BitTable = NULL;

/*------------------------------------------------------------------------
Function Name: Opposite
Description:
   This function returns the opposite of the color represented by this
   object. The opposite of a color is defined as the color which is 
   obtained by a bitwise NOT
Returns:
   The opposite of the color represented by this object
------------------------------------------------------------------------*/
Color Color::Opposite()
{
   return Color(~r, ~g, ~b);
}

/*------------------------------------------------------------------------
Function Name: Lighten
Description:
   This function returns an object representing the color represented by
   this object with more white added to it. 
Parameters:
   int colorValue - the amount of white to add to this color
Returns:
   The lightened version of the color represented by this object
------------------------------------------------------------------------*/
Color Color::Lighten(BYTE colorValue)
{
   BYTE newRed;
   BYTE newGreen;
   BYTE newBlue;

   //We must prevent overflow or underflow
   if(r + colorValue > 255)
      newRed = 255;
   else if(r + colorValue < 0)
      newRed = 0;
   else
      newRed = r + colorValue;

   if(g + colorValue > 255)
      newGreen = 255;
   else if(g + colorValue < 0)
      newGreen = 0;
   else
      newGreen = g + colorValue;

   if(b + colorValue > 255)
      newBlue = 255;
   else if(b + colorValue < 0)
      newBlue = 0;
   else
      newBlue = b + colorValue;

   return Color(newRed, newGreen, newBlue);
}

/*------------------------------------------------------------------------
Function Name: Darken
Description:
   This function returns an object representing the color represented by
   this object with more black added to it. 
Parameters:
   int colorValue - the amount of black to add to this color
Returns:
   The darkened version of the color represented by this object
------------------------------------------------------------------------*/
Color Color::Darken(BYTE colorValue)
{
   BYTE newRed;
   BYTE newGreen;
   BYTE newBlue;

   //We must prevent overflow or underflow
   if(r - colorValue > 255)
      newRed = 255;
   else if(r - colorValue < 0)
      newRed = 0;
   else
      newRed = r - colorValue;

   if(g - colorValue > 255)
      newGreen = 255;
   else if(g - colorValue < 0)
      newGreen = 0;
   else
      newGreen = g - colorValue;

   if(b - colorValue > 255)
      newBlue = 255;
   else if(b - colorValue < 0)
      newBlue = 0;
   else
      newBlue = b - colorValue;

   return Color(newRed, newGreen, newBlue);
}

//Function to create the static pixel format lookup tables,
//It is assumed that colorDepth is either CD_16BIT, CD_24BIT, or CD_32BIT
void Color::CreateLookupTables(DDPIXELFORMAT& pixelFormat, UINT colorDepth)
{
   assert(colorDepth == CD_16BIT || colorDepth == CD_24BIT || 
          colorDepth == CD_32BIT);

   //Delete any previous lookup tables and set them to NULL
   if(red16BitTable != NULL)
      delete red16BitTable;
   if(green16BitTable != NULL)
      delete green16BitTable;
   if(blue16BitTable != NULL)
      delete blue16BitTable;

   if(red24BitTable != NULL)
      delete red24BitTable;
   if(green24BitTable != NULL)
      delete green24BitTable;
   if(blue24BitTable != NULL)
      delete blue24BitTable;

   if(red32BitTable != NULL)
      delete red32BitTable;
   if(green32BitTable != NULL)
      delete green32BitTable;
   if(blue32BitTable != NULL)
      delete blue32BitTable;

   red16BitTable = green16BitTable = blue16BitTable = NULL;
   red24BitTable = green24BitTable = blue24BitTable = NULL;
   red32BitTable = green32BitTable = blue32BitTable = NULL;

   switch(colorDepth)
   {
      case CD_16BIT:
         red16BitTable = new USHORT[256];
         green16BitTable = new USHORT[256];
         blue16BitTable = new USHORT[256];

         Create16BitLookupTable(pixelFormat.dwRBitMask, red16BitTable);
         Create16BitLookupTable(pixelFormat.dwGBitMask, green16BitTable);
         Create16BitLookupTable(pixelFormat.dwBBitMask, blue16BitTable);
         break;
      case CD_24BIT:
         red24BitTable = new UINT[256];
         green24BitTable = new UINT[256];
         blue24BitTable = new UINT[256];

         Create24Or32BitLookupTable(pixelFormat.dwRBitMask, red24BitTable);
         Create24Or32BitLookupTable(pixelFormat.dwGBitMask, green24BitTable);
         Create24Or32BitLookupTable(pixelFormat.dwBBitMask, blue24BitTable);
         break;
      case CD_32BIT:
         red32BitTable = new UINT[256];
         green32BitTable = new UINT[256];
         blue32BitTable = new UINT[256];

         Create24Or32BitLookupTable(pixelFormat.dwRBitMask, red32BitTable);
         Create24Or32BitLookupTable(pixelFormat.dwGBitMask, green32BitTable);
         Create24Or32BitLookupTable(pixelFormat.dwBBitMask, blue32BitTable);
         break;
   }
}

//Creates a lookup table based on bitMask, it is assumed that luTable
//is a pointer to an array of size 256, this function can be used
//for 16-bit color depth only
void Color::Create16BitLookupTable(DWORD bitMask, USHORT* luTable)
{
   assert(luTable != NULL);

   for(int i = 0; i < 256; i++)
      luTable[i] = USHORT(((i * __int64(bitMask)) / 255) & bitMask);
}

//Creates a lookup table based on bitMask, it is assumed that luTable
//is a pointer to an array of size 256, this function can be used
//for 24- or 32-bit color depths (since there is no 24-bit data type)
void Color::Create24Or32BitLookupTable(DWORD bitMask, UINT* luTable)
{
   assert(luTable != NULL);

   for(int i = 0; i < 256; i++)
      luTable[i] = UINT(((i * __int64(bitMask)) / 255) & bitMask);
}
