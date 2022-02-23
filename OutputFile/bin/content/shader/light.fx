#ifndef _LIGHT_FX
#define _LIGHT_FX
#include "value.fx"
#include "function.fx"

///////////////////////////////////
// Direction Light Shader
#define ViewNormalTargetTex g_tex_0
#define ViewPosTargetTex    g_tex_1
#define iLightIdx            g_int_0
///////////////////////////////////


struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPos : SV_Position;
};

struct PS_OUT
{
    float4 vDiffuseLight : SV_Target0;
    float4 vSpecularLight : SV_Target1;
    float4 vShadow : SV_Target2;
};

VS_OUT VS_DirLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    //vOutput.vPos = mul(float4(_in.vPos, 1.f), g_matWorldViewProj);
    output.vPos = float4(_in.vPos.xy * 2.f, 0.f, 1.f); // 스크린 좌표 전체
    return output;
}

PS_OUT PS_DirLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float2 vUV = _in.vPos.xy / g_vResolution;
    float3 vViewPos = ViewPosTargetTex.Sample(Sample_Point, vUV).xyz;
    if (0.f == vViewPos.z)
    {
        clip(-1);
    }

    float3 vViewNormal = ViewNormalTargetTex.Sample(Sample_Point, vUV).xyz;
    TLightColor color = CalLight3D(iLightIdx, vViewPos, vViewNormal);
    
    output.vDiffuseLight.xyz = color.vDiffuse.xyz + color.vAmbient.xyz;
    output.vSpecularLight.xyz = color.vSpeculer.xyz;
    output.vDiffuseLight.w = 1.f;
    output.vSpecularLight.w = 1.f;
    //output.vShadow 
    return output;
}
#endif