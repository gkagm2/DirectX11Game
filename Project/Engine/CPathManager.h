#pragma once
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

	vector<tstring> GetFilesInDirectory(const tstring& _absolutePath, const tstring& _filter);
};