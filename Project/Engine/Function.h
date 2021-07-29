#pragma once
#include "Global.h"
#include "CMaterial.h"

template<typename T1, typename T2>
void Safe_Delete_UnorderedMap(unordered_map<T1, T2>& _umap) {
	auto iter = _umap.begin();
	for (; iter != _umap.end(); ++iter) {
		if (nullptr != iter->second) {
			delete iter->second;
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
		if (nullptr != _vec[i])
			delete _vec[i];
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

class CResource;
template<typename T>
void SaveResourceToFile(SharedPtr<T> _pRes, FILE* _pFile) {
	UINT iCheck = 0;

	if (nullptr != _pRes)
		iCheck = 1;

	FWrite(iCheck, _pFile);

	if (nullptr != _pRes) {
		SaveStringToFile(_pRes->GetKey(), _pFile);
		SaveStringToFile(_pRes->GetRelativePath(), _pFile);
	}
}

template<typename T>
void LoadResourceFromFile(SharedPtr<T> _pRes, FILE* _pFile) {
	UINT iCheck = 0;

	FRead(iCheck, _pFile);
	if (iCheck) {
		tstring strKey, strRelativePath;
		LoadStringFromFile(strKey, _pFile);
		LoadStringFromFile(strRelativePath, _pFile);

		_pRes = CResourceManager::GetInstance()->LoadRes<T>(strKey, strRelativePath);
	}
	else
		_pRes = nullptr;
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

inline void StringToTString(const string& _in, tstring& _out) {
#ifdef UNICODE
	StringToWString(_in, _out);
#elif
	_out = _in;
#endif
}