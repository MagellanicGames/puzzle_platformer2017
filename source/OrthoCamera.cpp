#include "..\include\OrthoCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

OrthoCamera::OrthoCamera(glm::vec3 position, int width, int height, glm::vec3 center /*= glm::vec3(0.0)*/, glm::vec3 upVector /*= glm::vec3(0.0, 1.0, 0.0)*/)
	:
	mPosition(position),
	mCenter(center),
	mUp(upVector),
	mRotationMatrix(1.0),
	mTranslationMatrix(1.0),
	mIs3d(true),
	mIsFps(false)
{
	m3dPosition_offset = glm ::vec3(35, 35, 100);
	m2dPosition_offset = glm::vec3(0, 0, 100);
	mOffset = m3dPosition_offset;
	float nearClip, farClip;

	nearClip = -1000.0f;
	farClip = 1000.0f;
	float left = (float)-(width / 2.0f);
	float right = (float)(width / 2.0f);
	float bottom = (float)-(height / 2.0f);
	float top = (height / 2.0f);
	mProjection = glm::ortho(left,right , bottom , top, nearClip, farClip);
	
	mView = glm::lookAt(position, center, upVector);	
}


OrthoCamera::~OrthoCamera()
{
}


void OrthoCamera::updatePosition(glm::vec3 pos)
{
	mPosition = pos + glm::vec3(0.0f, 7.5f, 10.0f);
	mCenter = pos;
	mView = glm::lookAt(mPosition, mCenter, mUp);
}

void OrthoCamera::passParameters(const ptr<Shader> & shader)
{
	shader->use();
	glUniformMatrix4fv(shader->uniform("view"), 1, GL_FALSE, glm::value_ptr(glm::lookAt(mPosition, mCenter, mUp)));
	glUniformMatrix4fv(shader->uniform("projection"), 1, GL_FALSE, glm::value_ptr(mProjection));
	glUniform3fv(shader->uniform("camPos"), 1, glm::value_ptr(mPosition));
}


void OrthoCamera::update(float dt)
{

	static float xspeed = 70.0f;
	static float yspeed = 70.0f;

	if (mIsFps)
		return;
	if (mIs3d)
	{
		
			float x = mOffset.x;
			float y = mOffset.y;

			if (x < m3dPosition_offset.x)
				mOffset.x += xspeed * dt;
			else
				mOffset.x = m3dPosition_offset.x;
			if (y < m3dPosition_offset.y)
				mOffset.y += yspeed * dt;
			else
				mOffset.y = m3dPosition_offset.y;
		
	}
	else
	{
		
			float x = mOffset.x;
			float y = mOffset.y;

			if (x > m2dPosition_offset.x)
				mOffset.x -= xspeed * dt;
			else
				mOffset.x = m2dPosition_offset.x;
			if (y > m2dPosition_offset.y)
				mOffset.y -= yspeed * dt;
			else
				mOffset.y = m2dPosition_offset.y;
		
	}
}

void OrthoCamera::focus(glm::vec3 playerPos)
{
	mPosition = playerPos + mOffset;
	mCenter = playerPos;
	
}

void OrthoCamera::set3D()
{
	mIs3d = true;
}

void OrthoCamera::set2D()
{
	mIs3d = false;
}

void OrthoCamera::setOverworld()
{
	setPerspective();
	//setOrthographic();
	mOffset = glm::vec3(0, 256 + 64, 256);
}
void OrthoCamera::setPerspective()
{
	float nearClip = 1.0f;
	float farClip = -1000.0f;
	mProjection = glm::perspective(glm::radians(52.0f), (float)Dim::VIEW_RES_X / (float)Dim::VIEW_RES_Y, nearClip, farClip);
	mView = glm::lookAt(mPosition, mCenter, mUp);
	mOffset = glm::vec3(64, 64, 256);
	mIsFps = true;
}

void OrthoCamera::setOrthographic()
{
	float nearClip = -1000.0f;
	float farClip = 1000.0f;
	float left = (float)-(Dim::RES_X / 2.0f);
	float right = (float)(Dim::RES_X / 2.0f);
	float bottom = (float)-(Dim::RES_Y / 2.0f);
	float top = (Dim::RES_Y / 2.0f);
	mProjection = glm::ortho(left, right, bottom, top, nearClip, farClip);
	mView = glm::lookAt(mPosition, mCenter, mUp);
	mIsFps = false;

	if (mIs3d)
		mOffset = m3dPosition_offset;
	else
		mOffset = m2dPosition_offset;
}

void OrthoCamera::rotate(glm::vec3 angle)
{
	//glm::quat quaternion(angle);
	//mPosition *= glm::rotate()
	//mView = glm::lookAt(mPosition, mCenter, mUp);
}

void OrthoCamera::translate(glm::vec3 trans)
{
	mPosition += trans;	
	mView = glm::lookAt(mPosition, mCenter, mUp);
}