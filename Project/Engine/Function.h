#pragma once
#include "Global.h"
#include "CMaterial.h"

template<typename T1, typename T2>
void Safe_Delete_UnorderedMap(unordered_map<T1, T2>& _umap) {
	auto iter = _umap.begin();
	for (; iter != _umap.end(); ++iter) {
		if (nullptr != iter->second) {
			delete iter->second;
			iter->second = nullptr;
		}
	}
	_umap.clear();
}

template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map) {
	auto iter = _map.begin();
	for (; iter != _map.end(); ++iter) {
		if (nullptr != iter->second) {
			delete iter->second;
			iter->second = nullptr;
		}
	}
	_map.clear();
}

template<typename T, UINT iSize>
void Safe_Delete_Array(T* (&_arr)[iSize]) {
	for (UINT i = 0; i < iSize; ++i) {
		if (nullptr != _arr[i]) {
			delete _arr[i];
			_arr[i] = nullptr;
		}
	}
}

template<typename T>
void Safe_Delete_Vector(vector<T>& _vec) {
	for (UINT i = 0; i < _vec.size(); ++i) {
		if (nullptr != _vec[i]) {
			delete _vec[i];
			_vec[i] = nullptr;
		}
			
	}
	_vec.clear();
}

void SaveStringToFile(const tstring& _str, FILE* _pFile);
void LoadStringFromFile(tstring& _str, FILE* _pFile);

template<typename T>
size_t FWrite(T& _data, FILE* _pFile, UINT _iElementCount = 1) {
	return fwrite(&_data, sizeof(T), _iElementCount, _pFile);
}

template<typename T>
size_t FRead(T& _data, FILE* _pFile, UINT _iElementCount = 1) {
	return fread(&_data, sizeof(T), _iElementCount, _pFile);
}

template<typename T>
uuid FWriteObj(T& _Data, FILE* _pFile) {
	bool isExist = false;
	if (_Data)
		isExist = true;
	FWrite(isExist, _pFile);
	
	uuid id{};
	if (isExist) {
		id = ((CObject*)_Data)->GetUUID();
		FWrite(id, _pFile);
	}
	return id;
}

bool FWriteObj2(CGameObject* _Obj, FILE* _pFile);


template<typename T>
uuid FReadObj(T** _data, FILE* _pFile) {
	bool isExist = false;
	FRead(isExist, _pFile);
	
	uuid id{};
	if (isExist) {
		FRead(id, _pFile);
		CObject::LinkObjectWhenSceneLoadEvn((CObject**)_data, id);
	}
	return id;
}

template<typename T1, typename T2>
bool FReadObj2(T1** _data, T2* _CurComponent, FILE* _pFile) {
	bool isExist = false;
	FRead(isExist, _pFile);
	if (isExist) {
		tstring strLocalAdr = {};
		LoadStringFromFile(strLocalAdr, _pFile);
		CObject::LinkObjectWhenSceneLoadEvn(_data, strLocalAdr, _CurComponent);
	}
	return true;
}

class CResource;
template<typename T>
void SaveResourceToFile(SharedPtr<T> _pRes, FILE* _pFile) {
	UINT iCheck = nullptr == _pRes ? 0 : 1;
	FWrite(iCheck, _pFile);

	if (nullptr != _pRes) {
		SaveStringToFile(_pRes->GetKey(), _pFile);
		SaveStringToFile(_pRes->GetRelativePath(), _pFile);
	}
}

template<typename T>
void LoadResourceFromFile(SharedPtr<T>& _pResOut, FILE* _pFile) {
	UINT iCheck = 0;

	FRead(iCheck, _pFile);
	if (iCheck) {
		tstring strKey, strRelativePath;
		LoadStringFromFile(strKey, _pFile);
		LoadStringFromFile(strRelativePath, _pFile);

		_pResOut = CResourceManager::GetInstance()->LoadRes<T>(strKey, strRelativePath);
	}
	else
		_pResOut = nullptr;
}

// 멀티바이트 -> 유니코드
inline void StringToWString(const string& _in, wstring& _out) {
	_out.assign(_in.begin(), _in.end());
}

// 유니코드 -> 멀티바이트
inline void WStringToString(const wstring& _in, string& _out) {
	_out.assign(_in.begin(), _in.end());
}

inline void TStringToString(const tstring& _in, string& _out) {
#ifdef UNICODE
	WStringToString(_in, _out);
#elif
	_out = _in;
#endif
}

inline void TStringToWString(const tstring& _in, wstring& _out) {
#ifdef UNICODE
	_out = _in;
#elif
	StringToWString(_in, _out);
#endif
}

inline void StringToTString(const string& _in, tstring& _out) {
#ifdef UNICODE
	StringToWString(_in, _out);
#elif
	_out = _in;
#endif
}

DWORD convert_ansi_to_unicode_string(
	__out std::wstring& unicode,
	__in const char* ansi,
	__in const size_t ansi_size
);

DWORD convert_unicode_to_ansi_string(
	__out std::string& ansi,
	__in const wchar_t* unicode,
	__in const size_t unicode_size
);
DWORD convert_unicode_to_utf8_string(
	__out std::string& utf8,
	__in const wchar_t* unicode,
	__in const size_t unicode_size
);
DWORD convert_utf8_to_unicode_string(
	__out std::wstring& unicode,
	__in const char* utf8,
	__in const size_t utf8_size
);

// Unicode -> UTF-8 변환 (ImGui에서 한글 변환할 때 사용)
string UnicodeToUTF8(const wstring& _wstr);

// UTF-8 -> Unicode 변환
wstring UTF8ToUnicode(const string& _str);

void StringToTStringVec(const vector<string>& _in, vector<tstring>& _out);
void TStringToStringVec(const vector<tstring>& _in, vector<string>& _out);

void StringToArr(const string& _in, char* _out, int _iSize);
void TStringToArr(const tstring& _in, char* _out, int _iSize);

namespace StringTool {
	void StringToLower(string& _str);
	void StringToUpper(string& _str);
}

#ifdef UNICODE
template<class T>
tstring to_tstring(const T& x) { return std::to_wstring(x); }
#elif
template<class T>
tstring to_tstring(const T& x) { return std::to_string(x); }
#endif