#include "ECSEngine.h"

void ECSEngine::UpdateSystems() {
	for (auto& system : systems)
		system->Update();
}

// Make the ECSEngine a Singleton.
ECSEngine& ECSEngine::GetInstance() {
	static ECSEngine Instance;
	return Instance;
}