#pragma once
#include <glad\glad.h>
#include <SFML\OpenGL.hpp>
#include "Shader.h"
#include <SFML\Graphics.hpp>
class Framebuffer
{
public:
	Framebuffer(int renderWidth = 1280, int renderHeight = 960,int screenWidth = 1280, int screenHeight = 960);
	~Framebuffer();

	GLuint id(){ return mFbo; };
	GLuint texture(){ return mTexture; };

	int getWidth(){ return mWidth; }
	int getHeight(){ return mHeight; }

	void bind();

	void display(Shader* shader);

private:

	GLuint			mFbo;
	GLuint			mRbo;
	GLuint			mTexture;
	GLuint			mVao, mVbo; //for drawing the framebuffer to screen

	int				mWidth, mHeight;
	int				mScreenWidth, mScreenHeight;
};


