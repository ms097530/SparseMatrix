/*#include "SparseMatrix.h"

// Implementation is based on the orthogonal list sparse matrix presented in "Data Structures & Algorithm Analysis
// in C++" (3rd Edition) by Clifford A. Shaffer. Implementation is original and only based on theory presented
// in the book.


template <typename NumericType>
SparseMatrix<NumericType>::SparseMatrix(int rows, int cols) //: numRows(rows), numCols(cols) 
{
	numRows = rows; numCols = cols; mRows = nullptr; mCols = nullptr;
}

template <typename NumericType>
SparseMatrix<NumericType>::~SparseMatrix()
{
	HeaderNode<NumericType>* prevHeader = mRows, currHeader = mRows;
	MatrixNode<NumericType>* prevMat, currMat;

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
	prevHeader = currHeader = mCols;
	while (currHeader != nullptr)
	//loop to delete the column headers
	{
		currHeader = currHeader->mNext;
		delete prevHeader;
		prevHeader = currHeader;
	}
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
	else if (header->mIndex < col)		//matching column not found - insert one
	{
		header = header->mNext = new HeaderNode<NumericType>(col, header->mNext, nullptr);
		currMat = header->mNode = new MatrixNode<NumericType>(value, row, col, nullptr, nullptr, nullptr, nullptr);
	}
	else								//matching column found - search nodes
	{
		currMat = findInCol(header->mNode, row);
		if (currMat->mRow == row)		//node already exists in this spot, replace value
		{
			currMat->mVal = value;
			return;
		}
		else							//currMat == node before where the node should go
			//up and down pointers are known based on currMat - initialize these pointers in ctor, change currMat's
			//down pointer to point to new node, and set currMat to the new node
			currMat = currMat->mDown = new MatrixNode(value, row, col, currMat, currMat->mDown, nullptr, nullptr);
	}

	//now find the appropriate row and place in that row so that left/right pointers can be adjusted
	header = findHeader(mRows, row);

	if (header->mIndex < row)		
	//inserted node will be first node in this row, so its left and right pointers are already set (nullptr)
	{
		header = header->mNext = new HeaderNode<NumericType>(row, header->mNext, currMat);
		return;
	}
	else							//matching row is found - search for node preceding currMat
	{
		MatrixNode<NumericType>* rowProbe;
		rowProbe = findInRow(header->mNode, col);
		currMat->mLeft = rowProbe;			//reassign pointers using rowProbe as the base
		currMat->mRight = rowProbe->mRight;
		rowProbe->mRight = currMat;			//reassign rowProbe's right pointer to point to inserted node
	}

	
	
	/*if (mCols == nullptr)	//no non-zero values (insert first non-zero node)
	{
		mCols = new HeaderNode<NumericType>(col, nullptr, nullptr);
		mRows = new HeaderNode<NumericType>(row, nullptr, nullptr);
		mCols->mNode = new MatrixNode < NumericType(value, row, col, nullptr, nullptr, nullptr, nullptr);
		mRows->mNode = mCols->mNode;
	}

	curr = mCols->mNode;
	prev = mCols;

	while (curr != nullptr && curr->mIndex < col)
	//loop to advance through the headers until finding the desired column or the spot where the column should be
	{
		prev = curr;
		curr = curr->mNext;
	}

	MatrixNode<NumericType>* currMat, prevMat;

	if (curr == nullptr || curr->mIndex > col)	//no column exists for the desired column or need to add column 
												//between header nodes
	{
		prev->mNext = new HeaderNode<NumericType>(col, curr, nullptr);

		//add matrix node to new HeaderNode as the first node in the column - left and right pointers may change
		currMat = prev->mNext->mNode = new MatrixNode<NumericType>(value, row, col, nullptr, nullptr, nullptr, nullptr);
		
	}
	else										//curr->mIndex == col - found correct column
	{
		prevMat = curr->mNode;
		currMat = prevMat->mDown;

		if (row < prevMat->mRow)		//insert as first node
		{
			currMat = curr->mNode = new MatrixNode(value, row, col, nullptr, curr->mNode, nullptr, nullptr);
		}
		else
		{
			while (currMat != nullptr && currMat->mRow < row)
			{
				prevMat = currMat;
				currMat = currMat->mDown;
			}
			currMat = prevMat->mDown = new MatrixNode<NumericType>(value, row, col, prevMat, currMat, nullptr, nullptr);
		}	
	}

	//Now currMat points to the node we inserted, and the left and right pointers may need adjusted.
	//So, we must now traverse down the row headers and adjust the currMat's left and right pointers,
	//as well as neighboring pointers.    
	
}


template <typename NumericType>
void SparseMatrix<NumericType>::remove(int row, int col)
{
	if (row >= numRows || row < 0 || col >= numCols || col < 0)		//check for invalid input
		throw std::out_of_range("Out of bounds of matrix - removal.");


	HeaderNode<NumericType>* header, prev, curr;
	MatrixNode<NumericType>* prevMat, currMat;

	header = findHeader(mRows, row);

	if (header == nullptr || header->mIndex != row)		//no such node in matrix
		return;

	currMat = findInRow(header->mNode, col);

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

		if (currMat->mLeft == nullptr && currMat->mRight == nullptr)	//only node in this row, delete header node too
		{
			if (mRows->mIndex == row)		//this row was first header node in mRows
			{
				mRows = header->mNext;
				delete header;
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
				delete header;
			}
		}

		if (currMat->mUp == nullptr && currMat->mDown == nullptr)		//only node in this column, delete header node too
		{
			//header = findHeader(mCols, col);

			if (mCols->mIndex == col)		//this row was first header node in mCols
			{
				HeaderNode<NumericType> temp = mCols;
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
		
		currMat->mLeft = nullptr, currMat->mRight = nullptr, currMat->mUp = nullptr, currMat->mDown = nullptr;
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
		MatrixNode<NumericType> matNode = findInCol(header->mNode, row);
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
	MatrixNode<NumericType>* forward, prev;

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
	for (int i = 0; i < mRows; ++i)
	{
		for (int j = 0; j < mCols; j++)
			std::cout << getVal(i, j) << " ";
		std::cout << std::endl;
	}
}

template <typename NumericType>
SparseMatrix<NumericType> SparseMatrix<NumericType>::operator+(const SparseMatrix<NumericType>& second)
{
	if (this->numRows != second->numRows || this->numCols != second->numCols)		//check that matrices are same size
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
	if (this->numCols != second->numRows)
		throw std::invalid_argument("First operand's number of columns must match second operand's number of rows.");

	SparseMatrix<NumericType> result(this->numRows, second->numCols);
	HeaderNode<NumericType>* header1 = this->mRows, header2 = second->mCols;
	MatrixNode<NumericType>* currMat1, currMat2;
	NumericType value;

	//values only need to be inserted into resulting matrix if a row exists at a certain level
	while (header1 != nullptr)		//go through all rows of the first matrix - if there is no header for a row
	{
		while (header2 != nullptr)
		{
			value = 0;
			currMat1 = header1->mNode, currMat2 = header2->mNode;
			while (currMat1 != nullptr || currMat2 != nullptr)
			{
				if (currMat1->mRow > currMat2->mCol)	//need to advance currMat2
					currMat2 = currMat2->mDown;
				else if (currMat1->mRow < currMat2->mCol)
					currMat1 = currMat1->mRight;
				else
					value += currMat1->mVal * currMat2->mVal;
			}
			if (value != 0)
				result.insert(header1->mIndex, header2->mIndex, value);
			header2 = header2->mNext;
		}
		header1 = header1->mNext;
		header2 = second->mCols;
	}
}


template <typename NumericType>
HeaderNode<NumericType>* SparseMatrix<NumericType>::findHeader(HeaderNode<NumericType>* head, int index)
//helper function that returns either the Header node whose index matches the "index" parameter or the node just
//before where it should be or nullptr if the end of the list is reached
//will check return value to see if == nullptr, retVal->mIndex < index, or retVal->mIndex == index
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
MatrixNode<NumericType>* SparseMatrix<NumericType>::findInRow(MatrixNode<NumericType> head, int col)
//helper function that will return the node before where the desired node should be if no match is found in the given row
{
	MatrixNode<NumericType> prev = head, curr = prev->mRight;

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
MatrixNode<NumericType>* SparseMatrix<NumericType>::findInCol(MatrixNode<NumericType> head, int row)
//helper function that will return the node before where the desired node should be if no match is found in the given column

{
	MatrixNode<NumericType> prev = head, curr = prev->mDown;

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
*/