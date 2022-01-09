#pragma once
class CWayPoint2D
{
	vector<Vector2> m_vecWays;
public:
	void Init();
	void Update();

public:
	void AddWayPoint(const Vector2& _vPoint);
	void DeleteWayPoint(int _iIdx);

public:
	CWayPoint2D();
	virtual ~CWayPoint2D();
};