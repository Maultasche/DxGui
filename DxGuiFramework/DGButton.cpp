/*------------------------------------------------------------------------
File Name: DGButton.cpp
Description: This file contains the implementation of the DG::Button 
   class, which is a a button control. DGButton can be used for any button  
   that has 2 states (depressed and normal), such as a normal push button, 
   a radio button, or check box.
Version:
   1.0.0    25.08.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

#define BN_EDGE_WIDTH               2

#define BN_DEFAULT_TEXT_RED         0
#define BN_DEFAULT_TEXT_GREEN       0
#define BN_DEFAULT_TEXT_BLUE        0

#define BN_DEFAULT_BUTTON_RED       180
#define BN_DEFAULT_BUTTON_GREEN     180
#define BN_DEFAULT_BUTTON_BLUE      180

Button::Button(UINT winID, Window* parentWin) :
   Window(winID, true),
   buttonState(BS_NORMAL),
   buttonType(BT_PUSHBUTTON),
   enabled(true),
   pressure(false),
   edgeWidth(BN_EDGE_WIDTH),
   textColor(Color(BN_DEFAULT_TEXT_RED, BN_DEFAULT_TEXT_GREEN, 
   BN_DEFAULT_TEXT_BLUE)),
   buttonColor(Color(BN_DEFAULT_BUTTON_RED, BN_DEFAULT_BUTTON_GREEN, 
   BN_DEFAULT_BUTTON_BLUE)),
   displayWindow(NULL)

{
   SetWindowType(WT_BUTTON);
   SetParent(parentWin);
}

Button::Button(UINT winID, Window* parentWin, const int xPos, 
                   const int yPos,  const int width, const int height,
                   UINT aButtonType,  char* text, Font font) :
   Window(winID, true, xPos, yPos, width, height),
   buttonState(BS_NORMAL),
   buttonType(aButtonType),
   enabled(true),
   pressure(false),
   buttonFont(font),
   edgeWidth(BN_EDGE_WIDTH),
   textColor(Color(BN_DEFAULT_TEXT_RED, BN_DEFAULT_TEXT_GREEN, 
   BN_DEFAULT_TEXT_BLUE)),
   buttonColor(Color(BN_DEFAULT_BUTTON_RED, BN_DEFAULT_BUTTON_GREEN, 
   BN_DEFAULT_BUTTON_BLUE)),
   displayWindow(NULL)   
{
   SetWindowType(WT_BUTTON);
   SetParent(parentWin);

   strcpy(buttonText, text);
}

Button::Button(UINT winID, Window* parentWin, 
                   const Area& dimensions, UINT aButtonType, 
                   char* text, Font font) :
   Window(winID, true, dimensions),
   buttonState(BS_NORMAL),
   buttonType(aButtonType),
   enabled(true),
   pressure(false),
   buttonFont(font),
   edgeWidth(BN_EDGE_WIDTH),
   textColor(Color(BN_DEFAULT_TEXT_RED, BN_DEFAULT_TEXT_GREEN, 
   BN_DEFAULT_TEXT_BLUE)),
   buttonColor(Color(BN_DEFAULT_BUTTON_RED, BN_DEFAULT_BUTTON_GREEN, 
   BN_DEFAULT_BUTTON_BLUE)),
   displayWindow(NULL)  
{
   SetWindowType(WT_BUTTON);
   SetParent(parentWin);

   strcpy(buttonText, text);
}

Button::Button(UINT winID, Window* parentWin, const Point& position, 
                   const Point& size, UINT aButtonType, char* text, 
                   Font font) :
   Window(winID, true, position, size),
   buttonState(BS_NORMAL),
   buttonType(aButtonType),
   enabled(true),
   pressure(false),
   buttonFont(font),
   edgeWidth(BN_EDGE_WIDTH),
   textColor(Color(BN_DEFAULT_TEXT_RED, BN_DEFAULT_TEXT_GREEN, 
   BN_DEFAULT_TEXT_BLUE)),
   buttonColor(Color(BN_DEFAULT_BUTTON_RED, BN_DEFAULT_BUTTON_GREEN, 
   BN_DEFAULT_BUTTON_BLUE)),
   displayWindow(NULL)  
{
   SetWindowType(WT_BUTTON);
   SetParent(parentWin);

   strcpy(buttonText, text);
}

/*------------------------------------------------------------------------
Function Name: SetFont
Description:
   This function sets the font that the button uses when it displays
   text.
Parameters: Font font - the font to be used
Preconditions: The button has the style BT_TEXTBUTTON
------------------------------------------------------------------------*/

void Button::SetFont(Font font)
{
   assert(buttonType == BT_TEXTBUTTON);

   buttonFont = font;
   ((Label*)displayWindow)->SetFont(buttonFont);
}

/*------------------------------------------------------------------------
Function Name: GetFont
Description:
   This function returns the font that the button uses when it displays
   text.
Parameters:
Returns:
   The font to be used when the button displays text
Preconditions: The button has the style BT_TEXTBUTTON
------------------------------------------------------------------------*/

Font Button::GetFont()
{
   assert(buttonType == BT_TEXTBUTTON);

   return buttonFont;
}

/*------------------------------------------------------------------------
Function Name: SetText
Description:
   This function sets the text the button displays when it has the style
   BT_TEXTBUTTON.
Parameters: char* text - pointer to an arry holding the text to be 
   displayed
Preconditions: The button has the style BT_TEXTBUTTON
------------------------------------------------------------------------*/

void Button::SetText(const char* text)
{
   assert(buttonType == BT_TEXTBUTTON);

   strcpy(buttonText, text);
   ((Label*)displayWindow)->SetText(buttonText);
}

/*------------------------------------------------------------------------
Function Name: GetText
Description:
   This function returns the text the button displays when it has the 
   style BT_TEXTBUTTON.
Parameters: 
Returns:
   The text that the button is using
Preconditions: The button has the style BT_TEXTBUTTON
------------------------------------------------------------------------*/

char* const Button::GetText()
{
   assert(buttonType == BT_TEXTBUTTON);

   return buttonText;
}

/*------------------------------------------------------------------------
Function Name: SetState
Description:
   This function sets the state of the button.
Parameters: 
   UINT state - the state that the button is to be set to
------------------------------------------------------------------------*/

void Button::SetState(UINT state)
{
   buttonState = state;
}

/*------------------------------------------------------------------------
Function Name: SetType
Description:
   This function sets the type of the button. The button type may be any
   bitwise OR combination of the button type flags defined in DGButton.h.
Parameters: 
   UINT state - the type that the button is to be set to
------------------------------------------------------------------------*/

void Button::SetType(UINT type)
{
   //Change the display window depending on the type of button

   buttonType = type;
}

/*------------------------------------------------------------------------
Function Name: SetTextColor
Description:
   This function sets the color of the text that the button displays when
   it is of type BT_TEXTBUTTON.
Parameters: Color& color - the color to be used in drawing text
Preconditions: The button has the style BT_TEXTBUTTON
------------------------------------------------------------------------*/

void Button::SetTextColor(Color& color)
{
   assert(buttonType == BT_TEXTBUTTON);

   ((Label*)displayWindow)->SetForegroundColor(color);
}

/*------------------------------------------------------------------------
Function Name: SetButtonColor
Description:
   This function sets the color of the button.
Parameters: Color& color - the color to be used in drawing the button
------------------------------------------------------------------------*/

void Button::SetButtonColor(Color& color)
{
   buttonColor = color;
}

/*------------------------------------------------------------------------
Function Name: IsDisplayingImage
Description:
   This function returns whether this button is currently displaying an
   image. A button with a BT_IMAGEBUTTON style will only display an image 
   if it has been given the ID of a bitmap that is loaded.
Returns: true if the button is currently displaying an image, false 
   if the button is not displaying an image
------------------------------------------------------------------------*/

bool Button::IsDisplayingImage()
{
   bool displayingImage = false;

   if(buttonType & BT_IMAGEBUTTON)
   {
      if(displayWindow != NULL && 
         displayWindow->GetWindowType() == WT_IMAGE)
         displayingImage = true;
   }

   return displayingImage;
}

/*------------------------------------------------------------------------
Function Name: SetBitmapID
Description:
   This function sets bitmap to be displayed in the button. This only has
   an effect when the button control has BT_IMAGEBUTTON style.
Parameters: UINT bitmapID - the ID of the bitmap to be displayed
Preconditions: A bitmap assigned to bitmapID must be loaded
------------------------------------------------------------------------*/

void Button::SetBitmapID(UINT bitmapID)
{  
   if(buttonType & BT_IMAGEBUTTON)
   {
      //Delete the previous image
      if(displayWindow != NULL)
      {
         DestroyWindow(displayWindow->GetWindowID());
      }

      //Check to see if the bitmap is loaded
      Bitmap* bitmap = dgGraphics->GetBitmap(bitmapID);

      if(bitmap != NULL)
      {
         Point buttonSize = GetSize();
         buttonSize.Offset(-(BN_EDGE_WIDTH << 1), -(BN_EDGE_WIDTH << 1));

         Area childWindowArea(Point(BN_EDGE_WIDTH, BN_EDGE_WIDTH), 
            buttonSize);

         //It looks a bit better in 16x16 if we offset it by 1
         childWindowArea.Offset(1, 1);

         displayWindow = new Image(IDW_BN_CHILDWINDOW, this, 
            childWindowArea, bitmapID, true, Color(255, 255, 255));

         AddChildWindow(displayWindow);
      }
         
   }
}

/*------------------------------------------------------------------------
Function Name: GetBitmapID
Description:
   This function returns the ID of the image that is displayed in the
   button.
Returns: The bitmap ID of the image displayed in the button
Preconditions: IsDisplayingImage() returns true
------------------------------------------------------------------------*/

UINT Button::GetBitmapID()
{
   assert(IsDisplayingImage());

   return ((Image*)displayWindow)->GetBitmapID();
}

/*Overridden methods*/

void FC Button::OnDrawWindow(WindowSurface* surface)
{
   if(buttonState == BS_NORMAL && !pressure)
   {
      if(enabled)
         OnDrawNormal(surface);
      else
         OnDrawNormalDisabled(surface);
   }
   else
   {
      if(enabled)
         OnDrawDepressed(surface);
      else
         OnDrawDepressedDisabled(surface);
   }
}

void FC Button::OnCreate()
{
   Point buttonSize = GetSize();
   buttonSize.Offset(-(BN_EDGE_WIDTH << 1), -(BN_EDGE_WIDTH << 1));

   Area childWindowArea(Point(BN_EDGE_WIDTH, BN_EDGE_WIDTH), 
      buttonSize);

   if(buttonType & BT_TEXTBUTTON)
   {
      displayWindow = new Label(IDW_BN_CHILDWINDOW, this, 
         childWindowArea, buttonText, true, 
         Color(0, 0, 0), Color(255, 0, 0),
         DT_CENTER | DT_WORDBREAK | DT_SINGLELINE | DT_VCENTER, buttonFont, true);
  
      AddChildWindow(displayWindow);

      AddMessageHandler(GM_LABEL_LBUTTONDOWN, 
         new MsgHandlerFunction<Button>(this, OnChildWindowMessage));

      AddMessageHandler(GM_LABEL_LBUTTONUP, 
         new MsgHandlerFunction<Button>(this, OnChildWindowMessage));

      AddMessageHandler(GM_LABEL_LBUTTONDBLCLK, 
         new MsgHandlerFunction<Button>(this, OnChildWindowMessage));

      AddMessageHandler(GM_LABEL_MOUSEMOVE, 
         new MsgHandlerFunction<Button>(this, OnChildWindowMessage));

   }
   else if(buttonType & BT_IMAGEBUTTON)
   {
      AddMessageHandler(GM_IMAGE_LBUTTONDOWN, 
         new MsgHandlerFunction<Button>(this, OnChildWindowMessage));

      AddMessageHandler(GM_IMAGE_LBUTTONUP, 
         new MsgHandlerFunction<Button>(this, OnChildWindowMessage));

      AddMessageHandler(GM_IMAGE_LBUTTONDBLCLK, 
         new MsgHandlerFunction<Button>(this, OnChildWindowMessage));

      AddMessageHandler(GM_IMAGE_MOUSEMOVE, 
         new MsgHandlerFunction<Button>(this, OnChildWindowMessage));

   }
}

void FC Button::OnLButtonDown(int x, int y, BYTE* keyboardState)
{
   //Only do something if the button is enabled
   if(enabled)
   {
      Message* msg;

      //If there was no pressure before, tell the button it is 
      //under pressure and send a button down message to the parent
      if(!pressure)
      {
         msg = new Message(GM_BUTTON_DOWN,
            GetParent()->GetWindowID(), 0, 0, GetWindowID());
         GetParent()->SendMessage(msg);
         delete msg;

         pressure = true;
      }

      //Send a button pressure message to the parent
      msg = new Message(GM_BUTTON_PRESSURE,
         GetParent()->GetWindowID(), 0, 0, GetWindowID());
      GetParent()->SendMessage(msg);
      delete msg;
   }
}

void FC Button::OnLButtonDblClk(int x, int y, BYTE* keyboardState)
{
   //A double click is considered the same as a down button
   OnLButtonDown(x, y, keyboardState);
}

void FC Button::OnLButtonUp(int x, int y, BYTE* keyboardState)
{
   //Only do something if the the button is enabled
   if(enabled)
   {
      //Tell the button is no longer under pressure
      pressure = false;

      //Change the state only if this button is a toggle button
      if(buttonType & BT_TOGGLEBUTTON)
      {
         if(buttonState == BS_NORMAL)
            buttonState = BS_DEPRESSED;
         else
            buttonState = BS_NORMAL;
      }

      //Tell the parent that the button was clicked
      GetGui()->PostMessage(new Message(GM_BUTTON_CLICKED,
         GetParent()->GetWindowID(), 0, 0, GetWindowID()));
   }
}

void FC Button::OnMouseLeave(int x, int y, BYTE* keyboardState)
{
   //When the mouse leaves the window, any pressure is automatically
   //removed
   pressure = false;
}

void FC Button::OnWindowSizing()
{
   OnWindowSized();
}

void FC Button::OnWindowSized()
{
   //Size the display window so that it is the size of the button
   //minus the size of the edges on all sides
   if(displayWindow != NULL)
   {
      Point buttonSize = GetSize();
      buttonSize.Offset(-(BN_EDGE_WIDTH << 1), -(BN_EDGE_WIDTH << 1));
      displayWindow->SetSize(buttonSize);
   }
}

void Button::OnChildWindowMessage(Message* msg)
{
   //We need to change the message type and destination and have this
   //window handle the message
   msg->destination = GetWindowID();

   switch(msg->messageType)
   {
   case GM_LABEL_LBUTTONDOWN:
   case GM_IMAGE_LBUTTONDOWN:
      msg->messageType = GM_LBUTTONDOWN;
      break;
   case GM_LABEL_LBUTTONUP:
   case GM_IMAGE_LBUTTONUP:
      msg->messageType = GM_LBUTTONUP;
      break;
   case GM_LABEL_LBUTTONDBLCLK:
   case GM_IMAGE_LBUTTONDBLCLK:
      msg->messageType = GM_LBUTTONDBLCLK;
      break;
   case GM_LABEL_MOUSEMOVE:
   case GM_IMAGE_MOUSEMOVE:
      msg->messageType = GM_MOUSEMOVE;
      break;
   }

   HandleMessage(msg);
}

/*Overridable methods*/

/*------------------------------------------------------------------------
Function Name: OnDrawNormal
Description:
   This function draws the button in its normal enabled state
Parameters: WindowSurface* surface - the window surface to draw upon
------------------------------------------------------------------------*/

void FC Button::OnDrawNormal(WindowSurface* surface)
{
   //Fill the surface with the button color
   surface->FillSurface(buttonColor);

   Point size = GetSize();
   Color darkenedColor = buttonColor.Darken(50);
   Color lightenedColor = buttonColor.Lighten(50);
   Color drawColor;

   surface->LockSurface();

   //Draw the edges of the button
   for(UINT i = 0; i < edgeWidth; i++)
   {
      surface->DrawVerticalLine(0 + i, 0 + i, size.y - i - 1, 
         lightenedColor);
      surface->DrawHorizontalLine(0 + i, size.x - i - 1, 0 + i,
         lightenedColor);
      surface->DrawVerticalLine(size.x - i - 1, 0 + i + 1, size.y - i - 1,
         darkenedColor);
      surface->DrawHorizontalLine(0 + i + 1, size.x - i - 1, size.y - i - 1,
         darkenedColor);
   }

   surface->UnlockSurface();
}

/*------------------------------------------------------------------------
Function Name: OnDrawNormalDisabled
Description:
   This function draws the button in its normal disabled state
Parameters: WindowSurface* surface - the window surface to draw upon
------------------------------------------------------------------------*/

void FC Button::OnDrawNormalDisabled(WindowSurface* surface)
{

}

/*------------------------------------------------------------------------
Function Name: OnDrawDepressed
Description:
   This function draws the button in its depressed enabled state
Parameters: WindowSurface* surface - the window surface to draw upon
------------------------------------------------------------------------*/

void FC Button::OnDrawDepressed(WindowSurface* surface)
{
   //Fill the surface with the button color
   surface->FillSurface(buttonColor);

   Point size = GetSize();
   Color darkenedColor = buttonColor.Darken(50);
   Color lightenedColor = buttonColor.Lighten(50);

   surface->LockSurface();

   //Draw the edges of the button
   for(UINT i = 0; i < edgeWidth; i++)
   {
      surface->DrawVerticalLine(0 + i, 0 + i, size.y - i - 1, 
         darkenedColor);
      surface->DrawHorizontalLine(0 + i, size.x - i - 1, 0 + i,
         darkenedColor);
      surface->DrawVerticalLine(size.x - i - 1, 0 + i + 1, size.y - i - 1,
         lightenedColor);
      surface->DrawHorizontalLine(0 + i + 1, size.x - i - 1, size.y - i - 1,
         lightenedColor);
   }

   surface->UnlockSurface();
}

/*------------------------------------------------------------------------
Function Name: OnDrawDepressedDisabled
Description:
   This function draws the button in its depressed disabled state
Parameters: WindowSurface* surface - the window surface to draw upon
------------------------------------------------------------------------*/

void FC Button::OnDrawDepressedDisabled(WindowSurface* surface)
{

}

