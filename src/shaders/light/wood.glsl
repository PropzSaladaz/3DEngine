#version 330 core

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


in vec3 exNormal;
in vec3 exPosition;
in vec2 exTexcoord;

out vec4 FragColor;

uniform Light light;
uniform Material material;

uniform vec3 whiteColor;
uniform vec3 darkColor;

uniform sampler2D texture1;


void main()
{
    vec4 texColor = texture(texture1, exTexcoord);
    vec4 texSpecular = texture(texture1, exTexcoord);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(exNormal);
    vec3 lightDir = normalize(light.position - exPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    // reflect expects the 1st vector to be directed towards the refractable surface
    vec3 reflection = reflect(-lightDir, norm);
    float specularInt = pow(max(dot(normalize(-exPosition), normalize(reflection)), 0.0), material.shininess);
    vec3 specular = light.specular * (specularInt * material.specular);
    vec3 combinedSpecular = specular * (1 - texColor.r);

    vec3 color = mix(whiteColor, darkColor, texColor.r);
    FragColor = vec4((ambient + diffuse) * color + combinedSpecular, 1);
    // FragColor = vec4(mix(marble, marbleLight, k), 1);
    
}