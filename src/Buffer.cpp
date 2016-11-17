///
/// @file Buffer.cpp
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
    BufferImpl(size_t szBytes)
    {
        m_buffer = reinterpret_cast<char *>(::calloc(szBytes, 1));
        if (m_buffer)
        {
            m_szBytes = szBytes;
        }
    }

    virtual char *GetBuffer(size_t szOffset, size_t szBytes, bool bRealloc) override
    {
        // test to see if buffer is out of bounds
        if ((szOffset + szBytes) > m_szBytes)
        {
            if (!bRealloc)
            {
                return nullptr;
            }
            if (!Reallocate(szOffset + szBytes))
            {
                return nullptr;
            }
        }
        return &m_buffer[szOffset];
    }

    virtual size_t GetMaxSize() const override
    {
        return m_szBytes;
    }

    virtual bool Reallocate(size_t szBytes) override
    {
        void *pBuffer = ::realloc(m_buffer, szBytes);

        // reallocation failed if pBuffer is null and szNewsize != 0
        // note: if szNewSize == 0 then realloc will free memory
        if (pBuffer == nullptr && szBytes != 0)
        {
            return false;
        }

        m_buffer = reinterpret_cast<char *>(pBuffer);
        m_szBytes = szBytes;

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
    char *m_buffer;
    size_t m_szBytes;
};

Buffer::Ptr Buffer::Create(size_t m_szBytes)
{
    return make_shared<BufferImpl>(m_szBytes);
}
