#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>

// Temporary Variable
static bool LOGS_ARE_ENABLED = true;

// Base Class Definitions -- E, C, S
typedef size_t Entity; // Typedef "Entity" as a size_t because Entity is only an ID.
//struct Component { }; // Do not derive from this, not necessary.
class System { friend class ECSEngine; virtual void Update() = 0; };
class World {
	friend class ECSEngine;

protected:
	std::vector<Entity> activeEntities {}; // Unused as of now
	std::vector<Entity> sleepyEntities {}; // Unused as of now
	std::unordered_map<Entity, std::unordered_map<size_t, void*>> entityComponentMap {};

private:
	Entity AddEntity();
	void RemoveEntity(Entity entity);
	std::unordered_map<size_t, void*>* GetComponentMap(Entity entity);
};

// The Main Engine Class
// Use this class for all kind of factory/management/utility stuff related to ECS.
class ECSEngine {
public:
	// Identifier for the amount of unique component types.
	size_t uniqueComponentTypeCount = 0;

	// Make the ECSEngine a Singleton.
	static ECSEngine& GetInstance();

	// Gets the current world.
	World* GetWorld();

	// Updates all the added systems.
	void UpdateSystems();

public:
	#pragma region Entity
	Entity AddEntity();
	void RemoveEntity(Entity entity);
	#pragma endregion

	#pragma region Component
	// Instantiates an object of the given Component Type and associates it with the target Entity.
	template <typename T>
	T* AddComponent(Entity entity) {
		// Get the current world. This could be a parameter in the future but I didnt see any reason for now.
		World* world = GetWorld();
		if (!world) {
			if (LOGS_ARE_ENABLED)
				std::cout << "[Error] World does not exist.\n";
			return nullptr;
		}

		// Get the Component Map of the Entity.
		auto* componentMap = world->GetComponentMap(entity);

		// If the entity's component map was not found, return. It likely means that the entity doesn't exist either.
		if (!componentMap)
			return nullptr;

		// Check whether the target entity already has this component
		auto it = componentMap->find(GetComponentTypeID<T>());
		if (it != componentMap->end()) {
			if (LOGS_ARE_ENABLED) {
				std::cout << "[Warning] Entity already has this component type. Returning the existant one. NOT CREATING ANOTHER.\n";
				std::cout << "Component Type: " << GetComponentTypeName<T>() << "\n";
				std::cout << "Entity ID: " << entity << "\n";
			}
			return static_cast<T*>(it->second);
		}

		// Get the component's instantiated objects vector and create a new object.
		std::vector<T>& componentVector = GetComponents<T>();
		componentVector.emplace_back();

		// Get the newly created component object and assign it into our entity's component map.
		T* component = static_cast<T*>(&(componentVector.back()));
		component->owner = entity;
		size_t componentTypeID = GetComponentTypeID<T>();
		(*componentMap)[componentTypeID] = component;

		// Return the newly created component object.
		return component;
	}

	// Gets a given Component Type's instantiated object from the target Entity.
	template <typename T>
	T* GetComponent(Entity entity) {
		// Get the current world. This could be a parameter in the future but I didnt see any reason for now.
		World* world = GetWorld();
		if (!world) {
			if (LOGS_ARE_ENABLED)
				std::cout << "[Error] World does not exist.\n";
			return nullptr;
		}

		// Get the Component Map of the Entity.
		auto* componentMap = world->GetComponentMap(entity);

		// If the entity's component map was not found, return. It likely means that the entity doesn't exist either.
		if (!componentMap)
			return nullptr;

		// From the entity's component map, we now find the given component type's instantiated object.
		size_t componentTypeID = GetComponentTypeID<T>();
		auto it = componentMap->find(componentTypeID);

		// If the given component type is not added to the Entity, give an error.
		if (it == componentMap->end()) {
			if (LOGS_ARE_ENABLED) {
				std::cout << "[Error] This component type was not found in the entity.\n";
				std::cout << "Component Type: " << GetComponentTypeName<T>() << "\n";
				std::cout << "Entity ID: " << entity << "\n";
			}

			return nullptr;
		}

		// Return the found component instance.
		return static_cast<T*>(it->second);
	}

	// Removes a given Component Type's instantiated object from the target Entity.
	template <typename T>
	bool RemoveComponent(T& component) {
		// Get the current world. This could be a parameter in the future but I didnt see any reason for now.
		World* world = GetWorld();
		if (!world) {
			if (LOGS_ARE_ENABLED)
				std::cout << "[Error] World does not exist.\n\n";

			return false;
		}

		// Local variable for ease-of-use.
		Entity entity = component.owner;

		// Get the Component Map of the Entity.
		auto* componentMap = world->GetComponentMap(entity);

		// If the entity's component map was not found, return. It likely means that the entity doesn't exist either.
		if (!componentMap)
			return false;

		// Find the component from the entity's ComponentMap.
		size_t componentTypeID = GetComponentTypeID<T>();
		auto it = componentMap->find(componentTypeID);

		// If the component was not found in the entity's componentMap, we should return.
		if (it == componentMap->end()) {
			if (LOGS_ARE_ENABLED) {
				std::cout << "[Error] This component type was not found in the entity.\n";
				std::cout << "Component Type: " << GetComponentTypeName<T>() << "\n";
				std::cout << "Entity ID: " << entity << "\n\n";
			}

			return false;
		}

		// Find the component within the all instantiated component objects vector and delete it.
		{
			bool found = false;
			std::vector<T>& componentVector = GetComponents<T>();

			// Bruteforce search through the vector.
			for (size_t i = componentVector.size() - 1; i >= 0; --i) {
				if (componentVector[i].owner == entity) {
					found = true;
					componentVector.erase(componentVector.begin() + i);
					break;
				}
			}

			// If the component was somehow not found in the actual instantiation vector, something went really wrong.
			if (!found) {
				if (LOGS_ARE_ENABLED) {
					std::cout << "[Error] Requested deletion of the component type has corrupt pointer. This should NEVER happen.\n";
					std::cout << "Component Type: " << GetComponentTypeName<T>() << "\n";
					std::cout << "Entity ID: " << entity << "\n\n";
				}

				return false;
			}
		}

		// Remove the component from the entity's ComponentMap.
		componentMap->erase(it);

		return true;
	}
	
	// Gets a given ComponentType's type name.
	template <typename T>
	const char* GetComponentTypeName() { return T::typeName; }

	// Gets a given ComponentType's unique ID.
	template <typename T>
	size_t GetComponentTypeID() { return T::typeID; }

	// Gets a given ComponentType's all instances vector.
	template <typename T>
	std::vector<T>& GetComponents() { return T::allInstances; }
	#pragma endregion

	#pragma region System
	// Gets the given System's Singleton Object. This object never gets removed from the memory and is always existant.
	template <typename T>
	T* GetSystem() { return T::GetInstance(); }

	// Adds a given system type to the Update-Vector and the system gets updated on each tick.
	template <typename T>
	void AddSystem() {
		// If the system is already added, do not add it once again.
		if (T::GetInstance()->isSystemAlreadyAdded)
			return;

		// Set this variable in the system singleton object so it can not be added multiple times at the same time.
		T::GetInstance()->isSystemAlreadyAdded = true;

		// Push the system singleton object pointer into the update-vector.
		systems.push_back(T::GetInstance());
	}

	// Tries to remove a system from the update-vector. Update-vector, updates all systems inside it each tick.
	template <typename T>
	bool RemoveSystem() {
		auto* it = std::find(systems.begin(), systems.end(), T::GetInstance());

		// We couldn't find the specified system in our vector, return false.
		if (it == systems.end())
			return false;

		// Reset the variable so the system can be added back into the ECS engine.
		T::GetInstance()->isSystemAlreadyAdded = false;

		// Erase the pointer to the system from our update-vector.
		systems.erase(it);
	}

	#pragma endregion

// Private Member Variables && Hiding Constructor.
private:
	std::vector<System*> systems {};
	World currentWorld {};
	ECSEngine() = default;
};

#pragma region Essential Macros
	// Define 'ECS' as a shortcut for the Engine Singleton.
#define ECS ECSEngine::GetInstance()

// This macro defines the component as an ECS component.
// The purpose of this is to generate static member variables
// that provides huge performance boost and really easy management overall.
#define ECS_COMPONENT(ComponentName)																\
	friend class ECSEngine;																			\
	protected:																						\
		static inline std::vector<ComponentName> allInstances = std::vector<ComponentName>(10000);	\
		static inline const char* typeName = #ComponentName;										\
		static inline size_t typeID = ++ECS.uniqueComponentTypeCount;								\
																									\
	public:																							\
		Entity owner = -1;

	// This macro defines the class as an ECS system.
	// The purpose of this is to prohibit the developers to create
	// system instances manually instead of using the ECSEngine factory.
#define ECS_SYSTEM(SystemName)								\
	friend class ECSEngine;									\
	protected:												\
		bool isSystemAlreadyAdded = false;					\
															\
		SystemName() = default;								\
		static SystemName* GetInstance() {					\
			static SystemName* Instance = new SystemName(); \
			return Instance;								\
		}
#pragma endregion