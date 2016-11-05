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
#include "Buffer.h"

class BufferImpl : public Buffer
{
public:
    BufferImpl(size_t szBuffer)
    {
        m_buffer = ::calloc(szBuffer, 1);
        if (m_buffer)
        {
            m_szBuffer = szBuffer;
        }
    }

    virtual uint8_t *GetBuffer(size_t szIndex, size_t szLength, bool bRealloc) override
    {
        // test to see if buffer is out of bounds
        if ((szIndex + szLength) > m_szBuffer)
        {
            if (!bRealloc)
            {
                return nullptr;
            }
            if (!Reallocate(szIndex + szLength))
            {
                return nullptr;
            }
        }
        return &(static_cast<uint8_t *>(m_buffer)[szIndex]);
    }

    virtual size_t GetMaxSize() const override
    {
        return m_szBuffer;
    }

    virtual bool Reallocate(size_t szNewSize) override
    {
        void *pBuffer = ::realloc(m_buffer, szNewSize);

        // reallocation failed if pBuffer is null and szNewsize != 0
        // note: if szNewSize == 0 then realloc will free memory
        if (pBuffer == nullptr && szNewSize != 0)
        {
            return false;
        }

        // zero memory if buffer expanded
        if (szNewSize > m_szBuffer)
        {
            ::memset(&(static_cast<uint8_t *>(pBuffer)[m_szBuffer]), 0, szNewSize - m_szBuffer);
        }

        m_buffer = pBuffer;
        m_szBuffer = szNewSize;

        return true;
    }

    ~BufferImpl()
    {
        if (m_buffer)
        {
            ::free(m_buffer);
        }
    }

private:
    void *m_buffer;
    size_t m_szBuffer;
};

Buffer::Ptr Buffer::Create(size_t szBuffer)
{
    return make_shared<BufferImpl>(szBuffer);
}
