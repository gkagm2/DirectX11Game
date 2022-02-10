#pragma once
#include "CRenderer.h"
enum class E_SkyboxType {
	Cube,
	Sphere
};
tstring SkyboxTypeToStr(E_SkyboxType _eType);
enum class E_SkyboxCubeType {
	Front,
	Left,
	Right,
	Back,
	Top,
	Bottom,
	End
};

class CSkybox : public CRenderer
{
private:
	SharedPtr<CMaterial> m_pSkyboxMaterial;
	SharedPtr<CMesh> m_pSkyboxMesh;
	SharedPtr<CTexture> m_pSkyboxTexture;
	E_SkyboxType m_eSkyboxType;

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	virtual void Render() override;

public:
	SharedPtr<CMesh> GetMesh() { return m_pSkyboxMesh; }
	SharedPtr<CMaterial> GetMaterial() { return m_pSkyboxMaterial; }

	void SetSkyboxTexture(SharedPtr<CTexture> _pTexture);
	SharedPtr<CTexture> GetSkyboxTexture();

	void SetSkyboxType(E_SkyboxType _eType);
	E_SkyboxType GetCurSkyboxType() { return m_eSkyboxType; }

public:
	CLONE(CSkybox);
	CSkybox();
	virtual ~CSkybox() override;
};