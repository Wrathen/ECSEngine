// 16th September 2023
// v0.01 of ECSEngine
// Currently doesn't have an 'entity' container thing, YET.
//
// Next commits will focus on component containers (entity)
// and probably an entity container ("world" or "scene") as well :D
//
// And also we need Removing components, Getting component of a given entity etc...
// Shouldn't be hard to implement these stuff, but lets see...
//
// GitHub.com/wrathen

#include <iostream>
#include "ECS/Components/C_Health.h"
#include "ECS/Components/C_Experience.h"
#include "ECS/Systems/S_DealDamageToAll.h"
#include "Miscellaneous/Timer.h"

int main() {
	// Create 1'000'000 components in total.
	Timer benchmark1("Creation of components");
	for (int i = 0; i < 500'000; ++i) ECS.AddComponent<C_Health>(0);
	for (int i = 0; i < 500'000; ++i) ECS.AddComponent<C_Experience>(0);
	benchmark1.Log();

	// Add a temporary system that iterates over all C_Health and C_Experiences and manipulate data inside each component.
	ECS.AddSystem<S_DealDamageToAll>();

	// Benchmark updating all the added systems.
	Timer benchmark2("Iteration of components in a single frame");
	ECS.UpdateSystems();
	benchmark2.Log();

	// Debug console prints.
	std::cout << "\n" << ECS.GetComponentTypeName<C_Health>() << " has a unique type ID of " << ECS.GetComponentTypeID<C_Health>();
	std::cout << "\n" << ECS.GetComponentTypeName<C_Experience>() << " has a unique type ID of " << ECS.GetComponentTypeID<C_Experience>() << "\n\n";

	std::cout << "3337th health component has a current health value of: " << ECS.GetComponents<C_Health>()[3337].health;

	return 0;
}