#pragma once

#include <type_traits>


template <typename NumericType>
class SparseMatrix;

template<typename T>
class MatrixNode
{
	public:
		MatrixNode(T val, int row, int col, MatrixNode<T>* up, MatrixNode<T>* down,
			MatrixNode<T>* left, MatrixNode<T>* right);
		~MatrixNode();
		// allow SparseMatrix to manipulate internals of objects of this class
		friend class SparseMatrix<T>;
	private:
		// data value stored in node
		T mVal;
		// mRow and mCol allow node to be aware of its own position
		int mRow;
		int mCol;
		// each node will be connected all adjacent nodes through these pointers
		MatrixNode<T>* mUp,* mDown,* mLeft,* mRight;
};

// use provided values to set internal data appropriately
template <typename T>
MatrixNode<T>::MatrixNode(T val, int row, int col, MatrixNode<T>* up, MatrixNode<T>* down,
	MatrixNode<T>* left, MatrixNode<T>* right) : mVal(val), mRow(row), mCol(col),
	mUp(up), mDown(down), mLeft(left), mRight(right)
{ }

// node pointers are nullptr by default
template <typename T>
MatrixNode<T>::~MatrixNode()
{
	mUp = mDown = mLeft = mRight = nullptr;
}

