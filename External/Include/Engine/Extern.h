#pragma once

#define LAYOUT_DESCRIPTION_COUNT 3
extern D3D11_INPUT_ELEMENT_DESC g_arrLayoutDesc[LAYOUT_DESCRIPTION_COUNT];

extern TTransform g_transform;
extern TGlobalConst g_globalConst;
extern TRectTransform g_rectTransform;

extern tstring ComponentTypeToStr(E_ComponentType _eType);
extern UINT ComponentStrToIdx(const tstring& _strComponent);
extern tstring ResourceTypeToStr(E_ResourceType _eType);
extern tstring CameraProjectionTypeToStr(E_ProjectionType _eType);