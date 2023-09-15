#pragma once
#include "ECS/ECSEngine.h"

class S_DealDamageToAll : System {
	ECS_SYSTEM(S_DealDamageToAll)

	void Update() override;
};