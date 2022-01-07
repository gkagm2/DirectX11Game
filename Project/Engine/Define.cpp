#include "pch.h"

#ifdef _BUTCHER_GAME



tstring TagToString(E_Tag _eTag)
{
	tstring tagName = {};
	switch (_eTag) {
	case E_Tag::Player:
		tagName = _T("Player");
		break;
	case E_Tag::Bullet:
		tagName = _T("Bullet");
		break;
	case E_Tag::Object:
		tagName = _T("Object");
		break;
	case E_Tag::Enemy:
		tagName = _T("Enemy");
		break;
	case E_Tag::Item:
		tagName = _T("Item");
		break;
	case E_Tag::Wall:
		tagName = _T("Wall");
		break;
	default:
		assert(nullptr);
		break;
	}
	return tagName;
}
#endif




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
