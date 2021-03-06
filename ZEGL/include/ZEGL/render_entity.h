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

#pragma once;

#include "entity.h"

namespace ZEGL
{
	/**
	* RenderEntity is an extension of the Entity class and is used for
	* entities which are to be displayed on screen.
	*
	* \see [Entity]
	*/
	class RenderEntity : public Entity
	{
	public:
		/**
		* Constructor using a TextureAtlas.
		*
		* Used for an entity which has a texture containing multiple areas
		* that can be used for rendering different things.
		*
		* \param[in] texture Texture to use for the entity
		* \param[in] normalMap Corresponding normal map for the texture
		* \param[in] textureAtlas A texture atlas which defines the texture
		* \param[in] pos Initial position of the entity
		* \param[in] rot Initial rotation angle (in radians) of the entity
		* \param[in] xScale Scale factor of the entity
		* \param[in] yScale Scale factor of the entity
		*
		* \see [Texture][TextureAtlas]
		*/
		RenderEntity(const Texture& texture,
			const Texture& normalMap,
			const TextureAtlas& textureAtlas,
			const glm::vec3& pos = glm::vec3(0.0f),
			float rot = 0.0f,
			float xScale = -1.0f,
			float yScale = -1.0f);

		/**
		* Constructor using a TextureAtlas but default normal map.
		*
		* Used for an entity which has a texture containing multiple areas
		* that can be used for rendering different things. This uses a default
		* normal map texture.
		*
		* \param[in] texture Texture to use for the entity
		* \param[in] textureAtlas A texture atlas which defines the texture
		* \param[in] pos Initial position of the entity
		* \param[in] rot Initial rotation angle (in radians) of the entity
		* \param[in] xScale Scale factor of the entity
		* \param[in] yScale Scale factor of the entity
		*
		* \see [Texture][TextureAtlas]
		*/
		RenderEntity(const Texture& texture,
			const TextureAtlas& textureAtlas,
			const glm::vec3& pos = glm::vec3(0.0f),
			float rot = 0.0f,
			float xScale = -1.0f,
			float yScale = -1.0f);

		RenderEntity(const RenderEntity& renderEntity);
		void operator=(RenderEntity renderEntity) = delete;
		virtual ~RenderEntity() {}

		/**
		* Virtual update function to be implemented by any inherited classes
		*
		* Description in full
		*
		* \param[in] delta Delta frame time in milliseconds
		*/
		virtual void Update(float delta) {}

		/**
		* Calculate the texture coordinates based on a region.
		*
		* If the entity has a texture atlas then by passing in a region name this
		* will calculate the appropriate texture coordinates for the texture.
		*
		* \param[in] regionName Name of region for this entity
		*
		* \see [TextureAtlas]
		*/
		virtual void CalcTextureCoords(const std::string regionName);

		/**
		* Get the texture used by this entity.
		*
		* \return The texture used for this entity
		*
		* \see [Texture]
		*/
		inline Texture* GetTexture() { return &m_texture; }

		/**
		* Get the texture used by this entity for normal mapping.
		*
		* \return The texture used for this entity for normal mapping
		*
		* \see [Texture]
		*/
		inline Texture* GetNormalMap() { return &m_normalMap; }

	protected:
		TextureAtlas	m_textureAtlas;		/*!< The TextureAtlas for the entity */
		Texture			m_texture;			/*!< The render Texture for the entity */
		Texture			m_normalMap;		/*!< The normal map Texture for the entity */
	};
}