#ifndef _TESTSHADER
#define _TESTSHADER

#include "value.fx"
#include "struct.fx"

// Texture
// 해상도
// color

// 그룹 당 스레드 개수 지정
// 1024 제한

// CS 전용 Semantic

// SV_GroupID           : 그룹 인덱스(전체 그룹 중에 몇번재 그룹인지)
// SV_GroupThreadID     : 그룹 내에서의 스레드 인덱스(3차원)
// SV_GroupIndex        : 그룹 내에서의 스레드 인덱스를 1차원으로 표현
// SV_DispatchThreadID  : 전체 스레드 기준, 인덱스 값(3차원)
RWTexture2D<float4> g_rwTex : register(u0); // unordered access

[numthreads(32, 32, 1)]
void CS_Test(int3 _iThreadID : SV_DispatchThreadID)
{
    if(g_int_0 <= _iThreadID.x || g_int_1 <= _iThreadID.y)
        return;
    
    g_rwTex[_iThreadID.xy] = g_vec4_0;
}

#endif