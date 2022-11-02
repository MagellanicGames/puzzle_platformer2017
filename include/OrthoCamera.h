#pragma once
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include "Shader.h"
#include "Common.h"

class OrthoCamera
{
public:
	OrthoCamera(glm::vec3 position, int width, int height, glm::vec3 center = glm::vec3(0.0), glm::vec3 upVector = glm::vec3(0.0, 1.0, 0.0));
	~OrthoCamera();


	void passParameters(const ptr<Shader> & shader);
	void updatePosition(glm::vec3 pos);
	glm::vec3 pos() { return mPosition; }

	void update(float dt);
	void focus(glm::vec3 playerPos);
	void set3D();
	void set2D();
	void setOverworld();
	void rotate(glm::vec3 angle);
	void translate(glm::vec3 trans);

	void setOrthographic();
	void setPerspective();

private:
	bool		mIsFps;
	bool		mIs3d;
	glm::vec3	mOffset;
	glm::vec3	mPosition, mCenter, mUp;
	glm::mat4	mProjection;
	glm::mat4	mView;
	glm::mat4	mRotationMatrix;
	glm::mat4	mTranslationMatrix;
	glm::vec3	m3dPosition_offset;
	glm::vec3	m2dPosition_offset;
};

