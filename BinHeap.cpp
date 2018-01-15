#include<iostream>
#include<climits>
#include <fstream>
#include <string>
#include <cstring>
#include <limits>
#include <chrono>

using namespace std;

ifstream f1;
ofstream f2;

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

class MinHeap
{
    int *arr;
    int max_size;
    int heap_size;

public:

    MinHeap(int capacity);

    int parent(int i)
    {
        return (i-1)/2;
    }
    int left(int i)
    {
        return (2*i + 1);
    }
    int right(int i)
    {
        return (2*i + 2);
    }
    int getMin()
    {
        f2 << arr[0];
        return arr[0];
    }

    void MinHeapify(int i);
    void insert(int k);
    void decrease(int i, int new_val);
    int extract();
    void deleteKey(int i);
    void print();
};

MinHeap::MinHeap(int capacity)
{
    heap_size = 0;
    max_size = capacity;
    arr = new int[max_size];
}

void MinHeap::insert(int k)
{
    if (heap_size == max_size)
    {
        f2 << "overflow" << endl;
        return;
    }
    heap_size++;
    int i = heap_size - 1;
    arr[i] = k;
    while (i != 0 && arr[parent(i)] > arr[i])
    {
       swap(&arr[i], &arr[parent(i)]);
       i = parent(i);
    }
}

void MinHeap::decrease(int i, int new_val)
{
    if (i < 0 || i >= heap_size)
    {
        f2 << "not found" << endl;
        return;
    }
    if (arr[i] < new_val)
    {
        f2 << "error" << endl;
        return;
    }
    arr[i] = new_val;
    while (i != 0 && arr[parent(i)] > arr[i])
    {
       swap(&arr[i], &arr[parent(i)]);
       i = parent(i);
    }
}

int MinHeap::extract()
{
    if (heap_size <= 0)
    {
        f2 << "not found" << endl;
    }
    else if (heap_size == 1)
    {
        heap_size--;
        return arr[0];
    }
    else
    {
        int root = arr[0];
        arr[0] = arr[heap_size-1];
        heap_size--;
        MinHeapify(0);
        return root;
    }
}

void MinHeap::deleteKey(int i)
{
    decrease(i, INT_MIN);
    extract();
}

void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && arr[l] < arr[i])
        smallest = l;
    if (r < heap_size && arr[r] < arr[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(&arr[i], &arr[smallest]);
        MinHeapify(smallest);
    }
}

void MinHeap::print()
{
    if (heap_size == 0)
    {
        f2 << "empty" << endl;
        return;
    }
    for (int i=0; i<heap_size; i++) {
        f2 << arr[i] << " ";
    }
    f2 << endl;
}

class CurrentTime {
    std::chrono::high_resolution_clock m_clock;

public:
    uint64_t microseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>
        (m_clock.now().time_since_epoch()).count();
    }
};

int main()
{


/*
    int max_size = 100000;
    MinHeap h(max_size);

    for (int i=max_size; i>0; i--)
    {
        h.insert(i);
    }

    CurrentTime ct;
    auto start = ct.microseconds();


    for (int i=0; i<max_size; i++)
    {
        h.extract();
    }


    auto end = ct.microseconds();

    std::cout << "-- " << (end - start) << " microseconds." << std::endl;

*/

/*
    f1.open(argv[1]);
	f2.open(argv[2]);

    int max_size = 30;
    MinHeap h(max_size);

    string str;

    while (f1 >> str && !f1.eof()) {
        if (str == "insert") {
            int k;
            f1 >> k;
            h.insert(k);
        }
        else if (str == "print") {
            h.print();
        }
        else if (str == "extract") {
            h.extract();
        }
        else if (str == "decrease") {
            int i, k;
            f1 >> i >> k;
            h.decrease(i, k);
        }
        else if (str == "delete") {
            int i;
            f1 >> i;
            h.deleteKey(i);
        }
        else if (str == "getMin") {
            h.getMin();
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
