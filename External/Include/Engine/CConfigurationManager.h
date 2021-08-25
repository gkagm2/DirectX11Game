#pragma once
class CConfigurationManager : public CSingleton<CConfigurationManager>
{
	SINGLETON(CConfigurationManager)

private:
	tstring m_strLayerNames[MAX_SIZE_LAYER];

	tstring m_strFilter;
	tstring m_strRelConfigPath; // ȯ�� ������ ������ ��� ���
	tstring m_strAbsConfigPath; // ȯ�� ������ ������ ���� ���

	enum class E_FormatMode {
		binary,
		txt
	};
	E_FormatMode m_eCurFormatMode;
	tstring m_strFormatRead;
	tstring m_strFormatWrite;

public:
	void Init();
	void Update();

	void Save();
	void Load();

private:
	tstring m_strLayerFileName;
	void SaveLayer();
	void LoadLayer();

public:
	void SetFormatMode(E_FormatMode _eFormat);
	E_FormatMode GetFormatMode() { return m_eCurFormatMode; }
};