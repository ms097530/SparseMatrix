//#include "HeaderNode.h"
//#include "MatrixNode.h"
#include "SparseMatrix.h"

#include <iostream>

using namespace std;

SparseMatrix<int> myMatrix(3, 6);

int main()
{
	try
	{
		/*myMatrix.insert(0, 0, 1);
		myMatrix.insert(0, 1, 2);
		myMatrix.insert(0, 2, 3);
		myMatrix.insert(0, 3, 4);
		myMatrix.insert(0, 4, 5);
		myMatrix.insert(0, 5, 6);
		myMatrix.insert(1, 0, 1);
		myMatrix.insert(2, 5, 6);
		myMatrix.insert(1, 1, 2);
		myMatrix.insert(2, 2, 3);*/

		/*for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				myMatrix.insert(i, j, j + 1);
			}
		}

		myMatrix.print();
		cout << endl << endl;
		SparseMatrix<int> otherMatrix(myMatrix);
		otherMatrix.print();
		cout << endl << endl;*/

		/*for (int i = 2; i >= 0; --i)
		{
			for (int j = 5; j >= 0; --j)
			{
				myMatrix.insert(i, j, j + 1);
			}
		}*/

		//myMatrix.remove(1, 0);
		//myMatrix.remove(1, 1);
		//myMatrix.remove(1, 5);
		//myMatrix.remove(2, 5);
		//myMatrix.remove(0, 5);
		//myMatrix.insert(0, 0, 7);
		//myMatrix.print();
		//cout << endl;

		//myMatrix.insert(1, 4, 6);
		//myMatrix.print();
		//cout << endl;

		//myMatrix.insert(0, 0, 9);
		//myMatrix.print();
		//cout << endl;

		/*cout << myMatrix.getVal(0, 0) << endl;
		cout << myMatrix.getVal(1, 4) << endl;
		cout << myMatrix.getVal(2, 5) << endl;*/

		//myMatrix.remove(1, 2);
		//myMatrix.remove(0, 1);
		//myMatrix.remove(1, 1);
		//myMatrix.remove(1, 3);
		//myMatrix.remove(2, 4);
		//myMatrix.remove(0, 4);
		//myMatrix.remove(2, 4);
		//myMatrix.remove(1, 0);
		//myMatrix.remove(2, 3);
		//myMatrix.remove(0, 0);
		/*myMatrix.remove(2, 5);


		myMatrix.print();
		myMatrix.transpose();
		myMatrix.print();
		myMatrix.transpose();
		myMatrix.print();
		cout << endl << endl;

		SparseMatrix<int> myMatrix2(3, 6);
		myMatrix2.insert(0, 0, 5);
		myMatrix2.insert(1, 3, 3);
		myMatrix2.insert(2, 5, 2);*/
		//myMatrix2 = std::move(myMatrix + myMatrix2);
		/*for (int i = 0; i < 3; ++i)
		{
			SparseMatrix<int> myMatrix3 = myMatrix + myMatrix2;
			myMatrix3.print();
			cout << endl << endl;
		}*/
		//SparseMatrix<int> myMatrix4(3, 6);
		//myMatrix4 = std::move(myMatrix + myMatrix2);		//doesn't work

		//myMatrix2.print();
		//myMatrix3.print();
		//cout << endl;

		SparseMatrix<int> first(2, 4), second(4, 2);
		first.insert(0, 0, 3);
		first.insert(0, 1, 4);
		first.insert(0, 3, 7);
		first.insert(1, 0, 2);
		first.insert(1, 2, 5);
		second.insert(0, 0, 3);
		second.insert(0, 1, 2);
		second.insert(1, 0, 3);
		second.insert(1, 1, 2);
		second.insert(2, 0, 3);
		second.insert(2, 1, 2);
		second.insert(3, 0, 3);
		second.insert(3, 1, 2);
		//SparseMatrix<int> result = first * second;
		//second = (first * second);
		//result.print();
		second.print();
		first = second;
		first.print();
		cout << endl << endl;
		second.print();
		cout << endl << endl;
		/*SparseMatrix<int> third(second);
		third.print();
		cout << endl << endl;
		second.print();
		cout << endl << endl;*/
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}

	return 0;
}