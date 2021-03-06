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

#include "texture.h"
#include "texture_atlas.h"
#include <glm/glm.hpp>

namespace ZEGL
{
	class Camera;
	class Core;
	class Input;
	class Shader;

	/**
	* Holds common data in a structure so that it can easily be passed around
	* for use in other areas e.g. rendering using in an array using a vertex
	* buffer, without having to pass around the full class. Contains position,
	* rotation, scale and texture coordinate data.
	*
	* \see [Entity]
	*/
	struct EntityData
	{
		glm::vec3	m_pos;			/*!< Position - size 12 bytes, offset 0 */
		float		m_rot;			/*!< Rotation (radians) - size 4 bytes, offset 12 */
		float		m_xScale;		/*!< X Scale - size 4 bytes, offset 16 */
		float		m_yScale;		/*!< Y Scale - size 4 bytes, offset 20 */
		glm::vec2	m_texCoords[4];	/*!< Texture Coordinates - size 32 bytes, offset 24|32|40|48 */
	};

	/**
	* Class to be used as a base for developing anything which has the basic core
	* values of a position, rotation and scale. This class provides the core functionality
	* for manipulating these values.
	*/
	class Entity
	{
	public:
		/**
		* Constructor which optionally takes in values for position, rotation and scale
		* of the entity. Defaults are used if none are passed
		*
		* \param[in] pos Position of the entity
		* \param[in] rot Rotation of the entity (in radians)
		* \param[in] xScale Scale factor of the entity
		* \param[in] yScale Scale factor of the entity
		*/
		Entity(const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float xScale = 1.0f, float yScale = 1.0f)
		{
			m_data.m_pos = pos;
			m_data.m_rot = rot;
			m_data.m_xScale = xScale;
			m_data.m_yScale = yScale;
		}

		virtual ~Entity() {}

		virtual void ReadEntity() {}

		/**
		* Pure virtual update function to be implemented by any inherited classes
		*
		* Description in full
		*
		* \param[in] delta Delta frame time in milliseconds
		*/
		virtual void Update(float delta) = 0;

		/**
		* Get the current position of the entity.
		*
		* \return glm::vec3 representing the entity position
		*/
		inline glm::vec3 GetPos() const { return m_data.m_pos; }

		/**
		* Set the position of the entity using individual values.
		*
		* \param[in] x New X position component value
		* \param[in] y New Y position component value
		* \param[in] z New Z position component value
		*/
		inline void	SetPos(float x, float y, float z = 0.0f) { m_data.m_pos.x = x; m_data.m_pos.y = y; m_data.m_pos.z = z; }

		/**
		* Set the position of the entity using an existing glm::vec3.
		*
		* \param[in] pos New position value
		*/
		inline void	SetPos(const glm::vec3& pos) { m_data.m_pos = pos; }

		/**
		* Get the current rotation of the entity.
		*
		* \return entity rotation in radians
		*/
		inline float GetRot() const { return m_data.m_rot; }

		/**
		* Set the entity rotation.
		*
		* \param[in] rot Angle of rotation in radians
		*/
		inline void	SetRot(float rot) { m_data.m_rot = rot; }

		/**
		* Get the current x scale factor of the entity.
		*
		* \return X scale factor for the entity
		*/
		inline float GetXScale() const { return m_data.m_xScale; }

		/**
		* Set the entity x scale factor.
		*
		* \param[in] scale X scaling factor for the entity
		*/
		inline void	SetXScale(float scale) { m_data.m_xScale = scale; }

		/**
		* Get the current y scale factor of the entity.
		*
		* \return Y scale factor for the entity
		*/
		inline float GetYScale() const { return m_data.m_yScale; }

		/**
		* Set the entity y scale factor.
		*
		* \param[in] scale y scaling factor for the entity
		*/
		inline void	SetYScale(float scale) { m_data.m_yScale = scale; }

		/**
		* Scale the entity.
		*
		* \param[in] xScale Amount to scale the current X size by
		* \param[in] yScale Amount to scale the current Y size by
		*/
		inline void	Scale(float xScale, float yScale) { m_data.m_xScale *= xScale;  m_data.m_yScale *= yScale; }

		/**
		* Scale the entity.
		*
		* \param[in] scale Amount to scale the current X & Y size by
		*/
		inline void	Scale(float scale) { m_data.m_xScale *= scale;  m_data.m_yScale *= scale; }

		/**
		* Get the entity data structure
		*
		* \return Structure containing position, rotation, scale and texture coordinates
		*
		* \see [EntityData]
		*/
		inline const EntityData& GetData() const { return m_data; }

	protected:
		EntityData	m_data;		/*!< The EntityData for the entity */
	};
}