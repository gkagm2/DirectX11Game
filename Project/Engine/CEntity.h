#pragma once
class CEntity
{
private:
	tstring m_strName;

public:
	void SetName(const tstring& _strName) { m_strName = _strName; }
	const tstring& GetName() const { return m_strName; }
	
public:
	CEntity();
	virtual ~CEntity();
};