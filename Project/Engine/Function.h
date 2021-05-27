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