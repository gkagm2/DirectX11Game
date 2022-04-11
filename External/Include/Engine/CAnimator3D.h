#pragma once
#include "CComponent.h"

#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CAnimator3D : public CComponent
{
private:
	const vector<tMTBone>*		m_pVecBones;
	const vector<tMTAnimClip>*	m_pVecClip;

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // �ؽ��Ŀ� ������ ���� �������
	int							m_iFrameCount; // 30
	double						m_dCurTime;
	int							m_iCurClip; // Ŭ�� �ε���	

	int							m_iFrameIdx; // Ŭ���� ���� ������
	int							m_iNextFrameIdx; // Ŭ���� ���� ������
	float						m_fRatio;	// ������ ���� ����

	CStructuredBuffer*			m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���
	bool						m_bFinalMatUpdate; // ������� ���� ���࿩��

public:
	virtual void Awake() override {};
	virtual void Start() override {};
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate() override;

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
	void UpdateData();
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
	UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
	void ClearData();
private:
	void check_mesh(SharedPtr<CMesh> _pMesh);

public:
	CLONE(CAnimator3D);
	CAnimator3D();
	virtual ~CAnimator3D() override;
};