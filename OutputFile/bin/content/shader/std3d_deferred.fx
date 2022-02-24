#ifndef _STD3D_DEFERRED
#define _STD3D_DEFERRED
#include "value.fx"
#include "function.fx"

/////////////////////////////
// Render Point : Deferred
#define ColorTex        g_tex_0
#define NormalMapTex    g_tex_1
#define SpecularMapTex  g_tex_2

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

        // 탄젠트 스페이스에서 픽셀(현재 호출된 쉐이더)의 표면으로 전환하기 위한 회전행렬
        float3x3 matRotates =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal
        };
        vViewNormal = normalize(mul(vNormal, matRotates));
    }
    
    
    if (bSpecularMapTex)
    {
        // TODO (Jang) : 
    }
    
    vOutput.vColor.xyz = vObjColor.xyz;
    vOutput.vViewNormal.xyz = vViewNormal;
    vOutput.vViewPos.xyz = _in.vViewPos;
    
    vOutput.vColor.a = 1.f;
    vOutput.vViewNormal.a = 1.f;
    vOutput.vViewPos.a = 1.f;
    
    return vOutput;
}


/////////////////////////////////////
// Merge Shader (최종 장면 완성)
// Mesh : RectMesh
#define ColorTargetTex          g_tex_0
#define DiffuseTargetTex        g_tex_1
#define SpecularTargetTex       g_tex_2
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
    float4 vDiffuse = DiffuseTargetTex.Sample(Sample_Point, _in.vUV);
    float4 vSpecular = SpecularTargetTex.Sample(Sample_Point, _in.vUV);
    
    vOutColor = vColor * vDiffuse + vSpecular;
    vOutColor.a = 1.f;
    return vOutColor;
}
#endif
