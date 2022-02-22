#ifndef _STD3D
#define _STD3D
#include "value.fx"
#include "function.fx"

///////////////////////////////////////
//     Std3D Shader
#define DiffTex             g_tex_0     // color texture
#define NormalTex           g_tex_1     // normal map texture
#define bIsBindDiffTex      bTex_0
#define bIsBindNormalTex    bTex_1
#define CubeTex             g_texcube_0
#define bIsBindCubeTex      bTexCube_0
#define EnvironmentCubeTex      g_texcube_1 // 환경광 텍스쳐
#define bIsBindEnvironmentCubeTex bTexCube_1

///////////////////////////////////////

struct VTX_IN
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vLocalPos : POSITION1;
    
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

VTX_OUT VS_Std3D(VTX_IN _in)
{
    VTX_OUT vtxOut = (VTX_OUT) 0.f;
    
    // View Space에서 처리하게 하는 이유는 카메라에 대한 정보도 알 수 있기 때문이다. 카메라 정보를 이용하여 연산을 할 것이다. (View Space의 원점이 카메라 위치임. z축이 카메라가 바라보고 있는 방향임.)
    
    // Cube UV값 계산을 위해 설정.
    vtxOut.vLocalPos = _in.vPosition;
    
    // 정점의 프로젝션 까지 변환한 좌표
    vtxOut.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    // 정점의 뷰스페이스 좌표
    vtxOut.vViewPos = mul(float4(_in.vPosition, 1.f), g_matWorldView).xyz;
    
    // 정점의 뷰스페이스에서 법선벡터(표면 수직방향)
    vtxOut.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWorldView).xyz);
    vtxOut.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWorldView).xyz);
    vtxOut.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWorldView).xyz);
    
    vtxOut.vUV = _in.vUV;
    
    return vtxOut;
}

float4 PS_Std3D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.8f, 0.8f, 0.8f, 1.f);
    
    if (bIsBindDiffTex)
    {
        vOutColor.xyz = DiffTex.Sample(Sample_Anisotropic, _in.vUV).xyz;
    }
    else if (bIsBindCubeTex)
    {
        float3 vUV = normalize(_in.vLocalPos);
        vOutColor.xyz = CubeTex.Sample(Sample_Anisotropic, vUV).xyz;
    }
    
    // 노멀맵이 바인딩되어있으면 노멀맵의 노멀을 쓴다.
    float3 vViewNormal = _in.vViewNormal;
    if (bIsBindNormalTex)
    {
        // Normal map에 저장되어있던 노멀값(탄젠트 스페이스)
        float3 vNormal = NormalTex.Sample(Sample_Anisotropic, _in.vUV).rgb;
        vNormal = vNormal * 2.f - 1.f; // 0 ~ 1 사이의 값으로 변환
        
        // 탄젠트 스페이스에서 픽셀(현재 호출된 쉐이더)의 표면으로 전환하기 위한 회전행렬
        float3x3 matRotate =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal
        };
        
        vViewNormal = normalize(mul(vNormal, matRotate));
    }
     
    TLightColor tFinalLight = (TLightColor) 0.f;
    
    for (int i = 0; i < g_iLight3DCount; ++i)
    {
        TLightColor tCurLight = CalLight3D(i, _in.vViewPos, vViewNormal);
        tFinalLight.vDiffuse += tCurLight.vDiffuse;
        tFinalLight.vSpeculer += tCurLight.vSpeculer;
        tFinalLight.vAmbient += tCurLight.vAmbient;
    }
    
    //float4 vEnvironmentColor = (float4) 0.f;
    //if (bIsBindEnvironmentCubeTex)
    //{
    //    float3 vEyeReflect = normalize(reflect(vEyeToPosDir, vViewNormal));
    //    vEyeReflect = normalize(mul(float4(vEyeReflect, 0), g_matViewInv).xyz);
        
       
    //    vEnvironmentColor = EnvironmentCubeTex.Sample(Sample_Anisotropic, vEyeReflect);
    //    vOutColor.xyz += (vOutColor.xyz * vEnvironmentColor.xyz * 0.6f);
    //}
        
    vOutColor.xyz = vOutColor.xyz * tFinalLight.vDiffuse.xyz // 표면 빛의 진입 강도
                 + tFinalLight.vSpeculer.xyz // 표면에서 반사되어 카메라로 들어오는 빛
                 + vOutColor.xyz * tFinalLight.vAmbient.xyz; // 기본적으로 존재하는 최소한의 빛(환경광)
    
    return vOutColor;
}
#endif