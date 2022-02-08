#ifndef _SKYBOX_FX
#define _SKYBOX_FX

#include "value.fx"

struct VTX_IN
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
    float3 vNormal : NORMAL;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vViewNormal : NORMAL;
    float3 vViewPos : POSITION;
};

VTX_OUT VS_Skybox(VTX_IN _in)
{
    VTX_OUT vtxOut = (VTX_OUT) 0.f;
    return vtxOut;
}

float4 PS_Skybox(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 1.f, 1.f, 1.f);
    return vColor;
}

#endif