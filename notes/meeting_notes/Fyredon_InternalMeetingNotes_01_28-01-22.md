#### COMP5330M Group Project - Team Fyredon - Internal Meeting 01 Notes

##### 28-01-2022, 11:00AM

##### Participants : 

Niall Horn [sc20ngh], Sharjeel Qaiser [sc21srq], Isaiah Fergile-Leybourne [sc18ijf], Xiaoyuan Yang [sc21xy], Xiaoyuan Yang [sc21xy], Zechen Geng [ml20zg2], Tianyi Yan [sc21ty].

____

#### Team Name : Fyredon

**Niall :** Project Leader, VFX / Particle System Framework. Core Engine

**Izzy :** Scrum Master, Repository manager, Code Merger, overseeing, Repo health. Core Engine ?

**Shawn :** Physics ? Logo design. Core Engine

**Sharjeel :** ? Core Engine

**Tianyi :** ? Core Engine

**Zechen :** ? Core Engie ..

**Jingyuan Zhang :** ? 

###### Parmvir Grewal : MIA ? 

#### Target Game / Tech Demo : 

Top Down RTS Game (3D)

Inspired by games like Magicka and StarCraft 

Assets : Do Houdini demo for procedural asset generation (Niall) eg foliage, debris. Also look for CC licensed assets for the character and more artistic stuff Turbosquid is a good site to look at, non of us are very artistic, will be programmer art most likely. Can also use Blender to do some basic things. 

#### Engine Features / Subsystems

Base Language :  C++14 

Graphics API : Modern OpenGL (~4.3) with the possibility of moving to Vulkan when we get some more experience with it. 

2D render path : Top Down Camera, possibly use perspective projection so depth is still perceived. Deferred Shading makes sense with lots of lights, especially if particles and other entities can cast light onto world. May be an issue for the VR Render path, but not a big worry for now. 

VR render path : View from first person or above, with freedom to look / move around the environment), Izzy could look at this. Niall + Izzy have VR hardware at home, others could use Universities VR Hardware. Look at the use of reprojection to optimize frame rate.

VFX / Particle Framework : GPU based particle system for simulating magical like effects, various rendering modes : sprites, volumetric. Possibility of cloth simulation for character cloaks. Node based authoring tool for creating VFX assets.

Physics : Ragdolls for character assets, Collision Detection for all entities. Possibility of rigid bodies. 

Volumetric Fog, Ground Fog, could possibly link this to above framework, and have particles influence fog motion (eg like Returnal game). Rendering via 2.5D or 3D Ray-marching. 

Post Processing : Colour Grading, Bloom, Chromatic Aberration, Blurs. To modify framebuffer based on in-game events. 

Instancing / Optimized Rendering : Want to be able to draw many units efficiently, not just characters but also for environmental rendering sake (ground details, foliage, debris etc). Can do this via instancing or batching draw calls (draw indirect etc.).

Scripting : Possibility of runtime scripting language so game editor and runtime can be modified without re-compiling C++ code, eg using Python, Lua or some C++ subset. Could look at C++ hot reloading but might be out of scope in time frame. For reference see Unity (C#) or Godot's scripting language. 

Core Entity Component System for the creating of engine assets, eg Actor/Unit built up from Mesh Component (for render mesh), Physics Component (for collisions), VFX Component (for emitting particles) etc. 

##### Dependencies : 

Math Library : GLM, makes little sense to write our own given the time limitations, GLM also has vectorisation and we all have experience with it. Is header based and can be precompiled and linked if needed. 

GUI Library : DearImgui (ImGui) or Qt tbd.

Graphics API handling : OpenGL will need to ethier manually load functions or use GLEW. Most likely use GLFW for input handling and OpenGL Context creation and windowing (If Qt is not used). 

#### Development Tracking :

**Internal Weekly Meeting** : ***Fridays 11AM***, will make bi-weekly once project picks up. 

**Weekly Meeting with He** : ***TBD*** (hopefully Tuesday at 15:00PM)

Coding Style : Intelij Style, Beautifier, Style Guide (not a big prio for now). 

Task Tracking : Gitlab boards for feature tracking in Agile style.

Single person merging. Izzy overseeing code merging. Having multiple people approve pull requests will cause delays and is time consuming. Avoid multiple unit test passes required for merging (over the top Continuous integration workflow), focus on just checking project builds before merging. We could use GitLab CI for this. 

Documentation : GitLab via Wiki using Markdown. Can export to PDF for submission.

Each member keeps personal log book that is daily progression. 

Group report will be written in LaTeX. 

Discord for general chat, eventually will have smaller scale meetings for any task multiple people are working on.

Document Links (Gantt Chart, Tree Structure)

#### To-Do for next week: 

* Online / Cloud based Gantt Chart and Engine Subsystem Tree (Gitlab hosted)

* Meeting Notes hosted in GitLab repo (Setup by Izzy)

* Send Izzy Gitlab handles

* Logo Design (Shawn)

* Fill in specialization team roles

* References of other games, engines and papers that implement relveant tech

#### Agenda for Meeting with He :

Status on  Jingyuan Zhang and Parmvir Grewal

Setup weekly time slot with He.

