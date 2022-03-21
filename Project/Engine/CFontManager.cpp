#include "pch.h"
#include "CFontManager.h"
#include "CPathManager.h"
#include "CPerformanceMonitor.h"

#include "CDevice.h"

CFontManager::CFontManager()
	: m_pFW1Factory(nullptr)
	, m_pFontWrapper(nullptr)
{
}

CFontManager::~CFontManager()
{
	if(nullptr != m_pFW1Factory)
		m_pFW1Factory->Release();

	if (nullptr != m_pFontWrapper)
		m_pFontWrapper->Release();

	tstring strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += STR_DIR_PATH_Font;

	for (size_t i = 0; i < m_vecFontName.size(); ++i) {
		tstring strResultPath = strPath + m_vecFontName[i].GetPath();
#ifdef _UNICODE
		// wide
		if (RemoveFontResource(strResultPath.c_str()) == 0) {
			assert(nullptr);
		}
#elif
		string astrPath;
		TStringToString(strResultPath, astrPath);
		if (RemoveFontResourceA(astrPath.c_str()) == 0) {
			assert(nullptr);
		}
#endif
	}
}

void CFontManager::Init()
{
	LoadFont();

	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
	{
		assert(NULL);
	}
		
	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE.Get(), L"Arial", &m_pFontWrapper)))
	{
		assert(NULL);
	}
}

void CFontManager::Render()
{
	for (size_t i = 0; i < m_vecUIFont.size(); ++i) {
		wstring str = {};
		TStringToWString(m_vecUIFont[i].strName, str);
		DrawFont(str.c_str(), m_vecUIFont[i].fPosX, m_vecUIFont[i].fPosY, m_vecUIFont[i].fFontSize, m_vecUIFont[i].iColor, m_vecUIFont[i].eFlag);
	}
	m_vecUIFont.clear();
}

void CFontManager::DrawFont(const wchar_t * _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, FW1_TEXT_FLAG _eFlag)
{
	m_pFontWrapper->DrawString(
		CONTEXT.Get(),
		_pStr, // String
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		_eFlag      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontManager::DrawFont1(const wchar_t* _pStr, const wchar_t* _pFontFamily, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, FW1_TEXT_FLAG _eFlag)
{
	m_pFontWrapper->DrawString(
		CONTEXT.Get(),
		_pStr, // String
		_pFontFamily, // String (Font Type)
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		_eFlag      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontManager::DrawUIFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _iColor, FW1_TEXT_FLAG _eFlag)
{
	TFontInfo tInfo = {};
	tInfo.strName = _pStr;
	tInfo.fPosX = _fPosX;
	tInfo.fPosY = _fPosY;
	tInfo.fFontSize = _fFontSize;
	tInfo.iColor = _iColor;
	tInfo.eFlag = _eFlag;
	m_vecUIFont.push_back(tInfo);
}

void CFontManager::DrawTextLayout()
{
	/*IDWriteTextLayout layout;

	m_pFontWrapper->DrawTextLayout(CONTEXT.Get(),  )*/
}

void CFontManager::LoadFont()
{
	PM_START(_T("FontManager get files in directory"));
	// Font 경로에서 모든 경로를 가져온다.
	tstring strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += STR_DIR_PATH_Font;
	vector<tstring> vecFontFileNames = CPathManager::GetInstance()->GetFilesInDirectory(strPath, _T("*") STR_FONT_EXTENSION_TTF);
	PM_END();
	PM_START(_T("FontManager str processing"));
	for (size_t i = 0; i < vecFontFileNames.size(); ++i) {
		tstring strName = vecFontFileNames[i];
		// 확장명 떼어내기.
		TFont tFont{};
		tstring strNewName;
		for (int j = (int)strName.size() - 1; j >= 0; --j) {
			if (strName[j] == _T('.')) {
				strNewName = strName.substr(0, j );
				tstring strExtension = strName.substr(j, strName.size() - j + 1);
				tFont.strExtension = strExtension;
				break;
			}
		}
		// 경로 떼어내기
		for (int j = (int)strNewName.size() - 1; j >= 0; --j) {
			if (strNewName[j] == _T('\\')) {
				strName = strNewName.substr(j + 1, strNewName.size() - j);
				tFont.strName = strName;
				break;
			}
		}

		// 이름과 확장명 저장
		m_vecFontName.push_back(tFont);
	}
	PM_END();
	PM_START(_T("FontManager Add Font Resoruce"));
	strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += STR_DIR_PATH_Font;

	for (size_t i = 0; i < m_vecFontName.size(); ++i) {
#ifdef _UNICODE
		// wide
		tstring strResultPath = strPath + m_vecFontName[i].GetPath();
		if (AddFontResource(strResultPath.c_str()) == 0) {
			assert(nullptr);
		}
#elif
		string astrPath;
		TStringToString(m_vecFontName[i].GetPath().c_str(), astrPath);
		if (AddFontResourceA(astrPath.c_str()) == 0) {
			assert(nullptr);
		}
#endif
	}
	PM_END();
}

// 찾지 못하면 -1 리턴
int CFontManager::GetFontIdx(const tstring& _strName)
{
	for (int i = 0; i < m_vecFontName.size(); ++i) {
		if (m_vecFontName[i].strName == _strName) {
			return i;
			break;
		}
	}
	return -1;
}
