#ifndef _SHADOW_FX
#define _SHADOW_FX

#include "value.fx"

struct VS_OUT
{
    float3 vProjPos : POSITION;
    float4 vPosition : SV_Position;
};

VS_OUT VS_ShadowDepth(float3 _vPos : POSITION)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_vPos, 1.f), g_matWorldViewProj);
    output.vProjPos = output.vPosition.xyz / output.vPosition.w; // 진짜 투영좌표를 구하기 위해 w로 나눔
    
    return output;
}

float PS_ShadowDepth(VS_OUT _in) : SV_Target
{
    return _in.vProjPos.z;
}

#endif