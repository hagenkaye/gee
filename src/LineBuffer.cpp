///
/// @file LineBuffer.cpp
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
#include "LineBuffer.h"
#include "Utilities.h"

using namespace Util;

class LineBufferImpl : public LineBuffer
{
public:
    LineBufferImpl(size_t szBytes)
        : m_bOwnsBuffer(true)
        , m_szOffsetBuffer(0)
    {
        m_pBuffer = Buffer::Create(szBytes);
    }

    LineBufferImpl(Buffer::Ptr pBuffer, size_t szOffset, bool bOwnsBuffer)
        : m_bOwnsBuffer(bOwnsBuffer)
        , m_pBuffer(pBuffer)
        , m_szOffsetBuffer(szOffset)
    {
    }

    LineBufferImpl(const char *pkcBuffer)
        : m_bOwnsBuffer(true)
        , m_szOffsetBuffer(0)
    {
        m_pBuffer = Buffer::Create(::strlen(pkcBuffer) + 1);
        ::strcpy(m_pBuffer->GetBuffer(), pkcBuffer);
    }

    Ptr Split(size_t szPos) override
    {
        char *pntr = getPntrAtPos(szPos);
        Ptr pNextLine = LineBuffer::Create(pntr);
        *pntr = 0;
        return pNextLine;
    }

    void WriteBuffer(WriteBufferCallback callback, size_t szPos, size_t szCount) override
    {
        char *start = getPntrAtPos(szPos);
        char *end = advancePntrToNextUTF8char(start, szCount);

        callback(start, end - start);
    }

    bool InsertChars(const char *pkcBuffer, size_t szPos) override
    {
        insertChars(pkcBuffer, strlen(pkcBuffer), szPos);
        return true;
    }

    void InsertChars(Ptr pLineBuffer, size_t szPos) override
    {
        pLineBuffer->WriteBuffer([this, szPos](const char *pkcBuffer, size_t szBytes)
        {
            insertChars(pkcBuffer, szBytes, szPos);
        });
    }

    ~LineBufferImpl()
    {
        m_pBuffer.reset();
    }

protected:
    ///
    /// gets a pointer at n UTF8 character in the buffer
    ///
    /// @param[in] szPos position in line
    /// @return a pointer to that character.  If szPos points past the last
    ///         last character, it returns the pointer to the end of the line
    ///         which would be the null terminator
    char *getPntrAtPos(size_t szPos)
    {
        // at this point, if we haven't allocated any memory for this buffer
        // we need to do it now, allocate at least one char for an empty line
        if (!m_pBuffer || m_pBuffer->GetBuffer(m_szOffsetBuffer) == nullptr)
        {
            m_pBuffer = Buffer::Create(1);
            m_bOwnsBuffer = true;
            m_szOffsetBuffer = 0;
        }

        char *pntr = m_pBuffer->GetBuffer(m_szOffsetBuffer);
        return advancePntrToNextUTF8char(pntr, szPos);
    }



    /// expand buffer to accept new characters
    ///
    /// @param[in] szBytes additional amount of characters

    void expandBuffer(size_t szBytes)
    {
        size_t szCurrentBytes = ::strlen(m_pBuffer->GetBuffer());
        reallocateBuffer(szBytes + szCurrentBytes);
    }


    /// reallocate the buffer to a new size
    ///
    /// @param[in] szBytes the size of the buffer to reallocate.  Adds one to accomodate
    ///            the null terminator

    void reallocateBuffer(size_t szBytes)
    {
        if (m_bOwnsBuffer)
        {
            // add null terminator to requested size
            m_pBuffer->Reallocate(szBytes + 1);
        }
        else
        {
            Buffer::Ptr pNewBuffer = Buffer::Create(szBytes + 1);

            // do we need to copy anything from the existing buffer?
            const char *pkcSource = m_pBuffer->GetBuffer(m_szOffsetBuffer);
            size_t szSource = 0;
            if (pkcSource && (szSource = ::strlen(pkcSource)))
            {
                if (szSource > szBytes)
                {
                    szSource = szBytes;
                }
                ::strncpy(pNewBuffer->GetBuffer(), pkcSource, szSource);
            }

            // we now own the buffer
            m_pBuffer = pNewBuffer;
            m_szOffsetBuffer = 0;
            m_bOwnsBuffer = true;
        }
    }

    void insertChars(const char *pkcBuffer, size_t szBytes, size_t szPos)
    {
        if (szBytes)
        {
            expandBuffer(szBytes);
            char *pkcStart = getPntrAtPos(szPos);
            size_t szBytesToMove = ::strlen(pkcStart);
            ::memmove(pkcStart + szBytes, pkcStart, szBytesToMove);
            ::memmove(pkcStart, pkcBuffer, szBytes);
            *(pkcStart + szBytes + szBytesToMove) = 0;
        }
    }

private:
    bool m_bOwnsBuffer;
    Buffer::Ptr m_pBuffer;
    size_t m_szOffsetBuffer;
};

LineBuffer::Ptr LineBuffer::Create(size_t szBytes)
{
    return make_shared<LineBufferImpl>(szBytes);
}

LineBuffer::Ptr LineBuffer::Create(Buffer::Ptr pBuffer, size_t szOffset, bool bOwnsBuffer)
{
    return make_shared<LineBufferImpl>(pBuffer, szOffset, bOwnsBuffer);
}

LineBuffer::Ptr LineBuffer::Create(const char *pkcBuffer)
{
    return make_shared<LineBufferImpl>(pkcBuffer);
}