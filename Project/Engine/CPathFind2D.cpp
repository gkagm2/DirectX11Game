#include "pch.h"
#include "CPathFind2D.h"
#include "CSceneManager.h"
#include "CScene.h"

//const int CPathFind2D::m_iDirX[] = { 1, 0, -1, 0, 1, -1, 1, -1 };
//const int CPathFind2D::m_iDirY[] = { 0, 1, 0, -1, 1, 1, -1, -1 };

const int CPathFind2D::m_iDirX[] = { 1, 0, -1, 0, };
const int CPathFind2D::m_iDirY[] = { 0, 1, 0, -1, };
const int dirCnt = 4;

CPathFind2D::CPathFind2D()
{
	SetColRow(300, 300);
}

CPathFind2D::~CPathFind2D()
{
}

void CPathFind2D::Init()
{
}

void CPathFind2D::Update()
{
}

float CPathFind2D::_GetDistance(int x1, int y1, int x2, int y2)
{
	return sqrtf(float((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

bool CPathFind2D::_IsUnBlocked(int x, int y)
{
	set<Vector2>::iterator iter = m_setObstacleTile.begin();
	for (; iter != m_setObstacleTile.end(); ++iter) {
		Vector2 tilePos = *iter;
		if (tilePos.x == x && tilePos.y == y)
			return false;
	}
	return true;
}

bool CPathFind2D::_IsDestination(int x, int y, const Vector2& dest)
{
	if (x == (int)dest.x && y == (int)dest.y) {
		return true;
	}
	return false;
}

// 시작위치에서부터 목적지까지의 경로를 추적한다.
void CPathFind2D::_TracePath(const vector<vector<Cell>>& cellDetails, const Vector2& dest)
{
	int x = (int)dest.x;
	int y = (int)dest.y;

	m_stkPath.clear();

	while (!(cellDetails[y][x].parentX == x && cellDetails[y][x].parentY == y)) {
		m_stkPath.push_front(Vector2(x - GetOffsetPos().x,y - GetOffsetPos().y));

		int tempX = cellDetails[y][x].parentX;
		int tempY = cellDetails[y][x].parentY;
		x = tempX;
		y = tempY;
	}
}

bool CPathFind2D::IsValid(int x, int y)
{
	int iCol = m_iCol;
	int iRow = m_iRow;
	if (x < 0 || y < 0 || x >= iCol ||  y >= iRow)
		return false;
	return true;
}

bool CPathFind2D::FindPath(const Vector2& start, const Vector2& dest)
{
	const Vector2& vecOriginMin =  GetOffsetPos();
	Vector2 vStart = start + vecOriginMin;
	Vector2 vDest = dest + vecOriginMin;
	m_bFoundDestination = false;
	if (false == IsValid(vStart.x, vStart.y))
		return false;
	if (false == IsValid(vDest.x, vDest.y))
		return false;

	if (false == _IsUnBlocked(vStart.x, vStart.y))
		return false;
	if (false == _IsUnBlocked(vDest.x, vDest.y))
		return false;

	if (_IsDestination(vStart.x, vStart.y, vDest))
		return false;

	int iRow = m_iRow;
	int iCol = m_iCol;
	vector<vector<bool> > closedList(iRow, vector<bool>(iCol, false));
	vector<vector<Cell> > cellDetails(iRow, vector<Cell>(iCol));

	// init cells
	for (int i = 0; i < iRow; ++i) {
		for (int j = 0; j < iCol; ++j) {
			cellDetails[i][j].f = (std::numeric_limits<float>::max)();
			cellDetails[i][j].g = (std::numeric_limits<float>::max)();
			cellDetails[i][j].h = (std::numeric_limits<float>::max)();
			cellDetails[i][j].parentX = -1;
			cellDetails[i][j].parentY = -1;
		}
	}

	// 시작 노드를 초기화 한다.
	int iX = vStart.x, iY = vStart.y; // i = y, j = x
	cellDetails[iY][iX].f = 0.f;
	cellDetails[iY][iX].g = 0.f;
	cellDetails[iY][iX].h = 0.f;
	cellDetails[iY][iX].parentX = iX;
	cellDetails[iY][iX].parentY = iY;

	// open list를 만든다.
	set<pPair> openList;

	// 시작 지점의 f를 0으로 둔다.

	openList.insert(std::make_pair(0.f, std::make_pair(iX, iY)));


	while (!openList.empty()) {
		pPair p = *openList.begin();

		openList.erase(openList.begin()); // openList에 있는 vertex를 삭제.

		iX = p.second.first; // x
		iY = p.second.second;  // y
		closedList[iY][iX] = true;

		// 8방향의 successor를 생성한다.
		for (int d = 0; d < dirCnt; ++d) {
			int x = iX + m_iDirX[d];
			int y = iY + m_iDirY[d];

			if (false == IsValid(x, y))
				continue;

			if (true == _IsDestination(x, y, vDest)) {
				// 목적지 Cell의 부모를 설정한다.
				cellDetails[y][x].parentX = iX;
				cellDetails[y][x].parentY = iY;
				m_bFoundDestination = true;
				_TracePath(cellDetails, vDest);
				return true;
			}
			else if (false == closedList[y][x] && true == _IsUnBlocked(x, y)) {
				float gNew = cellDetails[iY][iX].g + 1.0f;
				float hNew = (float)_GetDistance(x, y, vDest.x, vDest.y);
				float fNew = gNew + hNew;

				// openList가 아니면 openList에 추가한다.
				// 이미 openList면 
				if (cellDetails[y][x].f == (std::numeric_limits<float>::max)() ||
					cellDetails[y][x].f > fNew) {
					openList.insert(std::make_pair(fNew, std::make_pair(x, y)));
					cellDetails[y][x].f = fNew;
					cellDetails[y][x].g = gNew;
					cellDetails[y][x].h = hNew;
					cellDetails[y][x].parentX = iX;
					cellDetails[y][x].parentY = iY;
				}
			}
		}
	}

	return false;
}

void CPathFind2D::AddObstacleTile(Vector2 _vPos)
{
	Vector2 vPos = Vector2((int)_vPos.x, (int)_vPos.y);
	vPos = vPos + GetOffsetPos();
	m_setObstacleTile.insert(vPos);
}

void CPathFind2D::DeleteObstacleTile(Vector2 _vPos)
{
	Vector2 vPos = Vector2((int)_vPos.x, (int)_vPos.y);
	vPos = vPos + GetOffsetPos();
	m_setObstacleTile.erase(vPos);
}

bool CPathFind2D::IsArrivedDestination()
{
	if (m_bFoundDestination == false)
		return false;

	list<Vector2>& path = GetPath();
	if (path.size() == 0)
		return true;
	return false;
}

bool CPathFind2D::IsObstacle(const Vector2& _vObstaclePos)
{
	int iCol = (int)(_vObstaclePos.x + GetOffsetPos().x);
	int iRow = (int)(_vObstaclePos.y + GetOffsetPos().y);
	for (auto iter = m_setObstacleTile.begin(); iter != m_setObstacleTile.end(); ++iter) {
		if (iCol == iter->x && iRow == iter->y)
			return true;
	}
	return false;
}