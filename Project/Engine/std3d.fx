#ifndef _STD3D
#define _STD3D
#include "value.fx"

// 임의로 Direction Light를 넣어줌
static float3 g_vLightDir = float3(1.0f, -1.0f, 1.0f);
static float3 g_vLightColor = float3(1.f, 0.0f, 1.f);
static float3 g_vLightReflect = float3(0.4f, 0.4f, 0.4f); // 반사각 강도가 최대일 때 빛을 얼마만큼 넣어줄 것인지
static float3 g_vAmbient = float3(0.15f, 0.15f, 0.15f);

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

///////////////////////////////////////
//     Std3D Shader
#define OutputTex   g_tex_0
#define SkyboxType      g_int_0
///////////////////////////////////////

VTX_OUT VS_Std3D(VTX_IN _in)
{
    VTX_OUT vtxOut = (VTX_OUT) 0.f;
    
    // View Space에서 처리하게 하는 이유는 카메라에 대한 정보도 알 수 있기 때문이다. 카메라 정보를 이용하여 연산을 할 것이다. (View Space의 원점이 카메라 위치임. z축이 카메라가 바라보고 있는 방향임.)
    
    // 정점의 프로젝션 까지 변환한 좌표
    vtxOut.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    // 정점의 뷰스페이스 좌표
    vtxOut.vViewPos = mul(float4(_in.vPosition, 1.f), g_matWorldView).xyz;
    
    // 정점의 뷰스페이스에서 법선벡터(표면 수직방향)
    vtxOut.vViewNormal = mul(float4(_in.vNormal, 0.f), g_matWorldView).xyz;
    vtxOut.vViewNormal = normalize(vtxOut.vViewNormal); // scale값도 적용되어 있으므로 normalize 해줌
    
    vtxOut.vUV = _in.vUV;
    
    
    return vtxOut;
}

float4 PS_Std3D(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.8f, 0.8f, 0.8f, 1.f);
    
     
    // View Space에서 light 방향 벡터
    float3 vLightViewDir = mul(float4(g_vLightDir, 0.f), g_matView).xyz; // g_vLightDir는 월드상에서 방향 벡터이므로 view 행렬만 곱함.
    vLightViewDir = normalize(vLightViewDir);
    float3 vLightViewOpDir = -vLightViewDir; // 빛이 향하고있는 방향의 반대방향
    
    
    // --  램버트 코사인 법칙을 이용하여 난반사광을 구함. --
    // 난반사광 계수
    float fDiffusePow = saturate(dot(vLightViewOpDir, _in.vViewNormal));
    
    // 표면에 반사된 방향벡터를 구한다.
    float3 vReflectDir = vLightViewDir + 2.f * dot(vLightViewOpDir, _in.vViewNormal) * _in.vViewNormal;
    vReflectDir = normalize(vReflectDir);
    
    // 카메라에서 물체의 해당 지점으로(픽셀) 향하는 벡터
    float3 vEyeToPosDir = normalize(_in.vViewPos); 
    
    // 반사각 강도
    float fReflectPow = saturate(dot(vReflectDir, -vEyeToPosDir));
    fReflectPow  = pow(fReflectPow, 20); // 제곱을 하여 하이라이트 범위 조절
    
    
    vColor.xyz = vColor.xyz * g_vLightColor * fDiffusePow // 표면 빛의 진입 강도
                 + g_vLightColor * g_vLightReflect * fReflectPow // 표면에서 반사되어 카메라로 들어오는 빛
                 +  vColor.xyz * g_vAmbient; // 기본적으로 존재하는 최소한의 빛

    //if (g_int_0) // textrue 존재 함
    //{
    //    vColor.xyz = g_tex_0.Sample(Sample_Point, _in.vUV).xyz * vColor.xyz * _in.fLightPow + (vColor.xyz * g_vAmbient);
    //}
    //else // texture 존재 안함
    //{
    //    vColor.xyz =  vColor.xyz * _in.fLightPow + (vColor.xyz * g_vAmbient);
    //}
    return vColor;
}
#endif