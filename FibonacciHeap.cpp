#include <stdlib.h>
#include <queue>
#include <cstddef>
#include <math.h>
#include <limits>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

ifstream f1;
ofstream f2;

template<class T>
class FibHeap
{
public:


class FibNode {
public:
    FibNode(T k, void *pl):key(k),mark(false),p(nullptr),left(nullptr),right(nullptr),child(nullptr),degree(-1),payload(pl) {}
    ~FibNode() {}
    T key;
    bool mark;
    FibNode *p;
    FibNode *left;
    FibNode *right;
    FibNode *child;
    int degree;
    void *payload;
};


FibHeap():n(0),min(nullptr) {}


void insert(FibNode *x)
{
    x->degree = 0;
    x->p = nullptr;
    x->child = nullptr;
    x->mark = false;
    if (min == nullptr)
    {
        min = x->left = x->right = x;
    }
    else
    {
        min->left->right = x;
        x->left = min->left;
        min->left = x;
        x->right = min;
        if ( x->key < min->key )
        {
            min = x;
        }
    }
    ++n;
}


FibNode* minimum()
{
   return min;
}


void print(FibNode* current = nullptr, FibNode* base = nullptr, int level = 0) const noexcept
{
        if (min == nullptr)
        {
            f2 << "empty" << endl;
            return;
        }
        if (current == nullptr){
            current = min;
            base = min;
        }

        f2 << current->key << " ";

        if (current->child != nullptr)
            print(current->child, current->child, level+1);
        if (current->right != base)
            print(current->right, base, level);
}


FibNode* find(T k, FibNode* current = nullptr, FibNode* base = nullptr, int level = 0)
{
    if (min == nullptr)
    {
        f2 << "empty" << endl;
    }
    else
    {
        if (current == nullptr)
        {
            current = min;
            base = min;
        }
        if (current->key == k)
        {
            return current;
        }

        if (current->child != nullptr)
            find(k,current->child, current->child, level+1);
        if (current->right != base)
            find(k,current->right, base, level);
    }

}


static FibHeap* union_fibheap(FibHeap *H1, FibHeap *H2)
{
    FibHeap* H = new FibHeap();
    H->min = H1->min;
    if ( H->min != nullptr && H2->min != nullptr )
    {
        H->min->right->left = H2->min->left;
        H2->min->left->right = H->min->right;
        H->min->right = H2->min;
        H2->min->left = H->min;
    }
    if ( H1->min == nullptr || ( H2->min != nullptr && H2->min->key < H1->min->key ) )
    {
        H->min = H2->min;
    }
    H->n = H1->n + H2->n;
    return H;
}


FibNode* extract_min()
{
    FibNode *z, *x, *next;
    FibNode ** childList;

    z = min;
    if (z != nullptr)
    {
        x = z->child;
        if (x != nullptr)
        {
            childList = new FibNode*[z->degree];
            next = x;
            for (int i = 0; i < (int)z->degree; i++)
            {
                childList[i] = next;
                next = next->right;
            }
            for (int i = 0; i < (int)z->degree; i++)
            {
                x = childList[i];
                min->left->right = x;
                x->left = min->left;
                min->left = x;
                x->right = min;
                x->p = nullptr;
            }
            delete [] childList;
        }
        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
        {
            min = nullptr;
        }
        else
        {
            min = z->right;
            consolidate();
        }
        n--;
    }
    return z;
}


void consolidate()
{
    FibNode* w, * next, * x, * y, * temp;
    FibNode** A, ** rootList;

    int d, rootSize;
    int max_degree = static_cast<int>(floor(log(static_cast<double>(n))/log(static_cast<double>(1 + sqrt(static_cast<double>(5)))/2)));

    A = new FibNode*[max_degree+2];
    std::fill_n(A, max_degree+2, nullptr);
    w = min;
    rootSize = 0;
    next = w;
    do
    {
        rootSize++;
        next = next->right;
    } while ( next != w );

    rootList = new FibNode*[rootSize];
    for ( int i = 0; i < rootSize; i++ )
    {
        rootList[i] = next;
        next = next->right;
    }
    for ( int i = 0; i < rootSize; i++ )
    {
        w = rootList[i];
        x = w;
        d = x->degree;
        while ( A[d] != nullptr )
        {
            y = A[d];
            if ( x->key > y->key )
            {
                temp = x;
                x = y;
                y = temp;
            }
            fib_heap_link(y,x);
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }
    delete [] rootList;
    min = nullptr;
    for ( int i = 0; i < max_degree+2; i++ )
    {
        if ( A[i] != nullptr )
        {
            if ( min == nullptr )
            {
                min = A[i]->left = A[i]->right = A[i];
            }
            else
            {
                min->left->right = A[i];
                A[i]->left = min->left;
                min->left = A[i];
                A[i]->right = min;
                if ( A[i]->key < min->key )
                {
                    min = A[i];
                }
            }
        }
    }
    delete [] A;
}


void fib_heap_link(FibNode* y, FibNode* x)
{
    y->left->right = y->right;
    y->right->left = y->left;
    if ( x->child != nullptr )
    {
        x->child->left->right = y;
        y->left = x->child->left;
        x->child->left = y;
        y->right = x->child;
    }
    else
    {
        x->child = y;
        y->right = y;
        y->left = y;
    }
    y->p = x;
    x->degree++;
    y->mark = false;
}


void decrease_key(FibNode* x, int k)
{
    FibNode* y;
    if ( k > x->key )
    {
        return;
    }
    x->key = k;
    y = x->p;
    if ( y != nullptr && x->key < y->key )
    {
        cut(x,y);
        cascading_cut(y);
    }
    if ( x->key < min->key )
    {
        min = x;
    }
}


void cut(FibNode* x, FibNode* y)
{
    if ( x->right == x )
    {
        y->child = nullptr;
    }
    else
    {
        x->right->left = x->left;
        x->left->right = x->right;
        if ( y->child == x )
        {
            y->child = x->right;
        }
    }
    y->degree--;
    min->right->left = x;
    x->right = min->right;
    min->right = x;
    x->left = min;
    x->p = nullptr;
    x->mark = false;
}


void cascading_cut(FibNode* y)
{
    FibNode* z;
    z = y->p;
    if ( z != nullptr )
    {
        if ( y->mark == false )
        {
            y->mark = true;
        }
        else
        {
            cut(y,z);
            cascading_cut(z);
        }
    }
}

void remove_fibnode(FibNode* x)
{
    decrease_key(x,std::numeric_limits<T>::min());
    FibNode *fn = extract_min();
    delete fn;
}


FibNode* topNode()
{
    return minimum();
}

T top()
{
   return minimum()->key;
}

void pop()
{
    if (n == 0)
    {
      f2 << "empty" << endl;
      return;
    }
    FibNode *x = extract_min();
    if (x)
      delete x;

}

FibNode* push(T k, void *pl)
{
    FibNode *x = new FibNode(k,pl);
    insert(x);
    return x;
}

FibNode* push(T k)
{
    return push(k,nullptr);
}


int n;
FibNode* min;


};


class CurrentTime {
    std::chrono::high_resolution_clock m_clock;

public:
    uint64_t microseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>
        (m_clock.now().time_since_epoch()).count();
    }
};


int main(int argc, char *argv[])
{




/*
    int max_size = 100000;
    FibHeap<int> fh;

    for (int i=0; i<max_size; i++)
    {
        fh.push(i);
    }

    CurrentTime ct;

    auto start = ct.microseconds();


    for (int i=0; i<max_size; i++)
    {
        fh.pop();
    }

    auto end = ct.microseconds();

    std::cout << "-- " << (end - start) << " microseconds." << std::endl;

*/



/*

  f1.open(argv[1]);
  f2.open(argv[2]);

  FibHeap<int> fh;

  string str;

  while (f1 >> str && !f1.eof())
    {
        if (str == "insert") {
            int k;
            f1 >> k;
            fh.push(k);
        }
        else if (str == "print") {
            fh.print();
            f2 << endl;
        }
        else if (str == "extract") {
            fh.pop();
        }
        else if (str == "decrease") {
            int v, k;
            f1 >> v >> k;
            fh.decrease_key(fh.find(v), k);
        }
        else if (str == "delete") {
            int v;
            f1 >> v;
            fh.remove_fibnode(fh.find(v));
        }
        else if (str == "getMin")
        {
            f2 << fh.top() << endl;
        }
        else if (str == "union")
        {


        }
        else {
            f2 << "error!\n";
            f1.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

  f1.close();
  f2.close();

*/

  return 0;

}
