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

#include "Convert.h"

// std::string与std::wstring互转（更安全）
std::wstring LPC2LPW(const std::string str) {
	int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	if (size == 0) return L"";

	std::vector<wchar_t> buffer(size + 1);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer.data(), size);
	buffer[size] = '\0';

	return std::wstring(buffer.data(), size - 1); // 排除 null 终止符
}
std::string LPW2LPC(const std::wstring wstr) {
	int size = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (size == 0) return "";

	std::vector<char> buffer(size + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, buffer.data(), size, nullptr, nullptr);
	buffer[size] = '\0';

	return std::string(buffer.data(), size - 1); // 排除 null 终止符
}

// UCHAR*的strlen
size_t ustrlen(const UCHAR* str) {
	size_t length = 0;
	while (*str != 0) { // 以0作为终止条件
		++length;
		++str;
	}
	return length;
}

// char*与unsigned char*互转
void CH2UCH(const char* str, UCHAR* res, size_t sz) {
	for (size_t i = 0; i < sz + 1; i++) {
		res[i] = static_cast<unsigned char>(str[i]);
	}
	return;
}
void UCH2CH(const UCHAR* str, char* res, size_t sz) {
	for (size_t i = 0; i < sz + 1; i++) {
		res[i] = static_cast<char>(str[i]);
	}
	return;
}

// UL与std::string互转（确保在范围内）
std::string UL2STR(const ULONG res) {
	if (res == 0) return "0";
	std::string ans = "";
	ULONG tmp = res;
	while (tmp) {
		ans = char((tmp % 10) + '0') + ans;
		tmp /= 10;
	}
	return ans;
}
ULONG STR2UL(const std::string str) {
	size_t sz = str.size();
	ULONG ans = 0;
	for (UINT indx = 0; indx < sz; indx++) {
		ans = ans * 10 + (str[indx] - '0');
	}
	return ans;
}

// UCHAR*与string互转
void UCH2STR(const UCHAR* data, CHAR* res, size_t sz) {
	for (size_t i = 0; i < sz; i++) {
		snprintf(res + i * 2, 3, "%02x", data[i]);
	}
	return;
}
void STR2UCH(const CHAR* hexStr, UCHAR* data, size_t sz) {
	for (size_t i = 0; i < sz * 2; i += 2) {
		CHAR byteStr[] = {hexStr[i], hexStr[i + 1], '\0'};
		data[i / 2] = (UCHAR)strtol(byteStr, NULL, 16);
	}
    return;
}
