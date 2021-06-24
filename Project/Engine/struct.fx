#ifndef _STRUCT
#define _STRUCT

struct TLightColor
{
    float4 vDiff;
    float4 vSpec;
    float4 vAmb;
};

struct TLightInfo
{
    TLightColor color;
    
    int     iLightType;
    float   fRange;
    float   fAngle;
    int     iPad;
};

#endif
