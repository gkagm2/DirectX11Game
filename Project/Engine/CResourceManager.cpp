#include "pch.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CEventManager.h"
#include "CGraphicsShader.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CCore.h"
#include "CPerformanceMonitor.h"
#include "CDevice.h"
#include "CTransform.h"
CResourceManager::CResourceManager() :
	m_bFixed(false)
{
}

CResourceManager::~CResourceManager()
{
	Safe_Delete_Vector(m_vecCloneMtrl);

	for (UINT i = 0; i < (UINT)E_ResourceType::End; ++i)
		Safe_Delete_UnorderedMap(m_umapResource[i]);

	Safe_Delete_UnorderedMap(m_umapDefaultTex);
}

void CResourceManager::Init()
{
	PM_START(_T("CResourceManager Init"));
	InitInputLayout();

	CreateDefaultMesh();
	CreateDefaultCircle2DMesh();
	CreateDefaultMesh3D();
	CreateDefaultShader();
	CreateDefaultTexture();
	CreateComputeShader();
	CreateDefaultMaterial();
	InitSound(); // FMOD 초기화
	PM_END();

	PM_START(_T("CResourceManager Load Resources From Dir"));
	E_SceneMode eSceneMode = CSceneManager::GetInstance()->GetSceneMode();
	if (eSceneMode == E_SceneMode::Stop) {
		// Texture 로딩
		LoadResourcesFromDir<CTexture>(_T("texture\\"), _T("*.*"));

		// 프리펩 로딩
		LoadResourcesFromDir<CPrefab>(_T("prefab\\"), _T("*.pref"));

		// 메터리얼 로딩
		LoadResourcesFromDir<CMaterial>(_T("material\\"), _T("*.mtrl"));
	}
	PM_END();
}

void CResourceManager::CreateDefaultMesh()
{
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	////////////////
	// RectMesh 생성

	// 버퍼 만들기
	VTX vertex;
	vertex.vPos = Vector3(-0.5f, 0.5f, 0.f);
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertex.vUV = Vector2(0.f, 0.f);

	vertex.vTangent = Vector3(1.f, 0.f, 0.f);
	vertex.vNormal = Vector3(0.f, 0.f, -1.f);
	vertex.vBinormal = Vector3(0.f, 1.f, 0.f);

	vecVtx.push_back(vertex);

	vertex.vPos = Vector3(0.5f, 0.5f, 0.f);
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertex.vUV = Vector2(1.f, 0.f);

	vertex.vTangent = Vector3(1.f, 0.f, 0.f);
	vertex.vNormal = Vector3(0.f, 0.f, -1.f);
	vertex.vBinormal = Vector3(0.f, 1.f, 0.f);

	vecVtx.push_back(vertex);

	vertex.vPos = Vector3(0.5f, -0.5f, 0.f);
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertex.vUV = Vector2(1.f, 1.f);

	vertex.vTangent = Vector3(1.f, 0.f, 0.f);
	vertex.vNormal = Vector3(0.f, 0.f, -1.f);
	vertex.vBinormal = Vector3(0.f, 1.f, 0.f);

	vecVtx.push_back(vertex);

	vertex.vPos = Vector3(-0.5f, -0.5f, 0.f);
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertex.vUV = Vector2(0.f, 1.f);

	vertex.vTangent = Vector3(1.f, 0.f, 0.f);
	vertex.vNormal = Vector3(0.f, 0.f, -1.f);
	vertex.vBinormal = Vector3(0.f, 1.f, 0.f);
	vecVtx.push_back(vertex);

	// create index buffer
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	/*
	0-------1
	|＼     |
	|  ＼   |
	|    ＼ |
	3-------2
	*/

	CMesh* pMesh = new CMesh();
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_RectMesh, pMesh); // AddResource<CMesh>(STR_KEY_RectMash, pMesh);

	///////////////////
	// RectLineMesh 생성
	/*
	0-------1
	|		|
	|		|
	|		|
	3-------2
	*/
	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_RectLineMesh, pMesh);

	///////////////////
	// Point Mesh 생성
	vertex = {};
	vecVtx.clear();
	vecIdx.clear();

	vertex.vPos = Vector3(0.f, 0.f, 0.f);
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertex.vUV = Vector2(0.f, 0.f);

	vecVtx.push_back(vertex);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_PointMesh, pMesh);


	////////////////////
	// line strip Mesh 생성
	vertex = {};
	vecVtx.clear();
	vecIdx.clear();

	vertex.vPos = Vector3(0.f, 0.f, 0.f);
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertex.vUV = Vector2(0.f, 0.f);

	vecVtx.push_back(vertex);
	vecIdx.push_back(0);
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_LineStripMesh, pMesh);

	////////////////////
	// line list Mesh 생성
	vertex = {};
	vecVtx.clear();
	vecIdx.clear();

	vertex.vPos = Vector3(0.f, 0.f, 0.f);
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertex.vUV = Vector2(0.5, 0.5f);
	vecVtx.push_back(vertex);
	vertex.vPos = Vector3(200.f, 200.f, 0.f);
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertex.vUV = Vector2(0.5, 0.5f);
	vecVtx.push_back(vertex);

	vecIdx.push_back(0); 
	vecIdx.push_back(1);
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX)* (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT)* (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_LineListMesh, pMesh);
}

void CResourceManager::CreateDefaultCircle2DMesh()
{
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	// Circle Mesh, Circle Line Mesh 만들기

	// 원점 설정
	VTX vtx = {};

	vtx.vPos = Vector3(0.f, 0.f, 0.f);
	vtx.vUV = Vector2(0.5f, 0.5f);
	Vector4 vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vtx.vColor = vColor;
	vecVtx.push_back(vtx);
	
	float fRadius = 0.5f;
	int iSidesCnt = 40; // 변의 개수
	float fRadian = XM_2PI / (float)iSidesCnt;

	// 테두리의 정점 설정
	for (int i = 0; i < iSidesCnt + 1; ++i) {
		vtx.vPos.x = cosf(fRadian * (float)i) * fRadius;
		vtx.vPos.y = sinf(fRadian * (float)i) * fRadius;
		vtx.vColor = vColor;

		vtx.vUV.x = (cosf(fRadian * (float)i) + 1.f) * 0.5f;
		vtx.vUV.y = (sinf(fRadian * (float)i) + 1.f) * 0.5f;
		vtx.vUV.y = 1.f - vtx.vUV.y;

		vtx.vTangent = Vector3(1.f, 0.f, 0.f);
		vtx.vNormal = Vector3(0.f, 0.f, -1.f);
		vtx.vBinormal = Vector3(0.f, 1.f, 0.f);

		vecVtx.push_back(vtx);
	}

	// 인덱스 설정

	// Circle Mesh
	for (int i = 0; i < iSidesCnt; ++i) {
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	CMesh* pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);
	AddRes(STR_KEY_CircleMesh, pMesh);

	// Circle Line Mesh 
	vecIdx.clear();
	for (int i = 0; i < iSidesCnt + 1; ++i)
		vecIdx.push_back(i + 1);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);
	AddRes(STR_KEY_CircleLineMesh, pMesh);
}

void CResourceManager::CreateDefaultMesh3D()
{
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	// =========
	// Cube Mesh
	// =========
	VTX vertices[24] = {};

	// 윗면
	vertices[0].vPos = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[0].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertices[0].vUV = Vector2(0.f, 0.f);
	vertices[0].vNormal = Vector3(0.f, 1.f, 0.f);
	vertices[0].vTangent = Vector3(1.f, 0.f, 0.f);
	vertices[0].vBinormal = Vector3(0.f, 0.f, -1.f);

	vertices[1].vPos = Vector3(0.5f, 0.5f, 0.5f);
	vertices[1].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertices[1].vUV = Vector2(1.f, 0.f);
	vertices[1].vNormal = Vector3(0.f, 1.f, 0.f);
	vertices[1].vTangent = Vector3(1.f, 0.f, 0.f);
	vertices[1].vBinormal = Vector3(0.f, 0.f, -1.f);

	vertices[2].vPos = Vector3(0.5f, 0.5f, -0.5f);
	vertices[2].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertices[2].vUV = Vector2(1.f, 1.f);
	vertices[2].vNormal = Vector3(0.f, 1.f, 0.f);
	vertices[2].vTangent = Vector3(1.f, 0.f, 0.f);
	vertices[2].vBinormal = Vector3(0.f, 0.f, -1.f);

	vertices[3].vPos = Vector3(-0.5f, 0.5f, -0.5f);
	vertices[3].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertices[3].vUV = Vector2(0.f, 1.f);
	vertices[3].vNormal = Vector3(0.f, 1.f, 0.f);
	vertices[3].vTangent = Vector3(1.f, 0.f, 0.f);
	vertices[3].vBinormal = Vector3(0.f, 0.f, -1.f);


	// 아랫 면	
	vertices[4].vPos = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[4].vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	vertices[4].vUV = Vector2(0.f, 0.f);
	vertices[4].vNormal = Vector3(0.f, -1.f, 0.f);
	vertices[4].vTangent = Vector3(-1.f, 0.f, 0.f);
	vertices[4].vBinormal = Vector3(0.f, 0.f, 1.f);

	vertices[5].vPos = Vector3(0.5f, -0.5f, -0.5f);
	vertices[5].vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	vertices[5].vUV = Vector2(1.f, 0.f);
	vertices[5].vNormal = Vector3(0.f, -1.f, 0.f);
	vertices[5].vTangent = Vector3(-1.f, 0.f, 0.f);
	vertices[5].vBinormal = Vector3(0.f, 0.f, 1.f);

	vertices[6].vPos = Vector3(0.5f, -0.5f, 0.5f);
	vertices[6].vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	vertices[6].vUV = Vector2(1.f, 1.f);
	vertices[6].vNormal = Vector3(0.f, -1.f, 0.f);
	vertices[6].vTangent = Vector3(-1.f, 0.f, 0.f);
	vertices[6].vBinormal = Vector3(0.f, 0.f, 1.f);

	vertices[7].vPos = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[7].vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	vertices[7].vUV = Vector2(0.f, 1.f);
	vertices[7].vNormal = Vector3(0.f, -1.f, 0.f);
	vertices[7].vTangent = Vector3(-1.f, 0.f, 0.f);
	vertices[7].vBinormal = Vector3(0.f, 0.f, 1.f);

	// 왼쪽 면
	vertices[8].vPos = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[8].vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	vertices[8].vUV = Vector2(0.f, 0.f);
	vertices[8].vNormal = Vector3(-1.f, 0.f, 0.f);
	vertices[8].vTangent = Vector3::Up;
	vertices[8].vBinormal = Vector3::Back;

	vertices[9].vPos = Vector3(-0.5f, 0.5f, -0.5f);
	vertices[9].vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	vertices[9].vUV = Vector2(1.f, 0.f);
	vertices[9].vNormal = Vector3(-1.f, 0.f, 0.f);
	vertices[9].vTangent = Vector3::Up;
	vertices[9].vBinormal = Vector3::Back;

	vertices[10].vPos = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[10].vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	vertices[10].vUV = Vector2(1.f, 1.f);
	vertices[10].vNormal = Vector3(-1.f, 0.f, 0.f);
	vertices[10].vTangent = Vector3::Up;
	vertices[10].vBinormal = Vector3::Back;

	vertices[11].vPos = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[11].vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	vertices[11].vUV = Vector2(0.f, 1.f);
	vertices[11].vNormal = Vector3(-1.f, 0.f, 0.f);
	vertices[11].vTangent = Vector3::Up;
	vertices[11].vBinormal = Vector3::Back;

	// 오른쪽 면
	vertices[12].vPos = Vector3(0.5f, 0.5f, -0.5f);
	vertices[12].vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	vertices[12].vUV = Vector2(0.f, 0.f);
	vertices[12].vNormal = Vector3(1.f, 0.f, 0.f);
	vertices[12].vTangent = Vector3::Down;
	vertices[12].vBinormal = Vector3::Back;

	vertices[13].vPos = Vector3(0.5f, 0.5f, 0.5f);
	vertices[13].vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	vertices[13].vUV = Vector2(1.f, 0.f);
	vertices[13].vNormal = Vector3(1.f, 0.f, 0.f);
	vertices[13].vTangent = Vector3::Down;
	vertices[13].vBinormal = Vector3::Back;

	vertices[14].vPos = Vector3(0.5f, -0.5f, 0.5f);
	vertices[14].vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	vertices[14].vUV = Vector2(1.f, 1.f);
	vertices[14].vNormal = Vector3(1.f, 0.f, 0.f);
	vertices[14].vTangent = Vector3::Down;
	vertices[14].vBinormal = Vector3::Back;

	vertices[15].vPos = Vector3(0.5f, -0.5f, -0.5f);
	vertices[15].vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	vertices[15].vUV = Vector2(0.f, 1.f);
	vertices[15].vNormal = Vector3(1.f, 0.f, 0.f);
	vertices[15].vTangent = Vector3::Down;
	vertices[15].vBinormal = Vector3::Back;

	// 뒷 면
	vertices[16].vPos = Vector3(0.5f, 0.5f, 0.5f);
	vertices[16].vColor = Vector4(1.f, 1.f, 0.f, 1.f);
	vertices[16].vUV = Vector2(0.f, 0.f);
	vertices[16].vNormal = Vector3(0.f, 0.f, 1.f);
	vertices[16].vTangent = Vector3::Right;
	vertices[16].vBinormal = Vector3::Up;

	vertices[17].vPos = Vector3(-0.5f, 0.5f, 0.5f);
	vertices[17].vColor = Vector4(1.f, 1.f, 0.f, 1.f);
	vertices[17].vUV = Vector2(1.f, 0.f);
	vertices[17].vNormal = Vector3(0.f, 0.f, 1.f);
	vertices[17].vTangent = Vector3::Right;
	vertices[17].vBinormal = Vector3::Up;

	vertices[18].vPos = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[18].vColor = Vector4(1.f, 1.f, 0.f, 1.f);
	vertices[18].vUV = Vector2(1.f, 1.f);
	vertices[18].vNormal = Vector3(0.f, 0.f, 1.f);
	vertices[18].vTangent = Vector3::Right;
	vertices[18].vBinormal = Vector3::Up;

	vertices[19].vPos = Vector3(0.5f, -0.5f, 0.5f);
	vertices[19].vColor = Vector4(1.f, 1.f, 0.f, 1.f);
	vertices[19].vUV = Vector2(0.f, 1.f);
	vertices[19].vNormal = Vector3(0.f, 0.f, 1.f);
	vertices[19].vTangent = Vector3::Right;
	vertices[19].vBinormal = Vector3::Up;

	// 앞 면
	vertices[20].vPos = Vector3(-0.5f, 0.5f, -0.5f);;
	vertices[20].vColor = Vector4(1.f, 0.f, 1.f, 1.f);
	vertices[20].vUV = Vector2(0.f, 0.f);
	vertices[20].vNormal = Vector3(0.f, 0.f, -1.f);
	vertices[20].vTangent = Vector3::Right;
	vertices[20].vBinormal = Vector3::Down;

	vertices[21].vPos = Vector3(0.5f, 0.5f, -0.5f);
	vertices[21].vColor = Vector4(1.f, 0.f, 1.f, 1.f);
	vertices[21].vUV = Vector2(1.f, 0.f);
	vertices[21].vNormal = Vector3(0.f, 0.f, -1.f);
	vertices[21].vTangent = Vector3::Right;
	vertices[21].vBinormal = Vector3::Down;

	vertices[22].vPos = Vector3(0.5f, -0.5f, -0.5f);
	vertices[22].vColor = Vector4(1.f, 0.f, 1.f, 1.f);
	vertices[22].vUV = Vector2(1.f, 1.f);
	vertices[22].vNormal = Vector3(0.f, 0.f, -1.f);
	vertices[22].vTangent = Vector3::Right;
	vertices[22].vBinormal = Vector3::Down;

	vertices[23].vPos = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[23].vColor = Vector4(1.f, 0.f, 1.f, 1.f);
	vertices[23].vUV = Vector2(0.f, 1.f);
	vertices[23].vNormal = Vector3(0.f, 0.f, -1.f);
	vertices[23].vTangent = Vector3::Right;
	vertices[23].vBinormal = Vector3::Down;

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	CMesh* pMesh = new CMesh();
	pMesh->Create(vertices, sizeof(VTX) * 24, vecIdx.data(), sizeof(UINT) * vecIdx.size(), D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_CubeMesh, pMesh); // AddResource<CMesh>(STR_KEY_RectMash, pMesh);

	vecVtx.clear();
	vecIdx.clear();

	// =============
	// Sphere Mesh
	// =============

	float fRadius = 0.5;
	VTX v = {};

	// Top
	v.vPos = Vector3(0.f, fRadius, 0.f);
	v.vUV = Vector2(0.5f, 0.f);
	v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vector3(1.f, 0.f, 0.f);
	v.vBinormal = Vector3(0.f, 0.f, 1.f);
	vecVtx.push_back(v);

	// Body
	UINT iStackCount = 40; // 가로 분할 개수
	UINT iSliceCount = 40; // 세로 분할 개수

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vector3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			v.vUV = Vector2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vTangent.Cross(v.vNormal, v.vBinormal);
			v.vBinormal.Normalize();

			vecVtx.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vector3(0.f, -fRadius, 0.f);
	v.vUV = Vector2(0.5f, 1.f);
	v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vector3(1.f, 0.f, 0.f);
	v.vBinormal = Vector3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);
	AddRes(STR_KEY_SphereMesh, pMesh);

	vecVtx.clear();
	vecIdx.clear();

	{
		// =============
		// Debug Sphere Mesh
		// =============

		float fRadius = 0.5;
		VTX v = {};

		// Top
		v.vPos = Vector3(0.f, fRadius, 0.f);
		v.vUV = Vector2(0.5f, 0.f);
		v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
		v.vNormal = v.vPos;
		v.vNormal.Normalize();
		v.vTangent = Vector3(1.f, 0.f, 0.f);
		v.vBinormal = Vector3(0.f, 0.f, 1.f);
		vecVtx.push_back(v);

		// Body
		UINT iStackCount = 5; // 가로 분할 개수
		UINT iSliceCount = 5; // 세로 분할 개수

		float fStackAngle = XM_PI / iStackCount;
		float fSliceAngle = XM_2PI / iSliceCount;

		float fUVXStep = 1.f / (float)iSliceCount;
		float fUVYStep = 1.f / (float)iStackCount;

		for (UINT i = 1; i < iStackCount; ++i)
		{
			float phi = i * fStackAngle;

			for (UINT j = 0; j <= iSliceCount; ++j)
			{
				float theta = j * fSliceAngle;

				v.vPos = Vector3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
					, fRadius * cosf(i * fStackAngle)
					, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
				v.vUV = Vector2(fUVXStep * j, fUVYStep * i);
				v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
				v.vNormal = v.vPos;
				v.vNormal.Normalize();

				v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
				v.vTangent.y = 0.f;
				v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
				v.vTangent.Normalize();

				v.vTangent.Cross(v.vNormal, v.vBinormal);
				v.vBinormal.Normalize();

				vecVtx.push_back(v);
			}
		}

		// Bottom
		v.vPos = Vector3(0.f, -fRadius, 0.f);
		v.vUV = Vector2(0.5f, 1.f);
		v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
		v.vNormal = v.vPos;
		v.vNormal.Normalize();

		v.vTangent = Vector3(1.f, 0.f, 0.f);
		v.vBinormal = Vector3(0.f, 0.f, -1.f);
		vecVtx.push_back(v);

		// 인덱스
		// 북극점
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			vecIdx.push_back(0);
			vecIdx.push_back(i + 2);
			vecIdx.push_back(i + 1);
		}

		// 몸통
		for (UINT i = 0; i < iStackCount - 2; ++i)
		{
			for (UINT j = 0; j < iSliceCount; ++j)
			{
				// + 
				// | \
								// +--+
				vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
				vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

				// +--+
				//  \ |
				//    +
				vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
				vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
				vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			}
		}

		// 남극점
		UINT iBottomIdx = (UINT)vecVtx.size() - 1;
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			vecIdx.push_back(iBottomIdx);
			vecIdx.push_back(iBottomIdx - (i + 2));
			vecIdx.push_back(iBottomIdx - (i + 1));
		}

		pMesh = new CMesh;
		pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);
		AddRes(STR_KEY_DebugSphereMesh, pMesh);

		vecVtx.clear();
		vecIdx.clear();
	}

	{
		// Cone Mesh 

		VTX vtx = {};
		
		int tDiv = 15;
		int yDiv = 6;
		double maxTheta = CMyMath::DegreeToRadian(360.0);
		double minY = -1.0;
		double maxY = 1.0;

		double dt = maxTheta / tDiv;
		double dy = (maxY - minY) / yDiv;

		for (int yi = 0; yi <= yDiv; yi++)
		{
			double y = minY + yi * dy;

			for (int ti = 0; ti <= tDiv; ti++)
			{
				float t = ti * dt;
				

				// Get Position
				{
					float r = (1 - y) / 2;
					float x = r * cosf(t);
					float z = r * sinf(t);
					vtx.vPos = Vector3(x, y, z);
				}
				
				// Get Normal
				{
					float x = 2 * cosf(t);
					float z = 2 * sinf(t);
					vtx.vNormal = Vector3(x, 1, z);
				}
				
				// Get Texture Coordinate
				{
					Matrix m = XMMatrixScaling(float(1.f / (2.f * PI)), -0.5f, 0.f);
					Vector2 p = Vector2(t, y);
					p = XMVector2Transform(p, m);
					vtx.vUV = p;
				}
				vecVtx.push_back(vtx);
			}
		}

		for (int yi = 0; yi < yDiv; yi++)
		{
			for (int ti = 0; ti < tDiv; ti++)
			{
				int x0 = ti;
				int x1 = (ti + 1);
				int y0 = yi * (tDiv + 1);
				int y1 = (yi + 1) * (tDiv + 1);

				vecIdx.push_back(x0 + y0);
				vecIdx.push_back(x0 + y1);
				vecIdx.push_back(x1 + y0);

				vecIdx.push_back(x1 + y0);
				vecIdx.push_back(x0 + y1);
				vecIdx.push_back(x1 + y1);
			}
		}

		CMesh* pMesh = new CMesh;
		pMesh->Create(vecVtx.data(), sizeof(VTX)* (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT)* (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);
		AddRes(STR_KEY_ConeMesh, pMesh);

		vecVtx.clear();
		vecIdx.clear();
	}
	{
		CMesh* pMesh = new CMesh;
		float fLen = 100.f;

		VTX vertex = {};
		vertex.vPos = Vector3(0.f, 0.f, 0.f);
		vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(vertex);

		
		vertex.vPos = Vector3(-0.5f * fLen,0.5f * fLen,0.5f * fLen);
		vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(vertex);

		vertex.vPos = Vector3(0.5f * fLen, 0.5f * fLen, 0.5f * fLen);
		vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(vertex);

		vertex.vPos = Vector3(0.5f * fLen, -0.5f * fLen, 0.5f * fLen);
		vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(vertex);

		vertex.vPos = Vector3(-0.5f * fLen, -0.5f * fLen, 0.5f * fLen);
		vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(vertex);


		vecIdx.push_back(0);
		vecIdx.push_back(1);
		vecIdx.push_back(2);

		vecIdx.push_back(0);
		vecIdx.push_back(2);
		vecIdx.push_back(3);

		vecIdx.push_back(0);
		vecIdx.push_back(3);
		vecIdx.push_back(4);

		vecIdx.push_back(0);
		vecIdx.push_back(4);
		vecIdx.push_back(1);

		pMesh->Create(vecVtx.data(), sizeof(VTX)* (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT)* (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);
		AddRes(STR_KEY_FrustumMesh, pMesh);

		vecVtx.clear();
		vecIdx.clear();
	}
}


void CResourceManager::CreateDefaultShader()
{
	// --------------------------
	// 기본 쉐이더 생성 (AlphaBlend Coveratge)
	CGraphicsShader* pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	// Shader Param
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	AddRes(STR_KEY_StdAlphaBlend_CoverageShader, pShader);

	// -----------------------
	// 기본 쉐이더 생성 (AlphaBlend)
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// OM
	pShader->SetBlendState(E_BlendState::AlphaBlend);
	// Shader Param
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	// Test Shader Param
	/*pShader->AddShaderParam({ E_ShaderParam::Int_0,	  _T("Test Param Int") });
	pShader->AddShaderParam({ E_ShaderParam::Float_0,   _T("Test Param Float") });
	pShader->AddShaderParam({ E_ShaderParam::Vector2_1, _T("Test Param Vec2") });
	pShader->AddShaderParam({ E_ShaderParam::Vector4_2, _T("Test Param Vec4") });
	pShader->AddShaderParam({ E_ShaderParam::Texture_0, _T("Output Texture1") });
	pShader->AddShaderParam({ E_ShaderParam::Texture_1, _T("Output Texture2") });*/

	AddRes(STR_KEY_StdAlphaBlendShader, pShader);

	//----------------------
	// Light2D 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderLight2D);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderLight2D);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// OM
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });

	AddRes(STR_KEY_StdLight2DShader, pShader);

	//----------------------
	// Light2D Cartoon 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderLight2DCartoon);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderLight2DCartoon);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// OM
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });

	AddRes(STR_KEY_StdLight2DCartoonShader, pShader);


	//----------------------
	// LineStript 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderLineStrip);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderLineStrip);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// OM
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Color") });

	AddRes(STR_KEY_StdLineStripShader, pShader);

	//----------------------
	// Collider2D 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::Collider);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderCollider2D);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderCollider2D);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Topology
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// OM (Output Merge)
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);
	pShader->SetBlendState(E_BlendState::Default);

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Default Rect Color") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_1, _T("Connect Rect Color") });

	AddRes(STR_KEY_Collider2DShader, pShader);

	//-----------------------
	// 타일맵 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_TileMapShader, STR_FUNC_NAME_VTXShaderTileMap);
	pShader->CreatePixelShader(STR_FILE_PATH_TileMapShader, STR_FUNC_NAME_PIXShaderTileMap);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Blend State
	pShader->SetBlendState(E_BlendState::AlphaBlend);

	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_0, _T("Tile x cnt") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_1, _T("Tile y cnt") });

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Atlas Texture") });

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector2_0, _T("Atlas Resolution") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector2_1, _T("Atlas Tile UV Size") });

	AddRes(STR_KEY_TileMapShader, pShader);

	//-----------------------
	// 빛 적용 타일맵 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_TileMapShader, STR_FUNC_NAME_VTXShaderTileMap);
	pShader->CreatePixelShader(STR_FILE_PATH_TileMapShader, STR_FUNC_NAME_PIXShaderTileMap);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Blend State
	pShader->SetBlendState(E_BlendState::AlphaBlend);

	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_0, _T("Tile x cnt") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_1, _T("Tile y cnt") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_2, _T("Light Enable") });

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Atlas Texture") });

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector2_0, _T("Atlas Resolution") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector2_1, _T("Atlas Tile UV Size") });
	AddRes(STR_KEY_TileMapLightShader, pShader);

	//-------------------------
	// 파티클 렌더 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::Particle);
	pShader->CreateVertexShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_VTX_Particle);
	pShader->CreateGeometryShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_GEO_Particle);
	pShader->CreatePixelShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_PIX_Particle);

	// Topology
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Blend
	pShader->SetBlendState(E_BlendState::AlphaBlend);

	// Depth Stencil
	pShader->SetDepthStencilState(E_DepthStencilState::No_Write);

	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Start Color") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_1, _T("End Color") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_2, _T("Start Scale") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_3, _T("End Scale") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Particle Texture") });

	AddRes(STR_KEY_ParticleShader, pShader);

	//-------------------------
	// Distortion(왜곡) 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::PostEffect);
	pShader->CreateVertexShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_VTX_Distortion);
	pShader->CreatePixelShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_PIX_Distortion);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Depth Stencil
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);

	AddRes(STR_KEY_DistortionShader, pShader);

	//---------------------------
	// FishEye Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::PostEffect);
	pShader->CreateVertexShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_VTX_FishEye);
	pShader->CreatePixelShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_PIX_FishEye);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Depth Stencil
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);

	AddRes(STR_KEY_FishEyeShader, pShader);

	//----------------------------
	// Blur Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::PostEffect);
	pShader->CreateVertexShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_VTX_Blur);
	pShader->CreatePixelShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_PIX_Blur);

	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);

	AddRes(STR_KEY_BlurShader, pShader);

	//----------------------------
	// Noise Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::PostEffect);
	pShader->CreateVertexShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_VTX_Noise);
	pShader->CreatePixelShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_PIX_Noise);

	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);

	AddRes(STR_KEY_NoiseShader, pShader);

	//----------------------------
	// Canvas Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Canvas);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXCanvasShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXCanvasShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	pShader->SetDepthStencilState(E_DepthStencilState::Less_Equal);
	// Shader Param
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Float_2, _T("Size") });
	AddRes(STR_KEY_CanvasShader, pShader);

	//----------------------------
	// ButtonUI Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Canvas);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXButtonUIShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXButtonUIShader);

	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	pShader->SetBlendState(E_BlendState::AlphaBlend);
	pShader->SetDepthStencilState(E_DepthStencilState::Less_Equal);

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Cur Color") });

	AddRes(STR_KEY_ButtonUIShader, pShader);

	//----------------------------
	// Fog2D Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXFog2DShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXFog2DShader);

	// rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// blend
	pShader->SetBlendState(E_BlendState::One_One); // TODO (Jang) : AlphaBlend여야 하지 않나..
	pShader->SetDepthStencilState(E_DepthStencilState::Less);
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Fog Texture") });
	AddRes(STR_KEY_Fog2DShader, pShader);

	//----------------------
	// Grid 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXGridShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXGridShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Topology
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// OM (Output Merge)
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);
	pShader->SetBlendState(E_BlendState::Default);
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Default Grid Color") });

	AddRes(STR_KEY_GridShader, pShader);


	// Line List 쉐이더 생성
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXLineShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXLineShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Topology
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// OM (Output Merge)
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);
	pShader->SetBlendState(E_BlendState::Default);
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Default Line Color") });

	AddRes(STR_KEY_LineListShader, pShader);

	//---------------------------
	// Std3D Shader (3D용 기본 쉐이더)

	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader3D, STR_FUNC_NAME_VTXShader3D);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader3D, STR_FUNC_NAME_PIXShader3D);

	pShader->SetRasterizerState(E_RasterizerState::CullBack);
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Diffuse Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_1, _T("Normal Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_0, _T("Shader Type") });
	AddRes(STR_KEY_Std3DShader, pShader);

	//----------------------------
	// Std3D Deferred Shader

	pShader = new CGraphicsShader(E_RenderTimePoint::Deferred);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader3DDeferred, STR_FUNC_NAME_VTXShader3DDeferred);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader3DDeferred, STR_FUNC_NAME_PIXShader3DDeferred);

	pShader->SetRasterizerState(E_RasterizerState::CullBack);
	pShader->SetBlendState(E_BlendState::Default); // 투명처리 X

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("ColorTexture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_1, _T("Normal Map Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_2, _T("Specular Map Texture") });
	AddRes(STR_KEY_Std3DDeferredShader, pShader);

	//----------------------------
	// Merge Shader 

	pShader = new CGraphicsShader(E_RenderTimePoint::None);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader3DDeferred, STR_FUNC_NAME_VTXShaderMerge);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader3DDeferred, STR_FUNC_NAME_PIXShaderMerge);

	pShader->SetRasterizerState(E_RasterizerState::CullBack);
	pShader->SetBlendState(E_BlendState::Default);
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0,  _T("Color Target Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_1,  _T("Diffuse Target Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_2,  _T("Specular Target Texture") });
	
	AddRes(STR_KEY_MergeShader, pShader);

	//----------------------------
	// Direction Light Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Light);
	pShader->CreateVertexShader(STR_FILE_PATH_ShaderLight, STR_FUNC_NAME_VTXDirLight);
	pShader->CreatePixelShader(STR_FILE_PATH_ShaderLight, STR_FUNC_NAME_PIXDirLight);
	pShader->SetRasterizerState(E_RasterizerState::CullBack);
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);
	pShader->SetBlendState(E_BlendState::Default);

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("View Normal Target Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_1, _T("View Position Target Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_2, _T("Shadow Depth Target Tex") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Matrix_0, _T("Directional cam VP mat") });


	AddRes(STR_KEY_DirectionLightShader, pShader);

	//----------------------------
	// Point Light Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Light);
	pShader->CreateVertexShader(STR_FILE_PATH_ShaderLight, STR_FUNC_NAME_VTXPointLight);
	pShader->CreatePixelShader(STR_FILE_PATH_ShaderLight, STR_FUNC_NAME_PIXPointLight);
	pShader->SetRasterizerState(E_RasterizerState::CullFront);
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);
	pShader->SetBlendState(E_BlendState::Default);

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("View Normal Target Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_1, _T("View Position Target Texture") });

	AddRes(STR_KEY_PointLightShader, pShader);
	
	//----------------------------
	// Spot Light Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Light);
	pShader->CreateVertexShader(STR_FILE_PATH_ShaderLight, STR_FUNC_NAME_VTXSpotLight);
	pShader->CreatePixelShader(STR_FILE_PATH_ShaderLight, STR_FUNC_NAME_PIXSpotLight);
	pShader->SetRasterizerState(E_RasterizerState::CullFront);
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);
	pShader->SetBlendState(E_BlendState::Default);

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("View Normal Target Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_1, _T("View Position Target Texture") });

	AddRes(STR_KEY_SpotLightShader, pShader);

	//----------------------------
	// Shadow Depth Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::ShadowDepth);
	pShader->CreateVertexShader(STR_FILE_PATH_ShaderShadow, STR_FUNC_NAME_VTXShadowDepth);
	pShader->CreatePixelShader(STR_FILE_PATH_ShaderShadow, STR_FUNC_NAME_PIXShadowDepth);
	pShader->SetRasterizerState(E_RasterizerState::CullBack);
	pShader->SetDepthStencilState(E_DepthStencilState::Less);
	AddRes(STR_KEY_ShadowDepthShader, pShader);

	//----------------------------
	// SkyBox Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_SkyboxShader, STR_FUNC_NAME_VTXSkybox);
	pShader->CreatePixelShader(STR_FILE_PATH_SkyboxShader, STR_FUNC_NAME_PIXSkybox);
	pShader->SetRasterizerState(E_RasterizerState::CullFront);
	pShader->SetBlendState(E_BlendState::Default);
	pShader->SetDepthStencilState(E_DepthStencilState::Less_Equal);
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_0, _T("Skybox type(0: cube, 1: sphere)") });
	AddRes(STR_KEY_SkyboxShader, pShader);

	//----------------------------
	// Decal Debug Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Decal);
	pShader->CreateVertexShader(STR_FILE_PATH_DecalShader, STR_FUNC_NAME_VTX_DebugDecal);
	pShader->CreatePixelShader(STR_FILE_PATH_DecalShader, STR_FUNC_NAME_PIX_DebugDecal);
	pShader->SetRasterizerState(E_RasterizerState::Wireframe);
	pShader->SetDepthStencilState(E_DepthStencilState::Less);
	pShader->SetBlendState(E_BlendState::Default);
	AddRes(STR_KEY_DecalDebugShader, pShader);

	//----------------------------
	// Decal Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Decal);
	pShader->CreateVertexShader(STR_FILE_PATH_DecalShader, STR_FUNC_NAME_VTX_Decal);
	pShader->CreatePixelShader(STR_FILE_PATH_DecalShader, STR_FUNC_NAME_PIX_Decal);
	pShader->SetRasterizerState(E_RasterizerState::CullFront);
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);
	pShader->SetBlendState(E_BlendState::DecalBlend);
	AddRes(STR_KEY_DecalShader, pShader);

	//----------------------------
	// Frustum Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::After);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader3D, STR_FUNC_NAME_VTX_Std3DFrame);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader3D, STR_FUNC_NAME_PIX_Std3DFrame);
	pShader->SetRasterizerState(E_RasterizerState::Wireframe);
	pShader->SetDepthStencilState(E_DepthStencilState::Less);
	pShader->SetBlendState(E_BlendState::Default);
	AddRes(STR_KEY_Std3DFrameShader, pShader);

	//----------------------------
	// Tessellation Test Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_TessellationShader, STR_FUNC_NAME_VTX_Tess);
	pShader->CreateHullShader(STR_FILE_PATH_TessellationShader, STR_FUNC_NAME_HS_Tess);
	pShader->CreateDomainShader(STR_FILE_PATH_TessellationShader, STR_FUNC_NAME_DS_Tess);
	pShader->CreatePixelShader(STR_FILE_PATH_TessellationShader, STR_FUNC_NAME_PIX_Tess);
	pShader->SetRasterizerState(E_RasterizerState::Wireframe);
	pShader->SetDepthStencilState(E_DepthStencilState::Less);
	pShader->SetBlendState(E_BlendState::Default);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	AddRes(STR_KEY_TessellationTestShader, pShader);

	//---------------------------
	// Terrain Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Deferred);
	pShader->CreateVertexShader(STR_FILE_PATH_TerrainShader, STR_FUNC_NAME_VTX_Terrain);
	pShader->CreateHullShader(STR_FILE_PATH_TerrainShader, STR_FUNC_NAME_HS_Terrain);
	pShader->CreateDomainShader(STR_FILE_PATH_TerrainShader, STR_FUNC_NAME_DS_Terrain);
	pShader->CreatePixelShader(STR_FILE_PATH_TerrainShader, STR_FUNC_NAME_PIX_Terrain);
	//pShader->SetRasterizerState(E_RasterizerState::Wireframe);
	pShader->SetDepthStencilState(E_DepthStencilState::Less);
	pShader->SetBlendState(E_BlendState::Default);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_0, _T("FaceX Count") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_1, _T("FaceZ Count") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Height Map Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_1, _T("Weight Map Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector2_0, _T("Height Map Resolution") });

	AddRes(STR_KEY_TerrainShader, pShader);

	//----------------------
	// Debug 렌더링용 Shader
	pShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_DebugSphereShader, STR_FUNC_NAME_VTXDebug3D);
	pShader->CreatePixelShader(STR_FILE_PATH_DebugSphereShader, STR_FUNC_NAME_PIXDebug3D);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::Wireframe);
	pShader->SetBlendState(E_BlendState::Default);

	// Shader Param
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Color") });

	AddRes(STR_KEY_DebugRenderingShader, pShader);
}

void CResourceManager::CreateDefaultMaterial()
{
	// 기본 재질 생성 (AlphaBlend Coverage)
	CMaterial* pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderAlphaBlendCV = LoadRes<CGraphicsShader>(STR_KEY_StdAlphaBlend_CoverageShader);
	pMtrl->SetShader(pShaderAlphaBlendCV);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl, pMtrl);

	// 기본 재질 생성 (AlphaBlend)
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderAlphaBlend = LoadRes<CGraphicsShader>(STR_KEY_StdAlphaBlendShader);
	pMtrl->SetShader(pShaderAlphaBlend);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes<CMaterial>(STR_KEY_StdAlphaBlendMtrl, pMtrl);

	// Line Strip
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderLineStrip = LoadRes<CGraphicsShader>(STR_KEY_StdLineStripShader);
	pMtrl->SetShader(pShaderLineStrip);
	pMtrl->SetData(E_ShaderParam::Vector4_0, Vector4{ 1.f,0.f,1.f,1.f });
	AddRes<CMaterial>(STR_KEY_LineStripMtrl, pMtrl);

	// Light2D 재질 설정
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderLight2D = LoadRes<CGraphicsShader>(STR_KEY_StdLight2DShader);
	pMtrl->SetShader(pShaderLight2D);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes<CMaterial>(STR_KEY_StdLight2DMtrl, pMtrl);

	// Light2D Animation 재질 설정
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderLight2DAnim = LoadRes<CGraphicsShader>(STR_KEY_StdLight2DCartoonShader);
	pMtrl->SetShader(pShaderLight2DAnim);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes<CMaterial>(STR_KEY_StdLight2DCartoonMtrl, pMtrl);

	// Collider2D 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderCollider2D = LoadRes<CGraphicsShader>(STR_KEY_Collider2DShader);
	pMtrl->SetShader(pShaderCollider2D);
	pMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.2f, 0.9f, 0.2f, 1.f)); // green
	pMtrl->SetData(E_ShaderParam::Vector4_1, Vector4(0.9f, 0.2f, 0.2f, 1.f)); // red
	AddRes(STR_KEY_Collider2DNoneColliedMtrl, pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(pShaderCollider2D);
	int iCollision = 1;
	pMtrl->SetData(E_ShaderParam::Int_0, &iCollision);
	pMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.2f, 0.9f, 0.2f, 1.f)); // green
	pMtrl->SetData(E_ShaderParam::Vector4_1, Vector4(0.9f, 0.2f, 0.2f, 1.f)); // red
	AddRes(STR_KEY_Collider2DCollisionMtrl, pMtrl);

	// 타일맵 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderTileMap = LoadRes<CGraphicsShader>(STR_KEY_TileMapShader);
	pMtrl->SetShader(pShaderTileMap);
	int iLightDisable = 0;
	pMtrl->SetData(E_ShaderParam::Int_2, &iLightDisable);
	AddRes(STR_KEY_TileMapMtrl, pMtrl);

	// 빛 적용된 타일맵 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderTileMapLight = LoadRes<CGraphicsShader>(STR_KEY_TileMapLightShader);
	pMtrl->SetShader(pShaderTileMapLight);
	int iLightEnable = 1;
	pMtrl->SetData(E_ShaderParam::Int_2, &iLightEnable);
	AddRes(STR_KEY_TileMapLightMtrl, pMtrl);

	// 파티클 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderParticle = LoadRes<CGraphicsShader>(STR_KEY_ParticleShader);
	pMtrl->SetShader(pShaderParticle);
	AddRes(STR_KEY_ParticleMtrl, pMtrl);

	// 왜곡(Distortion) 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderDistortion = LoadRes<CGraphicsShader>(STR_KEY_DistortionShader);
	pMtrl->SetShader(pShaderDistortion);

	SharedPtr<CTexture> pPostEffectTargetTex = LoadRes<CTexture>(STR_ResourceKey_PostEffectTargetTexture);
	pMtrl->SetData(E_ShaderParam::Texture_0, pPostEffectTargetTex.Get());
	AddRes(STR_KEY_DistortionMtrl, pMtrl);

	// FishEye 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderFishEye = LoadRes<CGraphicsShader>(STR_KEY_FishEyeShader);
	pMtrl->SetShader(pShaderFishEye);

	pPostEffectTargetTex = LoadRes<CTexture>(STR_ResourceKey_PostEffectTargetTexture);
	pMtrl->SetData(E_ShaderParam::Texture_0, pPostEffectTargetTex.Get());
	AddRes(STR_KEY_FishEyeMtrl, pMtrl);

	// Blur 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderBlur = LoadRes<CGraphicsShader>(STR_KEY_BlurShader);
	pMtrl->SetShader(pShaderBlur);
	pPostEffectTargetTex = LoadRes<CTexture>(STR_ResourceKey_PostEffectTargetTexture);
	pMtrl->SetData(E_ShaderParam::Texture_0, pPostEffectTargetTex.Get());
	AddRes(STR_KEY_BlurMtrl, pMtrl);

	// Noise 재질 생성 
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderNoise = LoadRes<CGraphicsShader>(STR_KEY_NoiseShader);
	pMtrl->SetShader(pShaderNoise);
	pPostEffectTargetTex = LoadRes<CTexture>(STR_ResourceKey_PostEffectTargetTexture);
	SharedPtr<CTexture> pNoiseTex = LoadRes<CTexture>(STR_FILE_PATH_NoiseTexture1);
	pMtrl->SetData(E_ShaderParam::Texture_0, pPostEffectTargetTex.Get());
	pMtrl->SetData(E_ShaderParam::Texture_1, pNoiseTex.Get());

	AddRes(STR_KEY_NoiseMtrl, pMtrl);


	// Canvas 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderCanvas = LoadRes<CGraphicsShader>(STR_KEY_CanvasShader);
	pMtrl->SetShader(pShaderCanvas);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes(STR_KEY_CanvasMtrl, pMtrl);

	// Fog 2D 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderFog2D = LoadRes<CGraphicsShader>(STR_KEY_Fog2DShader);
	pMtrl->SetShader(pShaderFog2D);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Fog2D).Get());
	AddRes(STR_KEY_Fog2DMtrl, pMtrl);

	// ButtonUI 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pButtonUI = LoadRes<CGraphicsShader>(STR_KEY_ButtonUIShader);
	pMtrl->SetShader(pButtonUI);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes(STR_KEY_ButtonUIMtrl, pMtrl);

	// Grid frame 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderLineRect = LoadRes<CGraphicsShader>(STR_KEY_GridShader);
	pMtrl->SetShader(pShaderLineRect);
	Vector4 vDefaultColor = Vector4(1.f, 1.f, 1.f, 1.f); // white
	pMtrl->SetData(E_ShaderParam::Vector4_0, vDefaultColor);
	AddRes(STR_KEY_GridMtrl, pMtrl);

	// Line List 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderLineList = LoadRes<CGraphicsShader>(STR_KEY_LineListShader);
	pMtrl->SetShader(pShaderLineList);
	AddRes(STR_KEY_LineListMtrl, pMtrl);

	// Std 3D 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderStd3D = LoadRes<CGraphicsShader>(STR_KEY_Std3DShader);
	pMtrl->SetShader(pShaderStd3D);
	AddRes(STR_KEY_Std3DMtrl, pMtrl);

	// Std Deferred 3D 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderStd3DDeferred = LoadRes<CGraphicsShader>(STR_KEY_Std3DDeferredShader);
	pMtrl->SetShader(pShaderStd3DDeferred);
	AddRes(STR_KEY_Std3DDeferredMtrl, pMtrl);

	// Direction light 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderDirectionLight = LoadRes<CGraphicsShader>(STR_KEY_DirectionLightShader);
	pMtrl->SetShader(pShaderDirectionLight);
	AddRes(STR_KEY_DirectionLightMtrl, pMtrl);

	// Point light 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderPointLight = LoadRes<CGraphicsShader>(STR_KEY_PointLightShader);
	pMtrl->SetShader(pShaderPointLight);
	AddRes(STR_KEY_PointLightMtrl, pMtrl);

	// Spot light 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderSpotLight = LoadRes<CGraphicsShader>(STR_KEY_SpotLightShader);
	pMtrl->SetShader(pShaderSpotLight);
	AddRes(STR_KEY_SpotLightMtrl, pMtrl);


	// Merge Shader 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderMerge = LoadRes<CGraphicsShader>(STR_KEY_MergeShader);
	pMtrl->SetShader(pShaderMerge);
	AddRes(STR_KEY_MergeMtrl, pMtrl);

	// Shadow Depth
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderShadowDepth = LoadRes<CGraphicsShader>(STR_KEY_ShadowDepthShader);
	pMtrl->SetShader(pShaderShadowDepth);
	AddRes(STR_KEY_ShadowDepthMtrl, pMtrl);

	// Skybox
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderSkybox = LoadRes<CGraphicsShader>(STR_KEY_SkyboxShader);
	pMtrl->SetShader(pShaderSkybox);

	SharedPtr<CTexture> pSkyboxCubeTex = LoadRes<CTexture>(STR_FILE_PATH_SkyboxCubeTexture);
	SharedPtr<CTexture> pSkyboxSphereTex = LoadRes<CTexture>(STR_FILE_PATH_SkyboxSphereTexture);

	pMtrl->SetData(E_ShaderParam::TextureCube_0, pSkyboxCubeTex.Get());
	pMtrl->SetData(E_ShaderParam::Texture_0, pSkyboxSphereTex.Get());
	AddRes(STR_KEY_SkyboxMtrl, pMtrl);

	// Decal Debug
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderDecalDebug = LoadRes<CGraphicsShader>(STR_KEY_DecalDebugShader);
	pMtrl->SetShader(pShaderDecalDebug);
	AddRes(STR_KEY_DecalDebugMtrl, pMtrl);

	// Decal
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderDecal = LoadRes<CGraphicsShader>(STR_KEY_DecalShader);
	pMtrl->SetShader(pShaderDecal);
	AddRes(STR_KEY_DecalMtrl, pMtrl);
	
	// Frustum
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderFrustum = LoadRes<CGraphicsShader>(STR_KEY_Std3DFrameShader);
	pMtrl->SetShader(pShaderFrustum);
	AddRes(STR_KEY_Std3DFrameMtrl, pMtrl);

	// Tessellation Test
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderTessTest = LoadRes<CGraphicsShader>(STR_KEY_TessellationTestShader);
	pMtrl->SetShader(pShaderTessTest);
	AddRes(STR_KEY_TessellationTestMtrl, pMtrl);

	// Terrain Mtrl
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderTerrain = LoadRes<CGraphicsShader>(STR_KEY_TerrainShader);
	pMtrl->SetShader(pShaderTerrain);
	AddRes(STR_KEY_TerrainMtrl, pMtrl);

	// Debug Rendering Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(_T("DebugMaterial"));
	SharedPtr<CGraphicsShader> pShaderDebugRendering = LoadRes<CGraphicsShader>(STR_KEY_DebugRenderingShader);
	pMtrl->SetShader(pShaderDebugRendering);
	AddRes(STR_KEY_DebugRenderingMtrl, pMtrl);
}

#include "CTestShader.h"
#include "CParticleUpdateShader.h"
#include "CRaycastShader.h"
#include "CHeightMapShader.h"
#include "CWeightMapShader.h"

void CResourceManager::CreateComputeShader()
{
	// TODO (Jang) : Test용 컴퓨트 쉐이더를 생성하고있음. 나중에 고치기
	CComputeShader* pShader = new CTestShader;
	pShader->CreateComputeShader(STR_FILE_PATH_TestComputeShader, "CS_Test");
	AddRes(STR_KEY_TestComputeShader, pShader);

	/////////////////////////
	// Particle Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(STR_FILE_PATH_ParticleUpdateShader, STR_FUNC_NAME_ParticleUpdate);
	AddRes(STR_KEY_ParticleUpdateShader, pShader);

	// Raycast Shader
	pShader = new CRaycastShader;
	pShader->CreateComputeShader(STR_FILE_PATH_RaycastShader, STR_FUNC_NAME_Raycast);
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("HeightMapTex") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Camera Pos") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_1, _T("Camera Dir") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_0, _T("FaceX") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_1, _T("FaceZ") });
	AddRes(STR_KEY_RaycastShader, pShader);

	// Height Map Shader
	pShader = new CHeightMapShader;
	pShader->CreateComputeShader(STR_FILE_PATH_HeightMapShader, STR_FUNC_NAME_HeightMap);
	AddRes(STR_KEY_HeightMapShader, pShader);

	// Weight Map Shader
	pShader = new CWeightMapShader;
	pShader->CreateComputeShader(STR_FILE_PATH_WeightMapShader, STR_FUNC_NAME_WeightMap);
	AddRes(STR_KEY_WeightMapShader, pShader);
}

void CResourceManager::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
		assert(nullptr);

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResourceManager::InitInputLayout()
{
	g_vecLayoutDesc.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"POSITION",						//SemanticName;
			0,								//SemanticIndex;
			DXGI_FORMAT_R32G32B32_FLOAT,	//Format; (offset 0부터 12byte인것을 포멧으로 알려준다.)
			0,								//InputSlot;
			0,								//AlignedByteOffset;
			D3D11_INPUT_PER_VERTEX_DATA,	//InputSlotClass (시멘틱이 정점마다 존재한다 (덩어리 단계로))
			0								//InstanceDataStepRate;
		}
	);
	g_vecLayoutDesc.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"COLOR",						//SemanticName;
			0,								//SemanticIndex;
			DXGI_FORMAT_R32G32B32A32_FLOAT,	//Format; (12byte ~28)
			0,								//InputSlot;
			12,								//AlignedByteOffset;
			D3D11_INPUT_PER_VERTEX_DATA,	//InputSlotClass
			0								//InstanceDataStepRate;
		}

	);
	g_vecLayoutDesc.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"TEXCOORD",						//SemanticName;
			0,								//SemanticIndex;
			DXGI_FORMAT_R32G32_FLOAT,		//Format;
			0,								//InputSlot;
			28,								//AlignedByteOffset;
			D3D11_INPUT_PER_VERTEX_DATA,	//InputSlotClass
			0								//InstanceDataStepRate;
		}
	);

	g_vecLayoutDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	g_vecLayoutDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	g_vecLayoutDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 });
}

void CResourceManager::CreateDefaultTexture()
{
	tstring strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += STR_FILE_PATH_NoiseTexture1;
	CTexture* pNoiseTex = new CTexture();
	pNoiseTex->Load(strPath);
	m_umapDefaultTex.insert(std::make_pair(STR_FILE_PATH_NoiseTexture1, pNoiseTex));

	strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += STR_FILE_PATH_NoiseTexture2;

	CTexture* pNoiseTex2 = new CTexture();
	pNoiseTex2->Load(strPath);
	m_umapDefaultTex.insert(std::make_pair(STR_FILE_PATH_NoiseTexture2, pNoiseTex2));


	pNoiseTex->UpdateData(E_ShaderStage::All, REGISTER_NUM_NoiseTexture);
	
	g_globalConst.vNoiseResolution = Vector2((float)pNoiseTex->GetResolution().x, (float)pNoiseTex->GetResolution().y);

	strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += STR_FILE_PATH_SkyboxSphereTexture;

	CTexture* pSkyboxSphereTex = new CTexture();
	pSkyboxSphereTex->Load(strPath);
	m_umapDefaultTex.insert(std::make_pair(STR_FILE_PATH_SkyboxSphereTexture, pSkyboxSphereTex));

	strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += STR_FILE_PATH_SkyboxCubeTexture;

	CTexture* pSkyboxCubeTex = new CTexture();
	pSkyboxCubeTex->Load(strPath);
	m_umapDefaultTex.insert(std::make_pair(STR_FILE_PATH_SkyboxCubeTexture, pSkyboxCubeTex));

	Vector2 vResolution = CDevice::GetInstance()->GetRenderResolution();
	m_pPostEffectTargetTex = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_PostEffectTargetTexture, (UINT)vResolution.x, (UINT)vResolution.y, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	//  2021 11 19 fixed
	//SharedPtr<CTexture> pTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_FILE_PATH_NoiseTexture1);
	//CResourceManager::GetInstance()->LoadRes<CTexture>(STR_FILE_PATH_NoiseTexture2);

	//pTexture->UpdateData(E_ShaderStage::All, REGISTER_NUM_NoiseTexture);

	//g_globalConst.vNoiseResolution = Vector2((float)pTexture->GetResolution().x, (float)pTexture->GetResolution().y);
}

// param(_iBindFlag) : D3D11_BIND_FLAG
SharedPtr<CTexture> CResourceManager::CreateTexture(const tstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag)
{
	SharedPtr<CTexture> pTexture = FindRes<CTexture>(_strKey);
	if (nullptr != pTexture) {
		assert(nullptr && _T("텍스쳐 생성 실패"));
		return pTexture;
	}

	pTexture = new CTexture;
	pTexture->Create(_iWidth, _iHeight, _eFormat, _iBindFlag);
	AddRes<CTexture>(_strKey, pTexture.Get());
	m_bFixed = true;
	return pTexture;
}

SharedPtr<CTexture> CResourceManager::CreateTexture(const tstring& _strKey, ComPtr<ID3D11Texture2D> _pTexture2D)
{
	SharedPtr<CTexture> pTexture = FindRes<CTexture>(_strKey);
	if (nullptr != pTexture) {
		assert(nullptr && _T("텍스쳐 생성 실패"));
		return pTexture;
	}

	pTexture = new CTexture;
	pTexture->Create(_pTexture2D);
	AddRes<CTexture>(_strKey, pTexture.Get());
	m_bFixed = true;
	return pTexture;
}

void CResourceManager::ChangeResourceKeyEvn(const tstring& _strKey, const tstring& _strKeyChange, E_ResourceType _eResourceType)
{
	TEvent even = {};
	even.eType = E_EventType::Chagne_ResourceKey;
	tstring* pStrKey = new tstring(_strKey);
	tstring* pStrKeyChange = new tstring(_strKeyChange);
	even.lparam = (DWORD_PTR)pStrKey;
	even.wparam = (DWORD_PTR)pStrKeyChange;
	even.mparam= (DWORD_PTR)_eResourceType; 
	
	CEventManager::GetInstance()->AddEvent(even);
}

void CResourceManager::DeleteCopiedMaterialEvn(const tstring& _strKey)
{
	TEvent even = {};
	even.eType = E_EventType::Remove_Material;
	tstring* pStrKey = new tstring(_strKey);
	even.lparam = (DWORD_PTR)pStrKey;
	CEventManager::GetInstance()->AddEvent(even);
}

void CResourceManager::DeleteCustomResourceEvn(const tstring& _strKey, E_ResourceType _eResourceType)
{
	TEvent even = {};
	even.eType = E_EventType::Remove_Resource;
	tstring* pStrKey = new tstring(_strKey);
	even.lparam = (DWORD_PTR)pStrKey;
	even.wparam = (DWORD_PTR)_eResourceType;
	CEventManager::GetInstance()->AddEvent(even);
}

void CResourceManager::_DeleteCopiedMaterial(const tstring& _strKey)
{
	// 복사된 메터리얼에서 찾기.
	SharedPtr<CMaterial> pMtrl = FindRes<CMaterial>(_strKey);
	tstring strRelativePath = pMtrl->GetRelativePath();
	tstring strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += strRelativePath;

	if (pMtrl.Get()) {
		{ // 복사된 메터리얼
			vector<CMaterial*>::iterator iter = m_vecCloneMtrl.begin();
			for (; iter != m_vecCloneMtrl.end(); ++iter) {
				if ((*iter)->GetKey() == _strKey) {
					m_bFixed = true;
					CMaterial* pMtrl = *iter;
					SAFE_DELETE(pMtrl);
					m_vecCloneMtrl.erase(iter);
					break;
				}
			}
		}
		{ // 리소스쪽 메터리얼
			unordered_map<tstring, CResource*>::iterator iter = m_umapResource[(UINT)E_ResourceType::Material].begin();
			for (; iter != m_umapResource[(UINT)E_ResourceType::Material].end(); ++iter) {
				if ((*iter).first == _strKey) {
					m_bFixed = true;
					CResource* pMtrl = iter->second;
					SAFE_DELETE(pMtrl);
					m_umapResource[(UINT)E_ResourceType::Material].erase(iter);
					break;
				}
			}
		}
	}

	if (-1 == _tremove(strPath.c_str())) {
		//assert(nullptr && _T("파일 삭제 실패"));
	}
}

/// <param name="_strOldKey">경로명 + 키 이름 + 확장명으로 되어있는 Key</param>
/// <param name="_strNewKey">키 이름</param>
/// <param name="_eResourceType">리소스 타입</param>
bool CResourceManager::_ChangeResourceKey(const tstring& _strOldKey, const tstring& _strNewKey, E_ResourceType _eResourceType)
{
	bool bIsChangedName = false;

	if (_strOldKey == _strNewKey)
		return bIsChangedName;

	SharedPtr<CResource> pResource = nullptr;
	switch (_eResourceType) {
	case E_ResourceType::Prefab: {
		SharedPtr<CPrefab> pPrefab = FindRes<CPrefab>(_strOldKey).Get();
		if (nullptr != pPrefab && pPrefab->GetProtoObj()) {
			// 파일 이름 변경
			tstring strOldFilePath = CPathManager::GetInstance()->GetContentPath() + pPrefab->GetRelativePath();

			tstring strNewPrefabKey = STR_FILE_PATH_Prefab + _strNewKey + _T(".pref");
			tstring strNewFilePath = CPathManager::GetInstance()->GetContentPath() + strNewPrefabKey;
			if (_trename(strOldFilePath.c_str(), strNewFilePath.c_str()) == -1) {
				assert(nullptr && _T("Prefab 파일 이름을 바꿀 수 없습니다."));
				return bIsChangedName = false;
			}
				

			// 리소스의 Key를 바꿔서 다시 리소스에 저장
			pPrefab->SetKey(strNewPrefabKey);
			
			tstring strNewRelativePath = strNewPrefabKey;
			pPrefab->SetRelativePath(strNewRelativePath);
			pPrefab->Save(strNewRelativePath);

			// 리소스에서 삭제, 삽입
			unordered_map<tstring, CResource*>::iterator it = m_umapResource[(UINT)_eResourceType].find(_strOldKey);
			if (it != m_umapResource[(UINT)_eResourceType].end()) {
				m_umapResource[(UINT)_eResourceType].erase(it);
				AddRes(strNewPrefabKey, pPrefab.Get());
			}
				
			bIsChangedName = true;
		}
	}
		break;
	case E_ResourceType::Material: {
		SharedPtr<CMaterial> pMtrl = FindRes<CMaterial>(_strOldKey).Get();
		if (nullptr != pMtrl && nullptr != pMtrl.Get()) {
			// 파일 이름 변경
			tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
			strFilePath += pMtrl->GetRelativePath();
			tstring strOldFilePath = CPathManager::GetInstance()->GetContentPath() + pMtrl->GetRelativePath();

			tstring strNewKeyName = STR_DIR_PATH_Material + _strNewKey + _T(".mtrl");
			tstring strNewFilePath = CPathManager::GetInstance()->GetContentPath() + strNewKeyName;
				
			if (_trename(strOldFilePath.c_str(), strNewFilePath.c_str()) == -1) {
				assert(nullptr && _T("Material 파일 이름을 바꿀 수 없습니다."));
				return bIsChangedName = false;
			}
				

			// 이름 바꿔서 다시 리소스에 저장
			pMtrl->SetKey(strNewKeyName);

			tstring strRelativePath = strNewKeyName;
			pMtrl->SetRelativePath(strRelativePath);
			pMtrl->Save(strRelativePath);

			// 리소스에서 삭제, 삽입
			unordered_map<tstring, CResource*>::iterator it = m_umapResource[(UINT)_eResourceType].find(_strOldKey);
			if (it != m_umapResource[(UINT)_eResourceType].end()) {
				if (it->second)
				m_umapResource[(UINT)_eResourceType].erase(it);
				AddRes(strNewKeyName, pMtrl.Get());
			}
			bIsChangedName = true;
		}
	}
		break;
	case E_ResourceType::GraphicsShader:
		assert(nullptr && _T("아직 쓰기에는 위험하다"));
		pResource = FindRes<CGraphicsShader>(_strOldKey).Get();
		break;
	case E_ResourceType::ComputeShader:
		assert(nullptr && _T("아직 쓰기에는 위험하다"));
		pResource = FindRes<CComputeShader>(_strOldKey).Get();
		break;
	case E_ResourceType::Mesh:
		assert(nullptr && _T("아직 쓰기에는 위험하다"));
		pResource = FindRes<CMesh>(_strOldKey).Get();
		break;
	case E_ResourceType::Texture:
		assert(nullptr && _T("아직 쓰기에는 위험하다"));
		pResource = FindRes<CTexture>(_strOldKey).Get();
		break;
	case E_ResourceType::Sound:
		//pResource = FindRes<CSound>(_strKey).Get();
			// TOOD : 해야 됨.
		assert(nullptr && _T("미완성"));
		return bIsChangedName;
		break;
	default:
		assert(nullptr && _T("ResourceType : 알수없는 enum 값"));
		return bIsChangedName;
	}
	m_bFixed = true;
	return m_bFixed;
}

bool CResourceManager::_DeleteCustomResource(const tstring& _strKey, E_ResourceType _eResourceType)
{
	bool bIsDeleted = false; 
	SharedPtr<CResource> pResource = nullptr;
	switch (_eResourceType) {
	case E_ResourceType::Prefab:
		pResource = FindRes<CPrefab>(_strKey).Get();
		break;
	case E_ResourceType::Material:
		assert(nullptr && _T("아직 쓰기에는 위험하다"));
		pResource = FindRes<CMaterial>(_strKey).Get();
		break;
	case E_ResourceType::GraphicsShader:
		assert(nullptr && _T("아직 쓰기에는 위험하다"));
		pResource = FindRes<CGraphicsShader>(_strKey).Get();
		break;
	case E_ResourceType::ComputeShader:
		assert(nullptr && _T("아직 쓰기에는 위험하다"));
		pResource = FindRes<CComputeShader>(_strKey).Get();
		break;
	case E_ResourceType::Mesh:
		pResource = FindRes<CMesh>(_strKey).Get();
		break;
	case E_ResourceType::Texture:
		assert(nullptr && _T("아직 쓰기에는 위험하다"));
		pResource = FindRes<CTexture>(_strKey).Get();
		break;
	case E_ResourceType::Sound:
		//pResource = FindRes<CSound>(_strKey).Get();
			// TOOD : 해야 됨.
		assert(nullptr && _T("미완성"));
		return bIsDeleted;
		break;
	default:
		assert(nullptr && _T("ResourceType : 알수없는 enum 값"));
		return bIsDeleted;
	}
		
	if (nullptr == pResource.Get())
		return bIsDeleted;

	// 존재할경우 삭제한다.

	E_ResourceType eResourceType = pResource->GetResourceType();
	auto iter = m_umapResource[(UINT)eResourceType].begin();
	for (; iter != m_umapResource[(UINT)eResourceType].end(); ++iter) {
		if ((*iter).first == _strKey) {
			bIsDeleted = true;
			break;
		}
	}
	if (bIsDeleted) {
		SAFE_DELETE(iter->second);
		m_umapResource[(UINT)eResourceType].erase(iter);
	}

	m_bFixed = true;

	return bIsDeleted;
}

void CResourceManager::RenewResourcesFromDir(E_ResourceType _eType)
{
	unordered_map<tstring, CResource*>::iterator iter = m_umapResource[(UINT)_eType].begin();
	int iCnt = 0;
	for (; iter != m_umapResource[(UINT)_eType].end(); ++iter) {
		tstring strRelativePath = iter->second->GetRelativePath();
		iter->second->Save(strRelativePath);
		_tcprintf(_T("[Saved]%s [%d]\n"), strRelativePath.c_str(), ++iCnt); // Using
	}
}

void CResourceManager::GetResourceKeys(E_ResourceType _eType, vector<tstring>& _vecOut)
{
	_vecOut.clear();
	for (const auto& pair : m_umapResource[(UINT)_eType])
		_vecOut.push_back(pair.first);
}