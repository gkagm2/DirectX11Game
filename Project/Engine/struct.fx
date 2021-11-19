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
    TLightColor color;
    
    float4  vLightPos;
    float4  vLightDir;
    int     iLightType;
    float   fRange;
    float   fAngle;
    int     iPad;
};

struct TTileInfo
{
    float2 vLeftTop;
    float2 vRightBottom;
    float2 vTileSize;
    float2 vLeftTopUV; // 타일의 좌상단 좌표 UV
    float2 vRightBottomUV; // 타일의 우하단좌표 UV
    float2 vTileSizeUV;
    
    //float2 vLeftTopUV; // 타일의 좌상단 좌표 UV
    //float2 vRightBottomUV; // 타일의 좌하단 좌표 UV
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
    uint m_iSpawnCount;      // 생성할 파티클의 최대 개수
    uint m_iCurActivedCount; // 현재 활성화되어있는 파티클의 개수
    uint m_iPad[2];
};
#endif
