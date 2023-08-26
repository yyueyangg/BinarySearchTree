# include <queue>
# include <stack>
# include <iostream>

using namespace std;

# ifndef BINARY_SEARCH_TREE
# define BINARY_SEARCH_TREE

template<class T>
class Stack : public stack<T>
{
    T push(const T& el)
    {
        push(el);
    }

    T pop()
    {
        T temp = stack<T>::top();
        stack<T>::pop();
        return temp;
    }
};

template<class T>
class Queue : public queue<T>
{
    public:
    void enqueue(const T& el)
    {
        push(el);
    }

    T dequeue()
    {
        T temp = queue<T>::front();
        queue<T>::pop();
        return temp;
    }
};

template<class T>
class BSTNode
{
    public:
    BSTNode()
    {
        left = right = 0;
    }

    BSTNode(const T& e, BSTNode<T> *l = 0, BSTNode<T> *r = 0)
    {
        el = e;
        left = l;
        right = r;
    }

    T el;
    BSTNode<T> *left, *right;
};

template<class T>
class BST
{
    public:
    BST()
    {
        root = 0;
    }

    ~BST()
    {
        clear();
    }

    void clear()
    {
        clear(root);
        root = 0;
    }

    bool isEmpty() const
    {
        return root == 0;
    }

    void preorder()
    {
        preorder(root);
    }

    void inorder()
    {
        inorder(root);
    }

    void postorder()
    {
        postorder(root);
    }

    T* search(const T& el) const
    {
        return search(root, el);
    }

    void breadthFirst()
    {
        Queue<BSTNode<T>*> queue;
        BSTNode<T> *p = root;
        if (p!=0)
        {
            queue.enqueue(p);
            while (!queue.empty())
            {
                p = queue.dequeue();
                visit(p);
                if (p->left != 0)
                {
                    queue.enqueue(p->left);
                }
                if (p->right != 0)
                {
                    queue.enqueue(p->right);
                }
            }
        }
    }

    // left child pushed after right child to be on top of stack
    void iterativePreorder()
    {
        Stack<BSTNode<T>*> travStack;
        BSTNode<T> *p = root;
        if (p != 0)
        {
            travStack.push(p);
            while (!travStack.empty());
            {
                p = travStack.pop();
                visit(p);
                if (p->right != 0)
                {
                    travStack.push(p->right);
                }
                if (p->left != 0)
                {
                    travStack.push(p->left);
                }
            }
        }
    }

    void iterativeInorder()
    {
        Stack<BSTNode<T>*> travStack;
        BSTNode<T> *p = root;
        while (p!=0)
        {
            // stack the right child (if any) and 
            // the node itself when gg left 
            while(p!=0)
            {
                if (p->right)
                {
                    travStack.push(p->right);
                }
                
                travStack.push(p);
                p = p->left;
            }

            // pop a node with no left child 
            // visit it and all nodes with no right child 
            p = travStack.pop();
            while (!travStack.empty() && p->right==0)
            {
                visit(p);
                p = travStack.pop();
            }

            // visit also the first node with a right child (if any)
            visit(p);
            if (!travStack.empty())
            {
                p = travStack.pop();
            }
            else 
            {
                p = 0;
            }
        }
    }

    void iterativePostorder()
    {
        Stack<BSTNode<T>*> travStack;
        BSTNode<T> *p = root, *q = root;
        while (p!=0)
        {
            for (; p->left!=0; p=p->left)
            {
                travStack.push(p);
            }

            while (p->right==0 || p->right==q)
            {
                visit(p);
                q = p;
                if (travStack.empty())
                {
                    return;
                }
                p = travStack.pop();
            }

            travStack.push(p);
            p = p->right;
        }
    }

    void MorrisInorder()
    {
        BSTNode<T> *p = root, *temp;
        while (p!=0)
        {
            if (p->left == 0)
            {
                visit(p);
                p = p->right;
            }

            else 
            {
                /*
                go to the rightmost node of the left
                subtree of to the temporary parent of p
                */
               temp = p->left;
               while (temp->right != 0 && temp->right != p)
               {
                    temp = temp->right;
               }
                /*
                if 'true', rightmost node was reached, make it a 
                temporary parent of the current root 
                */
               if (temp->right == 0)
               {
                    temp->right = p;
                    p = p->left;
               }
               /*
               else, a temporary parent has been found
               visit node p and then cut the right pointer 
               of the current parent, whereby it ceases to 
               be a parent 
               */
                else 
                {
                    visit(p);
                    temp->right = 0;
                    p = p->right;
                }
            }
        }
    }

    void insert(const T& el)
    {
        BSTNode<T> *p = root, *prev = 0;
        while (p!=0)
        {
            // find a place for inserting new node 
            // (find the parent of the node inserted)
            prev = p;   // prev will be the parent 
            if (el < p->el)
            {
                p = p->left;
            }
            else 
            {
                p = p->right;
            }
        }

        if (root == 0)  // if tree is empty
        {
            root = new BSTNode<T>(el);
        }
        else if (el < prev->el)
        {
            prev->left = new BSTNode<T>(el);
        }
        else 
        {
            prev->right = new BSTNode<T>(el);
        }
    }

    void deleteByMerging(BSTNode<T>*& node)
    {
        BSTNode<T> *temp = node;
        if (node!=0)
        {
            if (node->right == 0)
            {
                node = node->left;
            }
            else if (node->left == 0)
            {
                node = node->right;
            }
            else 
            {
                // find rmn of left subtree 
                temp = node->left;
                while (temp->right != 0)
                {
                    temp = temp->right;
                }
                // rmn now the parent of right subtree
                temp->right = node->right;
                temp = node;
                node = node->left;
            }

            delete temp;
        }
    }

    void findAndDeleteByMerging(const T& el)
    {
        BSTNode<T> *node = root, *prev = 0;
        while (node!=0)
        {
            if (node->el == el)
            {
                break;
            }
            prev = node;
            if (el < node->el)
            {
                node = node->left;
            }
            else 
            {
                node = node->right;
            }
        }

        if (node != 0 && node->el == el)
        {
            if (node==root)
            {
                deleteByMerging(root);
            }
            else if (prev->left == node)
            {
                deleteByMerging(prev->left);
            }
            else 
            {
                deleteByMerging(prev->right);
            }
        }
        else if (root!=0)
        {
            cout << "Element " << el << " is not in the tree\n";
        }
        else 
        {
            cout << "The tree is empty\n";
        }
    }

    void deleteByCopying(BSTNode<T> *& node)
    {
        BSTNode<T> *prev, *temp = node;
        if (node->right == 0)
        {
            node = node->left;
        }
        else if (node->left == 0)
        {
            node = node->right;
        }
        else 
        {
            temp = node->left;
            prev = node;
            while (temp->right != 0)
            {
                prev = temp;
                temp = temp->right;
            }
            // overwrite with predecessor 
            // predecessor cos rmn of left subtree
            node->el = temp->el;
            // if no temp = temp->right
            // means first node->left
            if (prev == node)
            {
                // just filling in the gaps;
                prev->left = temp->left;
            }
            else 
            {
                // prev->right fill in the gap with temp->left
                // cos temp overwrite node alr 
                // there is no temp->right
                // cos at that time temp is rightmost node alr 
                prev->right = temp->left;
            }
        }

        delete temp;
    }

    void findAndDeleteByCopying(const T& el)
    {
        BSTNode<T> *node = root, *prev = 0;
        while (node!=0)
        {
            if (node->el == el)
            {
                break;
            }
            prev = node;
            if (el < node->el)
            {
                node = node->left;
            }
            else 
            {
                node = node->right;
            }
        }

        if (node != 0 && node->el == el)
        {
            if (node==root)
            {
                deleteByCopying(root);
            }
            else if (prev->left == node)
            {
                deleteByCopying(prev->left);
            }
            else 
            {
                deleteByCopying(prev->right);
            }
        }
        else if (root!=0)
        {
            cout << "Element " << el << " is not in the tree\n";
        }
        else 
        {
            cout << "The tree is empty\n";
        }
    }



    void balance(T data[], int first, int last)
    {
        if (first<=last)
        {
            int middle = (first+last)/2;
            insert(data[middle]);
            balance(data, first, middle-1);
            balance(data, middle+1, last);
        }
    }

    protected:
    BSTNode<T>* root;
    void clear(BSTNode<T>*);

    T* search(BSTNode<T>* p, const T& el) const
    {
        while (p!=0)
        {
            if (el == p->el)
            {
                return &p->el;
            }
            else if (el < p->el)
            {
                p = p->left;
            }
            else 
            {
                p = p->right;
            }
        }
        return(0);
    }

    void preorder(BSTNode<T>* p)
    {
        if (p!=0)
        {
            visit(p);
            preorder(p->left);
            preorder(p->right);
        }
    }

    void inorder(BSTNode<T>* p)
    {
        if (p!=0)
        {
            inorder(p->left);
            visit(p);
            inorder(p->right);
        }
    }

    void postorder(BSTNode<T>* p)
    {
        if (p!=0)
        {
            postorder(p->left);
            postorder(p->right);
            visit(p);
        }
    }

    virtual void visit(BSTNode<T>* p)
    {
        cout << p->el << ' ';
    }
};

# endif


