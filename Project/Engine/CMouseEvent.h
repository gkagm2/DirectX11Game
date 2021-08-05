#pragma once

struct MousePoint {
	int x;
	int y;
};

class CMouseEvent
{
public:
	enum class EventType
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

private:
	EventType m_eEventType;
	int m_iX;
	int m_iY;

public:
	inline bool IsValid() const { return m_eEventType != EventType::Invalid; }
	inline EventType GetType() const { return m_eEventType; }
	inline MousePoint GetPos() const { return { m_iX, m_iY }; }
	inline int GetPosX() const { return m_iX; }
	inline int GetPosY() const { return m_iY; }

public:
	
	CMouseEvent();
	CMouseEvent(const EventType type, const int x, const int y);
	virtual ~CMouseEvent();
};