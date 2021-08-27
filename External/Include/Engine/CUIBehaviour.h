#pragma once
#include "CBehaviour.h"
class CUIBehaviour : public CBehaviour
{
public:
	CLONE(CUIBehaviour);
	explicit CUIBehaviour();
	explicit CUIBehaviour(E_ComponentType _eComponentType); //Component TypeÀÏ °æ¿ì
	virtual ~CUIBehaviour() override;
};