#pragma once

struct TMousePoint {
	int x;
	int y;
};

enum class E_MouseEventType
{
	LPress,
	LRelease,
	RPress,
	RRelease,
	MPress,
	MRelease,
	WheelUp,
	WheelDown,
	Move,
	RAW_MOVE,
	Invalid
};

class CMouseEvent
{
public:
	

private:
	E_MouseEventType m_eEventType;
	int m_iX;
	int m_iY;

public:
	inline bool IsValid() const { return m_eEventType != E_MouseEventType::Invalid; }
	inline E_MouseEventType GetType() const { return m_eEventType; }
	inline TMousePoint GetPos() const { return { m_iX, m_iY }; }
	inline int GetPosX() const { return m_iX; }
	inline int GetPosY() const { return m_iY; }

public:
	
	CMouseEvent();
	CMouseEvent(const E_MouseEventType type, const int x, const int y);
	virtual ~CMouseEvent();
};