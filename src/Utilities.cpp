///
/// @file Utilities.cpp
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
#include "Utilities.h"

size_t Util::numUTF8chars(const char *pkcBuffer)
{
    size_t szCount = 0;
    if (pkcBuffer)
    {
        while (*pkcBuffer)
        {
            if (((*pkcBuffer & 0x80) == 0) || (*pkcBuffer & 0xC0) == 0xC0)
            {
                szCount++;
            }
            pkcBuffer++;
        }
    }
    return szCount;
}

char *Util::advancePntrToNextUTF8char(char *pcBuffer, size_t szCount)
{
    size_t szCurrentPos = 0;
    if (pcBuffer)
    {
        while (*pcBuffer)
        {
            if (szCurrentPos == szCount)
            {
                break;
            }
            if (((*pcBuffer & 0x80) == 0) || (*pcBuffer & 0xC0) == 0xC0)
            {
                szCurrentPos++;
            }
            pcBuffer++;
        }
    }
    return pcBuffer;
}

char *Util::nextLine(char *pcBuffer, Util::LineEnding &rLineEnding, bool bMoreToCome)
{
    while (*pcBuffer)
    {
        if (*pcBuffer == 0x0d || *pcBuffer == 0x0a)
        {
            if (*pcBuffer == 0x0d)
            {
                if (pcBuffer[1] == 0x0a)
                {
                    rLineEnding = Util::CRLF;
                    *pcBuffer = 0;
                    pcBuffer++;
                }
                else if (bMoreToCome && pcBuffer[1] == 0)
                {
                    // might be an incomplete line
                    rLineEnding = Util::NONE;
                    return nullptr;
                }
                else
                {
                    rLineEnding = Util::CR;
                }
            }
            else
            {
                rLineEnding = Util::LF;
            }
            *pcBuffer = 0;
            pcBuffer++;
            return pcBuffer;
        }
        pcBuffer++;
    }
    rLineEnding = Util::NONE;
    return nullptr;
}

