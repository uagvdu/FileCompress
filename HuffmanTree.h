#define _CRT_SECURE_NO_WARNINGS
#pragma once 
#include"HeapSort.h"


template<class T>
struct HuffmanNode
{
	HuffmanNode *_left;
	HuffmanNode *_right;
	HuffmanNode *_parent;
	T _data;
	HuffmanNode(T data)
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _data(data)
	{}

	

};



template<class T>
struct LessHuf
{
	bool operator()(HuffmanNode<T> *a, HuffmanNode<T> *b)
	{
		return a->_data < b->_data ? 1 : 0;
	}
};


template<class T>
class HuffmanTree
{
	typedef HuffmanNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}

	HuffmanTree(T* a,size_t size,const T& invalue)  //invalue :��ֹ_count = 0�����ݽ���ѣ�����huffman��
	{
		Heap<Node*, LessHuf<T>> minheap;  //�����������ָ��
		for (int i = 0; i < size ; i++)
		{
			if (a[i] != invalue)
			{
				Node* cur = new Node(a[i]);
				minheap.Push(cur);
			}
			
		}

		while (minheap.Size()>1)  //ɾ������������һ�����ܻ����ֻʣһ���ڵ�����
		{
			Node* left = minheap.Top();
			minheap.Pop();
			Node* right = minheap.Top();
			minheap.Pop();
			Node* parent = new Node (left->_data + right->_data);

			left->_parent = parent;
			right->_parent = parent;
			parent->_left = left;
			parent->_right = right;
			minheap.Push(parent);
			
		}
		_root = minheap.Top();
	}

	Node* Getroot()
	{
		return _root;
	}
protected:
	Node* _root;
};

void HuffmanTest()
{

	int b[] = { 0, 1, 2, 4, 5, 6, 7, 8, 9 };
	HuffmanTree<int> hf(b, sizeof(b) / sizeof(b[0]),77);

}