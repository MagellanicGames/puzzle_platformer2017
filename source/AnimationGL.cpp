#include "..\include\AnimationGL.h"
#include "..\include\Shape.h"
#include "..\include\Drawing.h"
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\mat3x3.hpp>
#include <iostream>
#include "..\include\Animation.h"

AnimationGL::AnimationGL(TextureArray ta /*=TextureArray()*/)
	:
	m_frameindex(0),
	m_frametime(0.4f),
	m_frametimer(0.4f),
	m_loop(false),
	m_start(false),
	m_finished(false),
	DrawableGL(ta)	
{
	DrawableGL::m_facingDirection = Dir::FRONT;
	assignShape();
}

AnimationGL::AnimationGL(TextureArray ta ,Dir facingDir )
	:
	m_frameindex(0),
	m_frametime(0.4f),
	m_frametimer(0.4f),
	m_loop(false),
	m_start(false),
	m_finished(false),	
	DrawableGL(ta)
{	
	DrawableGL::m_facingDirection = facingDir;
	assignShape();
}

AnimationGL::AnimationGL(const AnimationGL & a)
	:
	m_frameindex(a.m_frameindex),
	m_frametime(a.m_frametime),
	m_frametimer(a.m_frametimer),
	m_loop(a.m_loop),
	m_start(a.m_start),
	m_finished(a.m_finished),
	m_frames(a.m_frames),
	m_totalframes(a.m_totalframes)
{
	m_name = a.m_name;
	m_facingDirection = a.m_facingDirection;
	m_textureArray = a.m_textureArray;
	assignShape();
}

AnimationGL::AnimationGL(const AnimationGL & a, Dir facingDir)
	:
	m_frameindex(a.m_frameindex),
	m_frametime(a.m_frametime),
	m_frametimer(a.m_frametimer),
	m_loop(a.m_loop),
	m_start(a.m_start),
	m_finished(a.m_finished),
	m_frames(a.m_frames),
	m_totalframes(a.m_totalframes)
{
	DrawableGL::m_flip = a.m_flip;
	m_name = a.m_name;
	m_textureArray = a.m_textureArray;
	if (facingDir == Dir::SAME)
		m_facingDirection = a.m_facingDirection;
	else
		m_facingDirection = facingDir;

	assignShape();
	
}

AnimationGL::AnimationGL(TextureArray ta, const Animation & animation,int animationStartX, int animationStartY,Dir facingDir /*FRONT*/)
	:
	m_frameindex(animation.m_frameindex),
	m_frametime(animation.m_frametime),
	m_frametimer(animation.m_frametimer),
	m_loop(animation.m_loop),
	m_start(animation.m_start),
	m_finished(animation.m_finished)	
{

	m_textureArray = ta;
	m_facingDirection = facingDir;
	assignShape();

	GLuint startFrame = animationStartX + (animationStartY * ta.m_widthInTiles);

	for (unsigned int i = 0; i < animation.m_totalframes; i++)
	{
		m_frames.push_back(startFrame + i);
	}
	m_totalframes = m_frames.size();
}

void AnimationGL::assignShape()
{
	switch (m_facingDirection)
	{
	case Dir::FRONT:m_shape = Drawing::m_planeShape.get(); break;
	case Dir::RIGHT:m_shape = Drawing::m_planeRightShape.get(); break;
	case Dir::TOP:m_shape = Drawing::m_planeTopShape.get(); break;
	}
	if (m_shape == nullptr)
	{
		m_shape = Drawing::m_planeShape.get();
		std::cout << "Potentially invalid shape assigned to animationgl, using default\n";
	}
}

AnimationGL::~AnimationGL() {}

void AnimationGL::f_draw(const ptr<Shader> & shader)
{
	if (m_frames.size() < 1)
		return;
	
	
	glm::mat4 model(1.0f); //identity
	model = glm::translate(model, m_position);
	model *= glm::mat4_cast(m_rotation);
	model = glm::scale(model, m_scale);

	glUniformMatrix4fv(Drawing::m_shader->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	glm::mat3 normMat = glm::transpose(glm::mat3(glm::inverse(model)));
	glUniformMatrix3fv(Drawing::m_shader->uniform("normMat"), 1, GL_FALSE, glm::value_ptr(normMat));
	glUniform1i(Drawing::m_shader->uniform("texArrayIndex"), m_frames[m_frameindex]);

	DrawableGL::f_draw(shader);
}

void AnimationGL::f_update(const float dt)
{
	if (m_frametimer > m_frametime) //if for some reason dt bugs out timer, set to correct time
		m_frametimer = m_frametime;


	if (m_frametimer > 0.0f) //subtract from timer
	{
		m_frametimer -= dt;
	}
	else  if (m_start)//reset timer and assign next frame
	{
		if (m_frameindex < m_totalframes - 1) //increment frame index
		{
			m_frameindex++;
			m_frametimer = m_frametime; //reset timer
		}
		else if (m_frameindex == m_totalframes - 1) //if at last frame, reset to 0
		{
			if (!m_loop)
			{
				m_start = false;
				m_finished = true;
			}
			else
			{
				m_frameindex = 0;
				m_frametimer = m_frametime; //reset timer
			}

		}
	}
}

void AnimationGL::f_loadFrames(std::vector<GLuint> frames)
{
	m_frames.insert(m_frames.end(), frames.begin(), frames.end());
	m_totalframes = m_frames.size();
}

void AnimationGL::f_loadFrames(int x, int y, int numFrames)
{

	for (int i = 0; i < numFrames; i++)
	{
		GLuint frameIndex = x + (y * (m_textureArray.m_widthInTiles));
		m_frames.push_back(frameIndex);

		if (x < m_textureArray.m_widthInTiles)
		{
			x++;
		}
		else
		{
			x = 0;

			if (y < m_textureArray.m_heightInTiles)
				y++;
			else
				std::cout << "error in loading Animation frame, out of y range\n";
		}
	}
	m_totalframes = m_frames.size();
}

void AnimationGL::f_addFrame(GLuint frameRef)
{
	m_frames.push_back(frameRef);
	m_totalframes = m_frames.size();
}



void AnimationGL::f_increment_frameindex()
{
	if (m_frameindex < m_totalframes - 1)
		m_frameindex += 1;
}

void AnimationGL::f_reset(bool start)
{
	m_frameindex = 0;
	m_frametimer = m_frametime;
	m_start = start;
	m_finished = false;
}

void AnimationGL::f_start_animation(bool loop /*=false*/)
{
	m_frametimer = m_frametime;
	m_start = true;
	m_loop = loop;
}

void AnimationGL::f_reverse_frames()
{

	std::vector<GLuint> tmp;

	for (int i = m_totalframes - 1; i > -1; i--)
	{
		tmp.push_back(m_frames[i]);
	}

	m_frames.clear();

	for (int i = 0; i < m_totalframes; i++)
	{
		m_frames.push_back(tmp[i]);
	}

}

bool AnimationGL::f_is_playing()
{
	if (!m_start)
		return false;
	else if (m_finished)
		return false;
	else
		return true;
}

void AnimationGL::f_set_current_frame(int frame_number)
{
	if (frame_number < m_totalframes)
	{
		m_frameindex = frame_number;
	}
	else
	{
		std::cout << "Frame number exceeds number of frames.  Animation::f_set_current_frame\n";
	}
}