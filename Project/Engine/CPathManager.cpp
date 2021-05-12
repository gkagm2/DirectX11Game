#include "pch.h"
#include "CPathManager.h"
CPathManager::CPathManager() {
}
CPathManager::~CPathManager() {
}

void CPathManager::Init() {
	TCHAR path[MAX_PATH] = _T("");
	GetCurrentDirectory(MAX_PATH, path);
	
	int iLen = (int)_tcslen(path);

	for (int i = iLen - 1; i > 0; --i) {
		if (_T('\\') == path[i]) {
			path[i] = _T('\0');
			break;
		}
	}

	m_strContentPath = path;
	m_strContentPath += STR_FILE_PATH_Content;
}