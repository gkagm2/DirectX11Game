#pragma once
#include <Engine\CScript.h>

enum class E_ModuleLevel_ca {
	Nemesis,//�ƹ��͵� ���� // Command Module������ ����Ѵ�.
	Alpha,	//�ʷϻ�
	Bravo,	//�����
	Charlie,//��Ȳ��
	Delta,	//������
	Echo,	//��ȫ��
	Foxtrot,//�����
	Golf,	//�Ķ���
	Hotel,	//�ϴû�
	India,	//����
	Juliet,	//���
	Kilo,	//ȸ��
	End,
};
extern const TCHAR* ModuleLevelSTR_ca[(UINT)E_ModuleLevel_ca::End];

enum class E_ModuleType_ca {
	Girder1x1,
	Girder1x2,
	Laser,
	Booster,
	Command,
	End,
};
extern const TCHAR* ModuleTypeSTR_ca[(UINT)E_ModuleType_ca::End];

enum class E_ModuleSize_ca {
	Size1x1,
	Size1x2,
	Size_OnlyMain,
	End
};

enum class E_Direction_ca{
	Forward,
	Left,
	Right,
	Back,
};
struct TModuleConnector_ca {
	Vector3 vPosition;	// ���� ��ġ (object coordination position)
	Vector3 vDirection;	// ���� ����
	bool bIsMain;		// Main ���ᱸ
	bool bIsConnectable;// ���� �������� ����
	E_Direction_ca eDirection;
};

class CModuleScript_ca : public CScript
{
private:
	vector<TModuleConnector_ca> m_vecModuleConnectors;
	TModuleConnector_ca* m_pParentModuleConnector; // ����� �θ��� ��� ������

	CGameObject* m_pParentModuleGameObj;
	vector<CGameObject*> m_pChildModuleGameObj;

	E_ModuleLevel_ca m_eModuleLevel;// ����
	E_ModuleType_ca m_eModuleType;	// Ÿ��
	E_ModuleSize_ca m_eModuleSize;	// ��� ������
	float m_fWeight;				// ����  
	float m_fHp;					// ü��

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	E_ModuleLevel_ca GetModuleLevel() { return m_eModuleLevel; }
	E_ModuleType_ca GetModuleType() { return m_eModuleType; }
	E_ModuleSize_ca GetModuleSize() { return m_eModuleSize; }

	float GetWeight() { return m_fWeight; }
	void SetWeight(float _fWeight) { m_fWeight = _fWeight; }
	float GetHp() { return m_fHp; }
	void SetHp(float _fHp) { m_fHp = _fHp; }

protected:
	void _SetModuleType(E_ModuleType_ca _eModuleType) { m_eModuleType = _eModuleType; }

public:
	void SetModuleLevel(E_ModuleLevel_ca _eModuleLevel);
	void SetModuleSize(E_ModuleSize_ca _eModuleSize);

	vector<TModuleConnector_ca>& GetConnectors() { return m_vecModuleConnectors; }

	// ������ϴ� ��ġ
	const Vector3& FindNearestConnectionPosition(const Vector3& _vPosition);
	TModuleConnector_ca& FindNearestConnector(const Vector3& _vPosition);
	// �����ϴ� ��ġ
	const Vector3& GetMainConnectionPosition();
	const Vector3& GetMainConnectionLocalPosition();
	TModuleConnector_ca& MainConnector();
	

	void ConnectModule(TModuleConnector_ca& _tOtherConnector);
	void DisConnectModule();
	

private:
	virtual void _InitModuleLevel(E_ModuleLevel_ca _eLevel) {};
	virtual void _InitModuleSize(E_ModuleSize_ca _eModuleSize);

	/*void ConnectPart(CGameObject _pPart, TPartConnectPoint_ca _tConnectPoint);
	void DisconnectPart();*/

protected:
	void AddModuleConnectPoint(const TModuleConnector_ca& _tModuleConnectPoint) { m_vecModuleConnectors.push_back(_tModuleConnectPoint); }
protected:
	void _ClearModuleConnectPoint() { m_vecModuleConnectors.clear(); }

public:
	CLONE(CModuleScript_ca);
	CModuleScript_ca();
	CModuleScript_ca(UINT _iScriptNum, E_ModuleType_ca _eModuleType, E_ModuleLevel_ca _eModuleLevel, E_ModuleSize_ca _eModuleSize);
	virtual ~CModuleScript_ca() override;
};