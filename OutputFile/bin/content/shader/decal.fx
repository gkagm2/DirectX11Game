#ifndef _DECAL_FX
#define _DECAL_FX
#include "value.fx"

//-----------------------
// Decal Debug shader
// cull mode : wire frame
// blend state : default
// depth stencil : depth test, no write
// render time point : Decal
//-----------------------

struct PS_OUT
{
    float4 vColorTarget : SV_Target;
    float4 vSpecularTarget : SV_Target1;
};

float4 VS_DebugDecal(float3 _vPos : POSITION) : SV_Position
{
    float4 vProjPos = mul(float4(_vPos, 1.f), g_matWorldViewProj);
    return vProjPos;
}

PS_OUT PS_DebugDecal(float4 _vPos : SV_Position)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // TODO (Jang) : 초록색 테스트
    output.vColorTarget = float4(1.f, 0.f, 1.f, 1.f);
    
    return output;
}
#endif