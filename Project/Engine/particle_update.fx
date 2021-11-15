#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE
#include "value.fx"
#include "struct.fx"
#include "function.fx"

#define	GRAVITY	9.8f

// ----------------------
// Particle Update Shader
#define iParticleMaxCount   g_int_0
#define iParticleShape      g_int_1
#define iGravityEnable      g_int_2
#define fStartSpeed         g_float_0
#define fEndSpeed           g_float_1

#define fMinLifeTime        g_float_2
#define fMaxLifeTime        g_float_3

#define vObjectPos          g_vec4_0
#define vRadius             g_vec4_1
// ----------------------

RWStructuredBuffer<TParticle> g_particle : register(u0);
RWStructuredBuffer<TSharedParticleData> g_sharedParticle : register(u1);


// 0 ~ 1 사이 랜덤
void ParticleFunc(inout RWStructuredBuffer<TParticle> tParticle, in int3
_iThreadID)
{
    if (iParticleShape == 0)
    {
        // 랜덤 포지션 및 방향 세팅            
        float fKey = (float) _iThreadID.x / (float) iParticleMaxCount;
    
        float4 vRand = (float) 0.f;
        vRand.x = Rand(fKey);
        vRand.y = Rand(vRand.x);
        vRand.z = Rand(vRand.y);
        vRand.w = Rand(vRand.z);
    
        vRand = vRand * 2.f - 1.f;
                        
        g_particle[_iThreadID.x].vWorldPos = vObjectPos.xyz + (vRadius.xyz * vRand.xyz);
        g_particle[_iThreadID.x].vWorldDir = normalize(vRand.xyz);
        g_particle[_iThreadID.x].fCurTime = 0.f;
        g_particle[_iThreadID.x].fMaxTime = fMinLifeTime + (fMaxLifeTime - fMinLifeTime) * vRand.w;
    }
    else if (iParticleShape == 1)
    {
        // 랜덤 포지션 및 방향 세팅            
        float fKey = (float) _iThreadID.x / (float) iParticleMaxCount;
    
        float4 vRand = (float) 0.f;
        vRand.x = Rand(fKey);
        vRand.y = Rand(vRand.x);
        vRand.z = Rand(vRand.y);
        vRand.w = Rand(vRand.z);
    
        vRand = vRand * 2.f - 1.f;
                        
        g_particle[_iThreadID.x].vWorldPos = vObjectPos.xyz + (vRadius.xyz * vRand.xyz) * 10.f;
        g_particle[_iThreadID.x].vWorldDir = -(normalize(vRand.xyz));
        g_particle[_iThreadID.x].fCurTime = 0.f;
        g_particle[_iThreadID.x].fMaxTime = fMinLifeTime + (fMaxLifeTime - fMinLifeTime) * vRand.w;
    }
    // using gravity (cone shader)
    else if (iParticleShape == 2)
    {
        // 시작 위치는 동일함.       
        g_particle[_iThreadID.x].vWorldPos = vObjectPos.xyz;
        g_particle[_iThreadID.x].vWorldDir = float3(0.f, 1.f, 0.f);
        g_particle[_iThreadID.x].fCurTime = 0.f;
        g_particle[_iThreadID.x].fMaxTime = fMinLifeTime + (fMaxLifeTime - fMinLifeTime);
    }
}

void ParticleFunc2(inout RWStructuredBuffer<TParticle> tParticle, in
int3
_iThreadID)
{
    // 0 : Circle_Spread
    if (iParticleShape == 0)
    {
        float fLifeRatio = g_particle[_iThreadID.x].fCurTime / g_particle[_iThreadID.x].fMaxTime;
        float fCurSpeed = (fEndSpeed - fStartSpeed) * fLifeRatio + fStartSpeed;
    
        g_particle[_iThreadID.x].vWorldPos += g_particle[_iThreadID.x].vWorldDir * fCurSpeed * g_fDeltaTime;
    }
        // 1 : Circle_Combine
    if (iParticleShape == 1)
    {
        float fLifeRatio = g_particle[_iThreadID.x].fCurTime / g_particle[_iThreadID.x].fMaxTime;
        float fCurSpeed = (fEndSpeed - fStartSpeed) * fLifeRatio + fStartSpeed;
    
        g_particle[_iThreadID.x].vWorldPos += g_particle[_iThreadID.x].vWorldDir * fCurSpeed * g_fDeltaTime;
    }
        // 2 : Cone_Spread (gravity)
    if (iParticleShape == 2)
    {
        
        //float fLifeRatio = g_particle[_iThreadID.x].fCurTime / g_particle[_iThreadID.x].fMaxTime;
        //float fCurSpeed = (fEndSpeed - fStartSpeed) * fLifeRatio + fStartSpeed;
    
        //g_particle[_iThreadID.x].vWorldPos += g_particle[_iThreadID.x].vWorldDir * fCurSpeed * g_fDeltaTime;
    }
        // 3 : Edge
    else if (iParticleShape == 3)
    {
        float fLifeRatio = g_particle[_iThreadID.x].fCurTime / g_particle[_iThreadID.x].fMaxTime;
        float fCurSpeed = (fEndSpeed - fStartSpeed) * fLifeRatio + fStartSpeed;
    
        g_particle[_iThreadID.x].vWorldPos += g_particle[_iThreadID.x].vWorldDir * fCurSpeed * g_fDeltaTime;
    }
        // 4 : Sphere
    else if (iParticleShape == 4)
    {
            
    }
}


[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(int3 _iThreadID : SV_DispatchThreadID)
{
    // 담당 파티클이 비활성화 상태면 종료
    if (iParticleMaxCount <= _iThreadID.x)
        return;
    
    // 죽었으면
    if (!g_particle[_iThreadID.x].iAlive)
    {
        int iOriginValue = g_sharedParticle[0].m_iSpawnCount;
        int iExchanged = 0;
        
        while (0 < iOriginValue)
        {
            int iInputValue = iOriginValue - 1;
           
            //InterlockedExchange(g_Shared[0].m_iSpawnCount, iInputValue, iExchanged);                        
            InterlockedCompareExchange(g_sharedParticle[0].m_iSpawnCount, iOriginValue, iInputValue, iExchanged);
            
            // 당첨
            if (iExchanged == iOriginValue)
            {
                // 활성화
                g_particle[_iThreadID.x].iAlive = 1;
                break;
            }
            
            iOriginValue = iInputValue;
        }
             
        // 당첨된 파티클은 랜덤위치 셋팅
        if (1 == g_particle[_iThreadID.x].iAlive)
        {
            ParticleFunc(g_particle, _iThreadID);
        }
    }
    else // 살았으면
    {
        g_particle[_iThreadID.x].fCurTime += g_fDeltaTime;
    
        // 시간 초과 시 죽음
        if (g_particle[_iThreadID.x].fMaxTime < g_particle[_iThreadID.x].fCurTime)
        {
            g_particle[_iThreadID.x].iAlive = 0;
            g_particle[_iThreadID.x].fFallTime = 0.f;
            return;
        }
    
        ParticleFunc2(g_particle, _iThreadID);

        // 중력 사용
        if (iGravityEnable)
        {
            g_particle[_iThreadID.x].vWorldPos.xy +=
				g_particle[_iThreadID.x].vWorldDir.xy *
				g_particle[_iThreadID.x].fSpeed * g_fDeltaTime;
            
            g_particle[_iThreadID.x].fFallTime += g_fDeltaTime * 10.f;
            g_particle[_iThreadID.x].fFallStartY = 3.f;
            g_particle[_iThreadID.x].fSpeed = 20.f;
            float Velocity = 0.1f * g_particle[_iThreadID.x].fSpeed *
					g_particle[_iThreadID.x].fFallTime;

            g_particle[_iThreadID.x].vWorldPos.y =
					g_particle[_iThreadID.x].fFallStartY +
					(Velocity - 0.5f * GRAVITY * g_particle[_iThreadID.x].fFallTime * g_particle[_iThreadID.x].fFallTime);
        }
    }
}


#endif