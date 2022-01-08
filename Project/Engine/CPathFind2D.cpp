#include "pch.h"
#include "CPathFind2D.h"
#include "CSceneManager.h"
#include "CScene.h"

const int CPathFind2D::m_iDirX[] = { 1, 0, -1, 0, 1, -1, 1, -1 };
const int CPathFind2D::m_iDirY[] = { 0, 1, 0, -1, 1, 1, -1, -1 };

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

float CPathFind2D::GetDistance(int x1, int y1, int x2, int y2)
{
	return sqrtf(float((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

bool CPathFind2D::IsUnBlocked(int x, int y)
{
	return true;
}

bool CPathFind2D::IsDestination(int x, int y, const Vector2& dest)
{
	if (x == (int)dest.x && y == (int)dest.y)
		return true;
	return false;
}

// ������ġ�������� ������������ ��θ� �����Ѵ�.
void CPathFind2D::TracePath(const vector<vector<Cell>>& cellDetails, const Vector2& dest)
{
	int x = (int)dest.x;
	int y = (int)dest.y;

	m_stkPath.clear();

	while (!(cellDetails[y][x].parentX == x && cellDetails[y][x].parentY == y)) {
		m_stkPath.push_front(Vector2(x,y));

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
	m_bFoundDestination = false;
	if (false == IsValid(start.x, start.y))
		return false;
	if (false == IsValid(dest.x, dest.y))
		return false;

	if (false == IsUnBlocked(start.x, start.y))
		return false;
	if (false == IsUnBlocked(dest.x, dest.y))
		return false;

	if (IsDestination(start.x, start.y, dest))
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

	// ���� ��带 �ʱ�ȭ �Ѵ�.
	int iX = start.x, iY = start.y; // i = y, j = x
	cellDetails[iY][iX].f = 0.f;
	cellDetails[iY][iX].g = 0.f;
	cellDetails[iY][iX].h = 0.f;
	cellDetails[iY][iX].parentX = iX;
	cellDetails[iY][iX].parentY = iY;

	// open list�� �����.
	set<pPair> openList;

	// ���� ������ f�� 0���� �д�.

	openList.insert(std::make_pair(0.f, std::make_pair(iX, iY)));


	while (!openList.empty()) {
		pPair p = *openList.begin();

		openList.erase(openList.begin()); // openList�� �ִ� vertex�� ����.

		iX = p.second.first; // x
		iY = p.second.second;  // y
		closedList[iY][iX] = true;

		// 8������ successor�� �����Ѵ�.
		for (int d = 0; d < 8; ++d) {
			int x = iX + m_iDirX[d];
			int y = iY + m_iDirY[d];

			if (false == IsValid(x, y))
				continue;

			if (true == IsDestination(x, y, dest)) {
				// ������ Cell�� �θ� �����Ѵ�.
				cellDetails[y][x].parentX = iX;
				cellDetails[y][x].parentY = iY;
				m_bFoundDestination = true;
				TracePath(cellDetails, dest);
				return true;
			}
			else if (false == closedList[y][x] && true == IsUnBlocked(x, y)) {
				float gNew = cellDetails[iY][iX].g + 1.0f;
				float hNew = (float)GetDistance(x, y, dest.x, dest.y);
				float fNew = gNew + hNew;

				// openList�� �ƴϸ� openList�� �߰��Ѵ�.
				// �̹� openList�� 
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
	m_setObstacleTile.insert(_vPos);
}

void CPathFind2D::DeleteObstacleTile(Vector2 _vPos)
{
	m_setObstacleTile.erase(_vPos);
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

bool CPathFind2D::IsObstacle(int _iCol, int _iRow)
{
	for (auto iter = m_setObstacleTile.begin(); iter != m_setObstacleTile.end(); ++iter) {
		if (_iCol == iter->x && _iRow == iter->y)
			return true;
	}
	return false;
}