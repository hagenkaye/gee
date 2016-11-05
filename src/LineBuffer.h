#ifndef LineBuffer_h
#define LineBuffer_h
///
/// @file
/// @author Hagen Kaye <hagen.kaye@gmail.com>
///
/// @section LICENSE
/// MIT License
///
/// Copyright (c) 2016 Hagen Kaye
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///
/// @section DESCRIPTION
///
/// A class that stores and operates on a line of text
///
#include "Platform.h"
#include "Buffer.h"

class LineBuffer;
typedef std::list<shared_ptr<LineBuffer>> LineBuffers;
typedef LineBuffers::iterator LineBuffersIt;
typedef shared_ptr<LineBuffers> LineBuffersPtr;

class LineBuffer
{
public:
    typedef shared_ptr<LineBuffer> Ptr;
    typedef weak_ptr<LineBuffer> WeakPtr;

    ///
    /// Creates a buffer suitable for storing a small amount of text
    ///
    /// A LineBuffer created with this method will own the Buffer, in other
    /// words it may reallocate the buffer if a new size is required
    ///
    /// @param[in] szBuffer size of buffer to allocate in bytes
    /// @return a shared_ptr to a LineBuffer

    static Ptr Create(size_t szBuffer = 80);

    ///
    /// Creates a LineBuffer from an existing Buffer object
    ///
    /// param[in] pBuffer The buffer object to use
    /// param[in] szIndex The index into pBuffer
    /// param[in] bOwnsBuffer if true then the LineBuffer will own the Buffer
    ///           object
    /// @return a shared_ptr to a LineBuffer

    static Ptr Create(Buffer::Ptr pBuffer, size_t szIndex = 0, bool bOwnsBuffer = false);

protected:
    ///
    /// Destructor
    ///

    ~LineBuffer() {}

};

#endif