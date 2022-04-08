#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"

// --------------------
// Particle Render Shader
// Mesh : point mesh
#define vStartColor g_vec4_0.xyz
#define vEndColor   g_vec4_1.xyz

#define vStartScale g_vec4_2.xyz
#define vEndScale   g_vec4_3.xyz

#define particleTex g_tex_0
// --------------------

StructuredBuffer<TParticle> g_particle : register(t12);

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID; // 인스턴싱 ID 
};

struct VS_OUT
{
    float3 vViewPos : POSITION;
    float2 vUV : TEXCOORD;
    float fInstID : FOG; // 인스턴싱 ID
};

VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float3 vWorldPos = g_particle[_in.iInstID].vWorldPos;
    output.vViewPos = mul(float4(vWorldPos, 1.f), g_matView).xyz;
    // TODO (Jang) : Rot, Scale 완성하기
    
    output.vUV = _in.vUV;
    output.fInstID = _in.iInstID;
    
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float fInst : FOG;
};

[maxvertexcount(6)]
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _outputStream)
{
    if (0 == g_particle[(int) _in[0].fInstID].iAlive)
        return;
    
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    
    float fRatio = g_particle[(int)_in[0].fInstID].fCurTime / g_particle[(int)_in[0].fInstID].fMaxTime;
    float3 fCurScale = (vStartScale + (vEndScale - vStartScale) * fRatio) * 0.5f;
    
    // 0  1
    //
    // 3  2
    
    output[0].vPosition = mul(float4(_in[0].vViewPos + float3(-fCurScale.x, fCurScale.y, 0.f), 1.f), g_matProjection);
    output[1].vPosition = mul(float4(_in[0].vViewPos + float3(fCurScale.x, fCurScale.y, 0.f), 1.f), g_matProjection);
    output[2].vPosition = mul(float4(_in[0].vViewPos + float3(fCurScale.x, -fCurScale.y, 0.f), 1.f), g_matProjection);
    output[3].vPosition = mul(float4(_in[0].vViewPos + float3(-fCurScale.x, -fCurScale.y, 0.f), 1.f), g_matProjection);
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    output[0].fInst = _in[0].fInstID;
    output[1].fInst = _in[0].fInstID;
    output[2].fInst = _in[0].fInstID;
    output[3].fInst = _in[0].fInstID;
    
    _outputStream.Append(output[0]);
    _outputStream.Append(output[1]);
    _outputStream.Append(output[2]);
    _outputStream.RestartStrip();
    
    _outputStream.Append(output[0]);
    _outputStream.Append(output[2]);
    _outputStream.Append(output[3]);
    
    _outputStream.RestartStrip(); // 모양 설정
}

float4 PS_Particle(GS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    float fRatio = g_particle[(int) _in.fInst].fCurTime / g_particle[(int) _in.fInst].fMaxTime;
    float3 vCurColor = vStartColor + (vEndColor - vStartColor) * fRatio;
    
    vOutColor = particleTex.Sample(Sample_Anisotropic, _in.vUV);
    vOutColor.rgb *= vCurColor;
    return vOutColor;
}

#endif