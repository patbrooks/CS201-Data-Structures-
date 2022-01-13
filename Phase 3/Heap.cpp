#include <iostream>
#include "CDA.cpp"

using namespace std;

template <typename keytype>

class Heap
{
private:
    CDA<keytype> heapArray;

    int getRight(int parent)
    {

        return (2 * parent) + 2;
    }

    int getLeft(int parent)
    {
        return (2 * parent) + 1;
    }

    void minHeapify(int parentIndex)
    {
        int left = getLeft(parentIndex);
        int right = getRight(parentIndex);
        int smallest = parentIndex;
        if (heapArray.Length() - 1 >= left && heapArray[left] < heapArray[parentIndex])
        {
            smallest = left;
        }
        if (heapArray.Length() - 1 >= right && heapArray[right] < heapArray[smallest])
        {
            smallest = right;
        }
        if (smallest != parentIndex)
        {
            swap(heapArray[parentIndex], heapArray[smallest]);
            minHeapify(smallest);
        }
    }

public:
    Heap()
    {
    }

    Heap(Heap &toCopy)
    {
        heapArray = toCopy.heapArray;
    }

    Heap &operator=(Heap &toCopy)
    {
        heapArray = toCopy.heapArray;
        return *this;
    }

    Heap(keytype k[], int s)
    {
        for (int i = 0; i < s; i++)
        {
            heapArray.AddEnd(k[i]);
        }
        for (int i = heapArray.Length() - 1; i >= 0; i--)
        {
            minHeapify(i);
        }
    }

    ~Heap()
    {

    }

    keytype peekKey()
    {
        return heapArray[0];
    }

    keytype extractMin()
    {
        keytype temp = heapArray[0];
        heapArray[0] = heapArray[heapArray.Length() - 1];
        heapArray.DelEnd();
        minHeapify(0);
        return temp;
    }

    void insert(keytype k)
    {
        heapArray.AddEnd(k);
        int i = heapArray.Length() - 1;
        while (heapArray[i] < heapArray[(i - 1) / 2])
        {
            swap(heapArray[i], heapArray[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    void printKey()
    {
        for (int i = 0; i < heapArray.Length(); i++)
        {
            cout << heapArray[i] << " ";
        }
    }
};