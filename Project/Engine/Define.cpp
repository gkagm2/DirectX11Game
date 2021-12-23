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

tstring ParticleShapeToStr(E_ParticleShape _eType)
{
	tstring strShape = _T("");
	switch (_eType) {
	case E_ParticleShape::Circle_Combine:
		strShape = _T("Circle_Combine");
		break;
	case E_ParticleShape::Circle_Spread:
		strShape = _T("Circle_Spread");
		break;
	case E_ParticleShape::Cone_Spread:
		strShape = _T("Cone_Spread");
		break;
	case E_ParticleShape::Edge:
		strShape = _T("Edge");
		break;
	case E_ParticleShape::Sphere:
		strShape = _T("Sphere");
		break;
	default:
		assert(nullptr);
		break;
	}
	return strShape;
}
