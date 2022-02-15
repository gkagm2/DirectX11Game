#ifndef _FUNCTION
#define _FUNCTION
#include "value.fx"



TLightColor CalLight3D(int _iLightIdx, float3 _vViewPos, float3 _vViewNormal)
{
    TLightColor tLight = (TLightColor) 0.f;
    
    // View Space���� light ���� ����
    float3 vLightViewDir = mul(float4(g_Light3DBuffer[_iLightIdx].vLightDir.xyz, 0.f), g_matView).xyz; // g_vLightDir�� ����󿡼� ���� �����̹Ƿ� view ��ĸ� ����.
    vLightViewDir = normalize(vLightViewDir);
    float3 vLightViewOpDir = -vLightViewDir; // ���� �ݴ����
    
    // --  ����Ʈ �ڻ��� ��Ģ�� �̿��Ͽ� ���ݻ籤�� ����. --
    // ���ݻ籤(Ȯ�걤) ���
    float3 fDiffusePow = saturate(dot(vLightViewOpDir, _vViewNormal));
    
    // �ݻ籤 ���. �ݻ纤�� ����
    float3 vReflectDir = vLightViewDir + 2.f * dot(vLightViewOpDir, _vViewNormal) * _vViewNormal;
    vReflectDir = normalize(vReflectDir);
    
    // ī�޶󿡼� �ش� ��������(�ȼ�) ���ϴ� ����
    float3 vEyeToPosDir = normalize(_vViewPos);
    float fReflectPow = saturate(dot(-vReflectDir, vEyeToPosDir));
    fReflectPow = pow(fReflectPow, 20); // �����Ͽ� ���̶���Ʈ ���� ����
    
    tLight.vDiffuse.xyz = g_Light3DBuffer[_iLightIdx].color.vDiffuse.xyz * fDiffusePow;
    tLight.vSpeculer.xyz = g_Light3DBuffer[_iLightIdx].color.vSpeculer.xyz * fReflectPow;
    tLight.vAmbient.xyz = g_Light3DBuffer[_iLightIdx].color.vAmbient.xyz;
    return tLight;
}

static float gaussian5x5[25] =
{
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.023792, 0.094907, 0.150342, 0.094907, 0.023792,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
};

float gaussian5x5Sample(in int2 _uv, in Texture2D _tex)
{
    float4 fOut = (float4) 0.f;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            fOut += _tex[_uv + int2(i - 2, j - 2)] * gaussian5x5[i * 5 + j];
        }
    }
    return fOut.x;
}

// 0 ~ 1 ���� ����
float Rand(in float _fKey)
{
    // ���� ���� ��ġ, ����    
    float2 vUV = float2(_fKey + g_fAccTime, g_fAccTime);
    vUV.y += sin(vUV.x * 2 * 3.141592);
    
    if (vUV.x > 0)
        vUV.x = frac(vUV.x);
    else
        vUV.x = 1.f - abs(vUV.x);
    if (vUV.y > 0)
        vUV.y = frac(vUV.y);
    else
        vUV.y = 1.f - abs(vUV.y);
    vUV = vUV * g_vNoiseResolution;
    
    return gaussian5x5Sample(vUV, g_TexNoise);
}


#endif