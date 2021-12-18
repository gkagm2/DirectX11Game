#include "pch.h"
#include "Function.h"

void SaveStringToFile(const tstring& _str, FILE* _pFile)
{
	size_t iLen = _str.length();
	fwrite(&iLen, sizeof(size_t), 1, _pFile);
	fwrite(_str.data(), sizeof(TCHAR), iLen, _pFile);
}

void LoadStringFromFile(tstring& _str, FILE* _pFile)
{
	TCHAR szBuffer[1024] = {};
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);
	fread(szBuffer, sizeof(TCHAR), iLen, _pFile);
	_str = szBuffer;
}

void StringToTStringVec(const vector<string>& _in, vector<tstring>& _out)
{
	_out.clear();
	for (size_t i = 0; i < _in.size(); ++i) {
		tstring str;
		StringToTString(_in[i], str);
		_out.push_back(str);
	}
}

void TStringToStringVec(const vector<tstring>& _in, vector<string>& _out)
{
	_out.clear();
	for (size_t i = 0; i < _in.size(); ++i) {
		string str;
		TStringToString(_in[i], str);
		_out.push_back(str);
	}
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
	int iStrSize = (int)_str.size();
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
	int iStrSize = (int)_str.size();
	while (i < iStrSize) {
		if (std::islower(_str[i])) {
			_str[i] = std::toupper(_str[i]);
		}
		++i;
	}
}