/**
lottoPicks.cpp
Purpose: Generate lottery picks from a set of number strings

@author Jimmy Wong
@date   May 10, 2017
*/

// TODO:
// 1. Replace hard-coded numbers with constant variables
// 2. Add try-catch blocks to handle exceptions
// 3. Write proper unit test cases

#include <iostream>
#include <map>
#include <stack>
#include <stdlib>
#include <string>

// ----------------------- Data structure -------------------------
class BTreeNode
{
	char* ptrToStr;			// Pointer to number string
	unsigned int number;	// Lucky number
	unsigned int level;		// Tree level

public:
	BTreeNode* leftNode;
	BTreeNode* rightNode;

	void SetNumber( unsigned int number )
	{
		this->number = number;
	}

	unsigned int GetNumber()
	{
		return number;
	}

	unsigned int GetLevel()
	{
		return level;
	}

	BTreeNode( unsigned int level, unsigned int number, char* ptrToStr ) : ptrToStr( NULL ), leftNode( NULL ), rightNode( NULL )
	{
		// Level can only be initialized on instantiation of the tree node
		this->level = level;
		this->number = number;
		this->ptrToStr = ptrToStr;
	}
};
// ----------------------- End of data structure -------------------------


// ----------------------- Interface of Search algorithm -------------------------
class Search
{
protected:
	bool IsValidLuckyNumber( unsigned int number, const std::map<int, bool> *numberList ) {

		// Check if number has already been generated
		if( numberList->count( number ) != 0 )
			return false;

		// Check if number is between 1 and 59
		if( number < 1 || number > 59 )
			return false;

		return true;
	}

	bool IsNumberString( char* ptrToStr )
	{
		char* indexPtr = ptrToStr;

		while( indexPtr[0] != '\0' )
		{
			if( indexPtr[0] < '0' || indexPtr[0] > '9' )
				return false;
			indexPtr++;
		}
		return true;
	}

public:
	virtual bool FindLucky7s( char* ptrToStr, BTreeNode* ptrToCurrNode, std::stack<int>* ptrToStack ) = 0;
};

class SearchWithBTree : public Search
{
	std::map<int, bool> numberList;

public:
	// Recursive approach to find the solution
	bool FindLucky7s( char* ptrToStr, BTreeNode* ptrToCurrNode, std::stack<int>* ptrToStack )	
	{
		if( false == IsNumberString( ptrToStr ) )
			return false;

		// Generated 7 numbers
		if( 7 == ptrToCurrNode->GetLevel() )
		{
			// Yah, we reached the solution!!
			if( '\0' == ptrToStr[0] )
				return true;
			else // Keep trying...
				return false;
		}

		// Generate the 1-digit number for the left node
		unsigned int singleDigit = (unsigned int) (ptrToStr[0] - '0');
		if( true == IsValidLuckyNumber( singleDigit, &numberList ) )
		{
			numberList[singleDigit] = true;
			BTreeNode* leftChild = new BTreeNode( ptrToCurrNode->GetLevel() + 1,
				singleDigit,
				ptrToStr + 1 );	// Shift 1-digit right
			if( true == FindLucky7s( ptrToStr + 1, leftChild, ptrToStack ) )
			{
				ptrToStack->push( leftChild->GetNumber() );	// Save number for the valid picks!
				return true;
			}
			// Remove the number from hash table if the above path cannot reach a solution
			numberList.erase( singleDigit );

			// Else, follow-through to the right node
		}

		// Generate the 2-digit number for the right node
		unsigned int doubleDigits = (unsigned int) ((ptrToStr[0] - '0') * 10 + (ptrToStr[1] - '0'));
		if( true == IsValidLuckyNumber( doubleDigits, &numberList ) )
		{
			// Keep track of the generated number to preserve uniqueness
			numberList[doubleDigits] = true;
			BTreeNode* rightChild = new BTreeNode( ptrToCurrNode->GetLevel() + 1,
				doubleDigits,
				ptrToStr + 2 ); // Shift 2-digit right
			if( true == FindLucky7s( ptrToStr + 2, rightChild, ptrToStack ) )
			{
				ptrToStack->push( rightChild->GetNumber() );	// Save number for the valid picks!
				return true;
			}
			// Remove the number from hash table if the above path cannot reach a solution
			numberList.erase( doubleDigits );
		}

		// Trace back one level up
		return false;
	}
};
// ----------------------- End of Interface -------------------------


// Helper to filter out number strings that mathematically cannot use for lottery picks 
bool QuickCheckOK( char* ptrToStr )
{
	// For example, six 2-digit number and one 3-digit number OR less than 6-digit number
	if( strlen( ptrToStr ) > 14 || strlen( ptrToStr ) < 7 )
		return false;

	return true;
}


// Caller to specify the type of search algorithm at run-time
void GenerateLuckyDraws( char* ptrToArray[], unsigned int arraySize )
{
	Search* bTreeSearch = new SearchWithBTree();	// Strategy pattern: can be replaced with other implementations of search
	std::stack<int> outputStack;
	BTreeNode* ptrToRoot = NULL;

	// Loop through all number strings
	for( int i = 0; i < arraySize; i++ )
	{
		if( false == QuickCheckOK( ptrToArray[i] ) )	// Quick trick to save processing time
			continue;

		// Construct a new tree
		if( NULL != ptrToRoot )
			delete ptrToRoot;
		ptrToRoot = new BTreeNode( 0, 0, ptrToArray[i] );

		// If solution found, output the orginal strings and its lucky numbers
		if( true == bTreeSearch->FindLucky7s( ptrToArray[i], ptrToRoot, &outputStack ) )
		{
			std::cout << ptrToArray[i] << " ->";

			while( !outputStack.empty() )
			{
				std::cout << " " << outputStack.top();
				outputStack.pop();
			}
			std::cout << std::endl;
		}
	}
}


// Start of the program
void main()
{
	// This mimics the strings set read from the electronic format file
	char* stringArray[9];

	stringArray[0] = "569815571556";
	stringArray[1] = "4938532894754";
	stringArray[2] = "1234567";
	stringArray[3] = "472844278465445";
	stringArray[4] = "";					// Test for default case
	stringArray[5] = "ABCDEFG";				// Alphabetical string
	stringArray[6] = "1A234567";			// Alpha-numeric string
	stringArray[7] = "123456789012345";		// Test QuickCheckOK()
	stringArray[8] = "123456";				// Test QuickCheckOK()

	GenerateLuckyDraws( stringArray, sizeof( stringArray )/sizeof( char* ) );
}
