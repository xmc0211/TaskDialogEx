// Convert.h by XMC
#ifndef CONVERT_H
#define CONVERT_H

#include <windows.h>
#include <string>
#include <vector>
#include <iomanip>

// std::string��std::wstring��ת������ȫ��
std::wstring LPC2LPW(const std::string str);
std::string LPW2LPC(const std::wstring wstr);

// UCHAR*��strlen
size_t ustrlen(const UCHAR* str);

// char*��unsigned char*��ת
void CH2UCH(const char* str, UCHAR* res, size_t sz);
void UCH2CH(const UCHAR* str, char* res, size_t sz);

// UL��std::string��ת��ȷ���ڷ�Χ�ڣ�
std::string UL2STR(const ULONG res);
ULONG STR2UL(const std::string str);

// UCHAR*��ʮ�������ַ�����ת
void UCH2STR(const UCHAR* data, CHAR* res, size_t sz);
void STR2UCH(const CHAR* hexStr, UCHAR* data, size_t sz);

#endif
