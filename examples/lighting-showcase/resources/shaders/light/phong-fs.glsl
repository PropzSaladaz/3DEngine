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


void main()
{
    vec4 texColor = texture(texture1, exTexcoord);
    vec4 texSpecular = texture(texture1, exTexcoord);

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

    vec3 combinedSpecular = specular * pow(texColor.rgb, vec3(2, 2, 2));

    FragColor = vec4(ambient + diffuse + combinedSpecular, 1.0) * texColor;
    // FragColor = vec4(mix(marble, marbleLight, k), 1);
    
}