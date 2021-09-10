#pragma once

struct Cell {
	int parentX, parentY;
	float  f, g, h; // f:가장 낮은 노드 , g: , h: 휴리스틱
};

struct TTilePos {
	int x;
	int y;
	TTilePos(int _x, int _y) : x(_x), y(_y) {}
	TTilePos() : x(0), y(0) {}

	bool operator==(const TTilePos& _rhs) {
		if (x == _rhs.x && y == _rhs.y)
			return true;
		return false;
	}
	bool operator<(const TTilePos& rhs) const
	{
		if (x > rhs.x && y > rhs.y)
			return true;
		else if (x > rhs.x) {
			return true;
		}
		else if (y > rhs.y)
			return true;
		else
			return false;
	}
};

struct TTilePosComparae {
	bool operator()(const TTilePos& l, const TTilePos& r) {
		if (l.x > r.x)
			return true;
		else if (l.y > r.y)
			return true;
		else
			return false;
	}
};

// Tile Type
enum class E_TileType {
	Road,
	Sidewalk,
	Wall,
	Water,
	None
};

class CTileMap;
class CPathFinding
{
private:
	CTileMap* m_pTileMap;
	list<TTilePos> m_stkPath;
	set<E_TileType> m_setObstacleTile;

	bool m_bFoundDestination;

	// ↖ ↑ ↗ ← → ↙ ↓ ↘ (index 순서에 따른 뱡향)
	static const  int m_iDirX[8];
	static const int m_iDirY[8];

	typedef std::pair<int, int> Pair;
	typedef std::pair<float, std::pair<int, int> > pPair;

public:
	void Init();

protected:
	float GetDistance(int x1, int y1, int x2, int y2);
	bool IsUnBlocked(int x, int y);
	bool IsDestination(int x, int y, const TTilePos& dest);
	void TracePath(const vector<vector<Cell> >& cellDetails, const TTilePos& dest);

public:
	bool IsValid(int x, int y);
	bool FindPath(const TTilePos& start, const TTilePos& dest);

	list<TTilePos>& GetPath() { return m_stkPath; }
	const TTilePos& GetNextPath() { return m_stkPath.front();}

	// 장애물로 취급할 타일 타입 설정
	void AddObstacleTile(E_TileType _eTileType);
	void DeleteObstacleTile(E_TileType _eTileType);
	const set<E_TileType>& GetObstacleTiles() { return m_setObstacleTile; }

	bool IsArrivedDestination();
	bool IsFoundDestination() {	return m_bFoundDestination;	}
	bool IsObstacle(int _iCol, int _iRow);
public:
	CLONE(CPathFinding);
	CPathFinding(const CPathFinding& _origin);
public:
	CPathFinding();
	virtual ~CPathFinding();
};