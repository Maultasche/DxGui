/*------------------------------------------------------------------------
File Name: DGFont.cpp
Description: This file contains the implementation of the DG::Font class,
	which represents a font that is used in drawing text on the screen.
Version:
   1.0.0    10.03.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

//Default Constructor
Font::Font()
{
   strcpy(fontName, "MS Sans Serif");
   weight = FW_NORMAL;
   italic = FALSE;
   underline = FALSE;
   strikeout = FALSE;
   width = 0;
   height = 0;
   escapement = 0;
   orientation = 0;
   charSet = DEFAULT_CHARSET;
   outPrecision = OUT_CHARACTER_PRECIS;
   clipPrecision = CLIP_CHARACTER_PRECIS;
   quality = DEFAULT_QUALITY;
   pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

   fontHandle = CreateFont(height, width, escapement, orientation, weight,
      italic, underline, strikeout, charSet, outPrecision, clipPrecision,
      quality, pitchAndFamily, fontName);

   if(fontHandle == NULL)
   {
      char errorMsg[64];
      sprintf(errorMsg, "The font \"%s\" could not be created.");
      throw(new Exception(errorMsg, EC_CREATEFONT, ET_FONT, __FILE__,
         __LINE__));
   }
}

/*------------------------------------------------------------------------
Function Name: Constructor
Parameters:
   const char* p_FontName : the name of the font to create
   int p_pointSize : the point size of the font to create, from 0 to 72
   int p_weight : the thickness of the font to be created, acceptable
      values are FW_DONTCARE, FW_THIN, FW_EXTRALIGHT, FW_LIGHT, 
      FW_NORMAL, FW_MEDIUM, FW_SEMIBOLD, FW_BOLD, FW_EXTRABOLD, FW_HEAVY
   bool p_italic : whether an italicized font should be created
   bool p_underlined : whether an underlined font should be created
   bool p_strikeout : whether an strikeout font should be created
------------------------------------------------------------------------*/

Font::Font(const char* p_fontName, int p_height, int p_weight,
      bool p_italic, bool p_underline, bool p_strikeout)
{
   strcpy(fontName, p_fontName);
   weight = p_weight;
   italic = p_italic;
   underline = p_underline;
   strikeout = p_strikeout;
   width = 0;
   height = p_height;
   escapement = 0;
   orientation = 0;
   charSet = DEFAULT_CHARSET;
   outPrecision = OUT_CHARACTER_PRECIS;
   clipPrecision = CLIP_CHARACTER_PRECIS;
   quality = DEFAULT_QUALITY;
   pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

   fontHandle = CreateFont(height, width, escapement, orientation, weight,
      italic, underline, strikeout, charSet, outPrecision, clipPrecision,
      quality, pitchAndFamily, fontName);

   if(fontHandle == NULL)
   {
      char errorMsg[64];
      sprintf(errorMsg, "The font \"%s\" could not be created.");
      throw(new Exception(errorMsg, EC_CREATEFONT, ET_FONT, __FILE__,
         __LINE__));
   }
}


//Overloads the '=' operator
Font& Font::operator=(Font& font)
{
   strcpy(fontName, font.fontName);
   weight = font.weight;
   italic = font.italic;
   underline = font.underline;
   strikeout = font.strikeout;
   width = font.width;
   height = font.height;
   escapement = font.escapement;
   orientation = font.orientation;
   charSet = font.charSet;
   outPrecision = font.outPrecision;
   clipPrecision = font.clipPrecision;
   quality = font.quality;
   pitchAndFamily = font.pitchAndFamily;
   
   DeleteObject(fontHandle);

   fontHandle = CreateFont(height, width, escapement, orientation, weight,
      italic, underline, strikeout, charSet, outPrecision, clipPrecision,
      quality, pitchAndFamily, fontName);

   if(fontHandle == NULL)
   {
      char errorMsg[64];
      sprintf(errorMsg, "The font \"%s\" could not be created.");
      throw(new Exception(errorMsg, EC_CREATEFONT, ET_FONT, __FILE__,
         __LINE__));
   }

   return *this;
}

//Destructor
Font::~Font()
{
   DeleteObject(fontHandle);
}
