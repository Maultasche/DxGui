/*------------------------------------------------------------------------
File Name: DGWindow.h
Description: This file contains the DG::Window class, which represents a
   basic window in the GUI.
Version:
   1.0.0    10.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class Window
   {
   public:
      Window(UINT winID, bool control);
      Window(UINT winID, bool control, const Area& dimensions);
      Window(UINT winID, bool control, const Point& position, 
         const Point& size);
      Window(UINT winID, bool control, int xPos, int yPos,
         int width, int height);
      virtual ~Window();

      virtual void FC SetPosition(const Point& position);
      virtual void FC SetPosition(int xPos, int yPos);
      Point GetPosition(void) {return windowPosition;}

      virtual void FC SetSize(const Point& size);
      virtual void FC SetSize(int width, int height);
      Point GetSize(void) {return windowSize;}

      virtual void FC SetDimensions(const Area& dimensions);
      virtual void FC SetDimensions(const Point& position, 
         const Point& size);
      virtual void FC SetDimensions(int x, int y, int width, int height);
      Area GetDimensions(void) 
      {return Area(windowPosition, windowSize);}
      Area GetAbsDimensions(void)
      {return Area(absWindowPosition, windowSize);}

      Point GetAbsPosition(void) {return absWindowPosition;}

      virtual void FC HideWindow(void);
      virtual void FC ShowWindow(void);

      virtual void FC DrawWindow(void);

      bool IsWindowShowing(void) {return windowShowing;}
      bool IsControl(void) {return isControl;}

      UINT GetWindowID(void) {return windowID;}
      UINT GetWindowType(void) {return windowType;}

      void SetParent(Window* window) 
      {
         parentWindow = window;
         CalculateAbsCoords();
      }

      Window* GetParent(void) {return parentWindow;}

      //Child Window Functions
      void FC AddChildWindow(Window* childWindow);
      Window* FC GetWindow(UINT winID);
      bool FC DestroyWindow(UINT winID);
      void FC SetChildWindowDraw(bool draw) {drawChildWindows = draw;}
      bool FC GetChildWindowDraw(void) {return drawChildWindows;}
      void FC SetParentClipping(bool clipping) {parentClipping = clipping;}
      bool FC GetParentClipping(void) {return parentClipping;}
      void FC SetTopMostChildWindow(UINT winID);


      //Message Functions
      virtual bool FC SendMessage(Message* msg);
      virtual bool FC SendChildMessage(Message* msg);
      virtual void FC BroadcastMessage(Message* msg);
      virtual void FC AddMessageHandler(UINT messageID, 
         MsgHandlerInterface* function);
      virtual void FC RemoveMessageHandler(UINT messageID);
      virtual void FC HandleMessage(Message* msg);

      //Window Coordinate Functions
      bool FC AreCoordsInWindow(int x, int y);
      UINT FC GetWinIDOfCoords(int x, int y);
      UINT FC GetChildWinIDOfCoords(int x, int y);
      Point FC AbsToRelCoords(int x, int y);
      Point FC RelToAbsCoords(int x, int y);

      //Calls OnCreate or OnDestroy
      virtual void FC Create(void);
      virtual void FC Destroy(void);

   protected:
      void SetWindowType(UINT type) {windowType = type;}

      //Default Message Handlers
      virtual void FC OnSetFocus(UINT winID);
      virtual void FC OnLoseFocus(UINT winID);
      virtual void FC OnLButtonDown(int x, int y, BYTE* keyboardState);
      virtual void FC OnLButtonUp(int x, int y, BYTE* keyboardState);
      virtual void FC OnLButtonDblClk(int x, int y, BYTE* keyboardState);
      virtual void FC OnMButtonDown(int x, int y, BYTE* keyboardState);
      virtual void FC OnMButtonUp(int x, int y, BYTE* keyboardState);
      virtual void FC OnMButtonDblClk(int x, int y, BYTE* keyboardState);
      virtual void FC OnRButtonDown(int x, int y, BYTE* keyboardState);
      virtual void FC OnRButtonUp(int x, int y, BYTE* keyboardState);
      virtual void FC OnRButtonDblClk(int x, int y, BYTE* keyboardState);
      virtual void FC OnMouseMove(int x, int y, BYTE* keyboardState);
      virtual void FC OnMouseEnter(int x, int y, BYTE* keyboardState);
      virtual void FC OnMouseLeave(int x, int y, BYTE* keyboardState);
      virtual void FC OnKeyDown(BYTE keyCode, PBYTE keyboardState);
      virtual void FC OnKeyPressed(BYTE keyCode, PBYTE keyboardState);
      virtual void FC OnCharacter(char character, PBYTE keyboardState);
      virtual void FC OnWindowMoving(void);
      virtual void FC OnWindowMoved(void);
      virtual void FC OnWindowReposition(int xPos, int yPos);
      virtual void FC OnWindowSizing(void);
      virtual void FC OnWindowSized(void);
      virtual void FC OnWindowResize(int width, int height);
      virtual void FC OnShowWindow(void);
      virtual void FC OnHideWindow(void);
      virtual void FC OnDisplayChange(float xRelChange, float yRelChange,
         UINT colorDepth);
      virtual void FC OnCreate(void);
      virtual void FC OnDestroy(void);
      virtual void FC OnShowChildWindow(UINT winID);
      virtual void FC OnHideChildWindow(UINT winID);
      virtual void FC OnParentReposition(void);
      virtual void FC OnTimer(UINT timerID);
      virtual void FC OnDrawWindow(WindowSurface* surface);

      /*Handlers for control messages*/
      virtual void OnTitleBarDown(Message* msg);

   private:
      void CalculateAbsCoords(void);

      Point windowPosition;
      Point windowSize;

      Point absWindowPosition;

      UINT windowID;
      UINT windowType;

      bool windowShowing;
      bool isControl;

      Window* parentWindow;

      //The message handler table
      MessageTable messageTable;

      //Z-ordered child window lists
      LinkedList<Window> windowList;
      LinkedList<Window> controlList;
      LinkedList<Window> hiddenWindowList;

      bool isCreated;
      bool isDestroyed;
      bool drawChildWindows;
      bool parentClipping;
   };
}