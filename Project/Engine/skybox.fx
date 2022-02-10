#ifndef _SKYBOX_FX
#define _SKYBOX_FX

#include "value.fx"

///////////////////////////////
// Skybox
#define OutputTex g_tex_0
#define bIsBindTex0  bTex_0

#define MeshType g_int_0
///////////////////////////////

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
    vtxOut.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    vtxOut.vUV = _in.vUV;
    return vtxOut;
}

float4 PS_Skybox(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    if (bIsBindTex0)
    {
        vColor = OutputTex.Sample(Sample_Point, _in.vUV);

    }
    else
    {
    }
    return vColor;
}

#endif