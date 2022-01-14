#pragma once

struct Cell {
	int parentX, parentY;
	float  f, g, h; // f:가장 낮은 노드 , g: , h: 휴리스틱
};

class CPathFind2D
{
public:
	list<Vector2> m_stkPath;
	set<Vector2> m_setObstacleTile;

	bool m_bFoundDestination;

	// ↖ ↑ ↗ ← → ↙ ↓ ↘ (index 순서에 따른 뱡향)
	static const int m_iDirX[8];
	static const int m_iDirY[8];

	typedef std::pair<int, int> Pair;
	typedef std::pair<float, std::pair<int, int> > pPair;

	int m_iRow;
	int m_iCol;

	Vector2 m_vOriginPos;
public:
	void Init();
	void Update();

protected:
	// done
	float _GetDistance(int x1, int y1, int x2, int y2);

	bool _IsUnBlocked(int x, int y);
	// done
	bool _IsDestination(int x, int y, const Vector2& dest);
	// done
	void _TracePath(const vector<vector<Cell> >& cellDetails, const Vector2& dest);

public:

	bool IsValid(int x, int y);
	bool FindPath(const Vector2& start, const Vector2& dest);

	list<Vector2>& GetPath() { return m_stkPath; }
	const Vector2& GetNextPath() { return m_stkPath.front(); }

	// 장애물로 취급할 타일 타입 설정
	void AddObstacleTile(Vector2 _vPos);
	void DeleteObstacleTile(Vector2 _vPos);
	const set<Vector2>& GetObstacleTiles() { return m_setObstacleTile; }

	bool IsArrivedDestination();
	bool IsFoundDestination() { return m_bFoundDestination; }
	bool IsObstacle(const Vector2& _vObstaclePos);

	void SetColRow(int _iCol, int _iRow) { m_iCol = _iCol; m_iRow = _iRow; }
	void SetOriginPos(const Vector2& _vOriginPos){ m_vOriginPos = _vOriginPos; }
	Vector2 GetOffsetPos() { return -m_vOriginPos; }

private:
	int _GetCol() { return m_iCol; }
	int _GetRow() { return m_iRow; }
	
public:
	CPathFind2D();
	virtual ~CPathFind2D();
};