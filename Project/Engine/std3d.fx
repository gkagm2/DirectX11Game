#ifndef _STD3D
#define _STD3D
#include "value.fx"

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
    float3 vNormal : NORMAL;
    float4 vColor : COLOR;
};

VTX_OUT VS_Std3D(VTX_IN _in)
{
    VTX_OUT vtxOut = (VTX_OUT) 0.f;
    vtxOut.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    vtxOut.vUV = _in.vUV;
    
    vtxOut.vColor = _in.vColor;
    
    return vtxOut;
}

float4 PS_Std3D(VTX_OUT _in) : SV_Target
{
    float4 outColor = float4(0.9f, 0.1f, 0.1f, 1.f);
    outColor = _in.vColor;
    return outColor;
}
#endif