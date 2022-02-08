#ifndef _STD3D
#define _STD3D
#include "value.fx"

// ���Ƿ� Direction Light�� �־���
static float3 g_vLightDir = float3(1.0f, -1.0f, 1.0f);
static float3 g_vLightColor = float3(1.f, 0.0f, 1.f);
static float3 g_vLightReflect = float3(0.4f, 0.4f, 0.4f); // �ݻ簢 ������ �ִ��� �� ���� �󸶸�ŭ �־��� ������
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
    
    // View Space���� ó���ϰ� �ϴ� ������ ī�޶� ���� ������ �� �� �ֱ� �����̴�. ī�޶� ������ �̿��Ͽ� ������ �� ���̴�. (View Space�� ������ ī�޶� ��ġ��. z���� ī�޶� �ٶ󺸰� �ִ� ������.)
    
    // ������ �������� ���� ��ȯ�� ��ǥ
    vtxOut.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    // ������ �佺���̽� ��ǥ
    vtxOut.vViewPos = mul(float4(_in.vPosition, 1.f), g_matWorldView).xyz;
    
    // ������ �佺���̽����� ��������(ǥ�� ��������)
    vtxOut.vViewNormal = mul(float4(_in.vNormal, 0.f), g_matWorldView).xyz;
    vtxOut.vViewNormal = normalize(vtxOut.vViewNormal); // scale���� ����Ǿ� �����Ƿ� normalize ����
    
    vtxOut.vUV = _in.vUV;
    
    
    return vtxOut;
}

float4 PS_Std3D(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.8f, 0.8f, 0.8f, 1.f);
    
     
    // View Space���� light ���� ����
    float3 vLightViewDir = mul(float4(g_vLightDir, 0.f), g_matView).xyz; // g_vLightDir�� ����󿡼� ���� �����̹Ƿ� view ��ĸ� ����.
    vLightViewDir = normalize(vLightViewDir);
    float3 vLightViewOpDir = -vLightViewDir; // ���� ���ϰ��ִ� ������ �ݴ����
    
    
    // --  ����Ʈ �ڻ��� ��Ģ�� �̿��Ͽ� ���ݻ籤�� ����. --
    // ���ݻ籤 ���
    float fDiffusePow = saturate(dot(vLightViewOpDir, _in.vViewNormal));
    
    // ǥ�鿡 �ݻ�� ���⺤�͸� ���Ѵ�.
    float3 vReflectDir = vLightViewDir + 2.f * dot(vLightViewOpDir, _in.vViewNormal) * _in.vViewNormal;
    vReflectDir = normalize(vReflectDir);
    
    // ī�޶󿡼� ��ü�� �ش� ��������(�ȼ�) ���ϴ� ����
    float3 vEyeToPosDir = normalize(_in.vViewPos); 
    
    // �ݻ簢 ����
    float fReflectPow = saturate(dot(vReflectDir, -vEyeToPosDir));
    fReflectPow  = pow(fReflectPow, 20); // ������ �Ͽ� ���̶���Ʈ ���� ����
    
    
    vColor.xyz = vColor.xyz * g_vLightColor * fDiffusePow // ǥ�� ���� ���� ����
                 + g_vLightColor * g_vLightReflect * fReflectPow // ǥ�鿡�� �ݻ�Ǿ� ī�޶�� ������ ��
                 +  vColor.xyz * g_vAmbient; // �⺻������ �����ϴ� �ּ����� ��

    //if (g_int_0) // textrue ���� ��
    //{
    //    vColor.xyz = g_tex_0.Sample(Sample_Point, _in.vUV).xyz * vColor.xyz * _in.fLightPow + (vColor.xyz * g_vAmbient);
    //}
    //else // texture ���� ����
    //{
    //    vColor.xyz =  vColor.xyz * _in.fLightPow + (vColor.xyz * g_vAmbient);
    //}
    return vColor;
}
#endif