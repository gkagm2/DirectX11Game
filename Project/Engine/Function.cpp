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

void StringTool::StringToLower(string& _str)
{
	int i = 0;
	int iStrSize = _str.size();
	while (i < iStrSize) {
		if (std::isupper(_str[i])) {
			_str[i] = std::tolower(_str[i]);
		}
		++i;
	}
}

void StringTool::StringToUpper(string& _str)
{
	int i = 0;
	int iStrSize = _str.size();
	while (i < iStrSize) {
		if (std::islower(_str[i])) {
			_str[i] = std::toupper(_str[i]);
		}
		++i;
	}
}