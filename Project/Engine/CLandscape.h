#pragma once
#include "CComponent.h"
class CLandscape : public CComponent {
private:

public:
	virtual void UpdateData() override;
	virtual void FinalUpdate() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CLandscape);
	CLandscape();
	virtual ~CLandscape() override;
};