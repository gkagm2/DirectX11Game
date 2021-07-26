#include "pch.h"
#include "ResourceGUI.h"

ResourceGUI::ResourceGUI() :
	m_treeView{}
{
	SetName("Resource");
}

ResourceGUI::~ResourceGUI()
{
}

void ResourceGUI::Init()
{
}

void ResourceGUI::Update()
{
	_RenewResource();

	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);
	m_treeView.Update();
	ImGui::End();
}

void ResourceGUI::_RenewResource()
{
	// Resource���� ���� �� Renew

	// Resource ��� �߰�
}