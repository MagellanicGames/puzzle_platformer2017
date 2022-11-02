#include "..\include\Animation.h"
#include <iostream>

Animation::Animation()
	:
	m_frameindex(0),
	m_frametime(0.4f),
	m_frametimer(0.4f),
	m_animation_identifier(AnimationName::NOTSET),
	m_loop(false),
	m_start(false),
	m_finished(false)
{}
Animation::Animation(AnimationName identifier, float frametime)
	:
	m_frameindex(0),
	m_frametime(frametime),
	m_frametimer(frametime),
	m_animation_identifier(identifier),
	m_loop(false),
	m_start(false),
	m_finished(false)
{}

Animation::Animation(const Animation & a)
	:
	m_frameindex(a.m_frameindex),
	m_totalframes(a.m_totalframes),
	m_frametime(a.m_frametime),
	m_frametimer(a.m_frametimer),
	m_frames(a.m_frames),
	m_animation_identifier(a.m_animation_identifier),
	m_loop(a.m_loop),
	m_start(a.m_start),
	m_finished(a.m_finished)
{
}


Animation::~Animation()
{
}

void Animation::f_draw(sf::RenderWindow & window)
{
	if (m_frames.size() < 1)
		return;
	window.draw(m_frames[m_frameindex]);
	//window.draw(*m_frames[0]);
}

void Animation::f_update(const float dt)
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

void Animation::f_loadframes(Texture* sheet, int num_frames, int sprite_width, int sprite_height,int xpos, int ypos,bool set_origin_center/* = false*/)
{
	for (int i = 0; i < num_frames; ++i)
	{
		m_frames.push_back(Sprite(*sheet, sf::IntRect(sprite_width * (i + xpos), sprite_height * ypos, sprite_width, sprite_height)));	
		if(set_origin_center)
			m_frames[i].setOrigin(vecf((float)sprite_width * 0.5f,(float) sprite_height * 0.5f));
	}



	m_totalframes = m_frames.size();
}

void Animation::f_addFrame(Texture* sheet, int width, int height, int xpos, int ypos, bool setOriginCenter)
{
	Sprite frame(Sprite(*sheet, sf::IntRect(width * xpos, height * ypos, width, height)));
	if(setOriginCenter)
		frame.setOrigin(vecf((float)width * 0.5f, (float)height * 0.5f));
	m_frames.push_back(frame);
	m_totalframes = m_frames.size();
}
void Animation::f_addFrame(Sprite & sprite)
{
	m_frames.push_back(sprite);
	m_totalframes = m_frames.size();
}

void Animation::f_set_position(const vecf v)
{
	for (auto & s : m_frames)
		s.setPosition(vecf(v.x,v.y));
}

void Animation::f_set_scale(vecf scale)
{
	for (auto & s : m_frames)
		s.setScale(vecf(scale.x,scale.y));
}

void Animation::f_set_origin(vecf origin)
{
	for (auto & s : m_frames)
		s.setOrigin(vecf(origin.x,origin.y));
}

void Animation::f_increment_frameindex()
{
	if (m_frameindex < m_totalframes - 1)
		m_frameindex += 1;
}

void Animation::f_reset(bool start)
{
	m_frameindex = 0;
	m_frametimer = m_frametime;
	m_start = start;
	m_finished = false;
}

void Animation::f_start_animation(bool loop /*=false*/)
{
	m_frametimer = m_frametime;
	m_start = true;
	m_loop = loop;
}

void Animation::f_reverse_frames()
{
	
	std::vector<Sprite> tmp;

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

bool Animation::f_is_playing()
{
	if (!m_start)
		return false;
	else if (m_finished)
		return false;
	else
		return true;
}

Sprite Animation::f_get_frame(unsigned int n)const
{
	if (n > m_frames.size())
	{
		std::cout << "Index greater than num of frames, returning first frame\n";
		return m_frames[0];
	}
	else
	{
		return m_frames[n];
	}	
}

vecf Animation::f_size() const
{
	if (m_totalframes < 1)
		return vecf();
	else
		return vecf(m_frames[0].getGlobalBounds().width, m_frames[0].getGlobalBounds().height);
}

void Animation::f_set_current_frame(int frame_number)
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

void Animation::f_flipFrames()
{
	for (auto &f : m_frames)
	{
		f.setScale(vecf(f.getScale().x * -1.0f,f.getScale().y));
	}
}

vecf Animation::f_pos() 
{ 
	return vecf(m_frames[0].getPosition().x, m_frames[0].getPosition().y);
}