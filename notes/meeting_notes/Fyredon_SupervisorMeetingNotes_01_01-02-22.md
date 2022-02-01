#### COMP5330M Group Project - Team Fyredon - Supervisor Meeting 01 Notes

##### 01-02-2022, 15:00PM

##### Participants : 

Jingyuan, Izzy, Niall, Sharjeel, Shawn, Tianyi, Zechen

Dr He Wang

____

##### Agenda 

- Initial Ideas / Roles for project
- Possible missing students in our group

Currently we do not have a Gantt chart or an Engine Subsystem hierarchy as we are still in too early development to formulate these, not everyone has picked a focus / specialization etc.

____

Testing Role : Game as oppose to Unit testing engine code, not too concerned about that at the moment to be honest, but we can discuss. Could be a shared role.

Showcasing the tech demo and engine : How will we showcase the engine, presentation, tech demo, video etc. Likewise for the game itself. I think it will be a bit of both, we can discuss this. Again not a big prio as this is only week 2 !

Dissect gameplay for techniques needed : Breakdown what the ideal gameplay would involve, what subsystems would be needed to make this possible, will it be custom or third party libraries etc. Eg Ragdoll Physics Engine, Path planning.

Third Party libs : We could build a small api wrapper around parts we want to use to match our own engine API style and make access easier. Need to decide what third party libs eg for Physics we could use. 

Build System : Especially important if we have third party libs API and ABI compat is essential, need to manage tool chain and compilers. (Visual studio versions need to match as VS version defines compiler version). C++ Standard has already been discussed : C++14. 

Break development into multiple iterations : Eg first revision is the core engine building blocks, to get up and running and so forth. From then on keep main branch clean so it can be workable / submit-able. We probs should all develop / push to a dev branch and only merge PR's to main/master. Can discuss with Izzy.

Scoping document initial plan :  This is not graded, but a vital part of the assignment and will need to be updated later. Describes the core features and subsystems, the roles and research on each. Need to decide how we are gonna do this together. 

