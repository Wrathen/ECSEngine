#pragma once
#include <vector>

// Base Class Definitions -- E, C, S
struct Entity { size_t ID; };
struct Component { };
class System { friend class ECSEngine; virtual void Update() = 0; };

// The Main Engine Class
// Use this class for all kind of factory/management/utility stuff related to ECS.
class ECSEngine {
public:
	// Identifier for the amount of unique component types.
	size_t uniqueComponentTypeCount = 0;

	// Make the ECSEngine a Singleton.
	static ECSEngine& GetInstance();

	void UpdateSystems();

public:
	#pragma region Component Template Functions
	// Add a Component to a given Entity.
	template <typename ComponentType>
	ComponentType AddComponent(size_t entityID) {
		auto& componentsVector = GetComponents<ComponentType>();
		componentsVector.emplace_back();
		return componentsVector.back();
	}

	// Get a given ComponentType's type name.
	template <typename T>
	const char* GetComponentTypeName() { return T::typeName; }

	// Get a given ComponentType's unique ID.
	template <typename T>
	size_t GetComponentTypeID() { return T::typeID; }

	// Get a given ComponentType's all instances vector.
	template <typename T>
	std::vector<T>& GetComponents() { return T::allInstances; }
	#pragma endregion

	#pragma region System Template Functions
	template <typename T>
	T* AddSystem() {
		T* newSystem = new T();
		systems.push_back(newSystem);
		return newSystem;
	}
	#pragma endregion

// Private Member Variables && Hiding Constructor.
private:
	std::vector<System*> systems;
	ECSEngine() = default;
};

#pragma region Macros
	// Define 'ECS' as a shortcut for the Engine Singleton.
	#define ECS ECSEngine::GetInstance()

	// This macro defines the component as an ECS component.
	// The purpose of this is to generate static member variables
	// that provides huge performance boost and really easy management overall.
	#define ECS_COMPONENT(ComponentName) \
	friend class ECSEngine;	\
	protected: \
		static inline std::vector<ComponentName> allInstances = std::vector<ComponentName>();	\
		static inline const char* typeName = #ComponentName;	\
		static inline size_t typeID = ++ECS.uniqueComponentTypeCount;	\
		\
	public:

	// This macro defines the class as an ECS system.
	// The purpose of this is to prohibit the developers to create
	// system instances manually instead of using the ECSEngine factory.
	#define ECS_SYSTEM(SystemName) \
	friend class ECSEngine; \
	protected: \
		SystemName() = default;
#pragma endregion