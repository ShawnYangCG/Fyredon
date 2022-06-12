#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/fbx_property_component.h"

fbx_property_component::fbx_property_component()
{
	this->componentName = "Animation";
	this->memmory_size = sizeof(fbx_property_component);
}

fbx_property_component::fbx_property_component(float startTime, float endTime, float speed) {
	this->componentName = "Animation";
	this->startTime = startTime;
	this->endTime = endTime;
	this->speed = speed;
	this->memmory_size = sizeof(fbx_property_component);
}

void fbx_property_component::GUI_struct()
{
	ImGui::InputFloat("speed", &this->speed);
	ImGui::InputFloat("start time", &this->startTime);
	ImGui::InputFloat("end time", &this->endTime);
	if (ImGui::Button("play"))
	{
		if (this->isPlay) {//play -> pause
			this->isPlay = false;
			pause_time_start = clock();

		}
		else {// pause -> play
			this->isPlay = true;
			if (!this->isReset) {
				pause_time_end = clock();
				this->pause_time += this->pause_time_end - this->pause_time_start;
			}
		}
		if (this->isReset) {
			pause_time_start = pause_time_end;
			this->timer_start = clock();
			this->isReset = false;
			this->pause_time = 0;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		this->isPlay = false;
		this->isReset = true;
		this->currentFrameIndex = this->startTime;
	}
	ImGui::InputInt("current frame", &this->currentFrameIndex);
	checkValid();
}

void fbx_property_component::tick()
{
	playAnimation();
}

void fbx_property_component::play()
{
	if(!this->isPlay) {// pause -> play
		this->isPlay = true;
		if (!this->isReset) {
			pause_time_end = clock();
			this->pause_time += this->pause_time_end - this->pause_time_start;
		}
	}
	if (this->isReset) {
		pause_time_start = pause_time_end;
		this->timer_start = clock();
		this->isReset = false;
		this->pause_time = 0;
	}
}

void fbx_property_component::stop()
{
	if (this->isPlay) {//play -> pause
		this->isPlay = false;
		pause_time_start = clock();
	}
}

void fbx_property_component::reset()
{
	this->isPlay = false;
	this->isReset = true;
	this->currentFrameIndex = this->startTime;
}

void fbx_property_component::playAnimation()
{
	fbx_entity* FBXentity = (fbx_entity*)this->parent;
	fbx_mesh* mesh = (fbx_mesh*)((mesh_component*)FBXentity->getComponentByName("Mesh"))->mesh;
	if (this->isPlay) {
		this->timer_current = clock();
		time_t T1 = this->timer_current - this->timer_start;
		this->currentFrameIndex =
			(int)(this->startTime + (T1 - this->pause_time) / this->secondPerFrame * this->speed)
			% (int)(this->endTime - this->startTime);
	}
	mesh->playCurrentFrame(this->currentFrameIndex);
}

void fbx_property_component::checkValid()
{
	if (this->startTime < 0)
		this->startTime = 0;
	if (this->endTime < this->startTime)
		this->startTime = this->endTime;
	if (this->currentFrameIndex < this->startTime)
		this->currentFrameIndex = this->startTime;
	if (this->currentFrameIndex > this->endTime)
		this->currentFrameIndex = this->endTime;
}

void fbx_property_component::setPlay()
{
	this->isPlay = true;
	if (this->isReset) {
		this->timer_start = clock();
		this->isReset = false;
	}
}
