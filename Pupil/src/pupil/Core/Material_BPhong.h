#pragma once
#include "pupil/Core/Core.h"

// http://devernay.free.fr/cours/opengl/materials.html

#include <glm/glm.hpp>

namespace Pupil {

	enum Material_BPhong_Type {
		emerald = 0, jade, obsidian, pearl, ruby, turquoise,
		brass, bronze, chrome, copper, gold, silver,
		black_plastic, cyan_plastic, green_plastic, red_plastic,
		white_plastic, yellow_plastic, black_rubber, cyan_rubber,
		green_rubber, red_rubber, white_rubber, yellow_rubber // 24 ¸ö
	};

	static struct Material {
		Material() {
			mal[ 0].ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);       mal[ 0].diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);    mal[ 0].specular = glm::vec3(0.633f, 0.727811f, 0.633f);             mal[ 0].shininess = 0.6f;
			mal[ 1].ambient = glm::vec3(0.135f, 0.2225f, 0.1575f);        mal[ 1].diffuse = glm::vec3(0.54f, 0.89f, 0.63f);             mal[ 1].specular = glm::vec3(0.316228f, 0.316228f, 0.316228f);       mal[ 1].shininess = 0.1f;
			mal[ 2].ambient = glm::vec3(0.05375f, 0.05f, 0.06625f);       mal[ 2].diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f);       mal[ 2].specular = glm::vec3(0.332741f, 0.328634f, 0.346435f);       mal[ 2].shininess = 0.3f;
			mal[ 3].ambient = glm::vec3(0.25f, 0.20725f, 0.20725f);       mal[ 3].diffuse = glm::vec3(1.0f, 0.829f, 0.829f);            mal[ 3].specular = glm::vec3(0.296648f, 0.296648f, 0.296648f);       mal[ 3].shininess = 0.088f;
			mal[ 4].ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f);     mal[ 4].diffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f);    mal[ 4].specular = glm::vec3(0.727811f, 0.626959f, 0.626959f);       mal[ 4].shininess = 0.6f;
			mal[ 5].ambient = glm::vec3(0.1f, 0.18725f, 0.1745f);         mal[ 5].diffuse = glm::vec3(0.396f, 0.74151f, 0.69102f);      mal[ 5].specular = glm::vec3(0.297254f, 0.30829f, 0.306678f);        mal[ 5].shininess = 0.1f;
			mal[ 6].ambient = glm::vec3(0.329412f, 0.223529f, 0.027451f); mal[ 6].diffuse = glm::vec3(0.780392f, 0.568627f, 0.113725f); mal[ 6].specular = glm::vec3(0.992157f, 0.941176f, 0.807843f);       mal[ 6].shininess = 0.21794872f;
			mal[ 7].ambient = glm::vec3(0.2125f, 0.1275f, 0.054f);        mal[ 7].diffuse = glm::vec3(0.714f, 0.4284f, 0.18144f);       mal[ 7].specular = glm::vec3(0.393548f, 0.271906f, 0.166721f);       mal[ 7].shininess = 0.2f;
			mal[ 8].ambient = glm::vec3(0.25f, 0.25f, 0.25f);             mal[ 8].diffuse = glm::vec3(0.4f, 0.4f, 0.4f);                mal[ 8].specular = glm::vec3(0.774597f, 0.774597f, 0.774597f);       mal[ 8].shininess = 0.6f;
			mal[ 9].ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f);      mal[ 9].diffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f);      mal[ 9].specular = glm::vec3(0.256777f, 0.137622f, 0.086014f);       mal[ 9].shininess = 0.1f;
			mal[10].ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);      mal[10].diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);    mal[10].specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);       mal[10].shininess = 0.4f;
			mal[11].ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f);    mal[11].diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);    mal[11].specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);       mal[11].shininess = 0.4f;
			mal[12].ambient = glm::vec3(0.0f, 0.0f, 0.0f);                mal[12].diffuse = glm::vec3(0.01f, 0.01f, 0.01f);             mal[12].specular = glm::vec3(0.50f, 0.50f, 0.50f);                   mal[12].shininess = 0.25f;
			mal[13].ambient = glm::vec3(0.0f, 0.1f, 0.06f);               mal[13].diffuse = glm::vec3(0.0f, 0.50980392f, 0.50980392f);  mal[13].specular = glm::vec3(0.50196078f, 0.50196078f, 0.50196078f); mal[13].shininess = 0.25f;
			mal[14].ambient = glm::vec3(0.0f, 0.0f, 0.0f);                mal[14].diffuse = glm::vec3(0.1f, 0.35f, 0.1f);               mal[14].specular = glm::vec3(0.45f, 0.55f, 0.45f);                   mal[14].shininess = 0.25f;
			mal[15].ambient = glm::vec3(0.0f, 0.0f, 0.0f);                mal[15].diffuse = glm::vec3(0.5f, 0.0f, 0.0f);                mal[15].specular = glm::vec3(0.7f, 0.6f, 0.6f);                      mal[15].shininess = 0.25f;
			mal[16].ambient = glm::vec3(0.0f, 0.0f, 0.0f);                mal[16].diffuse = glm::vec3(0.55f, 0.55f, 0.55f);             mal[16].specular = glm::vec3(0.70f, 0.70f, 0.70f);                   mal[16].shininess = 0.25f;
			mal[17].ambient = glm::vec3(0.0f, 0.0f, 0.0f);                mal[17].diffuse = glm::vec3(0.5f, 0.5f, 0.0f);                mal[17].specular = glm::vec3(0.60f, 0.60f, 0.50f);                   mal[17].shininess = 0.25f;
			mal[18].ambient = glm::vec3(0.02f, 0.02f, 0.02f);             mal[18].diffuse = glm::vec3(0.01f, 0.01f, 0.01f);             mal[18].specular = glm::vec3(0.4f, 0.4f, 0.4f);                      mal[18].shininess = 0.078125f;
			mal[19].ambient = glm::vec3(0.0f, 0.05f, 0.05f);              mal[19].diffuse = glm::vec3(0.4f, 0.5f, 0.5f);                mal[19].specular = glm::vec3(0.04f, 0.7f, 0.7f);                     mal[19].shininess = 0.078125f;
			mal[20].ambient = glm::vec3(0.0f, 0.05f, 0.0f);               mal[20].diffuse = glm::vec3(0.4f, 0.5f, 0.4f);                mal[20].specular = glm::vec3(0.04f, 0.7f, 0.04f);                    mal[20].shininess = 0.078125f;
			mal[21].ambient = glm::vec3(0.05f, 0.0f, 0.0f);               mal[21].diffuse = glm::vec3(0.5f, 0.4f, 0.4f);                mal[21].specular = glm::vec3(0.7f, 0.04f, 0.04f);                    mal[21].shininess = 0.078125f;
			mal[22].ambient = glm::vec3(0.05f, 0.05f, 0.05f);             mal[22].diffuse = glm::vec3(0.5f, 0.5f, 0.5f);                mal[22].specular = glm::vec3(0.7f, 0.7f, 0.7f);                      mal[22].shininess = 0.078125f;
			mal[23].ambient = glm::vec3(0.05f, 0.05f, 0.0f);              mal[23].diffuse = glm::vec3(0.5f, 0.5f, 0.4f);                mal[23].specular = glm::vec3(0.7f, 0.7f, 0.04f);                     mal[23].shininess = 0.078125f;
		}
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
	}mal[25];

	 inline const Material& GetMaterial(Material_BPhong_Type type) {
		 return mal[type];
	 }
};