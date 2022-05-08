
	// Update Cameras in ECS world using this on next camera_comp tick() oppose to here.
	// ToDo : Below code is deprecated, remove when stable : 

	/*
	// Update Camera State for all camera components
	// Old pre ECS solution : //camera.update_camera(window, 1.f, dt, delta_yaw, delta_pitch);
	World& world = World::get_world();

	// Terrible solution currently (Have to fetch and cast camera enttity and component). Only works on camera 0.
	camera_entity* cam_ent = static_cast<camera_entity*>(world.getCameraByIndex(0));
	static_cast<camera_component*>(cam_ent->getComponentByIndex(0))->camera->update_camera(window, 1.f, dt, delta_yaw, delta_pitch);

	// Update Aspect Ratio if changed
	if (GLFWState.width && GLFWState.height)
	{
		float ar = width / height;
		camera.aspect_ratio = ar;
	} 
	*/