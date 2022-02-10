#ifndef _SKYBOX_FX
#define _SKYBOX_FX

#include "value.fx"

///////////////////////////////
// Skybox

#define OutputTex g_tex_0            // sphere 출력 텍스쳐
#define OutputCubeTex g_texcube_0    // cube 출력 텍스쳐
#define bIsBindCubeTex bTexCube_0
#define bIsBindTex  bTex_0

#define MeshType g_int_0 // 0 : Cube, 1 : Sphere

///////////////////////////////

struct VTX_SKYBOX_IN
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_SKYBOX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vLocalPos : POSITION;
};

VTX_SKYBOX_OUT VS_Skybox(VTX_SKYBOX_IN _in)
{
    VTX_SKYBOX_OUT vtxOut = (VTX_SKYBOX_OUT) 0.f;
    
    // 로컬 공간을 뷰 공간으로 보고, 뷰 행렬의 회전만 적용한다. (카메라가 바라보고 있는 방향의 스카이박스를 z축 앞에 배치하기 위함)
    float3 vViewPos = mul(float4(_in.vPosition, 0.f), g_matView).xyz;
   
    // 투영시킨다.
    float4 vProjPos = mul(float4(vViewPos, 1.f), g_matProjection);
    
    /*
    V : View Space
    ProjMat : Projection Matrix
    (Vx, Vy, Vz) * ProjMat = (Proj.x * Vz, proj.y * Vz, Proj.z * Vz, Vz);
    
    vProjPos의 각 요소마다 vViewPos.z가 곱해져있음. 레스터라이져 단계에서 vViewPos.w를 나눠지게하여 z값(depth)을 1로 만들어 가장 멀리 있도록 만든다. 따라서 z를 1로 만들기위해 vViewPos.z의 값(vProjPos.w)을 넣어줌. 
    */
    vProjPos.z = vProjPos.w;
    
    vtxOut.vPosition = vProjPos;
    vtxOut.vUV = _in.vUV;
    
    vtxOut.vLocalPos = _in.vPosition; // CubeTexture 샘플링 UV 용도
    return vtxOut;
}

float4 PS_Skybox(VTX_SKYBOX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.7f, 0.7f, 0.7f, 1.f);
   
    if (MeshType == 0 /*Cube*/) 
    {
        if (bIsBindCubeTex)
        {
            float3 vUV = normalize(_in.vLocalPos);
            vColor = OutputCubeTex.Sample(Sample_Anisotropic, vUV);
        }
        else
        {
        }
    }
    else if (MeshType == 1 /*Sphere*/) 
    {
        if (bIsBindTex)
        {
            vColor = OutputTex.Sample(Sample_Anisotropic, _in.vUV);
        }
        else
        {
        }
    }
    return vColor;
}

#endif