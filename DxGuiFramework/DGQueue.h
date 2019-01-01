/*------------------------------------------------------------------------
File Name: DGQueue.h
Description: A templated queue class
   The templated item should be a pointer, otherwise this class could be
   very inefficient. This class is derived from DG::LinkedList, as a queue
   is really only a type of linked list. The Enqueue and Dequeue functions
   will probably be the only ones to be regularly used, but all of
   the linked list functions are there if needed.
Version:
   1.0.0    05.01.2001  Created the file
   1.1.0    10.02.2001  Class Name changed from Queue to DGQueue. 
                        A default constructor was added.
   2.0.0    02.06.2002  Changed the file to use namespaces and adapt
      to Visual Studio .NET
------------------------------------------------------------------------*/

#pragma once

#include <assert.h>
#include "DGLinkedList.h"

#define NO_ID  -1

namespace DG
{
   template <class Data>
   class Queue : public LinkedList<Data>
   {
   public:
      //Constructor: destroyItems specifies whether the data 
      //in the queue should be deleted when the queue is destroyed
      Queue(bool destroyItems) : LinkedList<Data>(destroyItems) {}
      Queue() : LinkedList<Data>() {}

      //Destructor
      ~Queue() {}

      void Enqueue(Data* data, int id = NO_ID);
      Data* Dequeue(void);
      Data* Dequeue(int id);

   protected:
   private:

   };

   /*------------------------------------------------------------------------
   Function Name: Enqueue
   Parameters:
      Data* data : the data to be added to the queue
      int id : the id to give the data, defaults to NO_ID
   Description:
      Adds the given data item to the end of the queue
   ------------------------------------------------------------------------*/
   template<class Data>
   void Queue<Data>::Enqueue(Data* data, int id)
   {
      Append(data, id);
   }

   /*------------------------------------------------------------------------
   Function Name: Dequeue
   Parameters: void
   Description:
      The data at the front of the queue is removed from the queue and 
      returned. The data is *not* deleted, so there should be a 
      pointer to the data, otherwise calling this function will cause 
      a memory leak. If the list is empty, NULL is returned
   ------------------------------------------------------------------------*/
   template<class Data>
   Data* Queue<Data>::Dequeue(void)
   {
      return RemoveFirst();
   }

   /*------------------------------------------------------------------------
   Function Name: Dequeue
   Parameters:
      int id : the id associated with the item to be removed
   Description:
      The first item in the queue associated with the specified id is 
      removed from the queue and returned. This is not necessarily the 
      first item in the queue, just the first item with that id. 
      The data is *not* deleted, so there should be a pointer to the data, 
      otherwise calling this function will cause a memory leak.
      If there is no item with the specified id or the queue is empty,
      NULL is returned.
   ------------------------------------------------------------------------*/
   template<class Data>
   Data* Queue<Data>::Dequeue(int id)
   {
      return RemoveById(id);
   }
}