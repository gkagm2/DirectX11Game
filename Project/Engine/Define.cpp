#include "pch.h"

#ifdef _BUTCHER_GAME



tstring LayerToString(E_Layer _eLayer)
{
	tstring layerName = {};
	switch (_eLayer) {
	case E_Layer::Default:
		layerName = _T("Default");
		break;
	case E_Layer::Object:
		layerName = _T("Object");
		break;
	case E_Layer::BackgroundMap:
		layerName = _T("BackgroundMap");
		break;
	case E_Layer::WayPoint_Tool:
		layerName = _T("WayPoint_Tool");
		break;
	case E_Layer::Wall:
		layerName = _T("Wall");
		break;
	case E_Layer::ObjectParticle:
		layerName = _T("ObjectParticle");
		break;
	case E_Layer::TileMap:
		layerName = _T("TileMap");
		break;
	case E_Layer::UI:
		layerName = _T("UI");
		break;
	case E_Layer::CLIENT:
		layerName = _T("CLIENT");
		break;
	default:
		if (!((int)_eLayer >= 0 && (int)_eLayer < MAX_SIZE_LAYER))
			assert(nullptr);
		break;
	}
	return layerName;
}

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
	case E_Tag::DangerObj:
		tagName = _T("DangerObj");
		break;
	case E_Tag::Player_Bullet:
		tagName = _T("Player_Bullet");
		break;
	case E_Tag::Enemy_Bullet:
		tagName = _T("Enemy_Bullet");
		break;
	case E_Tag::Player_GroundCheck:
		tagName = _T("Player_GroundCheck");
		break;
	case E_Tag::Enemy_GroundCheck:
		tagName = _T("Enemy_GroundCheck");
		break;
	case E_Tag::ObjParticle:
		tagName = _T("ObjParticle");
		break;
	case E_Tag::Trigger:
		tagName = _T("Trigger");
		break;
	default:
		if (!((int)_eTag >= 0 && (int)_eTag < MAX_SIZE_TAG))
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
