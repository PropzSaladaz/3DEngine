#version 330 core

in vec3 exNormal;
in vec3 exPosition;
in vec3 exLightPos;

out vec4 FragColor;

uniform vec4 inColor;
uniform vec4 lightColor;


void main()
{
    // ambient
    float ambientStrength = 0.15;
    vec4 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(exNormal);
    vec3 lightDir = normalize(exLightPos - exPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor * 1.1;

    // specular
    // reflect expects the 1st vector to be directed towards the refractable surface
    vec3 reflection = reflect(-lightDir, norm);
    float specularInt = pow(max(dot(normalize(-exPosition), normalize(reflection)), 0.0), 8);
    float specularStrength = 0.6;
    vec4 specular = specularStrength * specularInt * lightColor;


    FragColor = (ambient + diffuse + specular) * inColor;
    
}