#pragma once
class CObject
{
private:
	tstring m_strName;
public:
	const tstring& GetName() const { return m_strName; }
	void SetName(const tstring& _strName) { m_strName = _strName; }

public:
	CObject();
	virtual ~CObject();
};