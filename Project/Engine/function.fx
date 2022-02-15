#ifndef _FUNCTION
#define _FUNCTION
#include "value.fx"

static const int direction_Type = 0;
static const int point_Type = 1;
static const int spot_Type = 2;
static const int reflect_Pow = 20;



TLightColor CalLight3D(int _iLightIdx, float3 _vViewPos, float3 _vViewNormal)
{
    TLightColor tLight = (TLightColor) 0.f;
    TLightInfo tInfo = g_Light3DBuffer[_iLightIdx];
    
    float fDiffusePow = 0.f;
    float fReflectPow = 0.f;
    
    float3 vLightViewPos = mul(float4(tInfo.vLightPos.xyz, 1.f), g_matView).xyz;
    
    // �������� �Ÿ��� ���� ���� ����
    float fRatio = 1.f;
    
    if (direction_Type == tInfo.iLightType)
    {
        // View Space���� light ���� ����
        float3 vLightViewDir = mul(float4(tInfo.vLightDir.xyz, 0.f), g_matView).xyz; // g_vLightDir�� ����󿡼� ���� �����̹Ƿ� view ��ĸ� ����.
        vLightViewDir = normalize(vLightViewDir);
        float3 vLightViewOpDir = -vLightViewDir; // ���� �ݴ����
        
        // --  ����Ʈ �ڻ��� ��Ģ�� �̿��Ͽ� ���ݻ籤�� ����. --
        // ���ݻ籤(Ȯ�걤) ���
        fDiffusePow = saturate(dot(vLightViewOpDir, _vViewNormal));
        
        // �ݻ籤 ���. �ݻ纤�� ����
        float3 vReflectDir = vLightViewDir + 2.f * dot(vLightViewOpDir, _vViewNormal) * _vViewNormal;
        vReflectDir = normalize(vReflectDir);
        
        // ī�޶󿡼� �ش� ��������(�ȼ�) ���ϴ� ����
        float3 vEyeToPosDir = normalize(_vViewPos);
        fReflectPow = saturate(dot(-vReflectDir, vEyeToPosDir));
        fReflectPow = pow(fReflectPow, reflect_Pow); // �����Ͽ� ���̶���Ʈ ���� ����
    }
    else if (point_Type == tInfo.iLightType)
    {
        // View space���� �������κ��� ǥ�鿡 ���� ����� �Ÿ��� ����
        float3 vLightViewDir = _vViewPos - vLightViewPos;
        float fDistance = length(vLightViewDir);
        vLightViewDir = normalize(vLightViewDir);
        float3 vLightViewOpDir = -vLightViewDir; // ǥ�鿡�� ������ ���ϴ� ����
        
        
        // ����籤(Ȯ�걤) ���
        float fDiffusePow = saturate(dot(vLightViewOpDir, _vViewNormal));
        
        // �ݻ籤 ���, �ݻ纤�� 
        float3 vReflectDir = vLightViewDir + 2.f * dot(vLightViewOpDir, _vViewNormal) * _vViewNormal;
        vReflectDir = normalize(vReflectDir);
        
        // ī�޶󿡼� �ش� ��������(�ȼ�) ���ϴ� ����
        float3 vEyeToPosDir = normalize(_vViewPos);
        fReflectPow = saturate(dot(-vReflectDir, vEyeToPosDir));
        fReflectPow = pow(fReflectPow, reflect_Pow);
        
        fRatio = saturate(cos((fDistance / tInfo.fRange)) * (PI / 2.f));
    }
    else if (spot_Type == tInfo.iLightType)
    {
        // TODO (Jang) : spot light ����
    }
    
    tLight.vDiffuse.xyz = tInfo.color.vDiffuse.xyz * fDiffusePow * fRatio;
    tLight.vSpeculer.xyz = tInfo.color.vSpeculer.xyz * fReflectPow * fRatio;
    tLight.vAmbient.xyz = tInfo.color.vAmbient.xyz * fRatio;
        
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