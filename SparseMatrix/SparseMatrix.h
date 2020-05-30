#pragma once

#include "HeaderNode.h"
#include "MatrixNode.h"
#include <exception>
#include <utility>
#include <stdexcept>
#include <iostream>

template <typename NumericType>
class SparseMatrix
{
	public:
		SparseMatrix(int rows, int cols);		//ctor
		~SparseMatrix();						//destructor
		SparseMatrix(const SparseMatrix<NumericType>& other);			//copy ctor
		SparseMatrix<NumericType>& operator=(const SparseMatrix<NumericType>& other);		//copy assignment
		SparseMatrix(SparseMatrix<NumericType>&& other) noexcept;		//move ctor
		SparseMatrix<NumericType>& operator=(SparseMatrix<NumericType>&& other) noexcept;	//move assignment

		void insert(int row, int col, NumericType value);
		void remove(int row, int col);
		NumericType getVal(int row, int col) const;
		void transpose();
		void print() const;

		SparseMatrix<NumericType> operator+(const SparseMatrix<NumericType>& second);
		SparseMatrix<NumericType> operator*(const SparseMatrix<NumericType>& second);
	private:
		static_assert(std::is_arithmetic<NumericType>::value, "Data value must be numeric.");
		int numRows, numCols;
		HeaderNode<NumericType>* mRows = nullptr,* mCols = nullptr;
		HeaderNode<NumericType>* findHeader(HeaderNode<NumericType>* head, int index) const;
		MatrixNode<NumericType>* findInRow(MatrixNode<NumericType>* head, int col) const;
		MatrixNode<NumericType>* findInCol(MatrixNode<NumericType>* head, int row) const;
		void cleanup(SparseMatrix<NumericType>* garbage);
};



// Implementation is based on the orthogonal list sparse matrix presented in "Data Structures & Algorithm Analysis
// in C++" (3rd Edition) by Clifford A. Shaffer. Implementation is original and only based on theory presented
// in the book.


template <typename NumericType>
SparseMatrix<NumericType>::SparseMatrix(int rows, int cols) : numRows(rows), numCols(cols) , mRows(nullptr), mCols(nullptr)
{
	//numRows = rows; numCols = cols; mRows = nullptr; mCols = nullptr;
}

template <typename NumericType>
SparseMatrix<NumericType>::~SparseMatrix()
{
	cleanup(this);
	
}

template <typename NumericType>
SparseMatrix<NumericType>::SparseMatrix(const SparseMatrix<NumericType>& other) : numRows(other.numRows), 
	numCols(other.numCols), mRows(nullptr), mCols(nullptr)
{
	std::cout << "Copy ctor called" << std::endl;

	HeaderNode<NumericType>* currHeader = other.mRows;
	MatrixNode<NumericType>* currMat;
	

	while (currHeader != nullptr)
	{
		currMat = currHeader->mNode;
		while (currMat != nullptr)
		{
			this->insert(currMat->mRow, currMat->mCol, currMat->mVal);
			currMat = currMat->mRight;
		}
		currHeader = currHeader->mNext;
	}
}

template <typename NumericType>
SparseMatrix<NumericType>& SparseMatrix<NumericType>::operator=(const SparseMatrix<NumericType>& other)
{
	std::cout << "Copy assignment called" << std::endl;
	if (&other == this)
		return *this;
	cleanup(this);
	this->numRows = other.numRows; this->numCols = other.numCols;
	this->mRows = this->mCols = nullptr;

	HeaderNode<NumericType>* currHeader = other.mRows;
	MatrixNode<NumericType>* currMat;

	while (currHeader != nullptr)
	{
		currMat = currHeader->mNode;
		while (currMat != nullptr)
		{
			this->insert(currMat->mRow, currMat->mCol, currMat->mVal);
			currMat = currMat->mRight;
		}
		currHeader = currHeader->mNext;
	}
	return *this;
}

template <typename NumericType>
SparseMatrix<NumericType>::SparseMatrix(SparseMatrix<NumericType>&& other) noexcept
{
	std::cout << "Move ctor called" << std::endl;
	std::swap(this->mRows, other.mRows);
	std::swap(this->mCols, other.mCols);
	std::swap(this->numCols, other.numCols);
	std::swap(this->numRows, other.numRows);
}

template <typename NumericType>
SparseMatrix<NumericType>& SparseMatrix<NumericType>::operator=(SparseMatrix<NumericType>&& other) noexcept
{
	std::cout << "Move assignment called" << std::endl;
	if (&other == this)
		return *this;

	std::swap(this->mRows, other.mRows);
	std::swap(this->mCols, other.mCols);
	std::swap(this->numCols, other.numCols);
	std::swap(this->numRows, other.numRows);
	return *this;
}

template <typename NumericType>
void SparseMatrix<NumericType>::insert(int row, int col, NumericType value)
//input:  row number, column number, and data value to store in the node - 
//		  rows and cols start from 0 and go to mRows-1 and mCols-1 respectively
//output: new node is inserted at appropriate row/col if no node already exists there,
//		  data value is overwritten if there is already a node at that location.
//		  exception thrown if out of bounds
{
	if (row >= numRows || row < 0 || col >= numCols || col < 0)		//check for invalid input
		throw std::out_of_range("Out of bounds of matrix - insertion.");
	if (value == 0)
		return;

	HeaderNode<NumericType>* header;
	MatrixNode<NumericType>* currMat;

	header = findHeader(mCols, col);

	if (header == nullptr)		//no non-zero values in matrix
	{
		mCols = new HeaderNode<NumericType>(col);
		mRows = new HeaderNode<NumericType>(row);
		mCols->mNode = mRows->mNode = new MatrixNode<NumericType>(value, row, col, nullptr, nullptr, nullptr, nullptr);
		return;
	}
	else if (header->mIndex < col)		//matching column not found - insert one after header
	{
		header = header->mNext = new HeaderNode<NumericType>(col, header->mNext, nullptr);
		currMat = header->mNode = new MatrixNode<NumericType>(value, row, col, nullptr, nullptr, nullptr, nullptr);
	}
	else if (header->mIndex > col)		//Header node needed as first header node
	{
		header = mCols = new HeaderNode<NumericType>(col, mCols, nullptr);
		currMat = mCols->mNode = new MatrixNode<NumericType>(value, row, col, nullptr, nullptr, nullptr, nullptr);
	}
	else								//matching column found - search nodes
	{
		currMat = findInCol(header->mNode, row);
		if (currMat->mRow == row)		//node already exists in this spot, replace value
		{
			currMat->mVal = value;
			return;
		}
		else if (currMat->mRow > row)	//node is new first node for this header
		{
			header->mNode = currMat = new MatrixNode<NumericType>(value, row, col, nullptr, header->mNode, nullptr, nullptr);
			currMat->mDown->mUp = currMat;
		}
		else							//currMat == node before where the node should go
			//up and down pointers are known based on currMat - initialize these pointers in ctor, change currMat's
			//down pointer to point to new node, and set currMat to the new node
			currMat = currMat->mDown = new MatrixNode<NumericType>(value, row, col, currMat, currMat->mDown, nullptr, nullptr);
	}

	//now find the appropriate row and place in that row so that left/right pointers can be adjusted
	header = findHeader(mRows, row);

	if (header->mIndex < row)
	//inserted node will be first node in this row, so its left and right pointers are already set (nullptr)
	{
		header->mNext = new HeaderNode<NumericType>(row, header->mNext, currMat);		
	}
	else if (header->mIndex > row)
		//need new header node as first header node
	{
		mRows = new HeaderNode<NumericType>(row, mRows, currMat);
	}
	else							//matching row is found - search for node preceding currMat
	{
		MatrixNode<NumericType>* rowProbe;
		rowProbe = findInRow(header->mNode, col);		//will either be less than or greater than col 
		if (rowProbe->mCol > col)			//new node will be new first node in this row
		{
			currMat->mRight = header->mNode;
			header->mNode->mLeft = currMat;
			header->mNode = currMat;
		}
		else
		{
			currMat->mLeft = rowProbe;			//reassign pointers using rowProbe as the base
			currMat->mRight = rowProbe->mRight;
			rowProbe->mRight = currMat;			//reassign rowProbe's right pointer to point to inserted node
		}
	}
}


template <typename NumericType>
void SparseMatrix<NumericType>::remove(int row, int col)
{
	if (row >= numRows || row < 0 || col >= numCols || col < 0)		//check for invalid input
		throw std::out_of_range("Out of bounds of matrix - removal.");


	HeaderNode<NumericType>* header1,* header2,* prev,* curr;
	MatrixNode<NumericType>* currMat;

	header1 = findHeader(mRows, row);

	if (header1 == nullptr || header1->mIndex != row)		//no such node in matrix
		return;

	currMat = findInRow(header1->mNode, col);

	if (currMat->mCol != col)							//no such node in matrix
		return;
	else												//got a match - have a node to delete
	{
		if (currMat->mLeft != nullptr)					//make sure any neighboring nodes have their pointers adjusted
			currMat->mLeft->mRight = currMat->mRight;
		if (currMat->mUp != nullptr)
			currMat->mUp->mDown = currMat->mDown;
		if (currMat->mRight != nullptr)
			currMat->mRight->mLeft = currMat->mLeft;
		if (currMat->mDown != nullptr)
			currMat->mDown->mUp = currMat->mUp;

		if (currMat->mLeft == nullptr && currMat->mRight != nullptr)
			header1->mNode = currMat->mRight;
		if (currMat->mUp == nullptr && currMat->mDown != nullptr)
		{
			header2 = findHeader(mCols, col);
			header2->mNode = currMat->mDown;
		}

		if (currMat->mLeft == nullptr && currMat->mRight == nullptr)	//only node in this row, delete header node too
		{
			if (mRows->mIndex == row)		//this row was first header node in mRows
			{
				mRows = header1->mNext;
				delete header1;
			}
			else							
			{
				prev = mRows; curr = mRows->mNext;			
				while (curr->mIndex != row)
				{
					prev = curr;
					curr = curr->mNext;
				}
				prev->mNext = curr->mNext;
				delete header1;
			}
		}

		if (currMat->mUp == nullptr && currMat->mDown == nullptr)		//only node in this column, delete header node too
		{		

			if (mCols->mIndex == col)		//this row was first header node in mCols
			{
				HeaderNode<NumericType>* temp = mCols;
				mCols = mCols->mNext;
				delete temp;
			}
			else
			{
				prev = mCols; curr = mCols->mNext;
				while (curr->mIndex != row)
				{
					prev = curr;
					curr = curr->mNext;
				}
				prev->mNext = curr->mNext;
				delete curr;
			}
		}

		currMat->mLeft = nullptr; currMat->mRight = nullptr; currMat->mUp = nullptr; currMat->mDown = nullptr;
		//reassign node to be deleted's pointers to nullptr in case there is ever a reason to implement the node's
		//destructor

		delete currMat;
	}
}

template <typename NumericType>
NumericType SparseMatrix<NumericType>::getVal(int row, int col) const
{
	if (row >= numRows || row < 0 || col >= numCols || col < 0)		//check for invalid input
		return -1;

	HeaderNode<NumericType>* header = findHeader(mCols, col);
	if (header == nullptr || header->mIndex != col)		//no non-zero values
		return 0;
	else
	{
		MatrixNode<NumericType>* matNode = findInCol(header->mNode, row);
		if (matNode->mRow == row)
			return matNode->mVal;
		else
			return 0;
	}
}

template <typename NumericType>
void SparseMatrix<NumericType>::transpose()
{
	std::swap(mCols, mRows);
	std::swap(numCols, numRows);

	HeaderNode<NumericType>* header = mRows;
	MatrixNode<NumericType>* forward,* prev;

	while (header != nullptr)				//ends when all row headers have been gone through
	{
		forward = prev = header->mNode;
		while (forward != nullptr)			//ends when end of new row is reached
		//loop to iterate through nodes and swap left/up pointers and down/right pointers (these pointer pairs perform
		//the same function as far as direction movement - up and left move "backwards" and right and down move "forward"
		{
			forward = forward->mDown;		//move currMat ahead, temp still points to the previous node - mDown points 
											//to the "right" node after transpose
			std::swap(prev->mUp, prev->mLeft);			//		ex: ( 5 4 3 ) T     ( 5 6 )
			std::swap(prev->mRight, prev->mDown);		//			( 6 7 8 ) =		( 4 7 )
			std::swap(prev->mRow, prev->mCol);			//							( 3 8 )


			prev = forward;					//move prev forward to the next node

		}
		header = header->mNext;
	}
}


template <typename NumericType>
void SparseMatrix<NumericType>::print() const
{
	for (int i = 0; i < numRows; ++i)
	{
		for (int j = 0; j < numCols; ++j)
			std::cout << getVal(i, j) << " ";
		std::cout << std::endl;
	}
}

template <typename NumericType>
SparseMatrix<NumericType> SparseMatrix<NumericType>::operator+(const SparseMatrix<NumericType>& second)
{
	if (this->numRows != second.numRows || this->numCols != second.numCols)		//check that matrices are same size
		throw std::invalid_argument("Sizes do not match.");

	SparseMatrix<NumericType> result(this->numRows, this->numCols);			//create matrix of same size to return
	NumericType sum;

	for (int i = 0; i < this->numRows; ++i)
		for (int j = 0; j < this->numCols; ++j)
		{
			sum = this->getVal(i, j) + second.getVal(i, j);
			if (sum != 0)
				result.insert(i, j, sum);
		}

	return result;

}

template <typename NumericType>
SparseMatrix<NumericType> SparseMatrix<NumericType>::operator*(const SparseMatrix<NumericType>& second)
{
	if (this->numCols != second.numRows)
		throw std::invalid_argument("First operand's number of columns must match second operand's number of rows.");

	SparseMatrix<NumericType> result(this->numRows, second.numCols);
	HeaderNode<NumericType>* header1 = this->mRows,* header2 = second.mCols;
	MatrixNode<NumericType>* currMat1,* currMat2;
	NumericType value;

	//values only need to be inserted into resulting matrix if a row exists at a certain level
	while (header1 != nullptr)		//go through all rows of the first matrix - if there is no header for a row
	{
		while (header2 != nullptr)
		{
			value = 0;
			currMat1 = header1->mNode, currMat2 = header2->mNode;
			while (currMat1 != nullptr && currMat2 != nullptr)
			{
				if (currMat1->mCol > currMat2->mRow)	//need to advance currMat2
					currMat2 = currMat2->mDown;
				else if (currMat1->mCol < currMat2->mRow)		//need to advance currMat1
					currMat1 = currMat1->mRight;
				else										//row and column match so get non-zero multiplication
				{
					value += currMat1->mVal * currMat2->mVal;
					currMat1 = currMat1->mRight;
					currMat2 = currMat2->mDown;
				}
			}
			if (value != 0)
				result.insert(header1->mIndex, header2->mIndex, value);
			header2 = header2->mNext;
		}
		header1 = header1->mNext;
		header2 = second.mCols;
	}
	return result;
}


template <typename NumericType>
HeaderNode<NumericType>* SparseMatrix<NumericType>::findHeader(HeaderNode<NumericType>* head, int index) const
//helper function that returns either the Header node whose index matches the "index" parameter or the node just
//before where it should be or nullptr if the end of the list is reached
//will check return value to see if == nullptr, retVal->mIndex < index, or retVal->mIndex == index
//retVal will have mIndex greater than index if head points to a header with higher index (first header is greater)
{
	if (head == nullptr)  //empty list 
		return nullptr;
	if (head->mIndex == index)
		return head;

	HeaderNode<NumericType>* prev = head, * curr = head->mNext;

	while (curr != nullptr && curr->mIndex < index)
		//iterate through the header nodes until a match or the first node with a greater index is found
		//curr will point to one of these nodes
	{
		prev = curr;
		curr = curr->mNext;
	}

	if (curr != nullptr && curr->mIndex == index)
		return curr;
	else
		return prev;
}

template <typename NumericType>
MatrixNode<NumericType>* SparseMatrix<NumericType>::findInRow(MatrixNode<NumericType>* head, int col) const
//helper function that will return the node before where the desired node should be if no match is found in the given row
{
	MatrixNode<NumericType>* prev = head,* curr = prev->mRight;

	while (curr != nullptr && curr->mCol < col)
	{
		prev = curr;
		curr = curr->mRight;
	}
	if (curr != nullptr && curr->mCol == col)
		return curr;
	else
		return prev;
}

template <typename NumericType>
MatrixNode<NumericType>* SparseMatrix<NumericType>::findInCol(MatrixNode<NumericType>* head, int row) const
//helper function that will return the node before where the desired node should be if no match is found in the given column

{
	MatrixNode<NumericType>* prev = head,* curr = prev->mDown;

	while (curr != nullptr && curr->mRow < row)
	{
		prev = curr;
		curr = curr->mDown;
	}
	if (curr != nullptr && curr->mRow == row)
		return curr;
	else
		return prev;
}

template <typename NumericType>
void SparseMatrix<NumericType>::cleanup(SparseMatrix<NumericType>* garbage)
{
	HeaderNode<NumericType>* prevHeader = garbage->mRows, * currHeader = garbage->mRows;
	MatrixNode<NumericType>* prevMat, * currMat;

	while (currHeader != nullptr)
		//loop to delete all matrix nodes and the row headers
	{
		prevMat = currMat = currHeader->mNode;
		while (currMat != nullptr)
		{
			currMat = currMat->mRight;
			delete prevMat;
			prevMat = currMat;
		}
		currHeader = currHeader->mNext;
		delete prevHeader;
		prevHeader = currHeader;
	}
	prevHeader = currHeader = garbage->mCols;
	while (currHeader != nullptr)
		//loop to delete the column headers
	{
		currHeader = currHeader->mNext;
		delete prevHeader;
		prevHeader = currHeader;
	}
}
