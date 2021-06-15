#pragma once

class CCollider;
class CCollisionManager : public CSingleton<CCollisionManager>
{
	SINGLETON(CCollisionManager)

private:

#pragma region Bit flag version
	// UINT arrLayerFlag[(UINT)E_Layer::End];  
#pragma endregion


	bitset<(UINT)E_Layer::End> m_bitsetCollisionGroup[(UINT)E_Layer::End];
	vector<CCollider*> m_vecObjs;

public:
	void Update();

public:
	void CollisionByLayer(UINT _iLayerOneIdx, UINT _iLayerTwoIdx); // 레이어간 충돌

	void SetOnOffCollision(E_Layer _eLayerOne, E_Layer _eLayerTwo, bool _bIsOn); // 선택한 두 레이어들끼리 충돌을 할지 말지 Flag 세팅
	void ClearAllCollisionLayer() {
		// memset(arrLayerFlag, 0, size(UINT) * (UINT)E_Layer::End);
		for (UINT i = 0; i < (UINT)E_Layer::End; ++i)
			m_bitsetCollisionGroup[i].reset();
	}
};