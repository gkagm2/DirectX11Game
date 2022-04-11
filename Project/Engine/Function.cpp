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

#include "CGameObject.h"
bool FWriteObj2(CGameObject* _Obj, FILE* _pFile)
{
	bool isExist = false;
	if (_Obj)
		isExist = true;
	FWrite(isExist, _pFile);
	if (isExist) {
		tstring strLocalAdr = _Obj->GetLocalAddressTotal();
		SaveStringToFile(strLocalAdr, _pFile);
	}
	return true;
}

DWORD convert_ansi_to_unicode_string(std::wstring& unicode, const char* ansi,const size_t ansi_size) 
{
	DWORD error = 0;
	do {
		if ((nullptr == ansi) || (0 == ansi_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}
		unicode.clear();

		int required_cch = ::MultiByteToWideChar(
			CP_ACP,
			0,
			ansi, static_cast<int>(ansi_size),
			nullptr, 0
		);
		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}
		unicode.resize(required_cch);

		if (0 == ::MultiByteToWideChar(
			CP_ACP,
			0,
			ansi, static_cast<int>(ansi_size),
			const_cast<wchar_t*>(unicode.c_str()), static_cast<int>(unicode.size())
		)) {
			error = ::GetLastError();
			break;
		}
	} while (false);

	return error;
}

DWORD convert_unicode_to_ansi_string(std::string& ansi, const wchar_t* unicode, const size_t unicode_size)
{
	DWORD error = 0;
	do {
		if ((nullptr == unicode) || (0 == unicode_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		ansi.clear();

		int required_cch = ::WideCharToMultiByte(
			CP_ACP,
			0,
			unicode, static_cast<int>(unicode_size),
			nullptr, 0,
			nullptr, nullptr
		);

		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		ansi.resize(required_cch);

		if (0 == ::WideCharToMultiByte(
			CP_ACP,
			0,
			unicode, static_cast<int>(unicode_size),
			const_cast<char*>(ansi.c_str()), static_cast<int>(ansi.size()),
			nullptr, nullptr
		)) {
			error = ::GetLastError();
			break;
		}
	} while (false);

	return error;
}

DWORD convert_unicode_to_utf8_string(std::string& utf8, const wchar_t* unicode, const size_t unicode_size)
{
	DWORD error = 0;
	do {
		if ((nullptr == unicode) || (0 == unicode_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		utf8.clear();

		int required_cch = ::WideCharToMultiByte(
			CP_UTF8,
			WC_ERR_INVALID_CHARS,
			unicode, static_cast<int>(unicode_size),
			nullptr, 0,
			nullptr, nullptr
		);

		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		utf8.resize(required_cch);

		if (0 == ::WideCharToMultiByte(
			CP_UTF8,
			WC_ERR_INVALID_CHARS,
			unicode, static_cast<int>(unicode_size),
			const_cast<char*>(utf8.c_str()), static_cast<int>(utf8.size()),
			nullptr, nullptr
		)) {
			error = ::GetLastError();
			break;
		}
	} while (false);

	return error;
}

DWORD convert_utf8_to_unicode_string(std::wstring& unicode, const char* utf8, const size_t utf8_size)
{
	DWORD error = 0;
	do {
		if ((nullptr == utf8) || (0 == utf8_size)) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		unicode.clear();

		int required_cch = ::MultiByteToWideChar(
			CP_UTF8,
			MB_ERR_INVALID_CHARS,
			utf8, static_cast<int>(utf8_size),
			nullptr, 0
		);
		if (0 == required_cch) {
			error = ::GetLastError();
			break;
		}

		unicode.resize(required_cch);

		if (0 == ::MultiByteToWideChar(
			CP_UTF8,
			MB_ERR_INVALID_CHARS,
			utf8, static_cast<int>(utf8_size),
			const_cast<wchar_t*>(unicode.c_str()), static_cast<int>(unicode.size())
		)) {
			error = ::GetLastError();
			break;
		}
	} while (false);

	return error;
}

string UnicodeToUTF8(const wstring& _wstr)
{
	char strUtf8[256] = { 0, };
	int nLen = WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), _wstr.size(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), _wstr.size(), strUtf8, nLen, NULL, NULL);

	return string(strUtf8);
}

wstring UTF8ToUnicode(const string& _str)
{
	wchar_t strUnicode[256] = { 0, };
	int nLen = MultiByteToWideChar(CP_UTF8, 0, _str.c_str(), _str.size(), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, _str.c_str(), _str.size(), strUnicode, nLen);

	return wstring(strUnicode);
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

Matrix GetMatrixFromFbxMatrix(const FbxAMatrix& _mat)
{
	Matrix mat;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j)
			mat.m[i][j] = (float)_mat.Get(i, j);
	}
	return mat;
}