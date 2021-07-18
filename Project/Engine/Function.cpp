#include "pch.h"
#include "Function.h"

void SaveStringToFile(const tstring& _str, FILE* _pFile)
{
	UINT iLen = (UINT)_str.length();
	fwrite(&iLen, sizeof(UINT), iLen, _pFile);
	fwrite(_str.data(), sizeof(TCHAR), iLen, _pFile);
}

void LoadStringFromFile(tstring& _str, FILE* _pFile)
{
	TCHAR szBuffer[1024] = {};
	UINT iLen = 0;
	fread(&iLen, sizeof(UINT), 1, _pFile);
	fread(szBuffer, sizeof(TCHAR), iLen, _pFile);
	_str = szBuffer;
}