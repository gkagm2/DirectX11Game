#include "pch.h"
#include "CPathManager.h"
#include <io.h>
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

#ifdef ENVIRONMENT32
#define T_FIND_DATA _tfinddata_t
#define T_FIND_FIRST _tfindfirst
#define T_FIND_NEXT _tfindnext
#endif
#ifdef ENVIRONMENT64
#define T_FIND_DATA _tfinddata64_t
#define T_FIND_FIRST _tfindfirst64
#define T_FIND_NEXT _tfindnext64
#endif

tstring CPathManager::GetRelativePath(const TCHAR* _filePath)
{
	tstring strFilePath = _filePath;

	size_t iAbsLen = m_strContentPath.size();
	size_t iFullLen = strFilePath.length();

	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);

	return strRelativePath;
}

vector<tstring> CPathManager::GetFilesInDirectory(const tstring& _absolutePath, const tstring& _filter)
{
	tstring searching = _absolutePath + _filter;
	vector<tstring> return_;
	tstring contentPath = CPathManager::GetInstance()->GetContentPath();
	tstring relativePath = _absolutePath.substr(contentPath.size(), _absolutePath.size() - 1);


	T_FIND_DATA fd;
	const auto handle = T_FIND_FIRST(searching.c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)
		return return_;

	auto result = 0;
	do
	{
		if (fd.attrib & _A_SUBDIR) { // 디렉토리면
			tstring dirName = fd.name;
			if (_T(".") != dirName && _T("..") != dirName) {
				tstring subPath = _absolutePath + dirName + _T("\\");
				vector<tstring> vec = GetFilesInDirectory(subPath, _filter);
				for (int i = 0; i < vec.size(); ++i)
					return_.push_back(vec[i]);
			}
		}
		else {
			tstring fileName = fd.name;
			fileName = relativePath + fileName;
			return_.push_back(fileName);
		}

		result = T_FIND_NEXT(handle, &fd);
	} while (result != -1);

	_findclose(handle);

	return return_;
}