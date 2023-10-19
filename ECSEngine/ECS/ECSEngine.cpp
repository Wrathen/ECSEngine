#include "ECSEngine.h"

// Make the ECSEngine a Singleton.
ECSEngine& ECSEngine::GetInstance() {
	static ECSEngine Instance;
	return Instance;
}

#pragma region World
// Gets the current world.
World* ECSEngine::GetWorld() { return &currentWorld; }

std::unordered_map<size_t, void*>* World::GetComponentMap(Entity entity) {
	// Find the given Entity's component map.
	auto it = entityComponentMap.find(entity);

	// If we couldn't find the Entity's component map, it means the entity does NOT exist.
	if (it == entityComponentMap.end()) {
		std::cout << "[Error] Entity is not found in the world!\n";
		std::cout << "EntityID: " << entity << "\n";
		return nullptr;
	}

	return &it->second;
}
#pragma endregion

#pragma region Entity
Entity ECSEngine::AddEntity() { return GetWorld()->AddEntity(); }
Entity World::AddEntity() {
	Entity entityID = entityComponentMap.size();
	entityComponentMap[entityID] = std::unordered_map<size_t, void*>();
	return entityID;
}

void ECSEngine::RemoveEntity(Entity entity) { GetWorld()->RemoveEntity(entity); }
void World::RemoveEntity(Entity entity) {
	GetComponentMap(entity)->clear();
}

#pragma endregion

#pragma region Component
#pragma endregion

#pragma region System
void ECSEngine::UpdateSystems() {
	for (auto& system : systems)
		system->Update();
}
#pragma endregion