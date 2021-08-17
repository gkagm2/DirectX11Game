#include "pch.h"
#include "Function.h"

void SaveStringToFile(const tstring& _str, FILE* _pFile)
{
	int iLen = (int)_str.length();
	fwrite(&iLen, sizeof(int), 1, _pFile);
	fwrite(_str.data(), sizeof(TCHAR), (size_t)iLen, _pFile);
}

void LoadStringFromFile(tstring& _str, FILE* _pFile)
{
	TCHAR szBuffer[1024] = {};
	int iLen = 0;
	fread(&iLen, sizeof(int), 1, _pFile);
	fread(szBuffer, sizeof(TCHAR), (size_t)iLen, _pFile);
	_str = szBuffer;
}

void StringToArr(const string& _in, char* _out, int _iSize)
{
	strcpy_s(_out, _iSize, _in.c_str());
}

void TStringToArr(const tstring& _in, char* _out, int _iSize)
{
	string str;
	TStringToString(_in, str);
	strcpy_s(_out, _iSize, str.c_str());
}

