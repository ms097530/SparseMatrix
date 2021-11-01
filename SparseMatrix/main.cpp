//#include "HeaderNode.h"
//#include "MatrixNode.h"
#include "SparseMatrix.h"

#include <iostream>

using namespace std;



int main()
{
	try
	{
		// Testing insertion
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
		
		// Testing matrix multiplication
		auto result = (first * second);  // this operation called Move Ctor
		result.print();
		
		// Testing copy assignment
		cout << "First before copy" << endl;
		first.print();
		cout << "Second" << endl;
		second.print();		
		first = second;
		cout << "First after having second copied to it" << endl;
		first.print();
		cout << endl << endl;
		cout << "Second after copying its value to first" << endl;
		second.print();
		cout << endl << endl;

		// Testing copy construction
		SparseMatrix<int> third(second);
		cout << "Third" << endl;
		third.print();
		cout << endl << endl;
		cout << "Second after copying its value to third" << endl;
		second.print();
		cout << endl << endl;

		// Testing removal
		second.remove(0, 1);
		cout << "Second after removing a value" << endl;
		second.print();
		second.insert(0, 1, 5);
		cout << endl << endl;
		cout << "Second after putting a new value in the same spot as a value was deleted" << endl;
		second.print();
		cout << endl << endl;
		cout << "Removing same value from second" << endl;
		second.remove(0, 1);		
		second.print();
		cout << endl << endl;
		cout << "Attempting to remove a value that is already 0" << endl;
		second.remove(0, 1);
		second.print();
		cout << endl << endl;

		// Testing transpose
		cout << "First before transpose" << endl;
		first.print();
		first.transpose();
		cout << "First after transpose" << endl;
		first.print();
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}

	return 0;
}