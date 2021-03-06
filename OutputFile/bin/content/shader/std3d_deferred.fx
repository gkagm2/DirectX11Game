#ifndef _STD3D_DEFERRED
#define _STD3D_DEFERRED
#include "value.fx"
#include "function.fx"

/////////////////////////////
// Render Point : Deferred
#define ColorTex        g_tex_0 // 물체의 순수 색상
#define NormalMapTex    g_tex_1
#define SpecularMapTex  g_tex_2 // 반사계수

#define bColorTex       bTex_0
#define bNormalMapTex   bTex_1
#define bSpecularMapTex bTex_2
/////////////////////////////

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;

    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_OUT
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};  

VS_OUT VS_Std3D_Deferred(VS_IN _in)
{
    VS_OUT vOutput = (VS_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }
    
    vOutput.vPos = mul(float4(_in.vPos, 1.f), g_matWorldViewProj);
    vOutput.vUV = _in.vUV;

    // 정점의 뷰스페이스 좌표
    vOutput.vViewPos = mul(float4(_in.vPos, 1.f), g_matWorldView).xyz;
    
    // 정점의 뷰스페이스에서의 법선벡터(표면 수직방향)
    vOutput.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWorldView).xyz);
    vOutput.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWorldView).xyz);
    vOutput.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWorldView).xyz);

    return vOutput;
}

struct PS_OUT
{
    float4 vColor       : SV_Target0;
    float4 vViewNormal  : SV_Target1;
    float4 vViewPos     : SV_Target2;
    float4 vData        : SV_Target3;
};

PS_OUT PS_Std3D_Deferred(VS_OUT _in)
{
    PS_OUT vOutput = (PS_OUT) 0.f;

    float3 vObjColor = float3(1.f, 0.f, 1.f);
    if (bColorTex)
        vObjColor = ColorTex.Sample(Sample_Anisotropic, _in.vUV).xyz;

    // 노멀맵이 바인딩 된 경우 
    float3 vViewNormal = _in.vViewNormal;
    if (bNormalMapTex)
    {
        // normal map에 저장되어있던 노멀값(탄젠트 스페이스)
        float3 vNormal = NormalMapTex.Sample(Sample_Anisotropic, _in.vUV).xyz;
        vNormal = vNormal * 2.f - 1.f; // -1 ~ 1 사이로 변환

        // 탄젠트 스페이스에서 픽셀(현재 호출된 쉐이더)의 표면좌표로 전환하기 위한 회전행렬
        float3x3 matRotates =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal
        };
        vViewNormal = normalize(mul(vNormal, matRotates));
    }
    
    // 스펙큘러 텍스쳐 존재 시
    float4 vMtrlCoeff = float4(1.f, 1.f, 1.f, 1.f);
    if (bSpecularMapTex)
    {
        vMtrlCoeff = SpecularMapTex.Sample(Sample_Anisotropic, _in.vUV);
    }
    
    vOutput.vColor.xyz = vObjColor.xyz;
    // 디퍼드 단계에서는 반투명 처리를 하지 않음.
    vOutput.vColor.a = 1.f;
    
    //output.vColor.r = encode(float4(vObjectColor, 1.f));
    vOutput.vViewNormal.xyz = vViewNormal;
    vOutput.vViewPos.xyz = _in.vViewPos;
    
    
    vOutput.vViewNormal.a = 1.f;
    vOutput.vViewPos.a = 1.f;
    
    vOutput.vData.r = encode(vMtrlCoeff);
    
    return vOutput;
}


/////////////////////////////////////
// Merge Shader (최종 장면 완성)
// Mesh : RectMesh
#define ColorTargetTex          g_tex_0
#define DiffuseTargetTex        g_tex_1
#define SpecularTargetTex       g_tex_2
#define ShadowPowTargetTex      g_tex_3
#define DataTargetTex           g_tex_4
/////////////////////////////////////

struct VS_MERGE_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_MERGE_OUT
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};


VS_MERGE_OUT VS_MergeShader(VS_MERGE_IN _in)
{
    VS_MERGE_OUT vOutput = (VS_MERGE_OUT) 0.f;

    vOutput.vPos = float4(_in.vPos * 2.f, 1.f); // projection space로 만듬. 2 size(-1 ~ 1)
    vOutput.vUV = _in.vUV;
    
    return vOutput;
}

float4 PS_MergeShader(VS_MERGE_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    float4 vColor = ColorTargetTex.Sample(Sample_Point, _in.vUV);
    //float4 vColor = decode(ColorTargetTex.Sample(Sample_Point, _in.vUV).r);
    float4 vDiffuse_Ambi = DiffuseTargetTex.Sample(Sample_Point, _in.vUV); // diffuse와 ambient가 섞여있음.
    float4 vSpecular = SpecularTargetTex.Sample(Sample_Point, _in.vUV);
    float fShadowPow = ShadowPowTargetTex.Sample(Sample_Point, _in.vUV).r;
    float4 vMtrlCoeff = decode(DataTargetTex.Sample(Sample_Point, _in.vUV).r);
    
    if (0.f == vColor.a) // 알파가 0일 경우 빛에 영향받지 않음
    {
        vOutColor = vColor;
    }
    else if (0.f == fShadowPow)
    {
        //vOutColor = vColor * vDiffuse_Ambi + vSpecular;
        vOutColor = vColor * vDiffuse_Ambi + (vSpecular * vMtrlCoeff);
        vOutColor.a = 1.f;
    }
    else
    {
        float fRatio = saturate(1.f - fShadowPow);
        //vOutColor = (vColor * vDiffuse_Ambi + vSpecular) * fRatio;
        vOutColor = (vColor * vDiffuse_Ambi + (vSpecular * vMtrlCoeff)) * fRatio;
        vOutColor.a = 1.f;
    }
    
    return vOutColor;
}
#endif