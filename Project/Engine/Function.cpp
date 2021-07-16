#include "pch.h"
#include "Function.h"

void SaveStringToFile(const tstring& _str, FILE* _pFile)
{
	int iLen = _str.length();
	fwrite(&iLen, sizeof(int), iLen, _pFile);
	fwrite(_str.data(), sizeof(TCHAR), iLen, _pFile);
}

void LoadStringFromFile(tstring& _str, FILE* _pFile)
{
	TCHAR szBuffer[1024] = {};
	int iLen = 0;
	fread(&iLen, sizeof(int), 1, _pFile);
	fread(szBuffer, sizeof(TCHAR), iLen, _pFile);
	_str = szBuffer;
}