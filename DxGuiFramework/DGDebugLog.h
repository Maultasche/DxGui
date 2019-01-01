/*------------------------------------------------------------------------
File Name: DGDebugLog.h
Description: This file contains the DGDebugLog class, which makes a 
   debug log out of the strings given to it.
Version:
   1.0.0    06.05.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class DebugLog
   {
   public:
      DebugLog();
      DebugLog(char* fileName);

      virtual ~DebugLog();

      void LogString(char* string);

   private:
	   std::fstream logFile;
   };
}
