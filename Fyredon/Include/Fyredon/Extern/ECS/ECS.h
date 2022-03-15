#pragma once
#include <vector>
#include <memory> // shared_ptr
#include <mutex>  // mutex

// serialization
#include <iostream>
#include <string>
#include <sstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#ifndef DEBUG_MSG
#include <stdio.h>
#define DEBUG_MSG(...) printf(__VA_ARGS__); printf("\n");
#else
#define DEBUG_MSG(...) ;
#endif

#define ASSERT(pass, msg, ...) {if(!pass) throw msg;}

#define SDATA_INIT(comp, data) comp.serialized = true; ECS::ECSMgr::ImportCompData(comp, data);
#define SDATA_BEGIN(comp, data) ECS::ECSMgr::ExportCompData(comp, data);
#define SDATA_END(comp, data) ECS::ECSMgr::ImportCompData(comp, data);


typedef unsigned UInt;
namespace ECS
{
	// Component Structure
	struct Component
	{
		virtual ~Component();
		virtual bool Empty() = 0;
		virtual bool Full();
		std::stringstream binaryStream;
		bool serialized = false;
		static UInt numCids;
	};

	// System Class
	class System
	{
	public:
		struct Ent;
		static void Update(double fixedDelta);
	};

	// Singleton ECSMgr
	class ECSMgr {
	public:
		typedef std::shared_ptr<ECSMgr> Ptr;
		~ECSMgr() {
			DEBUG_MSG("ECSMgr destructor called!");
		}
		ECSMgr(ECSMgr&) = delete;
		ECSMgr& operator=(const ECSMgr&) = delete;
		static Ptr Instance() {
			// thread safe, double checked lock
			if (_ptr == nullptr) {
				std::lock_guard<std::mutex> lk(_mutex);
				if (_ptr == nullptr) {
					_ptr = std::shared_ptr<ECSMgr>(new ECSMgr);
				}
			}
			return _ptr;
		}

	private:
		ECSMgr() {
			DEBUG_MSG("ECSMgr constructor called!");
		}
		static Ptr _ptr;
		static std::mutex _mutex;

	public:
		// Max entity number.
		static const unsigned kMaxEntities = 1024;

		// Max component number
		static const unsigned kMaxComponents = 8192;

		// Allocate the memory for entities and components.
		void Alloc();

		// Free the memory for entities and components.
		void Free();

		// Create an entity.
		UInt CreateEntity();

		// Return the count of all entities.
		unsigned GetEntityCount();

		// Return true if the entity has been created.
		bool Exists(UInt eid);

		// Destroy an entity and all its components.
		void Destroy(UInt eid);

		// Destroy all entities and components.
		void DestroyAll();


		// 
		// Component-related functions
		//

		// Add a component to the entity.
		static void AddComponent(UInt cid, UInt eid, Component* c);
		template<class T> inline static void AddComponent(UInt eid, T* c)
		{
			return AddComponent(T::cid, eid, c);
		}

		// Remove a component from the entity.
		static void RemoveComponent(UInt cid, UInt eid);
		template<class T> inline static void RemoveComponent(UInt eid)
		{
			return RemoveComponent(T::cid, eid);
		}

		// Get pointer to a component.
		static Component* GetComponent(UInt cid, UInt eid);
		template<class T> inline static T* GetComponent(UInt eid)
		{
			return static_cast<T*>(GetComponent(T::cid, eid));
		}

		// Get a vector of all Eids for the component.
		static const std::vector<UInt>& GetAllRelatedEntities(UInt cid);
		template<class T> inline static const std::vector<UInt>& GetAll()
		{
			return GetAllRelatedEntities(T::cid);
		}

		// Return the count of component related entities.
		unsigned GetEntityCount(UInt cid);
		template<class T> inline unsigned GetEntityCount()
		{
			return GetEntityCount(T::cid);
		}

		// Get reference of a component
		template<class T> inline static T& Get(UInt eid)
		{
			auto p = static_cast<T*>(GetComponent(T::cid, eid));
			if (p == nullptr) {
				ASSERT(false, "Entity ID does not exist");
			}
			else {
				return *p;
			}
		}

		// Load Component data
		template<class T, class U> inline static bool ExportCompData(T& component, U& rawData)
		{
			if (!component.serialized) {
				return false;
			}
			boost::archive::binary_iarchive binary_ia(component.binaryStream);
			binary_ia >> rawData;
			return true;
		}

		// Save Component data
		template<class T, class U> inline static bool ImportCompData(T& component, U& rawData)
		{
			if (!component.serialized) {
				return false;
			}
			boost::archive::binary_oarchive binary_oa(component.binaryStream);
			binary_oa << rawData;
			return true;
		}



		//
		// Interface of CraeteEntity
		//

		// Create an entity with some components.
		template <typename ...Args> UInt CreateEntity(Args... args)
		{
			UInt eid = CreateEntity();
			AddComponents(eid, args...);
			return eid;
		}
		template <class T, typename ...Args> static void AddComponents(UInt eid, T* c, Args... args)
		{
			AddComponent(T::cid, eid, c);
			AddComponents(eid, args...);
		}
		template <class T> static void AddComponents(UInt eid, T* c)
		{
			AddComponent(T::cid, eid, c);
		}

	private:
		/// Static pointers to the ECS data.
		static bool* _entities;
		static ECS::Component*** _components;
		static std::vector<UInt>* _componentEids;
	};

};