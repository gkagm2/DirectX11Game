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
    float4 vProjPos = (float4) 0.f;
    vProjPos = mul(float4(_vPos, 1.f), g_matWorldViewProj);
    return vProjPos;
}

PS_OUT PS_DebugDecal(float4 _vPos : SV_Position)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    output.vColorTarget = float4(0.2f, 0.8f, 0.2f, 0.f); // Alpha를 0으로 설정하여 
    
    return output;
}



// ====================
// Decal Shader
// Blend : 0 target : Alpha Blend (0번타겟 한정)
//       : 1 target : Add Blend (가산혼합 블랜딩)
// DepthStencil : DepthTest, No Write (깊이 체크 받고, 면을 다 채워서 호출하게되므로 데칼 체크하려고 배치한것임 실제 존재하지는 않음. 데칼물체 자체에 깊이를 남기지 않음.)
// Render Time Point : Decal

// g_tex_0 : Position Target
// g_tex_1 : Decal Texture
#define PositionTargetTex g_tex_0
#define DecalTex g_tex_1
#define IsDecalTex bTex_1
#define Lighting g_int_0
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

    // 육면체 Local Space기준으로 검사.
    if (-0.5f < vDecalLocalPos.x && vDecalLocalPos.x < 0.5f
        && -0.5f < vDecalLocalPos.y && vDecalLocalPos.y < 0.5f
        && -0.5f < vDecalLocalPos.z && vDecalLocalPos.z < 0.5f)
    {
        if (IsDecalTex)
        {
            // 육면체 -0.5 ~ 0.5 범위를 UV로 변환 (x,z 평면 기준)
            float2 vDecalUV = float2(vDecalLocalPos.x + 0.5f, 1.f - (vDecalLocalPos.z + 0.5f));
            
            output.vColorTarget = DecalTex.Sample(Sample_Anisotropic, vDecalUV);
            if (Lighting)
                output.vSpecularTarget = DecalTex.Sample(Sample_Anisotropic, vDecalUV);    
        }
        else
            output.vColorTarget = float4(1.f, 0.f, 0.f, 1.f);
    }
    else
    {
        clip(-1);
    }
            
    return output;
}

#endif