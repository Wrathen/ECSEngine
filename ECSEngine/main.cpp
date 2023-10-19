// 20th October 2023
// v0.02 of ECSEngine
// GitHub.com/wrathen

#include <iostream>
#include "ECS/Components/C_Health.h"
#include "ECS/Components/C_Experience.h"
#include "ECS/Systems/S_DealDamageToAll.h"
 
/*
	Known Issues:
		- Component pointers (returned from ECS.AddComponent()) gets corrupted after the vector being resized. 
	A quick solution to this was giving component object vectors the initial capacity of 10000 (line 266 ECSEngine.h). 
	But this is a bad solution. Work on this later. Probably will need to restructure stuff.

		- There is no "ECS.RemoveEntity()" function yet, make that happen.
*/


int main() {
	// Creating new Entities
	Entity player = ECS.AddEntity();
	Entity creature = ECS.AddEntity();

	// Adding Components into Entities.
	auto a1 = ECS.AddComponent<C_Health>(player);
	auto a2 = ECS.AddComponent<C_Health>(creature);
	auto a3 = ECS.AddComponent<C_Experience>(player);
	auto a4 = ECS.AddComponent<C_Experience>(creature);

	// Getting Components.
	if (C_Health* playerHealth = ECS.GetComponent<C_Health>(player)) {
		std::cout << "Player Health: " << playerHealth->health << "/" << playerHealth->maxHealth << "\n";
		std::cout << playerHealth->testString << "\n\n";
	}

	// Adding Systems.
	ECS.AddSystem<S_DealDamageToAll>();

	// Updating all the added systems.
	ECS.UpdateSystems();

	// Print out again after all systems are updated.
	if (C_Health* playerHealth = ECS.GetComponent<C_Health>(player)) {
		std::cout << "Player Health: " << playerHealth->health << "/" << playerHealth->maxHealth << "\n";
		std::cout << playerHealth->testString << "\n\n";
	}

	// Utility Functions that are available.
	std::cout << "\n" << ECS.GetComponentTypeName<C_Health>() << " has a unique type ID of " << ECS.GetComponentTypeID<C_Health>();
	std::cout << "\n" << ECS.GetComponentTypeName<C_Experience>() << " has a unique type ID of " << ECS.GetComponentTypeID<C_Experience>() << "\n\n";
	return 0;
}