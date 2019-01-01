/*------------------------------------------------------------------------
File Name: DGException.h
Description: This file contains the DG::Exception class, which is used for
	error handling within the DxGui Framework.
Version:
   1.0.0    13.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class Exception
   {
   public:
      //This is a message that can be given to the user of the application
      char errorMessage[512];

      //This is a code that describes the exact error
      UINT errorCode;

      //This describes the general type of error: DirectDraw, GUI, etc.
      UINT errorType;

      //This is the name of the file in which the exception was created
      char fileName[256];

      //This is the line number on which the exception was created
      int lineNumber;

      Exception(char* message, UINT code = EC_UNKNOWN, 
         UINT type = ET_UNKNOWN, char* file = "Unknown",
         int lineNum = 0)
      {
         strcpy(errorMessage, message);
         errorCode = code;
         errorType = type;
         strcpy(fileName, file);
         lineNumber = lineNum;
      }
   };
}