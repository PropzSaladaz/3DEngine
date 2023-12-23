#version 330 core

#define NR_POINT_LIGHTS 1;
#define NR_DIR_LIGHTS 1;
#define NR_FLASH_LIGHTS 1;

////////////////////////////////////////// Lights
#define MAX_NR_LIGHTS 10
#define DIR_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct LightProperties {
    bool isEnabled;
    int lightType;

    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // spotlight
    float spotOuterCosCutoff;
    float epsilon; // inner - outer CosCutoff

    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

///////////////////////////////////////// Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// In 
in vec3 exNormal;
in vec3 exPosition;
in vec2 exTexcoord;

// out
out vec4 FragColor;

// uniform 
uniform Material material;
uniform LightProperties Lights[MAX_NR_LIGHTS];

uniform vec3 whiteColor;
uniform vec3 darkColor;

uniform sampler2D texture1;

vec3 computeLight(LightProperties light, vec3 lightDir, vec3 normal, vec3 viewDir, bool attenuation, float intensity) {
    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normalize(viewDir), normalize(reflectDir)), 0.0), material.shininess);
    // combine results w/ intensity
    // vec3 textureCol = vec3(texture(texture1, exTexcoord));
    vec3 ambient  = light.ambient  *        material.ambient   * intensity;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse   * intensity;
    vec3 specular = light.specular * spec * material.specular  * intensity;
    // attenuation
    if (attenuation) {
        float distance = length(light.position - exPosition);
        float attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * distance + 
        light.quadraticAttenuation * (distance * distance));
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
    }
    // return (ambient + diffuse + specular);
    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(LightProperties light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    return computeLight(light, lightDir, normal, viewDir, false, 1.0);
}

vec3 CalcPointLight(LightProperties light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - exPosition);
    return computeLight(light, lightDir, normal, viewDir, true, 1.0);
}

vec3 CalcSpotLight(LightProperties light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - exPosition);
    float theta = max(dot(lightDir, normalize(-light.direction), 0.0));
    if (theta > light.spotOuterCosCutoff) {
        float intensity = clamp((theta - light.spotOuterCosCutoff) / light.epsilon, 0.0, 1.0);
         return computeLight(light, lightDir, normal, viewDir, true, intensity);
    }
    else
        return vec3(light.ambient);
}

vec3 CalcLight(vec3 normal, vec3 viewDir) {
    vec3 resultColor = vec3(0.0);
    for (int light = 0 ; light < MAX_NR_LIGHTS; light++) {
        if (!Lights[light].isEnabled) continue;
        switch (Lights[light].lightType) {
            case DIR_LIGHT:
                resultColor += CalcDirLight(Lights[light], normal, viewDir);
                break;
            case POINT_LIGHT:
                resultColor += CalcPointLight(Lights[light], normal, viewDir);
                break;
            case SPOT_LIGHT:
                resultColor += CalcSpotLight(Lights[light], normal, viewDir);
                break;
            default:
                break;
        }
    }
    return resultColor;
}

void main()
{
    vec4 texColor = texture(texture1, exTexcoord);
    vec4 texSpecular = texture(texture1, exTexcoord);

    vec3 resultColor = CalcLight(normalize(exNormal), normalize(-exPosition));

    vec3 color = mix(whiteColor, darkColor, texColor.r);
    FragColor = vec4(resultColor * color, 1.0);
}