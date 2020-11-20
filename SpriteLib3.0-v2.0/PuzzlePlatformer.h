#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PuzzlePlatformer : public Scene
{
public:
	PuzzlePlatformer(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;


protected:
	PhysicsPlaygroundListener listener;

	int ramp;

	int elevator1;
	int elevator2;
	int elevator3;

	int wall1;
	int wall2;

	int triangleWall1;
	int triangleWall2;
	int triangleWall3;

};

