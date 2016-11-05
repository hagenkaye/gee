#include "Buffer.h"

void Buffer_Tests()
{
    cout << "Running Buffer_Tests" << endl;

    Buffer::Ptr pBuffer = Buffer::Create(100);
    if (!pBuffer || pBuffer->GetMaxSize() != 100)
    {
        cout << "Could not allocate 100 bytes" << endl;
        exit(1);
    }
    pBuffer->Reallocate(200);
    if (pBuffer->GetMaxSize() != 200)
    {
        cout << "Could not reallocate to 200 bytes" << endl;
        exit(1);
    }

    cout << "Passed!" << endl << endl;
}