// Convert.cpp by XMC

#include "Convert.h"

// std::string��std::wstring��ת������ȫ��
std::wstring LPC2LPW(const std::string str) {
	int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	if (size == 0) return L"";

	std::vector<wchar_t> buffer(size + 1);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer.data(), size);
	buffer[size] = '\0';

	return std::wstring(buffer.data(), size - 1); // �ų� null ��ֹ��
}
std::string LPW2LPC(const std::wstring wstr) {
	int size = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (size == 0) return "";

	std::vector<char> buffer(size + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, buffer.data(), size, nullptr, nullptr);
	buffer[size] = '\0';

	return std::string(buffer.data(), size - 1); // �ų� null ��ֹ��
}

// UCHAR*��strlen
size_t ustrlen(const UCHAR* str) {
	size_t length = 0;
	while (*str != 0) { // ��0��Ϊ��ֹ����
		++length;
		++str;
	}
	return length;
}

// char*��unsigned char*��ת
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

// UL��std::string��ת��ȷ���ڷ�Χ�ڣ�
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

// UCHAR*��string��ת
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
