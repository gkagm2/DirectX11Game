#include <iostream>
#include <Windows.h>
/* // Static ������Ʈ ���� �� �Ʒ��� ���� ������� ��� ����
// Static lib ���
#include "../StaticLib/Static_Func.h"
#pragma comment(lib, "../x64/Debug/StaticLib.lib")
*/

/* Dll ������Ʈ ���� �� �Ʒ��� ���� ������� ��� ����
// Dynamic lib ��� (�Ͻ��� ��ŷ)
#include "../Dll/Dll_Func.h"
#pragma comment(lib, "../x64/Debug/Dll.lib")
*/


// Dynamic lib ��� (����� ����)
typedef int (*pFunc)(int, int);

using namespace std;

int main()
{
    // Static lib���
    /*
    int a = Add(10, 30);
    cout << a << "\n";

    int b = Minus(20, 40);
    cout << b << "\n";
    */

    // Dynamic lib��� (�Ͻ��� ��ŷ)
    /*
    int c = Mul(30, 5);
    cout << c << "\n";
    */

    // Dynamic lib��� (����� ��ŷ)
    HMODULE hModule = LoadLibrary(L"Dll.dll");

    if (nullptr == hModule)
        return 0;

    pFunc func = (pFunc)GetProcAddress(hModule, "Mul");
    int b = func(30, 560);
    std::cout << b << "\n";
    FreeLibrary(hModule);

    return 0;
}