/*------------------------------------------------------------------------
File Name: DGColor.h
Description: This file contains the DG::Color class, which is used to 
   store a standard pixel format. DG::Color can convert its standard color
   format into pixel formats matching the current pixel format and color
   depth.
Version:
   1.0.0    20.06.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class Color
   {
   public:
      BYTE r;
      BYTE g;
      BYTE b;

      Color()
      {
         r = 0;
         g = 0;
         b = 0;
      }

      Color(BYTE red, BYTE green, BYTE blue)
      {
         r = red;
         g = green;
         b = blue;
      }

      void SetColor(BYTE red, BYTE green, BYTE blue)
      {
         r = red;
         g = green;
         b = blue;
      }

      USHORT To16Bit(void)
      {
         return red16BitTable[r] | green16BitTable[g] | blue16BitTable[b];
      }

      UINT To24Bit(void)
      {
         return red24BitTable[r] | green24BitTable[g] | blue24BitTable[b];
      }

      UINT To32Bit(void)
      {
         return red32BitTable[r] | green32BitTable[g] | blue32BitTable[b];
      }

      COLORREF ToCOLORREF(void)
      {
         return ((b << 16) + (g << 8) + r);
      }

      Color Opposite(void);
      Color Lighten(BYTE colorValue);
      Color Darken(BYTE colorValue);

      //Function to create the static pixel format lookup tables,
      //It is assumed that colorDepth is either CD_16BIT, CD_24BIT, or CD_32BIT
      static void CreateLookupTables(DDPIXELFORMAT& pixelFormat, UINT colorDepth);

      //Creates a lookup table based on bitMask, it is assumed that luTable
      //is a pointer to an array of size 256, this function can be used
      //for 16-bit color depth only
      static void Create16BitLookupTable(DWORD bitMask, USHORT* luTable);

      //Creates a lookup table based on bitMask, it is assumed that luTable
      //is a pointer to an array of size 256, this function can be used
      //for 24- or 32-bit color depths (since there is no 24-bit data type)
      static void Color::Create24Or32BitLookupTable(DWORD bitMask, UINT* luTable);

   private:
      //Since we can't count on the pixel format being the same for every
      //video card, we must make static lookup tables, so that we don't
      //have to calculate where the colors go each time a pixel is drawn.
      static USHORT* red16BitTable;
      static USHORT* green16BitTable;
      static USHORT* blue16BitTable;

      static UINT* red24BitTable;
      static UINT* green24BitTable;
      static UINT* blue24BitTable;

      static UINT* red32BitTable;
      static UINT* green32BitTable;
      static UINT* blue32BitTable;
   };
}
