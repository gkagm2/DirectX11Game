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
    int     iLightType;
    float   fRange;
    float   fAngle;
    int     iPad;
};

#endif
