#include "pch.h"
#include "CCursor.h"
#include "dxgi.h"
#include "CTexture.h"
#include "CCore.h"

void CCursor::SetCursor(SharedPtr<CTexture> _pTexture, const Vector2 _vOffset)
{
    
}

void CCursor::Update()
{
    tagCURSORINFO lCursorInfo{};
    lCursorInfo.cbSize = sizeof(lCursorInfo);

    auto lBoolres = GetCursorInfo(&lCursorInfo);

    if (lBoolres == TRUE)
    {
        if (lCursorInfo.flags == CURSOR_SHOWING)
        {
            // currently lCursorInfo.hCursor has the shape of actual cursor that is coming to your system to modify it you can use the below line

            std::string path = "N:\\Project\\DirectX2019Game\\DirectX11Game\\OutputFile\\bin\\content\\texture\\Monster.bmp"; // this is path to the file where .cur file available in your system
            lCursorInfo.hCursor = LoadCursorFromFileA(path.c_str());
            // You can refer https://docs.microsoft.com/en-us/windows/win32/menurc/using-cursors for creating your own cursor

            auto lCursorPosition = lCursorInfo.ptScreenPos;
            auto lCursorSize = lCursorInfo.cbSize;
            HDC  lHDC = CCore::g_hDC;
            //lIDXGISurface1->GetDC(FALSE, &lHDC); // getting handle to draw the cursor
            // Draw the cursor
            DrawIconEx(
                lHDC,
                lCursorPosition.x,
                lCursorPosition.y,
                lCursorInfo.hCursor,
                30,
                30,
                0,
                0,
                DI_NORMAL | DI_DEFAULTSIZE);
            // Release the handle
            //lIDXGISurface1->ReleaseDC(nullptr);
        }
    }
}