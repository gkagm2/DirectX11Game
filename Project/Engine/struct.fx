#ifndef _STRUCT
#define _STRUCT

struct TLightColor
{
    float4 vDiffuse;
    float4 vSpeculer;
    float4 vAmbient;
};

struct TLightInfo
{
    int iIdx;
    TLightColor color;
    
    float4  vLightPos;
    float4  vLightDir;
    int     iLightType;
    float   fRange;
    float   fAngle;
};

struct TTileInfo
{
    int iTileIdx;
    int3 iTilePad;
};

struct TParticle
{
    float3 vWorldPos;
    float3 vWorldDir;
    
    float fCurTime;
    float fMaxTime;
    int iAlive;
    
    int iGravitiyEnable;
    float fFallTime;
    float fFallStartY;
    float fSpeed; // gravity speed
    int3 particle_padding;
};

struct TSharedParticleData
{
    uint m_iSpawnCount;      // ������ ��ƼŬ�� �ִ� ����
    uint m_iCurActivedCount; // ���� Ȱ��ȭ�Ǿ��ִ� ��ƼŬ�� ����
    uint m_iPad[2];
};
#endif
