#include "pch.h"
#include "CFontManager.h"
#include "CPathManager.h"

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

	if (nullptr != m_pFW1Factory1)
		m_pFW1Factory1->Release();

	if (nullptr != m_pFontWrapper1)
		m_pFontWrapper1->Release();

	tstring strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += _T("font\\OldNewspaperTypes.ttf");
#ifdef _UNICODE
	// wide
	if (RemoveFontResource(strPath.c_str()) == 0) {
		assert(nullptr);
	}
#elif
	string astrPath;
	TStringToString(strPath, astrPath);
	if (RemoveFontResourceA(astrPath.c_str()) == 0) {
		assert(nullptr);
	}
#endif
}

void CFontManager::Init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
	{
		assert(NULL);
	}
		
	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE.Get(), L"Arial", &m_pFontWrapper)))
	{
		assert(NULL);
	}


	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory1)))
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory1->CreateFontWrapper(DEVICE.Get(), L"Courier New", &m_pFontWrapper1)))
		assert(NULL);

	tstring strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += _T("font\\OldNewspaperTypes.ttf");
#ifdef _UNICODE
	// wide
	if (AddFontResource(strPath.c_str()) == 0) {
		assert(nullptr);
	}
#elif
	string astrPath;
	TStringToString(strPath, astrPath);
	if (AddFontResourceA(astrPath.c_str()) == 0) {
		assert(nullptr);
	}
#endif
}

void CFontManager::Render()
{
	DrawFont1(_T("Hellow"), _T("OldNewspaperTypes"), 300.f, 300.f, 30, FONT_RGBA(200, 30, 30, 255), FW1_CENTER);
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
	m_pFontWrapper1->DrawString(
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

void CFontManager::DrawTextLayout()
{
	/*IDWriteTextLayout layout;

	m_pFontWrapper->DrawTextLayout(CONTEXT.Get(),  )*/
}
