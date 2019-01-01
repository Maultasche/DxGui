/*------------------------------------------------------------------------
File Name: DGDynamicArray.h
Description: A templated dynamic array class
   This class acts as an array that grows when more room in the array
   is needed. The Grow method can be called manually by the programmer
   or will be called automatically when the programmer indexes an index
   that doesn't exist yet. The Shrink method must be called manually
   to shrink the array. The array cannot know automatically how much
   to shrink itself by, so the programmer has to specify this.
   The templated item should be a pointer, otherwise this class could be
   very inefficient, due to large amounts of copying. 
Version:
   1.0.0    27.05.2002  Created the file
   1.1.0    02.06.2002  Changed the file to use namespaces
------------------------------------------------------------------------*/

#pragma once

#define DA_DEFAULT_SIZE    10
#define DA_DEFAULT_GROW    10

namespace DG
{
   template <class Data>
   class DynamicArray
   {
   public:
      /*------------------------------------------------------------------------
      Function Name: Default Constructor
      ------------------------------------------------------------------------*/
      DynamicArray() :
         growAmount(DA_DEFAULT_GROW),
         arraySize(DA_DEFAULT_SIZE)
      {
         dynamicArray = new Data[arraySize];

         for(unsigned int i = 0; i < arraySize; i++)
            dynamicArray[i] = initialValue;
      }

      /*------------------------------------------------------------------------
      Function Name: Constructor
      Parameters:
         unsigned int initialSize : the inital number of indices to be 
            allocated in the dynamic array
         unsigned int growAmount : the amount by which the dynamic array should
            grow when it needs to grown
      Description:
      Notes: If a very high index is accessed, the array will grow so that
         the index exists. This could be greater than the value of growAmount.
         The initialValue member will be undefined when using this constructor.
      ------------------------------------------------------------------------*/
      DynamicArray(unsigned int initialSize, unsigned int growValue = 10) :
         growAmount(growValue),
         arraySize(initialSize)
      {
         dynamicArray = new Data[arraySize];

         for(int i = 0; i < arraySize; i++)
            dynamicArray[i] = initValue;
      }

      /*------------------------------------------------------------------------
      Function Name: Constructor
      Parameters:
         unsigned int initialSize : the inital number of indices to be 
            allocated in the dynamic array
         Data initValue : The value to initialize all the indices in the array
            with
         unsigned int growAmount : the amount by which the dynamic array should
            grow when it needs to grown
      Description:
      Notes: If a very high index is accessed, the array will grow so that
         the index exists. This could be greater than the value of growAmount
      ------------------------------------------------------------------------*/
      DynamicArray(unsigned int initialSize, Data initValue, 
         unsigned int growValue = 10) :
         growAmount(growValue),
         initialValue(initValue),
         arraySize(initialSize)
      {
         dynamicArray = new Data[arraySize];

         for(int i = 0; i < arraySize; i++)
            dynamicArray[i] = initValue;
      }

      /*------------------------------------------------------------------------
      Function Name: Destructor
      ------------------------------------------------------------------------*/
      ~DynamicArray()
      {
         if(dynamicArray != NULL)
         {
            delete[] dynamicArray;
            dynamicArray = NULL;
         }
      }


      /*------------------------------------------------------------------------
      Function Name: operator[]
      Description: The accessor operator that allows access to a particular
         index in the dynamic array
      ------------------------------------------------------------------------*/
      Data& operator[](unsigned int index)
      {
         //If the index doesn't currently exist, then it needs to be created
         if(arraySize <= index)
         {
            //If the accessed index is with one growth of the dynamic array,
            //then simply grow it
            if(index - arraySize < growAmount)
            {
               Grow(growAmount);
            }
            //Otherwise grow it as much as is necessary to reach the index
            else
            {
               Grow((index - arraySize) + 1);
            }
         }

         return dynamicArray[index];
      }

      void Grow(void) 
      {
         Grow(growAmount);
      }

      void Grow(unsigned int growValue)
      {
         //Allocate the new array
         unsigned int newArraySize = arraySize + growValue;
         Data* newArray = new Data[newArraySize];

         //Copy over the data from the old array
         for(unsigned int i = 0; i < arraySize; i++)
         {
            newArray[i] = dynamicArray[i];
         }

         //Initialize the rest of the new array
         for(unsigned int i = arraySize; i < newArraySize; i++)
         {
            newArray[i] = initialValue;
         }

         //Destroy the old array
         delete dynamicArray;

         //Set the new size
         dynamicArray = newArray;
         arraySize = newArraySize;
      }

      void Shrink(unsigned int shrinkValue)
      {
         //Allocate the new array
         unsigned int newArraySize = arraySize - shrinkValue;
         Data* newArray = new Data[newArraySize];

         //Copy over the data from the old array
         for(unsigned int i = 0; i < newArraySize; i++)
         {
            newArray[i] = dynamicArray[i];
         }

         //Destroy the old array
         delete dynamicArray;

         //Set the new size
         dynamicArray = newArray;
         arraySize = newArraySize;
      }

      void SetSize(unsigned int newSize)
      {
         //If the new size is smaller than the current size, then shrink
         //the array
         if(newSize < arraySize)
         {
            Shrink(arraySize - newSize);
         }

         //If the new size is larger than the current size, then grow
         //the array
         else if(newSize > arraySize)
         {
            Grow(newSize - arraySize);
         }

         //If the new size and the current size are the same, then
         //do nothing
      }

      unsigned int GetCurrentArraySize(void) {return arraySize;}

      void SetGrowAmount(unsigned int growValue) {growAmount = growValue;}
      unsigned int GetGrowAmount(void) {return growAmount;}

      void SetInitialValue(Data value) {initialValue = value;}
      Data GetInitialValue(void) {return initialValue;}


   private:
      unsigned int growAmount;
      unsigned int arraySize;
      Data initialValue;

      Data* dynamicArray;
   };
}
