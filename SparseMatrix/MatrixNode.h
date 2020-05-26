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
		//MatrixNode(const MatrixNode<T>& other);

		friend class SparseMatrix<T>;
	private:
		T mVal;
		int mRow;
		int mCol;
		MatrixNode<T>* mUp,* mDown,* mLeft,* mRight;
};


template <typename T>
MatrixNode<T>::MatrixNode(T val, int row, int col, MatrixNode<T>* up, MatrixNode<T>* down,
	MatrixNode<T>* left, MatrixNode<T>* right) : mVal(val), mRow(row), mCol(col),
	mUp(up), mDown(down), mLeft(left), mRight(right)
{ }

template <typename T>
MatrixNode<T>::~MatrixNode()
{
	mUp = mDown = mLeft = mRight = nullptr;
}

/*template <typename T>
MatrixNode<T>::MatrixNode(const MatrixNode<T>& other) : mVal(other.mVal), mRow(other.mRow), mCol(other.mCol),
	mUp(new MatrixNode<T>())
{
	
}*/
