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



// ====================
// Decal Shader
// Blend : 0 target : Alpha Blend
//       : 1 target : Add Blend
// DepthStencil : DepthTest, No Write
// Render Time Point : Decal

// g_tex_0 : Position Target
// g_tex_1 : Decal Texture
#define PositionTargetTex g_tex_0
#define DecalTex g_tex_1
#define IsDecalTex bTex_1
// =====================

float4 VS_Decal(float3 _vPos : POSITION) : SV_Position
{
    float4 vProjPos = (float4) 0.f;
        
    vProjPos = mul(float4(_vPos, 1.f), g_matWorldViewProj);
    
    return vProjPos;
}

PS_OUT PS_Decal(float4 _ScreenPos : SV_Position)
{
    PS_OUT output = (PS_OUT) 0.f;
        
    float2 vScreenUV = _ScreenPos.xy / g_vResolution.xy;
    float3 vViewPos = g_tex_0.Sample(Sample_Anisotropic, vScreenUV).xyz;
    
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vDecalLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
        
    if (-0.5f < vDecalLocalPos.x && vDecalLocalPos.x < 0.5f
        && -0.5f < vDecalLocalPos.y && vDecalLocalPos.y < 0.5f
        && -0.5f < vDecalLocalPos.z && vDecalLocalPos.z < 0.5f)
    {
        if (IsDecalTex)
        {
            float2 vDecalUV = float2(vDecalLocalPos.x + 0.5f, 1.f - (vDecalLocalPos.z + 0.5f));
            output.vColorTarget = DecalTex.Sample(Sample_Anisotropic, vDecalUV);
            output.vSpecularTarget = DecalTex.Sample(Sample_Anisotropic, vDecalUV);
        }
        else
            output.vColorTarget = float4(1.f, 0.f, 0.f, 1.f);
    }
    else
        clip(-1);
            
    return output;
}

#endif