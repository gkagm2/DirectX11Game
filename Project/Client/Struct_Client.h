#pragma once
#include "pch.h"
#include "imgui.h"

struct TRect {
	ImVec2 lt;
	ImVec2 ltUV;
	ImVec2 rt;
	ImVec2 rtUV;
	ImVec2 lb;
	ImVec2 lbUV;
	ImVec2 rb;
	ImVec2 rbUV;
};