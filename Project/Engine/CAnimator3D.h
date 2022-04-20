#pragma once
#include "CComponent.h"

#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CAnimation3D;

enum class E_AnimationState3D {
	Loop, // repeat 0 to 10 
	Once, // 0 to 10 and done
	PingPong, // repeat 0 to 10 and 10 to 0 
	Fixed,
	End,
};

class CAnimator3D : public CComponent
{
private:
	const vector<tMTBone>*		m_pVecBones;
	const vector<tMTAnimClip>*	m_pVecClip;

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보
	int							m_iFrameCount; // 30
	double						m_dCurTime;
	int							m_iCurClip; // 클립 인덱스	

	int							m_iFrameIdx; // 클립의 현재 프레임
	int							m_iNextFrameIdx; // 클립의 다음 프레임
	float						m_fRatio;	// 프레임 사이 비율

	CStructuredBuffer*			m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
	bool						m_bFinalMatUpdate; // 최종행렬 연산 수행여부

private:
	unordered_map<tstring, CAnimation3D*> m_unmapAnim;
	CAnimation3D* m_pCurAnimation;
	E_AnimationState3D m_eAnimationState;

	bool m_bPlayOnSceneStart; // Scene 이 시작할때 셋팅된 애니메이션 즉시 재생여부

public:
	virtual void Start() override {};
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
	int GetCurFrame() { return m_iFrameIdx; }
	int GetFrameCount() { return m_iFrameCount; }
	double GetCurTime() { return m_dCurTime; }
	const vector<tMTAnimClip>* GetAnimationClip() { return m_pVecClip; }
	const vector<tMTBone>* GetBone() { return m_pVecBones; }

	void ClearData();
private:
	void check_mesh(SharedPtr<CMesh> _pMesh);

public:
	void SetAnimationState(E_AnimationState3D _eState) { m_eAnimationState = _eState; }
	CAnimation3D* GetCurAnimation() { return m_pCurAnimation; }


public:
	CLONE(CAnimator3D);
	CAnimator3D();
	virtual ~CAnimator3D() override;
};