#pragma once
#include "Common.h"
#include "DrawableGL.h"
#include "Animation.h"



class AnimationGL : public DrawableGL
{
public:
	AnimationGL(TextureArray ta = TextureArray());
	AnimationGL(TextureArray ta,Dir facingDir);
	AnimationGL(const AnimationGL & a);
	AnimationGL(const AnimationGL & a, Dir facingDir);
	AnimationGL(TextureArray ta, const Animation & animation,int animationStartX,int animationStartY, Dir facingDir = Dir::FRONT);
	~AnimationGL();


	bool operator==(AnimationGL & a)
	{
		return a.m_name == m_name;
	}


	void f_draw(const ptr<Shader> & shader) override;
	void f_update(float dt) override;

	void f_loadFrames(std::vector<GLuint> frames);
	void f_loadFrames(int x, int y, int numFrames);
	void f_addFrame(GLuint frameRef);
	

	void f_set_frametime(float f) { m_frametime = f; }
	int f_totalframes()const { return m_totalframes; }
	void f_increment_frameindex();
	void f_reset(bool start = false);
	void f_start_animation(bool loop = false);
	void f_reverse_frames();
	bool f_is_playing();
	void f_set_loop(bool b) { m_loop = b; }

	void f_set_current_frame(int frame_number);
	

	int f_currentFrame()const { return m_frameindex; }

private:


	void assignShape();

	bool						m_start;
	bool						m_finished;;
	bool						m_loop;
	int							m_frameindex;
	int							m_totalframes;
	float						m_frametime;
	float						m_frametimer;
	std::vector<GLuint>			m_frames;


};