/*  ____ ____ ____ ____
 * ||Z |||E |||G |||L ||
 * ||__|||__|||__|||__||
 * |/__\|/__\|/__\|/__\|
 *
 * Copyright(c) 2014, Phil Sampson
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

#include "entity.h"
#include "shader.h"
#include "util.h"

class Light : public Entity
{
public:
	Light(const Shader& shader) :
		m_lightColor(glm::vec3(1.0f)),
		m_lightIntensity(1.0f),
		m_ambientColor(glm::vec3(0.2f)),
		m_ambientIntensity(0.2f),
		m_falloff(glm::vec3(0.4f, 3.0f, 20.0f)),
		m_shader(shader),
		Entity(glm::vec3(0.5f, 0.5f, 0.05f)) {}

	Light(const Shader& shader, const glm::vec3& pos,
		const glm::vec3& lightCol, float lightIntensity,
		const glm::vec3& ambientCol, float ambientIntensity, const glm::vec3& falloff) :
		m_lightColor(lightCol),
		m_lightIntensity(lightIntensity),
		m_ambientColor(ambientCol),
		m_ambientIntensity(ambientIntensity),
		m_falloff(falloff),
		m_shader(shader),
		Entity(pos) {}

	virtual ~Light() {}

	inline const glm::vec3& GetLightColor()		const { return m_lightColor; }
	inline const float GetLightIntensity()		const { return m_lightIntensity; }
	inline const glm::vec3& GetAmbientColor()	const { return m_ambientColor; }
	inline const float GetAmbientIntensity()	const { return m_ambientIntensity; }
	inline const glm::vec3& GetFalloff()		const { return m_falloff; }
	inline const Shader& GetShader()			const { return m_shader; }
	
	inline void SetLightColor(const glm::vec3& lightCol)		{ m_lightColor = lightCol; }
	inline void SetLightIntensity(float lightIntensity)			{ m_lightIntensity = lightIntensity; }
	inline void SetAmbientColor(const glm::vec3& ambientCol)	{ m_ambientColor = ambientCol; }
	inline void SetAmbientIntensity(float ambientIntensity)		{ m_ambientIntensity = ambientIntensity; }
	inline void SetFalloff(const glm::vec3& falloff)			{ m_falloff = falloff; }

private:
	glm::vec3	m_lightColor;
	float		m_lightIntensity;
	glm::vec3	m_ambientColor;
	float		m_ambientIntensity;
	glm::vec3	m_falloff;

	Shader		m_shader;
};