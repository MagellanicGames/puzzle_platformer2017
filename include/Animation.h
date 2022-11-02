#pragma once
#include "Common.h"

enum AnimationName{GRNBLOCK1,GRNBLOCK2,FLIPBLOCK,NOTSET};


class Animation
{
public:

	friend class AnimationGL;

	Animation();
	Animation(AnimationName identifier,float frametime = 0.2f);
	Animation(const Animation & a);
	~Animation();

	
	void f_draw(sf::RenderWindow & window);
	void f_update(const float dt);
	void f_loadframes(Texture* sheet,int num_frames,int sprite_width, int sprite_height,int xpos = 0,int ypos = 0, bool set_origin_center = false);
	void f_addFrame(Texture* sheet, int width, int height, int xpos, int ypos, bool setOriginCenter = false);
	void f_addFrame(Sprite & sprite);
	void f_set_origin(vecf o);
	void f_set_position(const vecf v);
	void f_set_scale(vecf scale);
	void f_set_frametime(float f) { m_frametime = f; }
	int f_totalframes()const { return m_totalframes; }
	void f_increment_frameindex();
	void f_reset(bool start = false);
	void f_start_animation(bool loop = false);
	void f_reverse_frames();
	bool f_is_playing();
	void f_set_loop(bool b) { m_loop = b; }
	void f_set_name(std::string s) { m_name = s; }
	void f_set_current_frame(int frame_number);
	void f_flipFrames();
	std::string& f_name() { return m_name; }
	Sprite f_get_frame(unsigned int n) const;
	AnimationName f_identifier() { return m_animation_identifier; }
	vecf f_pos();

	vecf f_size() const;

private:

	bool						m_start;
	bool						m_finished;;
	bool						m_loop;
	int							m_frameindex;
	int							m_totalframes;
	float						m_frametime;
	float						m_frametimer;
	std::vector<Sprite>			m_frames;
	std::string					m_name;
	AnimationName				m_animation_identifier;
};

