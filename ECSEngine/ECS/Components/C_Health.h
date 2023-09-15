#pragma once
#include "ECS/ECSEngine.h"

struct C_Health : public Component {
	ECS_COMPONENT(C_Health)

	float health = 100;
	float maxHealth = 100;
	float healthRegen = 0.5f;
};