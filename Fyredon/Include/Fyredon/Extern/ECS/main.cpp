#include "ECS.h"
#include <iostream>
#include <windows.h>

using namespace std;

auto ecsPtr = ECS::ECSMgr::Instance();
static UInt _id = 0;

// An example serialized data structure for serialized component 
struct HealthData {
	int hp, maxHP;
	HealthData(int _hp, int _maxHP)
	{
		hp = _hp;
		maxHP = _maxHP;
	}
	HealthData() : HealthData(0, 0) {}
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& hp;
		ar& maxHP;
	}
};

/// An example serialized component.
struct HealthComponent : ECS::Component
{
	HealthComponent(int _hp, int _maxHP)
	{
		HealthData rawData(_hp, _maxHP);
		auto& comp = *this;
		SDATA_INIT(comp, rawData);
	}
	HealthComponent() : HealthComponent(0, 0) {}

	virtual bool Empty() {
		HealthData rawData;
		auto& comp = *this;
		SDATA_BEGIN(comp, rawData);
		SDATA_END(comp, rawData);
		return rawData.hp == 0;
	}
	static UInt cid;
};
UInt HealthComponent::cid = _id++;

// An example normal component.
enum Character {
	None = 0,
	Player,
	Enemy
};
struct StatsComponent : ECS::Component
{
	int ATK, DEF;
	string name;
	Character type;
	StatsComponent(string _name, Character _type, int _atk, int _def)
	{
		name = _name;
		type = _type;
		ATK = _atk;
		DEF = _def;
	}

	StatsComponent() : StatsComponent("", None, 0, 0) {}

	virtual bool Empty() {
		return false;
	}

	static UInt cid;
};
UInt StatsComponent::cid = _id++;

// An example Ent.
struct ECS::System::Ent
{
	UInt id;
	HealthComponent& health;
	StatsComponent& stats;
	// add more components here if the system need.
	Ent(UInt _id) :
		health(ecsPtr->Get<HealthComponent>(_id)),
		stats(ecsPtr->Get<StatsComponent>(_id)),
		id(_id)
	{}
};

/// An example system.
struct BattleSystem : ECS::System
{
	static void Update(double fixedDelta)
	{
		auto all = ecsPtr->GetAll<HealthComponent>();

		// check battle result.
		Character winner = None;
		for (auto i : all) {
			if (!ecsPtr->Exists(i))
				continue;
			Ent e(i);
			if (winner == None) {
				winner = e.stats.type;
			}
			else {
				if (winner != e.stats.type) {
					winner = None;
					break;
				}
			}
		}
		if (winner != None) {
			if (winner == Player) {
				cout << "Congratulation! Player win." << endl;
			}
			else {
				cout << "Sorry! Enemy win." << endl;
			}
			ecsPtr->DestroyAll();
			return;
		}

		// if no in the same camp, attack each other
		for (auto i : all) {
			if (!ecsPtr->Exists(i))
				continue;
			Ent eUs(i);
			for (auto j : all) {
				if (i == j)
					continue;
				Ent eOther(j);
				if (eUs.stats.type == eOther.stats.type) {
					continue;
				}
				// attack, generate damage
				int damage = eUs.stats.ATK - eOther.stats.DEF;
				damage = (damage < 0) ? 0 : damage;

				// hit, health decrement
				HealthData rawData;
				auto& comp = eOther.health;
				SDATA_BEGIN(comp, rawData);
				rawData.hp -= damage;
				if (rawData.hp < 0)
					rawData.hp = 0;
				SDATA_END(comp, rawData);

				cout << eOther.stats.name << " has " << rawData.hp << "/" << rawData.maxHP << " hit points." << endl;
				if (rawData.hp <= 0) {
					ecsPtr->Destroy(eOther.id);
				}
			}
		}
	}
};

int main(int argc, const char* argv[])
{
	// create entities
	auto playerEid = ecsPtr->CreateEntity(
		new StatsComponent("Hero", Player, 7, 4),
		new HealthComponent(50, 50)
	);
	auto enemyEid = ecsPtr->CreateEntity(
		new StatsComponent("Monster", Enemy, 6, 6),
		new HealthComponent(20, 20)
	);

	// battle loop
	while (ecsPtr->GetEntityCount())
	{
		BattleSystem::Update(0.1);
		Sleep(100);
	}

	ecsPtr->Free();
	cout << "End Game!" << endl;
	return 0;
}