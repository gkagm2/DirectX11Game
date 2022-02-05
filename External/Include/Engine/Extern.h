#pragma once

extern vector< D3D11_INPUT_ELEMENT_DESC> g_vecLayoutDesc;

extern TTransform g_transform;
extern TGlobalConst g_globalConst;
extern TRectTransform g_rectTransform;

extern tstring ComponentTypeToStr(E_ComponentType _eType);
extern UINT ComponentStrToIdx(const tstring& _strComponent);
extern tstring ResourceTypeToStr(E_ResourceType _eType);
extern tstring CameraProjectionTypeToStr(E_ProjectionType _eType);


extern Vector4 ChangeColorUintToVector4(UINT _iColor);
extern UINT ChangeColorVector4ToUint(float _vColor[4]);