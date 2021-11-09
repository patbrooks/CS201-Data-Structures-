#include <iostream>
#include <time.h>

using namespace std;

template <typename keytype>
class Treap
{
private:
    struct node
    {
        keytype key;
        float priority;
        node *left = NULL;
        node *right = NULL;
        int subSize = 1;
    };

    void rotateL(node *&y)
    {
        node *x = y->right;

        node *r = y->left; // will not change
        node *g = x->left;
        node *b = x->right; // will not change

        x->subSize = y->subSize;
        y->subSize = 1 + (r != NULL ? r->subSize : 0) + (g != NULL ? g->subSize : 0);

        x->left = y;
        y->right = g;

        y = x;
    }

    void rotateR(node *&x)
    {
        node *y = x->left;

        node *r = y->left; // will not change
        node *g = y->right;
        node *b = x->right; // will not change

        y->subSize = x->subSize;
        x->subSize = 1 + (g != NULL ? g->subSize : 0) + (b != NULL ? b->subSize : 0);

        x->left = g;
        y->right = x;

        x = y;
    }

    void privateInsert(node* &root, keytype k, float p)
    {
        if (!root)
        {
            root = newNode(k,p);
            return;
        } 
        root->subSize++;
        privateInsert(k <= root->key ? root->left : root->right, k, p);

        if (root->left && root->left->priority < root->priority)
            rotateR(root);

        if (root->right && root->right->priority < root->priority)
            rotateL(root);
    }

    node *newNode(keytype k, float p)
    {
        node *temp = new node;
        temp->key = k;
        temp->priority = p;
        return temp;
    }

    void deleteTree(node *node)
    {
        if (node == NULL)
        {
            return;
        }

        /* first delete both subtrees */
        deleteTree(node->left);
        deleteTree(node->right);

        /* then delete the node */
        delete node;
    }

    bool privateRemove(node *&root, keytype key)
    {
        if (root == NULL)
            return false;
        root->subSize--;
        if (key < root->key)
            return privateRemove(root->left, key);
        if (key > root->key)
            return privateRemove(root->right, key);

        // Case 1: TreapNode to be deleted has no children (it is a leaf TreapNode)
        if (!root->left && !root->right)
        {
            delete root;
            root = NULL;
        }

        // Case 2: TreapNode to be deleted has only one child
        else if (!root->left || !root->right)
        {
            node *child = (root->left) ? root->left : root->right;

            node *old_root = root;
            root = child;

            delete old_root;
        }

        // Case 3: TreapNode to be deleted has two children
        else
        {
            if (root->left->priority > root->right->priority)
            {
                rotateL(root);
                privateRemove(root->left, key);
            }
            else
            {
                rotateR(root);
                privateRemove(root->right, key);
            }
        }

        return true;
    }

    void privateInOrder(node *root)
    {
        if (root == NULL)
            return;

        /* first recur on left child */
        privateInOrder(root->left);

        /* then print the key of node */
        cout << root->key << " ";

        /* now recur on right child */
        privateInOrder(root->right);
    }

    void privatePreOrder(node *root)
    {
        if (root== NULL)
            return;

        /* first print key of node */
        cout << root->key << " ";

        /* then recur on left subtree */
        privatePreOrder(root->left);

        /* now recur on right subtree */
        privatePreOrder(root->right);
    }

    void privatePostOrder(node *root)
    {
        if (root == NULL)
            return;

        // first recur on left subtree
        privatePostOrder(root->left);

        // then recur on right subtree
        privatePostOrder(root->right);

        // now deal with the node
        cout << root->key << " ";
    }

    void privateCopy(node *root, node *&copyNode)
    {
        if (root == NULL)
        {
            return;
        }

        copyNode->key = root->key;
        copyNode->priority = root->priority;
        copyNode->subSize = root->subSize;

        if (root->left != NULL)
        {
            copyNode->left = new node;
        }

        if (root->right != NULL) 
        {
            copyNode->right = new node;
        }

        privateCopy(root->left, copyNode->left);
        privateCopy(root->right, copyNode->right);
    }

    node* privateSelect(node* root, int pos)
    {
        if (pos == (root->left != NULL ? root->left->subSize : 0) + 1)
            return root;
        else if (pos < (root->left != NULL ? root->left->subSize : 0) + 1)
            return privateSelect(root->left, pos);
        else return privateSelect(root->right, pos - (root->left != NULL ? root->left->subSize : 0) - 1);
        return newNode(keytype(), ((float)rand() / (float)(RAND_MAX)));
    }

    node *root = NULL;

public:
    //Constructor
    Treap()
    {
        srand(time(NULL));
    }

    Treap(keytype k[], float p[], int s)
    {
        for (int i = 0; i < s; i++)
        {
            insert(k[i], p[i]);
        }
    }

    ~Treap()
    {
        deleteTree(root);
    }

    float search(keytype k)
    {
        node *temp = root;

        while (temp != NULL && temp->key != k)
        {
            if (temp->key < k)
            {
                temp = temp->right;
                continue;
            }

            if (temp->key > k)
            {
                temp = temp->left;
                continue;
            }
        }

        if (temp == NULL)
        {
            return -1;
        }
        return temp->priority;
    }

    void insert(keytype k, float p)
    {
        privateInsert(root, k, p);
    }

    void insert(keytype k)
    {
        privateInsert(root, k, ((float)rand() / (float)(RAND_MAX)));
    }

    int remove(keytype k)
    {
        if (search(k) == -1)
            return 0;
        privateRemove(root, k);
        return 1;
    }

    int rank(keytype k)
    {
        int r = 1;
        node* temp = root;

        while(temp != NULL)
        {   
            if (k < temp->key)
            {
                temp = temp->left;
            }
            
            else if(k > temp->key)
            {
                //if (temp->left !=NULL) cout << "temp->key = " << temp->key << " " << temp->left->subSize << '\n';
                r = r + 1 + (temp->left != NULL ? temp->left->subSize : 0);
                temp = temp->right;
            }
            else{
                //if (temp->left !=NULL) cout << "temp->key = " << temp->key << " " << temp->left->subSize << '\n';
                return r + (temp->left != NULL ? temp->left->subSize : 0);
            }
        }
        return 0;
    }

    int size()
    {
        return root->subSize;
    }

    void inorder()
    {
        privateInOrder(root);
        cout << '\n';
    }

    void preorder()
    {
        privatePreOrder(root);
        cout << '\n';
    }

    void postorder()
    {
        privatePostOrder(root);
        cout << '\n';
    }

    Treap(const Treap &source)
    {
        root = new node;
        privateCopy(source.root, root);
    }

    Treap& operator=(const Treap &source)
    {
        this->~Treap();
        root = new node;
        privateCopy(source.root, root);
        return *this;
    }

    keytype select(int pos){
        return privateSelect(root, pos)->key;
    }

    keytype successor(keytype k){
        int kRank = this->rank(k);
        if (kRank == root->subSize || kRank == 0) return k;
        return select(kRank + 1);
    }

    keytype predecessor(keytype k){
        int kRank = this->rank(k);
        if (kRank == 0 || kRank == 1) return k;
        return select(kRank - 1);
    }
};

// int main()
// {
//     // Treap<string> t;
//     // t.insert("Patrick");
// }
