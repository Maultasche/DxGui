/*------------------------------------------------------------------------
File Name: DGMessageLog.h
Description: This file contains the DG::MessageLog class, which makes a 
   log of all GUI messages
Version:
   1.0.0    05.05.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class MessageLog
   {
   public:
      MessageLog();
      MessageLog(char* fileName);

      virtual ~MessageLog();

      void LogMessage(Message* message);

   private:
	   std::fstream logFile;
   };
}
