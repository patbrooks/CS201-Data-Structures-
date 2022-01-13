#include <iostream>
#include <time.h>

using namespace std;

#ifndef CDA_CPP
#define CDA_CPP
template <typename elmtype>
class CDA
{
public:
    // Default Constructor
    CDA()
    {
        head = 0;
        tail = 0;
        size = 0;
        capacity = 1;
        array = new elmtype;
    }

    // Constructor
    CDA(int s)
    {
        size = s;
        capacity = s;
        head = 0;
        tail = size - 1;
        array = new elmtype[capacity];
    }

    // initialization constructor
    CDA(int s, elmtype i)
    {
        initialized = true;
        init = i; // The array should bact as though it has been initialized w/ val init, but be done in O(1) time using access method discussed in class
        size = s;
        capacity = s;
        head = 0;
        tail = size - 1;
        array = new elmtype[capacity]; // (1) Go over and/or ask questions
        ptrArray = new int *[capacity];
        indexArray = new int[capacity];
    }

    // Copy Constructor
    CDA(const CDA<elmtype> &copyMe)
    {
        size = copyMe.size; // Need the .operator to access private member vars
        capacity = copyMe.capacity;
        initialized = copyMe.initialized;
        head = copyMe.head;
        tail = copyMe.tail;
        array = new elmtype[capacity];
        ptrArray = new int *[capacity];
        indexArray = new int[capacity];
        indexArraysize = copyMe.indexArraysize;
        for (int i = 0; i <= size; i++)
        {
            array[i] = copyMe.array[i];
            // case 1
            if (initialized)
            {
                ptrArray[i] = &indexArray[i];
                indexArray[i] = i;
            }
        }
        // case 2
        if (!initialized)
        {
            if (size == 1)
            {
                delete ptrArray;
                delete indexArray;
            }
            else
            {
                delete[] ptrArray;
                delete[] indexArray;
            }
        }
    }

    CDA<elmtype> &operator=(const CDA<elmtype> &copyMe)
    {
        size = copyMe.size;
        capacity = copyMe.capacity;
        initialized = copyMe.initialized;
        head = copyMe.head;
        tail = copyMe.tail;
        array = new elmtype[capacity];
        ptrArray = new int *[capacity];
        indexArray = new int[capacity];
        indexArraysize = copyMe.indexArraysize;
        for (int i = 0; i <= size; i++)
        {
            array[i] = copyMe.array[i];
            if (initialized)
            {
                ptrArray[i] = &indexArray[i];
                indexArray[i] = i;
            }
        }
        if (!initialized)
        {
            if (size == 1)
            {
                delete ptrArray;
                delete indexArray;
            }
            else
            {
                delete[] ptrArray;
                delete[] indexArray;
            }
        }
        return (*this);
    }

    // Destructor
    ~CDA()
    {
        if (size == 1)
        {
            delete array;
            if (initialized)
            {
                delete ptrArray;
                delete indexArray;
            }
        }
        else
        {
            delete[] array;
            if (initialized)
            {
                delete[] ptrArray;
                delete[] indexArray;
            }
        }
    }

    elmtype &operator[](int i)
    {
        if (i < 0 || i > size - 1)
        {
            cout << "Out of Bounds: ";
            return trash;
        }
        if (initialized) // Checks if prtArray is point to valid indicies in indexArray
        {
            if ((void *)ptrArray[(i + head) % capacity] >= &indexArray[0] && (void *)ptrArray[(i + head) % capacity] <= &indexArray[indexArraysize - 1])
            {
                if (*ptrArray[(i + head) % capacity] == (i + head) % capacity)
                {
                    return array[(i + head) % capacity];
                }
            }
            // (2) Go over and/or ask questions
            if ((void *)ptrArray[(i + head) % capacity] < &indexArray[0] || (void *)ptrArray[(i + head) % capacity] > &indexArray[indexArraysize - 1] || *ptrArray[(i + head) % capacity] != (i + head) % capacity)
            {
                array[(i + head) % capacity] = init;
                ptrArray[(i + head) % capacity] = &indexArray[indexArraysize];
                indexArray[indexArraysize] = (i + head) % capacity;
                indexArraysize++;
                return array[(i + head) % capacity];
            }
        }
        return array[(i + head) % capacity];
    }

    void AddEnd(elmtype v)
    {
        if (size == capacity)
            grow();
        tail = (tail + 1) % capacity; // (3) Go over and/or ask questions
        array[tail] = v;
        size++;
        if (initialized) // (4)
        {
            if ((void *)ptrArray[tail] >= &indexArray[0] && (void *)ptrArray[tail] <= &indexArray[indexArraysize - 1] && *ptrArray[tail] == tail)
            {
                indexArraysize++;
                return;
            }
            ptrArray[tail] = &indexArray[indexArraysize];
            indexArray[indexArraysize] = tail;
            indexArraysize++;
        }
    }

    void AddFront(elmtype v) // (5)
    {
        if (size == capacity)
            grow();
        head = (head + capacity - 1) % capacity;
        array[head] = v;
        size++;
        if (initialized)
        {
            if ((void *)ptrArray[head] >= &indexArray[0] && (void *)ptrArray[head] <= &indexArray[indexArraysize - 1] && *ptrArray[head] == head)
            {
                indexArraysize++;
                return;
            }
            ptrArray[head] = &indexArray[indexArraysize];
            indexArray[indexArraysize] = head;
            indexArraysize++;
        }
    }

    void DelEnd() // (6)
    {
        if (size == 0)
            return;
        tail = (tail - 1 + capacity) % capacity;
        size--;
        if (size <= capacity / 4)
            shrink();
    }

    void DelFront() //(7)
    {
        if (size == 0)
            return;
        head = (head + 1) % capacity;
        size--;
        if (size <= capacity / 4 && size > 4)
            shrink();
    }

    int Length()
    {
        return size;
    }

    int Capacity()
    {
        return capacity;
    }

    int partition(int l, int r)
    {
        srand(time(NULL));
        int randomIndex = rand() % (r - l + 1) + l;
        swap((*this)[randomIndex], (*this)[r]);
        elmtype x = (*this)[r];
        int i = l;
        for (int j = l; j <= r - 1; j++)
        {
            if ((*this)[j] <= x)
            {
                swap((*this)[i], (*this)[j]);
                i++;
            }
        }
        swap((*this)[i], (*this)[r]);
        return i;
    }

    elmtype kthSmallest(int l, int r, int k) //(8)
    {                                        // Using Quickselect
        if (k > 0 && k <= r - l + 1)
        {
            int index = partition(l, r);
            if (index - l == k - 1)
                return (*this)[index];
            if (index - l > k - 1)
                return kthSmallest(l, index - 1, k);
            return kthSmallest(index + 1, r, k - index + l - 1);
        }
        return trash;
    }

    elmtype Select(int k)
    {
        return kthSmallest(0, size - 1, k);
    }

    void merge(int const left, int const mid, int const right) //(9)
    {
        int const subArrayOne = mid - left + 1;
        int const subArrayTwo = right - mid;

        // Create temp arrays
        elmtype *leftArray = new int[subArrayOne], *rightArray = new int[subArrayTwo];

        // Copy data to temp arrays leftArray[] and rightArray[]
        for (auto i = 0; i < subArrayOne; i++)
            leftArray[i] = (*this)[left + i];
        for (auto j = 0; j < subArrayTwo; j++)
            rightArray[j] = (*this)[mid + 1 + j];

        auto indexOfSubArrayOne = 0,   // Initial index of first sub-array
            indexOfSubArrayTwo = 0;    // Initial index of second sub-array
        int indexOfMergedArray = left; // Initial index of merged array

        // Merge the temp arrays back into array[left..right]
        while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo)
        {
            if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo])
            {
                (*this)[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
                indexOfSubArrayOne++;
            }
            else
            {
                (*this)[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
                indexOfSubArrayTwo++;
            }
            indexOfMergedArray++;
        }
        // Copy the remaining elements of
        // left[], if there are any
        while (indexOfSubArrayOne < subArrayOne)
        {
            (*this)[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
            indexOfMergedArray++;
        }
        // Copy the remaining elements of
        // right[], if there are any
        while (indexOfSubArrayTwo < subArrayTwo)
        {
            (*this)[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
            indexOfMergedArray++;
        }
    }

    void mergeSort(int const begin, int const end)
    {
        if (begin >= end)
            return; // Returns recursively

        auto mid = begin + (end - begin) / 2;
        mergeSort(begin, mid);
        mergeSort(mid + 1, end);
        merge(begin, mid, end);
    }

    void Sort()
    {
        mergeSort(0, size - 1);
    }

    int Search(elmtype e)
    { // asked for linear search, so easy
        for (int i = 0; i < size; i++)
        {
            if ((*this)[i] == e)
                return i;
        }
        return -1;
    }

    int BinSearch(elmtype e)
    {
        return binarySearch(0, size - 1, e);
    }

    int binarySearch(int l, int r, elmtype x) //(10)
    {
        if (r - l + 1 <= 3)
        {
            for (int i = l; i <= r; i++)
            {
                if ((*this)[i] == x)
                    return i;
                if ((*this)[i] > x)
                    return ~i;
                if (l + i == r)
                    return ~size;
            }
        }
        if (r >= l)
        {
            int mid = l + (r - l) / 2;

            // If the element is present at the middle
            // itself
            if ((*this)[mid] == x)
                return mid;

            // If element is smaller than mid, then
            // it can only be present in left subarray
            if ((*this)[mid] > x)
                return binarySearch(l, mid - 1, x);

            // Else the element can only be present
            // in right subarray
            return binarySearch(mid + 1, r, x);
        }

        // We reach here when element is not
        // present in array
        return -1;
    }

private:
    int size;
    int capacity;
    int head;
    int tail;
    elmtype *array;
    int **ptrArray;
    int *indexArray;
    int indexArraysize = 0;
    bool initialized = false;
    elmtype init;
    elmtype trash;

    // Helper Functions

    void grow() //(11a)
    {
        int NewCapacity = capacity * 2;
        elmtype *grownArray = new elmtype[NewCapacity];

        for (int i = 0; i < size; i++) // Make sure you study this code*********
        {
            grownArray[i] = array[(i + head) % capacity];
            if (initialized && (void *)ptrArray[(i + head) % capacity] >= &indexArray[0] && (void *)ptrArray[(i + head) % capacity] <= &indexArray[indexArraysize - 1] && *ptrArray[(i + head) % capacity] == (i + head) % capacity)
                grownArray[i] = init;
        }

        // Delete ptrArray and indexArray bc they are not needed once array is grown
        capacity = NewCapacity;
        head = 0;
        tail = size - 1;
        this->~CDA();
        array = grownArray;
        initialized = false;
    }

    void shrink() //(11b)
    {
        int NewCapacity = max(capacity / 2, 4);
        elmtype *shrunkArray = new elmtype[NewCapacity];
        for (int i = 0; i < size; i++)
        {
            shrunkArray[i] = (*this)[i];
        }
        this->~CDA();
        // Delete ptrArray and indexArray bc they are not needed once array is shrunk
        capacity = NewCapacity;
        head = 0;
        tail = size - 1;
        array = shrunkArray;
    }

    int partition(int array[], int low, int high) //(12) Go over and/or ask questoins
    {
        int pivot = array[high]; // pivot
        int i = (low - 1);       // Index of smaller element and indicates the right position of pivot found so far

        for (int j = low; j <= high - 1; j++)
        {
            // If current element is smaller than the pivot
            if (array[j] < pivot)
            {
                i++; // increment index of smaller element
                swap(array[i], array[j]);
            }
        }
        swap(array[i + 1], array[high]);
        return (i + 1);
    }
};
#endif