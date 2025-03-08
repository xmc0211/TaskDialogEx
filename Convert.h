// MIT License
//
// Copyright (c) 2025 xmc0211
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef CONVERT_H
#define CONVERT_H

#include <windows.h>
#include <string>
#include <vector>
#include <iomanip>

// std::string与std::wstring互转（更安全）
std::wstring LPC2LPW(const std::string str);
std::string LPW2LPC(const std::wstring wstr);

// UCHAR*的strlen
size_t ustrlen(const UCHAR* str);

// char*与unsigned char*互转
void CH2UCH(const char* str, UCHAR* res, size_t sz);
void UCH2CH(const UCHAR* str, char* res, size_t sz);

// UL与std::string互转（确保在范围内）
std::string UL2STR(const ULONG res);
ULONG STR2UL(const std::string str);

// UCHAR*与十六进制字符串互转
void UCH2STR(const UCHAR* data, CHAR* res, size_t sz);
void STR2UCH(const CHAR* hexStr, UCHAR* data, size_t sz);

#endif
