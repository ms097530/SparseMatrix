#pragma once

#include "MatrixNode.h"

template <typename NumericType>
class SparseMatrix;

template <typename T>
class HeaderNode
{
	public:
		HeaderNode(int num);
		HeaderNode(int num, HeaderNode<T>* next, MatrixNode<T>* node);
		~HeaderNode();
		friend class SparseMatrix<T>;
	private:
		int mIndex;
		HeaderNode<T>* mNext;
		MatrixNode<T>* mNode;
};


template <typename T>
HeaderNode<T>::HeaderNode(int num) : mIndex(num), mNext(nullptr), mNode(nullptr) { }

template <typename T>
HeaderNode<T>::HeaderNode(int num, HeaderNode<T>* next, MatrixNode<T>* node) : mIndex(num), mNext(next), mNode(node) { }

template <typename T>
HeaderNode<T>::~HeaderNode()
{
	mNext = nullptr;
	mNode = nullptr;
}
