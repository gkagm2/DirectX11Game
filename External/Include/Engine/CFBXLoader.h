#pragma once
#include "Global.h"

#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#endif

//===============
// Struct of FBX 
//===============
struct tFbxMaterial
{
	TMaterialData	tMtrl;
	wstring			strMtrlName;
	wstring			strDiff;
	wstring			strNormal;
	wstring			strSpec;
};

struct tWeightsAndIndices
{
	int		iBoneIdx;
	double	dWeight;
};

struct tContainer
{
	wstring								strName;
	vector<Vector3>						vecPos;
	vector<Vector3>						vecTangent;
	vector<Vector3>						vecBinormal;
	vector<Vector3>						vecNormal;
	vector<Vector2>						vecUV;

	vector<Vector4>						vecIndices;
	vector<Vector4>						vecWeights;

	vector<vector<UINT>>				vecIdx;
	vector<tFbxMaterial>				vecMtrl;

	// Animation ???? ????
	bool								bAnimation;
	vector<vector<tWeightsAndIndices>>	vecWI;

	void Resize(UINT _iSize)
	{
		vecPos.resize(_iSize);
		vecTangent.resize(_iSize);
		vecBinormal.resize(_iSize);
		vecNormal.resize(_iSize);
		vecUV.resize(_iSize);
		vecIndices.resize(_iSize);
		vecWeights.resize(_iSize);
		vecWI.resize(_iSize);
	}
};

struct tKeyFrame
{
	FbxAMatrix  matTransform;
	double		dTime;
};

struct tBone
{
	wstring				strBoneName;
	int					iDepth;			// ???????? ????
	int					iParentIndx;	// ?θ? Bone ?? ?ε???
	FbxAMatrix			matOffset;		// Offset ????( -> ?Ѹ? -> Local)
	FbxAMatrix			matBone;
	vector<tKeyFrame>	vecKeyFrame;
};

struct tAnimClip
{
	wstring		strName;
	FbxTime		tStartTime;
	FbxTime		tEndTime;
	FbxLongLong	llTimeLength;
	FbxTime::EMode eMode;
};



class CMesh;

class CFBXLoader
{
private:
	FbxManager* m_pManager;
	FbxScene* m_pScene;
	FbxImporter* m_pImporter;

	vector<tContainer>				m_vecContainer;

	// Animation
	vector<tBone*>					m_vecBone;
	FbxArray<FbxString*>			m_arrAnimName;
	vector<tAnimClip*>				m_vecAnimClip;


public:
	void init();
	void LoadFbx(const wstring& _strPath);

public:
	int GetContainerCount() { return (int)m_vecContainer.size(); }
	const tContainer& GetContainer(int _iIdx) { return m_vecContainer[_iIdx]; }
	vector<tBone*>& GetBones() { return m_vecBone; }
	vector<tAnimClip*>& GetAnimClip() { return m_vecAnimClip; }

private:
	void LoadMeshDataFromNode(FbxNode* _pRoot);
	void LoadMesh(FbxMesh* _pFbxMesh);
	void LoadMaterial(FbxSurfaceMaterial* _pMtrlSur);

	void GetTangent(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetBinormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetNormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetUV(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);

	Vector4 GetMtrlData(FbxSurfaceMaterial* _pSurface, const char* _pMtrlName, const char* _pMtrlFactorName);
	wstring GetMtrlTextureName(FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty);

	void LoadTexture();
	void CreateMaterial();

	// Animation
	void LoadSkeleton(FbxNode* _pNode);
	void LoadSkeleton_Re(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx);
	void LoadAnimationClip();
	void Triangulate(FbxNode* _pNode);

	void LoadAnimationData(FbxMesh* _pMesh, tContainer* _pContainer);
	void LoadWeightsAndIndices(FbxCluster* _pCluster, int _iBoneIdx, tContainer* _pContainer);
	void LoadOffsetMatrix(FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer);
	void LoadKeyframeTransform(FbxNode* _pNode, FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform
		, int _iBoneIdx, tContainer* _pContainer);

	int FindBoneIndex(string _strBoneName);
	FbxAMatrix GetTransform(FbxNode* _pNode);

	void CheckWeightAndIndices(FbxMesh* _pMesh, tContainer* _pContainer);
public:
	CFBXLoader();
	~CFBXLoader();
};

