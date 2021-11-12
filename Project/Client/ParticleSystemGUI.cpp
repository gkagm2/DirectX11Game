#include "pch.h"
#include "ParticleSystemGUI.h"
#include <Engine\CParticleSystem.h>

ParticleSystemGUI::ParticleSystemGUI() :
	ComponentGUI(E_ComponentType::ParticleSystem)
{

}

ParticleSystemGUI::~ParticleSystemGUI()
{
}

void ParticleSystemGUI::Update()
{
	if (false == Start())
		return;


	//CParticleSystem* pParticleSystem = GetTargetObject()->ParticleSystem
	//();
	// TODO (Jang) : 파티클 만들기

	//pParticleSystem->

	End();
}