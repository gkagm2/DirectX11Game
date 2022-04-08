#pragma once


enum class E_FileAccessCheck {
	W_Possible = 02,// 쓰기 접근이 허용되는지 조사
	R_Possible = 04,// 읽기 접근이 허용되는지 조사
	RW_Possible = 06,// 읽기 및 쓰기가 허용되는지 조사
};

class CPathManager : public CSingleton<CPathManager>
{
	SINGLETON(CPathManager)
private:
	tstring m_strContentPath;
public:
	void Init();
	const TCHAR* GetContentPath() {
		return m_strContentPath.c_str();
	 }
	tstring GetRelativePath(const TCHAR* _filePath);

	vector<tstring> GetFilesInDirectory(const tstring& _absolutePath, const tstring& _filter);

public:
	bool CreateDir(const tstring& _absolutePath) {
		if (CreateDirectory(_absolutePath.c_str(), nullptr) ||
			ERROR_ALREADY_EXISTS == GetLastError()) {
			return false;
		}
		return true;
	}
	bool IsExistFileDir(const tstring& _absolutePath) {
		int result = _taccess_s(_absolutePath.c_str(), 0);
		return 0 == result;
	}
	bool CheckFileAccess(const tstring& _absolutePath, E_FileAccessCheck _eCheck) {
		int result = _taccess_s(_absolutePath.c_str(), (int)_eCheck);
		return 0 == result;
	}
};