#pragma once
#include "CComponent.h"
class CBehaviour : public CComponent
{
public:
	CLONE(CBehaviour);
	//explicit CBehaviour();
	explicit CBehaviour(E_ComponentType _eType);
	virtual ~CBehaviour() override;
};