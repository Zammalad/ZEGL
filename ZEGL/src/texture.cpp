/**
* Copyright(c) 2014, Phil Sampson (http://www.zamma.co.uk)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "texture.h"
#include "logger.h"
#include "stb_image.h"
#include "util.h"
#include <iostream>
#include <cassert>

using namespace ZEGL;

std::unordered_map<std::string, Texture::TextureData*> Texture::s_resourceMap;

Texture::TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data,
	GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp, GLenum* attachments) :
	m_textureID(new GLuint[numTextures]),
	m_textureTarget(textureTarget),
	m_numTextures(numTextures),
	m_frameBuffer(0),
	m_renderBuffer(0)
{
	m_width = width;
	m_height = height;

	InitTextures(data, filters, internalFormat, format, clamp);
	InitRenderTargets(attachments);
}

Texture::TextureData::~TextureData()
{
	if (*m_textureID)
	{
		glDeleteTextures(m_numTextures, m_textureID);
	}
	if (m_frameBuffer)
	{
		glDeleteFramebuffers(1, &m_frameBuffer);
	}
	if (m_renderBuffer)
	{
		glDeleteRenderbuffers(1, &m_renderBuffer);
	}
	if (m_textureID)
	{
		delete[] m_textureID;
	}
}

void Texture::TextureData::InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp)
{
	glGenTextures(m_numTextures, m_textureID);
	for (int i = 0; i < m_numTextures; i++)
	{
		glBindTexture(m_textureTarget, m_textureID[i]);

		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, filters[i]);

		if (clamp)
		{
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glTexImage2D(m_textureTarget, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);

		if (filters[i] == GL_NEAREST_MIPMAP_NEAREST ||
			filters[i] == GL_NEAREST_MIPMAP_LINEAR ||
			filters[i] == GL_LINEAR_MIPMAP_NEAREST ||
			filters[i] == GL_LINEAR_MIPMAP_LINEAR)
		{
			glGenerateMipmap(m_textureTarget);
			GLfloat maxAnisotropy;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			glTexParameterf(m_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, Util::Clamp(0.0f, 8.0f, maxAnisotropy));
		}
		else
		{
			glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		}
	}
}

void Texture::TextureData::InitRenderTargets(GLenum* attachments)
{
	if (attachments == 0)
		return;

	GLenum drawBuffers[32];      //32 is the max number of bound textures in OpenGL

	if (m_numTextures > 32)
	{
		LOG_ERROR("Too many textures (Max is 32) - " << m_numTextures);
		LOG_CLOSE();
		ASSERT(m_numTextures < 33, "Too many textures (Max is 32)!");
	}

	bool hasDepth = false;
	for (int i = 0; i < m_numTextures; i++)
	{
		if (attachments[i] == GL_DEPTH_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
		}
		else
		{
			drawBuffers[i] = attachments[i];
		}

		if (attachments[i] == GL_NONE)
		{
			continue;
		}

		if (m_frameBuffer == 0)
		{
			glGenFramebuffers(1, &m_frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], m_textureTarget, m_textureID[i], 0);
	}

	if (m_frameBuffer == 0)
	{
		return;
	}

	if (!hasDepth)
	{
		glGenRenderbuffers(1, &m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
	}

	glDrawBuffers(m_numTextures, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("Framebuffer creation failed!");
		ASSERT(0 != 0, "Framebuffer creation failed!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Texture::TextureData::Bind(int textureNum) const
{
	glBindTexture(m_textureTarget, m_textureID[textureNum]);
}

void Texture::TextureData::BindAsRenderTarget() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glViewport(0, 0, m_width, m_height);
}

Texture::Texture(const std::string& fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attachment)
{
	m_fileName = fileName;

	std::unordered_map<std::string, TextureData*>::const_iterator it = s_resourceMap.find(fileName);
	if (it != s_resourceMap.end())
	{
		m_textureData = it->second;
		m_textureData->AddReference();
	}
	else
	{
		int x, y, bytesPerPixel;
		unsigned char* data = stbi_load((fileName).c_str(), &x, &y, &bytesPerPixel, 4);

		if (data == nullptr)
		{
			LOG_ERROR("Unable to load texture - " << fileName)
			ASSERT(0 != 0, "Unable to load texture");
		}

		m_textureData = new TextureData(textureTarget, x, y, 1, &data, &filter, &internalFormat, &format, clamp, &attachment);
		stbi_image_free(data);

		s_resourceMap.insert(std::pair<std::string, TextureData*>(fileName, m_textureData));
	}
}

Texture::Texture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attachment)
{
	m_fileName = "";
	m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, clamp, &attachment);
}

Texture::Texture(const Texture& texture) :
	m_textureData(texture.m_textureData),
	m_fileName(texture.m_fileName)
{
	m_textureData->AddReference();
}

void Texture::operator=(Texture texture)
{
	char* temp[sizeof(Texture) / sizeof(char)];
	memcpy(temp, (void*)this, sizeof(Texture));
	memcpy((void*)this, (void*)&texture, sizeof(Texture));
	memcpy((void*)&texture, temp, sizeof(Texture));
}

Texture::~Texture()
{
	if (m_textureData && m_textureData->RemoveReference())
	{
		if (m_fileName.length() > 0)
		{
			s_resourceMap.erase(m_fileName);
		}

		Util::SafeDelete(m_textureData);
	}
}

void Texture::Bind(unsigned int unit) const
{
	if (unit > 31)
	{
		LOG_ERROR("Attempting to bind to invalid texture index (Should be 0 - 32) - " << unit);
		ASSERT(0 != 0, "Attempting to bind to invalid texture index (Should be 0 - 32)");
	}
	glActiveTexture(GL_TEXTURE0 + unit);
	m_textureData->Bind(0);
}

void Texture::BindAsRenderTarget() const
{
	m_textureData->BindAsRenderTarget();
}

