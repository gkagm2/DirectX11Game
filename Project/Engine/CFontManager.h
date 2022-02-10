#pragma once

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper")
#endif

// Return UINT
#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

// Return BYTE
#define FONT_R_FROM_RGBA(r) ((BYTE)r)
#define FONT_G_FROM_RGBA(g) ((BYTE)(g >> 8))
#define FONT_B_FROM_RGBA(b) ((BYTE)(b >> 16))
#define FONT_A_FROM_RGBA(a) ((BYTE)(a >> 24))


struct TFont {
	tstring strName;
	tstring strExtension;
	tstring GetPath() { return strName + strExtension; }
};



class CFontManager : public CSingleton<CFontManager>
{
	SINGLETON(CFontManager);

private:
	IFW1Factory*		m_pFW1Factory;
	IFW1FontWrapper*	m_pFontWrapper;

public:
	IFW1Factory* GetIFW1Factory() { return m_pFW1Factory; }
	IFW1FontWrapper* GetFontWrapper() { return m_pFontWrapper; }

public:
	vector<TFont> m_vecFontName;
	
public:
	void Init();
	void Render();
	void DrawFont(const wchar_t * _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, FW1_TEXT_FLAG _eFlag);
	void DrawFont1(const wchar_t* _pStr, const wchar_t* _pFontFamily, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, FW1_TEXT_FLAG _eFlag);

	void DrawTextLayout();

	void LoadFont();

	vector<TFont>& GetFontList() { return m_vecFontName; }
	int GetFontIdx(const tstring& _strName);
};