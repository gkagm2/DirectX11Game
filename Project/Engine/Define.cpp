#include "pch.h"

tstring LightTypeToStr(E_LightType _eType)
{
	tstring strLightType = _T("");
	switch (_eType) {
	case E_LightType::Direction:
		strLightType = _T("Direction");
		break;
	case E_LightType::Point:
		strLightType = _T("Point");
		break;
	case E_LightType::Spot:
		strLightType = _T("Spot");
		break;
	default:
		assert(nullptr);
		break;
	}
	return strLightType;
}