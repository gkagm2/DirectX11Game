#ifndef _STD3D
#define _STD3D
#include "value.fx"

// 임의로 Direction Light를 넣어줌
static float3 g_vLightDir = float3(1.0f, -1.0f, 1.0f);
static float3 g_vLightColor = float3(1.f, 1.f, 1.f);
static float3 g_vLightReflect = float3(0.4f, 0.4f, 0.4f);
static float3 g_vAmbient = float3(0.15f, 0.15f, 0.15f);

struct VTX_IN
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
    float3 vNormal : NORMAL;
    float4 vColor : COLOR;
    
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vViewNormal : NORMAL;
    float3 vViewPos : POSITION;
    float4 vColor : COLOR;
    float fLightPow : FOG;
};

VTX_OUT VS_Std3D(VTX_IN _in)
{
    VTX_OUT vtxOut = (VTX_OUT) 0.f;
    vtxOut.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    vtxOut.vUV = _in.vUV;
    
    vtxOut.vColor = _in.vColor;
    
    
    float3 vLightDir = -normalize(g_vLightDir);
    float3 vVtxWorldNormal = mul(float4(_in.vNormal, 0.f), g_matWorld).xyz;
    vVtxWorldNormal = normalize(vVtxWorldNormal);
    vtxOut.fLightPow = dot(vLightDir, vVtxWorldNormal);
    vtxOut.fLightPow = saturate(vtxOut.fLightPow);
    
    return vtxOut;
}

float4 PS_Std3D(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.8f, 0.8f, 0.8f, 1.f);

    if (g_int_0) // textrue 존재 함
    {
        vColor.xyz = g_tex_0.Sample(Sample_Point, _in.vUV).xyz * vColor.xyz * _in.fLightPow + (vColor.xyz * g_vAmbient);
    }
    else // texture 존재 안함
    {
        vColor.xyz =  vColor.xyz * _in.fLightPow + (vColor.xyz * g_vAmbient);
    }
    vColor.w = 1.f;
    return vColor;
}
#endif