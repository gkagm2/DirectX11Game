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
void Safe_Delete_Array(T* (&arr)[iSize]) {
	for (UINT i = 0; i < iSize; ++i) {
		if (nullptr != arr[i]) {
			delete arr[i];
			arr[i] = nullptr;
		}
	}
}