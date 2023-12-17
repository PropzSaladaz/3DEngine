#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


in vec3 exNormal;
in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exLightPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform Material material;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
    // ambient
    vec3 ambient = lightColor * material.ambient;

    // diffuse
    vec3 norm = normalize(exNormal);
    vec3 lightDir = normalize(exLightPos - exPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    // specular
    // reflect expects the 1st vector to be directed towards the refractable surface
    vec3 reflection = reflect(-lightDir, norm);
    float specularInt = pow(max(dot(normalize(-exPosition), normalize(reflection)), 0.0), material.shininess);
    vec3 specular = lightColor * (specularInt * material.specular);

    vec4 texColor = texture(texture1, exTexcoord);
    vec4 texSpecular = texture(texture2, exTexcoord);

    float specularVisibility = texSpecular.r;
    vec3 combinedSpecular = specular * specularVisibility;

    FragColor = vec4(ambient + diffuse, 1.0) * texColor + vec4(combinedSpecular, 1.0) * (vec4(texColor.rgb, 1.0));
    
}