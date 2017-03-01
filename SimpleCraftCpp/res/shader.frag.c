#version 330 core

struct Light {
    vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;  

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
  
uniform Material material;

out vec4 color;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;

void main()
{
	//Ambient
	float ambientStrength = 0.1f;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	float diff = max(dot(norm, light.direction), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	
	//Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec;

	//Combined
	vec3 result = (ambient + diffuse + specular) * objectColor;

	///*
	//gamma
	result = pow( result, vec3(0.8) );
	// contrast
	result = result* 0.25 + 0.75*result*result*(3.0-2.0*result);
	
    result = clamp( result, 0.0, 1.0 );
    //*/

	color = vec4(result, 1.0f);

	
}