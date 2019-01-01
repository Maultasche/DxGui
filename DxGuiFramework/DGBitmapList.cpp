/*------------------------------------------------------------------------
File Name: DGBitmap.cpp
Description: This file contains the implemenation of the DG::Bitmap class. 
	An object of this class is able to load as well as store bitmaps.
Version:
   1.0.0    25.02.2001  Created the file
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#include "DxGuiFramework.h"

using namespace DG;

/*Default Constructor*/
BitmapList::BitmapList() : LinkedList<Bitmap>()
{
   highestPriority = 1;
   lowestPriority = 1;
   totalMemory = 0;
   numOfLoadedBitmaps = 0;
}

/*------------------------------------------------------------------------
Function Name: Constructor
Parameters:
   bool destroyItems : whether the data contained in the list is to be
      destroyed when the list is destroyed
------------------------------------------------------------------------*/

BitmapList::BitmapList(bool destroyItems) : 
      LinkedList<Bitmap>(destroyItems)
{
   highestPriority = 1;
   lowestPriority = 1;
   totalMemory = 0;
   numOfLoadedBitmaps = 0;
}

/*------------------------------------------------------------------------
Function Name: RemoveFirstBitmapByPriority
Parameters:
   UINT priority : the priority of the bitmap to be removed
Description:
   This function iterates through the list of bitmaps and removes from 
   memory the first bitmap with the specified priority it encounters.
------------------------------------------------------------------------*/

void BitmapList::RemoveFirstBitmapByPriority(UINT priority)
{
   //If the list is empty
   if(numOfItems == 0)
      return;

   bool done = false;

   Node<Bitmap>* nodePtr = first;
   for(int i = 0; i < numOfItems && !done; i++)
   {
      Bitmap* bitmap = nodePtr->data;
      if(bitmap->IsLoaded() &&
         bitmap->GetPriority() == priority)
      {
         bitmap->DestroyBitmap();
         done = true;
         numOfLoadedBitmaps--;
      }

      nodePtr = nodePtr->next;
   }

   UpdatePriorities();
}

/*------------------------------------------------------------------------
Function Name: RemoveAllBitmapsByPriority
Parameters:
   UINT priority : the priority of the bitmap to be removed
Description:
   This function iterates through the list of bitmaps, removing from 
   memory all bitmaps with the specified priority.
------------------------------------------------------------------------*/

void BitmapList::RemoveAllBitmapsByPriority(UINT priority)
{
   //If the list is empty
   if(numOfItems == 0)
      return;

   Node<Bitmap>* nodePtr = first;
   for(int i = 0; i < numOfItems; i++)
   {
      Bitmap* bitmap = nodePtr->data;
      if(bitmap->IsLoaded() &&
         bitmap->GetPriority() == priority)
      {
         bitmap->DestroyBitmap();
         numOfLoadedBitmaps--;
      }

      nodePtr = nodePtr->next;
   }

   UpdatePriorities();
}

/*------------------------------------------------------------------------
Function Name: RemoveFirstLowestPriorityBitmap
Parameters:
Description:
   This function iterates through the list of bitmaps and removes from 
   memory the first bitmap with the lowest priority it encounters.
------------------------------------------------------------------------*/

void BitmapList::RemoveFirstLowestPriorityBitmap()
{
   //If the list is empty
   if(numOfItems == 0)
      return;

   bool done = false;

   Node<Bitmap>* nodePtr = first;
   for(int i = 0; i < numOfItems && !done; i++)
   {
      Bitmap* bitmap = nodePtr->data;
      if(bitmap->IsLoaded() &&
         bitmap->GetPriority() == lowestPriority)
      {
         bitmap->DestroyBitmap();
         done = true;
         numOfLoadedBitmaps--;
      }

      nodePtr = nodePtr->next;
   }

   UpdatePriorities();
}

/*------------------------------------------------------------------------
Function Name: RemoveAllLowestPriorityBitmaps
Parameters:
Description:
   This function iterates through the list of bitmaps, removing from 
   memory all bitmaps with the lowest priority.
------------------------------------------------------------------------*/

void BitmapList::RemoveAllLowestPriorityBitmaps()
{
   //If the list is empty
   if(numOfItems == 0)
      return;

   Node<Bitmap>* nodePtr = first;
   for(int i = 0; i < numOfItems; i++)
   {
      Bitmap* bitmap = nodePtr->data;
      if(bitmap->IsLoaded() &&
         bitmap->GetPriority() == lowestPriority)
      {
         bitmap->DestroyBitmap();
         numOfLoadedBitmaps--;
      }

      nodePtr = nodePtr->next;
   }

   UpdatePriorities();
}

/*------------------------------------------------------------------------
Function Name: RemoveAllBitmaps
Parameters:
Description:
   This function iterates through the list of bitmaps, removing
   each of the bitmaps from memory, but leaving the bitmap objects
   in the list.
------------------------------------------------------------------------*/

void BitmapList::RemoveAllBitmaps()
{
   //If the list is empty
   if(numOfItems == 0)
      return;

   Node<Bitmap>* nodePtr = first;
   for(int i = 0; i < numOfItems; i++)
   {
      Bitmap* bitmap = nodePtr->data;
      
      bitmap->DestroyBitmap();

      nodePtr = nodePtr->next;
   }

   numOfLoadedBitmaps = 0;
   highestPriority = 1;
   lowestPriority = 1;
}

/*------------------------------------------------------------------------
Function Name: RestoreBitmaps()
Parameters:
Description:
   This function iterates through the list of bitmaps, restoring their
   surfaces and reloading the bitmap.
------------------------------------------------------------------------*/

void BitmapList::RestoreBitmaps()
{
   //If the list is empty
   if(numOfItems == 0)
      return;

   Node<Bitmap>* nodePtr = first;
   for(int i = 0; i < numOfItems; i++)
   {
      Bitmap* bitmap = nodePtr->data;
      
      if(bitmap->IsLoaded())
         bitmap->RestoreBitmap();
         
      nodePtr = nodePtr->next;
   }
}

/*------------------------------------------------------------------------
Function Name: ProcessNewPriority
Parameters:
   UINT priority : the priority of the bitmap that is being added
Description:
   This function receives the priority of a bitmap that is being added
   to the list and updates the highest and lowest priorities if needed.
------------------------------------------------------------------------*/

void BitmapList::ProcessNewPriority(UINT priority)
{
   if(priority == 0)
      return;

   if(priority < highestPriority)
      highestPriority = priority;
   else
      lowestPriority = priority;
}

/*------------------------------------------------------------------------
Function Name: UpdatePriorities
Parameters:
Description:
   This function iterates through the list of bitmaps, updating the
   highest and lowest priorities. This function should be called if
   a bitmap is removed from the list.
------------------------------------------------------------------------*/

void BitmapList::UpdatePriorities()
{
   //If the list is empty
   if(numOfItems == 0)
      return;

   highestPriority = 1;
   lowestPriority = 1;

   UINT priority;
   Node<Bitmap>* nodePtr = first;
   for(int i = 0; i < numOfItems; i++)
   {
      Bitmap* bitmap = nodePtr->data;
      
      priority = bitmap->GetPriority();

      //We only look at those bitmaps who have a priority other than
      //0 and are loaded
      if(priority != 0 && bitmap->IsLoaded())
      {
         if(priority < highestPriority)
            highestPriority = priority;
         else if(priority > lowestPriority)
            lowestPriority = priority;
      }

      nodePtr = nodePtr->next;
   }
}

/*------------------------------------------------------------------------
Function Name: UpdateLoadedBitmaps
Parameters:
Description:
   This function iterates through the list of bitmaps, updating the
   number of loaded bitmaps. This function should be called if
   a bitmap is removed from the list.
------------------------------------------------------------------------*/

void BitmapList::UpdateLoadedBitmaps()
{
   //If the list is empty
   if(numOfItems == 0)
      return;

   numOfLoadedBitmaps = 0;

   Node<Bitmap>* nodePtr = first;
   for(int i = 0; i < numOfItems; i++)
   {
      Bitmap* bitmap = nodePtr->data;
      
      if(bitmap->IsLoaded())
         numOfLoadedBitmaps++;

      nodePtr = nodePtr->next;
   }
}

/*The following are just wrapper functions. They are overridden merely
  so the list can keep track of the lowest and highest priorities*/

void BitmapList::Insert(Bitmap* data, int id)
{
   if(data->IsLoaded())
      numOfLoadedBitmaps++;
   LinkedList<Bitmap>::Insert(data, id);
   ProcessNewPriority(data->GetPriority());
}

void BitmapList::Append(Bitmap* data, int id)
{
   if(data->IsLoaded())
      numOfLoadedBitmaps++;
   LinkedList<Bitmap>::Append(data, id);
   ProcessNewPriority(data->GetPriority());
}

void BitmapList::InsertAtPosition(int index, Bitmap* data, int id)
{
   if(data->IsLoaded())
      numOfLoadedBitmaps++;
   LinkedList<Bitmap>::InsertAtPosition(index, data, id);
   ProcessNewPriority(data->GetPriority());
}

void BitmapList::DeleteByIndex(int index)
{
   LinkedList<Bitmap>::DeleteByIndex(index);
   UpdatePriorities();
   UpdateLoadedBitmaps();
}

void BitmapList::DeleteById(int id)
{
   LinkedList<Bitmap>::DeleteById(id);
   UpdatePriorities();
   UpdateLoadedBitmaps();
}

void BitmapList::DeleteFirst()
{
   LinkedList<Bitmap>::DeleteFirst();
   UpdatePriorities();
   UpdateLoadedBitmaps();
}

void BitmapList::DeleteLast()
{
   LinkedList<Bitmap>::DeleteLast();
   UpdatePriorities();
   UpdateLoadedBitmaps();
}

void BitmapList::DeleteAll()
{
   LinkedList<Bitmap>::DeleteAll();
   highestPriority = 1;
   lowestPriority = 1;
   totalMemory = 0;
   numOfLoadedBitmaps = 0;
}

Bitmap* BitmapList::RemoveByIndex(int index)
{
   Bitmap* bitmap = LinkedList<Bitmap>::RemoveByIndex(index);
   UpdatePriorities();   
   if(bitmap->IsLoaded())
      numOfLoadedBitmaps--;
   return bitmap;
}

Bitmap* BitmapList::RemoveById(int id)
{
   Bitmap* bitmap = LinkedList<Bitmap>::RemoveById(id);
   if(bitmap->IsLoaded())
      numOfLoadedBitmaps--;
   UpdatePriorities();   
   return bitmap;
}

Bitmap* BitmapList::RemoveFirst()
{
   Bitmap* bitmap = LinkedList<Bitmap>::RemoveFirst();
   UpdatePriorities();
   if(bitmap->IsLoaded())
      numOfLoadedBitmaps--;
   return bitmap;
}

Bitmap* BitmapList::RemoveLast()
{
   Bitmap* bitmap = LinkedList<Bitmap>::RemoveLast();
   UpdatePriorities();   
   if(bitmap->IsLoaded())
      numOfLoadedBitmaps--;
   return bitmap;
}

void BitmapList::RemoveAll()
{
   LinkedList<Bitmap>::RemoveAll();
   highestPriority = 1;
   lowestPriority = 1;
   totalMemory = 0;
   numOfLoadedBitmaps = 0;
}