#include <iostream>
#include <math.h>
#include "CDA.cpp"
#
using namespace std;

template <typename keytype>

class FibHeap
{
private:
    struct node
    {
        node *prev = NULL;
        node *sibling = NULL;
        node *child = NULL;
        node *parent = NULL;
        keytype key;
        node() {}
        node(keytype k)
        {
            key = k;
        }
        int degree = 0;
    };

    // DLL vars
    node *head = NULL;
    node *tail = NULL;
    node *min = NULL;

    int numNodes = 0;

    void addRootList(node *&temp)
    {
        // Case 1: List is Empty (!tail)
        if (!tail)
        {
            head = tail = min = temp;
            temp->sibling = temp->prev = NULL;
            return;
        }
        // Case 2: List !Empty
        tail->sibling = temp;
        temp->prev = tail;
        tail = temp;
        tail->sibling = NULL;

        if (!min || temp->key < min->key) // !min = min is null
        {
            min = temp;
        }
    }

    void consolidate()
    {
        CDA<node *> consolidateArr((int)log2(numNodes) + 1, NULL);
        for (node *x = head; x != NULL; x = x->sibling)
        {
            int d = x->degree;
            while (consolidateArr[d] != NULL)
            {
                node *y = consolidateArr[d];
                if (x->key > y->key)
                {
                    swap(x, y);
                    x->sibling = y->sibling;
                }
                y->sibling = x->child;
                x->child = y;
                x->parent = y;
                x->degree++;
                consolidateArr[d] = NULL;
                d++;
            }
            consolidateArr[d] = x;
        }
        head = tail = min = NULL;
        for (int i = 0; i < consolidateArr.Length(); i++)
        {
            if (consolidateArr[i] != NULL)
            {
                addRootList(consolidateArr[i]);
            }
        }
    }

    void recursivePrintkey(node *node)
    {
        if (node)
        {
            cout << " " << node->key;
            recursivePrintkey(node->child);
            recursivePrintkey(node->sibling);
        }
    }

    void deleteFibHeap(node *node)
    {
        if (node != NULL){
            deleteFibHeap(node->child);
            deleteFibHeap(node->sibling);
        }
    }

    void copyFibHeap(node* origNode, node* &newNode)
    {
        if (origNode != NULL)
        {
            newNode = new node(origNode->key);
            newNode->degree = origNode->degree;
            copyFibHeap(origNode->child, newNode->child);
            copyFibHeap(origNode->sibling, newNode->sibling);
        }
    }

public:
    FibHeap()
    {
        
    }

    FibHeap(keytype k[], int s)
    {
        for (int i = 0; i < s; i++)
        {
            insert(k[i]);
        }

        consolidate();
    }

    ~FibHeap()
    {
        deleteFibHeap(head);
        head = NULL;
        tail = NULL;
        min = NULL;
    }

    FibHeap(FibHeap &toCopy){
        node *copy;
        for (node* listIterator = toCopy.head; listIterator != NULL; listIterator = listIterator->sibling)
        {
            copy = new node(listIterator->key);
            copy->degree = listIterator->degree;
            copyFibHeap(listIterator->child,copy->child);
            addRootList(copy);
        }
        numNodes = toCopy.numNodes;
    }

    FibHeap &operator=(FibHeap &toCopy)
    {
        this->~FibHeap();
        node *copy;
        for (node* listIterator = toCopy.head; listIterator != NULL; listIterator = listIterator->sibling)
        {
            copy = new node(listIterator->key);
            copy->degree = listIterator->degree;
            copyFibHeap(listIterator->child,copy->child);
            addRootList(copy);
        }
        numNodes = toCopy.numNodes;
        return *this;
    }


    keytype peekKey()
    {
        return min->key;
    }

    keytype extractMin()
    {
        keytype minVal = min->key;
        node *childTemp = min->child; // move every child (add evr child of min to root list)
        while (childTemp != NULL)
        {
            node *temp = childTemp->sibling;
            addRootList(childTemp);
            childTemp = temp;
            
        }

        if (min->prev != NULL)
            min->prev->sibling = min->sibling;
        if (min->sibling != NULL)
            min->sibling->prev = min->prev;
        if (min == head)
            head = min->sibling;
        if (min == tail)
            tail = min->prev;
        delete min;
        min = NULL;
        consolidate();
        numNodes--;
        return minVal;
    }

    void insert(keytype k)
    {
        // Case 1: List is Empty (!tail)
        if (!tail)
        {
            numNodes++;
            head = tail = min = new node(k);
            return;
        }
        // Case 2: List !Empty
        numNodes++;
        node *temp = new node(k);
        tail->sibling = temp;
        temp->prev = tail;
        tail = temp;

        if (!min || k < min->key) //! min = min is null
        {
            min = tail;
        }
    }

    void merge(FibHeap<keytype> &H2)
    {
        tail->sibling = H2.head;
        H2.head->prev = tail;
        tail = H2.tail;

        if (H2.min->key < min->key)
        {
            min = H2.min;
        }
        numNodes += H2.numNodes;

        H2.head = nullptr;
        H2.tail = nullptr;
        H2.min = nullptr;
        H2.numNodes = 0;
    }

    void printKey()
    {
        node *listIterator = head;
        while (listIterator != NULL)
        {
            cout << 'B' << listIterator->degree << endl << listIterator->key;
            recursivePrintkey(listIterator->child);
            cout << endl;
            listIterator = listIterator->sibling;
        }
        // node iterator that iterates through the root list then prints the degree of the node
    }
};