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
#define EnvironmentCubeTex      g_texcube_1 // ȯ�汤 �ؽ���
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
    
    // View Space���� ó���ϰ� �ϴ� ������ ī�޶� ���� ������ �� �� �ֱ� �����̴�. ī�޶� ������ �̿��Ͽ� ������ �� ���̴�. (View Space�� ������ ī�޶� ��ġ��. z���� ī�޶� �ٶ󺸰� �ִ� ������.)
    
    // Cube UV�� ����� ���� ����.
    vtxOut.vLocalPos = _in.vPosition;
    
    // ������ �������� ���� ��ȯ�� ��ǥ
    vtxOut.vPosition = mul(float4(_in.vPosition, 1.f), g_matWorldViewProj);
    // ������ �佺���̽� ��ǥ
    vtxOut.vViewPos = mul(float4(_in.vPosition, 1.f), g_matWorldView).xyz;
    
    // ������ �佺���̽����� ��������(ǥ�� ��������)
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
    
    // ��ָ��� ���ε��Ǿ������� ��ָ��� ����� ����.
    float3 vViewNormal = _in.vViewNormal;
    if (bIsBindNormalTex)
    {
        // Normal map�� ����Ǿ��ִ� ��ְ�(ź��Ʈ �����̽�)
        float3 vNormal = NormalTex.Sample(Sample_Anisotropic, _in.vUV).rgb;
        vNormal = vNormal * 2.f - 1.f; // 0 ~ 1 ������ ������ ��ȯ
        
        // ź��Ʈ �����̽����� �ȼ�(���� ȣ��� ���̴�)�� ǥ������ ��ȯ�ϱ� ���� ȸ�����
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
        
    vOutColor.xyz = vOutColor.xyz * tFinalLight.vDiffuse.xyz // ǥ�� ���� ���� ����
                 + tFinalLight.vSpeculer.xyz // ǥ�鿡�� �ݻ�Ǿ� ī�޶�� ������ ��
                 + vOutColor.xyz * tFinalLight.vAmbient.xyz; // �⺻������ �����ϴ� �ּ����� ��(ȯ�汤)
    
    return vOutColor;
}
#endif