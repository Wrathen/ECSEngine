#pragma once
#include "ECS/ECSEngine.h"

struct C_Experience : public Component {
	ECS_COMPONENT(C_Experience)

	float currentXP = 0;
	float maxXP = 100;
};