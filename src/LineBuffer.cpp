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

class LineBufferImpl : public LineBuffer
{
public:
    LineBufferImpl(size_t szBuffer)
        : m_bOwnsBuffer(true)
        , m_szIndexBuffer(0)
    {
        m_pBuffer = Buffer::Create(szBuffer);
    }

    LineBufferImpl(Buffer::Ptr pBuffer, size_t szIndex, bool bOwnsBuffer)
        : m_bOwnsBuffer(bOwnsBuffer)
        , m_pBuffer(pBuffer)
        , m_szIndexBuffer(szIndex)
    {
    }

    ~LineBufferImpl()
    {
        m_pBuffer.reset();
    }

private:
    /// counts the number of UTF8 characters in the line
    size_t numberOfChars()
    {
        size_t szCount = 0;
        uint8_t *pntr = m_pBuffer->GetBuffer(m_szIndexBuffer);
        if (pntr)
        {
            while (*pntr)
            {
                if (((*pntr & 0x80) == 0) || (*pntr & 0xC0) == 0xC0)
                {
                    szCount++;
                }
                pntr++;
            }
        }
        return szCount;
    }

    ///
    /// gets a pointer at n UTF8 character in the buffer
    ///
    /// @param[in] szPos position in line
    /// @return a pointer to that character.  If szPos points past the last
    ///         last character, it returns the pointer to the end of the line
    ///         which would be the null terminator
    uint8_t *getPntrAtPos(size_t szPos)
    {
        uint8_t *pntr = m_pBuffer->GetBuffer(m_szIndexBuffer);
        return getPntrAtPos(pntr, szPos);
    }

    /// given a pointer to a buffer, returns a pointer n chars away
    ///
    /// @param[in] pntr a pointer in the buffer
    /// @param[in] szPos number of characters to advance pntr
    /// @return a pointer szPos characters away, if szPos points past the
    ///         end of the line, it returns the pointer to the eol, which
    ///         would be the null terminator
    uint8_t *getPntrAtPos(uint8_t *pntr, size_t szPos)
    {
        size_t szCurrentPos = 0;
        if (pntr)
        {
            while (*pntr)
            {
                if (szCurrentPos == szPos)
                {
                    break;
                }
                if (((*pntr & 0x80) == 0) || (*pntr & 0xC0) == 0xC0)
                {
                    szCurrentPos++;
                }
                pntr++;
            }
        }
        return pntr;
    }

private:
    bool m_bOwnsBuffer;
    Buffer::Ptr m_pBuffer;
    size_t m_szIndexBuffer;
};

LineBuffer::Ptr LineBuffer::Create(size_t szBuffer)
{
    return make_shared<LineBufferImpl>(szBuffer);
}

LineBuffer::Ptr LineBuffer::Create(Buffer::Ptr pBuffer, size_t szIndex, bool bOwnsBuffer)
{
    return make_shared<LineBufferImpl>(pBuffer, szIndex, bOwnsBuffer);
}

