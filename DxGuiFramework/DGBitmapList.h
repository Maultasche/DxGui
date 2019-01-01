/*------------------------------------------------------------------------
File Name: DGBitmapList.h
Description: This file contains a derivation of DG::LinkedList that is 
	used to store and manage bitmaps.
Version:
   1.0.0    25.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

namespace DG
{
   class BitmapList : public LinkedList<Bitmap>
   {
   public:
      //Constructor: destroyItems specifies whether the data 
      //in the queue should be deleted when the queue is destroyed
      BitmapList(bool destroyItems);
      BitmapList();

      //Destructor
      ~BitmapList() {}

      //Wrapper Functions
      void Insert(Bitmap* data, int id = NO_ID);
      void Append(Bitmap* data, int id = NO_ID);
      void InsertAtPosition(int index, Bitmap* data, int id = NO_ID);

      void DeleteByIndex(int index);
      void DeleteById(int id);
      void DeleteFirst(void);
      void DeleteLast(void);
      void DeleteAll(void);

      Bitmap* RemoveByIndex(int index);
      Bitmap* RemoveById(int id);
      Bitmap* RemoveFirst(void);
      Bitmap* RemoveLast(void);
      void RemoveAll(void);

      //Real Functions
      void RemoveFirstBitmapByPriority(UINT priority);
      void RemoveAllBitmapsByPriority(UINT priority);
      void RemoveFirstLowestPriorityBitmap(void);
      void RemoveAllLowestPriorityBitmaps(void);
      void RemoveAllBitmaps(void);
      void RestoreBitmaps(void);

      UINT GetHighestPriority(void) {return highestPriority;}
      UINT GetLowestPriority(void) {return lowestPriority;}
      UINT GetTotalMemory(void) {return totalMemory;}
      UINT GetNumOfLoadedBitmaps(void) {return numOfLoadedBitmaps;}

   private:
      void ProcessNewPriority(UINT priority);
      void UpdatePriorities(void);
      void UpdateLoadedBitmaps(void);

      UINT highestPriority;
      UINT lowestPriority;
      UINT numOfLoadedBitmaps;
      UINT totalMemory;
   };
}
