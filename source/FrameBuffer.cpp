#include "..\include\FrameBuffer.h"
#include <iostream>
#include <vector>
#include <glm\mat4x4.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>



Framebuffer::Framebuffer(int renderWidth /*= 1280*/, int renderHeight/*= 960*/,int screenWidth,int screenHeight)
	:
	mWidth(renderWidth),
	mHeight(renderHeight),
	mScreenWidth(screenWidth),
	mScreenHeight(screenHeight)
{
	using namespace std;

	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

	glGenRenderbuffers(1, &mRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, mRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Create quad that will display the framebuffer------------------------------------------------------

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	std::vector<GLfloat> quad = {
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};



	GLuint positionAttribLocation = 0;
	GLuint texCoordsAttribLocation = 2;

	GLuint numPosElements = 2;
	GLuint numTexElements = 2;

	GLuint stride = (numPosElements + numTexElements) * sizeof(GLfloat);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* quad.size(), quad.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(positionAttribLocation);
	glVertexAttribPointer(positionAttribLocation, numPosElements , GL_FLOAT, GL_FALSE,stride, (GLvoid*)0);

	glEnableVertexAttribArray(texCoordsAttribLocation);
	glVertexAttribPointer(texCoordsAttribLocation, numTexElements , GL_FLOAT, GL_FALSE, stride, (GLvoid*)(numTexElements * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	
}


Framebuffer::~Framebuffer()
{
	glDeleteTextures(1, &mTexture);
	glDeleteRenderbuffers(1, &mRbo);
	glDeleteFramebuffers(1, &mFbo);
}

void Framebuffer::bind()
{
	glViewport(0, 0, mWidth, mHeight); //if the screen or framebuffer is changed the viewport must also be changed
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

void Framebuffer::display(Shader* shader)
{
	glBindFramebuffer(GL_FRAMEBUFFER,0); //unbind the drawn to framebuffer to display on-screen
		glClearColor(0.5f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		glViewport(0, 0, mScreenWidth, mScreenHeight); //if the screen or framebuffer is changed the viewport must also be changed
		shader->use();

		glBindVertexArray(mVao);	
		GLuint numVertices = 6;
		glBindTexture(GL_TEXTURE_2D, mTexture);				
		glDrawArrays(GL_TRIANGLES, 0, numVertices);

	glBindTexture(GL_TEXTURE_2D, 0);
}