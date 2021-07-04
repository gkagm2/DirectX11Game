#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE
#include "value.fx"
#include "struct.fx"

// ----------------------
// Particle Update Shader
#define iParticleCount g_int_0
#define fStartSpeed g_float_0
#define fEndSpeed g_float_1

// ----------------------

RWStructuredBuffer<TParticle> g_particle : register(u0);


[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(int3 _iThreadID : SV_DispatchThreadID)
{
    int id = _iThreadID.x;
    // ��� ��ƼŬ�� ��Ȱ��ȭ ���¸� ����
    if (iParticleCount <= id || !g_particle[id].iAlive)
        return;
    
    g_particle[id].fCurTime += g_fDeltaTime;
    
    // �ð� �ʰ� �� ����
    if (g_particle[id].fMaxTime < g_particle[id].fCurTime)
    {
        g_particle[id].iAlive = 0;
        return;
    }
    
    float fLifeRatio = g_particle[id].fCurTime / g_particle[id].fMaxTime;
    float fCurSpeed = (fEndSpeed - fStartSpeed) * fLifeRatio + fStartSpeed;
    
    g_particle[id].vWorldPos += g_particle[id].vWorldDir * fCurSpeed * g_fDeltaTime;
}
#endif