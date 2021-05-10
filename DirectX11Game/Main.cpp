#include <iostream>
#include <Windows.h>
/* // Static 프로젝트 빌드 후 아래와 같은 방식으로 사용 가능
// Static lib 방식
#include "../StaticLib/Static_Func.h"
#pragma comment(lib, "../x64/Debug/StaticLib.lib")
*/

/* Dll 프로젝트 빌드 후 아래와 같은 방식으로 사용 가능
// Dynamic lib 방식
#include "../Dll/Dll_Func.h"
#pragma comment(lib, "../x64/Debug/Dll.lib")
*/

using namespace std;

int main()
{
    // Static lib방식
    /*
    int a = Add(10, 30);
    cout << a << "\n";

    int b = Minus(20, 40);
    cout << b << "\n";
    */

    // Dynamic lib방식
    /*
    int c = Mul(30, 5);
    cout << c << "\n";
    */


    return 0;
}