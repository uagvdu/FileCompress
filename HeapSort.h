#pragma once 
#include"iostream"
using namespace std;
#include<vector>
#include<assert.h>


template <class T>
struct Less
{
	bool operator()(T a, T b)
	{
		return a < b ? 1 : 0;
	}
};



template <class T>
struct Big
{
	bool operator()(T a,T b)
	{
		return a>b ? 1 : 0;
	}
};

template<class T,class Cmp = Big<T>>
class Heap 
{
public:
	Heap ()
	{}
	Heap (T *a,size_t sz)
	{
		for (size_t i = 0; i < sz; i++)
		{
			_v.push_back(a[i]);
		}
		for (int i = (_v.size() - 2) / 2; i >= 0; i--)
		{
			_AdjustDown(i,_v.size());
		}

	}

	void HeapSort()  //‘ˆ–Ú”√¥Û∂—
	{
		int size = _v.size() ;
		while (size)
		{
			swap(_v[0], _v[size-1]);
			size--;
			_AdjustDown(0,size);
		}

	}

	void  Display()
	{
		for (int i = 0; i < _v.size(); i++)
		{
			cout << _v[i] << " ";
		}
	}

	void Push(const T& x)
	{
		_v.push_back(x);
		_Adjustup(_v.size()-1);

	}

	T Top()
	{
		return _v[0];
	}

	void Pop()
	{
		assert(_v.size());
		swap(_v[0],_v[_v.size()-1]);
		_v.pop_back();
		_AdjustDown(0,_v.size());
	}

	int Size()
	{
		return _v.size();
	}
protected:

	void _Adjustup(int child)
	{
		int  parent =(child - 1) / 2;
		while (child > 0 )
		{
			
			if (Cmp()(_v[child],_v[parent]) )
			{
				swap(_v[child], _v[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
				break;
		}
	}

	void _AdjustDown(int parent,int size)
	{
		int child = parent * 2 + 1;
		while (child < size)
		{
			if (child + 1 < size && Cmp()(_v[child + 1], _v[child]))
			{
				child = child + 1;
			}
		    if (Cmp()(_v[child],_v[parent]))
			{
				swap(_v[child], _v[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
				break;
		}

	}
protected:
	vector<T> _v;
};

void HeapSortTest()
{
	int a[] = { 2, 1, 4, 5, 0, 6, 3, 7, 8, 9 };
 	Heap <int, Big<int> >  hp(a, sizeof(a) / sizeof(a[0]));
	
	hp.Push(20);

	hp.Pop();
	
	//hp.HeapSort();
	hp.Display();
	cout << endl;
	hp.Pop();
	hp.Display();
}