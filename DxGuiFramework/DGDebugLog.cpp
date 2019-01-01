/*------------------------------------------------------------------------
File Name: DGDebugLog.cpp
Description: This file contains the implementation of the DG::DebugLog 
   class, which makes a debug log out of the strings given to it.
Version:
   1.0.0    06.05.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

DebugLog::DebugLog()
{
   logFile.open("DGDebugLog.txt", std::ios::out);
}

DebugLog::DebugLog(char* fileName)
{
   logFile.open(fileName, std::ios::out);
}

DebugLog::~DebugLog()
{
   logFile.close();
}

void DebugLog::LogString(char* string)
{
   logFile << string;
}
