#pragma once

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