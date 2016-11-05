#ifndef Buffer_h
#define Buffer_h
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
/// A class that dynamically allocates memory
///
#include "Platform.h"

class Buffer
{
public:
    typedef shared_ptr<Buffer> Ptr;
    typedef weak_ptr<Buffer> WeakPtr;

    ///
    /// Creates a buffer suitable for storing a small amount of text
    ///
    /// @param[in] szBuffer size of buffer to allocate in bytes
    /// @return a shared_ptr to a buffer

    static Ptr Create(size_t szBuffer = 80);

    ///
    /// Return a pointer to the buffer suitable for processing text
    ///
    /// @param[in] szIndex index offset into the allocated buffer
    /// @param[in] szLength optional parameter indicating the anticipated length needed
    /// @param[in] bRealloc if true, then realloc buffer to accomodate index and length
    /// @return a pointer to the buffer, null if out of bounds

    virtual uint8_t *GetBuffer(size_t szIndex = 0, size_t szLength = 0, bool bRealloc = false) = 0;

    ///
    /// Get the allocated size of the buffer
    ///
    /// @return the size of the buffer that was allocated

    virtual size_t GetMaxSize() const = 0;

    ///
    /// Reallocate the buffer to a new size
    ///
    /// @param[in] szNewSize the new size of the buffer
    /// @return true if allocation was successful, false otherwise

    virtual bool Reallocate(size_t szNewSize) = 0;

    ///
    /// Destructor
    ///

    virtual ~Buffer() {}
};

#endif
