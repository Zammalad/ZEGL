/*
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

#pragma once

#include "render_entity.h"
#include "shader.h"

namespace ZEGL
{
	class SpriteBatch
	{
	public:
		SpriteBatch();
		SpriteBatch(SpriteBatch const&) = delete;
		SpriteBatch& operator=(SpriteBatch const&) = delete;
		~SpriteBatch();

		void Init();

		void Begin(Game* game);
		void End();
	
		void Draw(RenderEntity& renderEntity);
		void Flush();

		void SetShader(Shader* shader);

	protected:
	private:
		void SetTexture(Texture* texture, Texture* normalMap = nullptr);

		std::vector<EntityData>	m_spriteData;

		Texture*				m_lastTexture;
		Texture*				m_lastNormalMap;

		Shader					m_defaultShader;
		Shader*					m_customShader;

		GLuint					m_VAO;
		GLuint					m_VAB;
		size_t					m_bytesAllocated;

		bool					m_isDrawing;
	};
}