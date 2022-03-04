#include "ECS.h"
#include <algorithm>

// Initialize singleton static variable.
ECS::ECSMgr::Ptr ECS::ECSMgr::_ptr = nullptr;
std::mutex ECS::ECSMgr::_mutex;

// Static pointers to the ECS data.
bool* ECS::ECSMgr::_entities = nullptr;
ECS::Component*** ECS::ECSMgr::_components = nullptr;
std::vector<UInt>* ECS::ECSMgr::_componentEids = nullptr;

//
// Entity::Component
//

ECS::Component::~Component()
{

}

bool ECS::Component::Full() const
{
	return !this->Empty();
}

//
// System
//

void ECS::System::Update(double fixedDelta)
{
}


//
// ECSMgr
//

void ECS::ECSMgr::Alloc() 
{
	if (_components != nullptr)
		return;

	_entities = new bool[kMaxEntities];
	for (UInt eid = 0; eid < kMaxEntities; ++eid)
		_entities[eid] = false;

	auto max = Component::numCids;
	_components = new Component * *[max];
	_componentEids = new std::vector<UInt>[Component::numCids];
	for (UInt cid = 0; cid < max; cid++)
	{
		_components[cid] = new Component * [kMaxEntities];

		// init pointers
		for (UInt eid = 0; eid < kMaxEntities; eid++)
			_components[cid][eid] = nullptr;
	}
}

void ECS::ECSMgr::Free()
{
	if (_components != nullptr)
	{
		DestroyAll();
		for (UInt cid = 0; cid < Component::numCids; cid++)
			if (_components[cid] != nullptr)
				delete[] _components[cid];
		delete[] _components;
	}

	if (_componentEids != nullptr)
		delete[] _componentEids;

	if (_entities != nullptr)
		delete[] _entities;

	_entities = nullptr;
	_components = nullptr;
	_componentEids = nullptr;
}

UInt ECS::ECSMgr::CreateEntity()
{
	Alloc();

	UInt eid = 1;
	
	// enable a new entity
	for (; eid < kMaxEntities && _entities[eid]; ++eid);

	if (eid < 1 || eid >= kMaxEntities)
	{
		Assert(false, "Maximum number of entities reached!");
		eid = 0;
	}
	else
	{
		_entities[eid] = true;
		DebugMsg("Entity %u created", eid);
	}

	return eid;
}

unsigned ECS::ECSMgr::GetEntityCount()
{
	int count = 0;
	if (_entities != nullptr)
	{
		for (UInt eid = 1; eid < kMaxEntities; ++eid)
			if (_entities[eid])
				++count;
	}
	return count;
}

unsigned ECS::ECSMgr::GetEntityCount(UInt cid)
{
	return (unsigned)GetAllRelatedEntities(cid).size();
}

bool ECS::ECSMgr::Exists(UInt eid)
{
	return _entities != nullptr && _entities[eid];
}

void ECS::ECSMgr::Destroy(UInt eid)
{
	if (eid == 0)
		return;
	DebugMsg("Entity %u being destroyed", eid);

	for (UInt cid = 0; cid < Component::numCids; cid++)
		RemoveComponent(cid, eid);
	_entities[eid] = false;
}

void ECS::ECSMgr::DestroyAll()
{
	unsigned count = 0;
	for (UInt eid = 1; eid < kMaxEntities; ++eid)
	{
		if (_entities[eid])
		{
			Destroy(eid);
			count++;
		}
	}
	DebugMsg("%u entities destroyed", count);
}

void ECS::ECSMgr::AddComponent(UInt cid, UInt eid, Component* c)
{
	if (c == nullptr)
		return;
	if (eid >= kMaxEntities || !_entities[eid] || cid >= Component::numCids)
	{
		Assert(false, "Invalid eid %u or cid %u", eid, cid);
		return;
	}

	DebugMsg("    Adding component cid %u eid %u (%x)", cid, eid, (int)(long)c);

	if (_components[cid][eid] != nullptr)
		RemoveComponent(cid, eid);

	// store component pointer
	_components[cid][eid] = c;

	// store component eid
	_componentEids[cid].push_back(eid);
}

void ECS::ECSMgr::RemoveComponent(UInt cid, UInt eid)
{
	if (eid >= kMaxEntities || !_entities[eid] || cid >= Component::numCids)
	{
		Assert(false, "Invalid eid %u or cid %u", eid, cid);
		return;
	}

	auto ptr = _components[cid][eid];
	if (ptr == nullptr)
		return;

	DebugMsg("    Removing component cid %u eid %u (%x)", cid, eid, (int)(long)ptr);

	// delete component pointer
	delete ptr;
	_components[cid][eid] = nullptr;

	// remove component eids
	auto& eids = _componentEids[cid];
	auto it = find(eids.begin(), eids.end(), eid);
	if (it != eids.end())
		it = eids.erase(it);
}

ECS::Component* ECS::ECSMgr::GetComponent(UInt cid, UInt eid)
{
	if (eid < kMaxEntities && cid < Component::numCids)
	{
		return _components[cid][eid];
	}
	return nullptr;
}

const std::vector<UInt>& ECS::ECSMgr::GetAllRelatedEntities(UInt cid)
{
	if (_componentEids != nullptr && cid < Component::numCids)
		return _componentEids[cid];
}


