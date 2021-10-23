#include "pch.h"
#include "AudioSourceGUI.h"
#include <Engine\CAudioSource.h>
#include <Engine\CGameObject.h>

AudioSourceGUI::AudioSourceGUI() :
	ComponentGUI(E_ComponentType::AudioSource)
{
}

AudioSourceGUI::~AudioSourceGUI()
{
}

void AudioSourceGUI::Update()
{
	if (false == Start())
		return;

	CAudioSource* pAudioSource = GetTargetObject()->AudioSource();

	// Check box
	//static bool no_border = pAudioSource->;
	//ImGui::Checkbox("ImGuiColorEditFlags_NoBorder", &no_border);

	End();
}