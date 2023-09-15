#include "S_DealDamageToAll.h"
#include "ECS/Components/C_Health.h"
#include "ECS/Components/C_Experience.h"

void LowerHealth() {
	for (C_Health& c : ECS.GetComponents<C_Health>())
		c.health -= 5;
}
void LowerXP() {
	for (C_Experience& c : ECS.GetComponents<C_Experience>())
		c.currentXP -= 5;
}

void S_DealDamageToAll::Update() {
	LowerHealth();
	LowerXP();
}