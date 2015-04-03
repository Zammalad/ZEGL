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


#pragma once

#include <glm/glm.hpp>

namespace ZEGL
{
	class Window;

	__declspec(align(16)) class Camera
	{
	public:
		Camera(const Window* window = nullptr);

		const glm::mat4& GetTransform(const Window* window);

		inline glm::vec3	GetPos()	const { return m_pos; }
		inline float		GetRot()	const { return m_rot; }
		inline float		GetZoom()	const { return m_zoom; }

		inline void			SetPos(float x, float y, float z = 0.0f)	{ m_pos.x = x; m_pos.y = y; m_pos.z = z; }
		inline void			SetPos(const glm::vec3& pos)				{ m_pos = pos; }
		inline void			SetRot(float rot)							{ m_rot = rot; }
		inline void			SetZoom(float zoom)							{ m_zoom = zoom; }

		void*				operator new(size_t i)		{ return _mm_malloc(i, 16); }
		void				operator delete(void* p)	{ _mm_free(p); }

	protected:
	private:
		struct Transformation
		{
			glm::mat4	m_matrix;
			glm::vec3	m_lastPos;
			float		m_lastRot;
			float		m_lastZoom;

			void Update(const glm::vec3& pos, float rot, float zoom)
			{
				m_lastPos = pos; m_lastRot = rot; m_lastZoom = zoom;
			}
		};

		void RecreateTransform(const Window* window);

		glm::vec3		m_pos;
		float			m_rot;
		float			m_zoom;
		Transformation	m_transform;
	};
}