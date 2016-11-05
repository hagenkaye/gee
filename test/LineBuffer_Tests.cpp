#include "LineBuffer.h"

void LineBuffer_Tests()
{
    cout << "Running LineBuffer_Tests" << endl;

	LineBuffer::Ptr pLineBuffer = LineBuffer::Create();
	if (!pLineBuffer)
	{
        cout << "Could not create a LineBuffer" << endl;
        exit(1);
	}

    cout << "Passed!" << endl << endl;
}