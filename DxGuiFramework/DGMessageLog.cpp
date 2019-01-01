/*------------------------------------------------------------------------
File Name: DGMessageLog.cpp
Description: This file contains the implementation of the DG::MessageLog 
   class, which makes a log of all GUI messages
Version:
   1.0.0    05.05.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

MessageLog::MessageLog()
{
   logFile.open("DGMessageLog.txt", std::ios::out);
}

MessageLog::MessageLog(char* fileName)
{
   logFile.open(fileName, std::ios::out);
}

MessageLog::~MessageLog()
{
   logFile.close();
}

void MessageLog::LogMessage(Message* message)
{
   Point point;

   switch(message->messageType)
   {
      case GM_NONE:
         logFile << "GM_NONE\n";
      case GM_FOCUS:
         logFile << "GM_FOCUS: " << message->data3 << "\n";
         break;
      case GM_LOSEFOCUS:
         logFile << "GM_LOSEFOCUS: " << message->data3 << "\n";
         break;
      case GM_LBUTTONDOWN:
         logFile << "GM_LBUTTONDOWN: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_LBUTTONUP:
         logFile << "GM_LBUTTONUP: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_LBUTTONDBLCLK:
         logFile << "GM_LBUTTONDBLCLK: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_MBUTTONDOWN:
         logFile << "GM_MBUTTONDOWN: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_MBUTTONUP:
         logFile << "GM_MBUTTONUP: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_MBUTTONDBLCLK:
         logFile << "GM_MBUTTONDBLCLK: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_RBUTTONDOWN:
         logFile << "GM_RBUTTONDOWN: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_RBUTTONUP:
         logFile << "GM_RBUTTONUP: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_RBUTTONDBKCLK:
         logFile << "GM_RBUTTONDBLCLK: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_MOUSEMOVE:
         logFile << "GM_MOUSEMOVE: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_MOUSEENTER:
         logFile << "GM_MOUSEENTER: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_MOUSELEAVE:
         logFile << "GM_MOUSELEAVE: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_KEYDOWN:
         logFile << "GM_KEYDOWN: winID - " << message->destination << " Key - "
            << message->data3 << "\n";
         break;
      case GM_KEYUP:
         logFile << "GM_KEYDOWN: winID - " << message->destination << " Key - "
            << message->data3 << "\n";
         break;
      case GM_CHARACTER:
         logFile << "GM_CHARACTER: winID - " << message->destination << " Key - "
            << (char)message->data3 << "\n";
         break;
      case GM_MOVING:
         logFile << "GM_MOVING: " << message->destination << "\n";
         break;
      case GM_MOVED:
         logFile << "GM_MOVED: " << message->destination << "\n";
         break;
      case GM_REPOSITION:
         logFile << "GM_REPOSITION: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_SIZING:
         logFile << "GM_SIZING: " << message->destination << "\n";
         break;
      case GM_SIZED:
         logFile << "GM_SIZED: " << message->destination << "\n";
         break;
      case GM_RESIZE:
         logFile << "GM_RESIZE: winID - " << message->destination << " Coords - "
            << message->data1 << "," << message->data2 << "\n";
         break;
      case GM_SHOWWINDOW:
         logFile << "GM_SHOWWINDOW: " << message->destination << "\n";
         break;
      case GM_HIDEWINDOW:
         logFile << "GM_HIDEWINDOW: " << message->destination << "\n";
         break;
      case GM_DISPLAYCHANGE:
      {
         ResolutionMessage* resMessage = (ResolutionMessage*)message;
         logFile << "GM_DISPLAYCHANGE: winID - " << message->destination << "Res - " 
            << resMessage->newXResPercentage << ", " << resMessage->newYResPercentage
            << "ColorDepth: " << resMessage->data3 << "\n";
         break;
      }
      case GM_CREATE:
         logFile << "GM_CREATE: " << message->destination << "\n";
         break;
      case GM_DESTROY:
         logFile << "GM_DESTROY: " << message->destination << "\n";
         break;
      case GM_HIDECHILDWINDOW:
         logFile << "GM_HIDECHILDWINDOW: " << message->destination << ", Child Wnd: "
            << message->data3 << "\n";
         break;
      case GM_SHOWCHILDWINDOW:
         logFile << "GM_SHOWCHILDWINDOW: " << message->destination << ", Child Wnd: "
            << message->data3 << "\n";
         break;
      default:
         //If the message was not a basic framework message, then we will
         //simply have to log it generically
         logFile << "Unknown Message Type: " << message->messageType << "\n";
   }
}
