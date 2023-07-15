#pragma once

#include "color.hpp"

struct MaterialDesc
{
	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;
	float specular_power;
};

class Material
{
public:
	Material() = default;
	explicit Material(MaterialDesc* material_desc)
		: _ambient(std::move(material_desc->ambient)),
		  _diffuse(std::move(material_desc->diffuse)),
		  _specular(std::move(material_desc->specular)),
		  _emissive(std::move(material_desc->specular)),
		  _specular_power(std::move(material_desc->specular_power))
	{}

	virtual ~Material() = default;

	inline const Color& getAmbient() const { return this->_ambient; }
	inline void setAmbient(Color new_ambient) { this->_ambient = std::move(new_ambient); }

	inline const Color& getDiffuse() const { return this->_diffuse; }
	inline void setDiffuse(Color new_diffuse) { this->_diffuse = std::move(new_diffuse); }

	inline const Color& getSpecular() const { return this->_specular; }
	inline void setSpecular(Color new_specular) { this->_specular = std::move(new_specular); }

	inline const float& getSpecularPower() const { return this->_specular_power; }
	inline void setSpecularPower(float new_specular_power)
	{
		this->_specular_power = std::move(new_specular_power);
	}

	inline const Color& getEmissive() const { return this->_emissive; }
	inline void setEmissive(Color new_emissive) { this->_emissive = std::move(new_emissive); }

	bool hasAlpha() const { return this->getAlpha() != 1.0f; }
	inline float getAlpha() const { return this->_diffuse.a(); }
	inline void setAlpha(float new_alpha) { this->_diffuse.a(new_alpha); }

private:
	Color _ambient;
	Color _diffuse;
	Color _specular;
	Color _emissive;
	float _specular_power;
};
