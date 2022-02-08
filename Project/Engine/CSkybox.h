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
	// Sphere Texture;
	SharedPtr<CMaterial> m_pSphereMaterial;
	SharedPtr<CMesh> m_pSphereMesh;
	SharedPtr<CTexture> m_pSphereTexture;
	// Cube Texture;

	SharedPtr<CMaterial> m_pCubeMaterial;
	SharedPtr<CMesh> m_pCubeMesh;
	SharedPtr<CTexture> m_pCubeTextures[(UINT)E_SkyboxCubeType::End];
	E_SkyboxType m_eSkyboxType;

public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;
	virtual void Render() override;

public:
	void SetSkyboxTexture_Sphere(SharedPtr<CTexture> _pTexture) { m_pSphereTexture = _pTexture; }
	void SetSkyboxTexture_Cube(SharedPtr<CTexture> _pTexture, E_SkyboxCubeType _eType){ m_pCubeTextures[(UINT)_eType] = _pTexture; }

	SharedPtr<CTexture> GetSkyboxTexture_Sphere() { return m_pSphereTexture; }
	SharedPtr<CTexture> GetSkyboxTexture_Cube(E_SkyboxCubeType _eType) { return m_pCubeTextures[(UINT)_eType]; }

	void SetSkyboxType(E_SkyboxType _eType) { m_eSkyboxType = _eType; }
	E_SkyboxType GetCurSkyboxType() { return m_eSkyboxType; }

public:
	CLONE(CSkybox);
	CSkybox();
	virtual ~CSkybox() override;
};