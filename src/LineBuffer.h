///
/// @file LineBuffer.h
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
#ifndef LineBuffer_h
#define LineBuffer_h
#include "Platform.h"
#include "Buffer.h"
#include "Utilities.h"

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
    /// @param[in] szBytes size of buffer to allocate in bytes
    /// @return a shared_ptr to a LineBuffer

    static Ptr Create(size_t szBytes = 80);

    ///
    /// Creates a LineBuffer from an existing Buffer object
    ///
    /// @param[in] pBuffer The buffer object to use
    /// @param[in] szOffset The index into pBuffer
    /// @param[in] bOwnsBuffer if true then the LineBuffer will own the Buffer
    ///           object
    /// @return a shared_ptr to a LineBuffer

    static Ptr Create(Buffer::Ptr pBuffer, size_t szOffset = 0, bool bOwnsBuffer = false);

    ///
    /// Creates a LineBuffer from a string
    ///
    /// A buffer is allocated and the string is copied into the line buffer
    ///
    /// @param[in] pkcBuffer a null terminated string to copy into the buffer
    /// @return a shared_ptr to a LineBuffer

    static Ptr Create(const char *pkcBuffer);

    ///
    /// Splits a line into two LineBuffers
    ///
    /// @param[in] szPos character position in line buffer to make the split.  If szPos points past
    ///            the end of the line, then the line is left intact and a blank line is
    ///            returned
    /// @return a LineBuffer Ptr to the second half of the line

    virtual Ptr Split(size_t szPos) = 0;

    ///
    /// Writes out a portion of the buffer to the by calling the supplied callback
    ///
    /// @param[in] callback Callback function to callback
    /// @param[in] szPos character position in LineBuffer to start
    /// @param[in] szCount maximum characters to WriteBuffer

    virtual void WriteBuffer(WriteBufferCallback callback, size_t szPos = 0, size_t szCount = std::numeric_limits<size_t>::max()) = 0;

    ///
    /// Inserts characters into LineBuffer
    ///
    /// This function can only insert a string that doesn't have a line break, in other words
    /// it won't split lines or create more LineBuffers to perform the insertion
    ///
    /// @param[in] pkcBuffer string of characters to insert
    /// @param[in] szPos position in LineBuffer to do the insertion, by default this is the end
    /// @return true if insertion was done, false if it failed (see reason above)

    virtual bool InsertChars(const char *pkcBuffer, size_t szPos = std::numeric_limits<size_t>::max()) = 0;

    virtual void InsertChars(Ptr pLineBuffer, size_t szPos = std::numeric_limits<size_t>::max()) = 0;

protected:
    ///
    /// Destructor
    ///

    ~LineBuffer() {}

};

#endif