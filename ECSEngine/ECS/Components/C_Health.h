#pragma once
#include "ECS/ECSEngine.h"

struct C_Health {
	ECS_COMPONENT(C_Health)

	float health = 100;
	float maxHealth = 100;
	float healthRegen = 0.5f;

	const char* testString = "Hey this is a test string like the old fox jumping all over the place!";
};