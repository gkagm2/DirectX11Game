#pragma once

class CScene;

struct TUniqueTempKey {
	tstring strKey;
	tstring strRelativePath;
};

class CResourceSaveLoad {
private:
	static ULONGLONG m_iUniqueKey;

private:
	static ULONGLONG _MakeUniqueKey() { return m_iUniqueKey++; }
public:
	static TUniqueTempKey CreateUniqueKey();
	static void SaveKey(const TUniqueTempKey& _tKey);
	static void LoadKey(const TUniqueTempKey& _tKey);

	static void Save();
	static void Load();
};