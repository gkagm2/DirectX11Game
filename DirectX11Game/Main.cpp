#include <iostream>
#include <Windows.h>
/* // Static ������Ʈ ���� �� �Ʒ��� ���� ������� ��� ����
// Static lib ���
#include "../StaticLib/Static_Func.h"
#pragma comment(lib, "../x64/Debug/StaticLib.lib")
*/

/* Dll ������Ʈ ���� �� �Ʒ��� ���� ������� ��� ����
// Dynamic lib ���
#include "../Dll/Dll_Func.h"
#pragma comment(lib, "../x64/Debug/Dll.lib")
*/

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

    // Dynamic lib���
    /*
    int c = Mul(30, 5);
    cout << c << "\n";
    */


    return 0;
}