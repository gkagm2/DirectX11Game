#ifndef _SKYBOX_FX
#define _SKYBOX_FX

#include "value.fx"

///////////////////////////////
// Skybox

#define OutputTex g_tex_0            // sphere ��� �ؽ���
#define OutputCubeTex g_texcube_0    // cube ��� �ؽ���
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
    
    // ���� ������ �� �������� ����, �� ����� ȸ���� �����Ѵ�. (ī�޶� �ٶ󺸰� �ִ� ������ ��ī�̹ڽ��� z�� �տ� ��ġ�ϱ� ����)
    float3 vViewPos = mul(float4(_in.vPosition, 0.f), g_matView).xyz;
   
    // ������Ų��.
    float4 vProjPos = mul(float4(vViewPos, 1.f), g_matProjection);
    
    /*
    V : View Space
    ProjMat : Projection Matrix
    (Vx, Vy, Vz) * ProjMat = (Proj.x * Vz, proj.y * Vz, Proj.z * Vz, Vz);
    
    vProjPos�� �� ��Ҹ��� vViewPos.z�� ����������. �����Ͷ����� �ܰ迡�� vViewPos.w�� ���������Ͽ� z��(depth)�� 1�� ����� ���� �ָ� �ֵ��� �����. ���� z�� 1�� ��������� vViewPos.z�� ��(vProjPos.w)�� �־���. 
    */
    vProjPos.z = vProjPos.w;
    
    vtxOut.vPosition = vProjPos;
    vtxOut.vUV = _in.vUV;
    
    vtxOut.vLocalPos = _in.vPosition; // CubeTexture ���ø� UV �뵵
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