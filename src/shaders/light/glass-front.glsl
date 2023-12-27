#version 330 core

#define NR_POINT_LIGHTS 1;
#define NR_DIR_LIGHTS 1;
#define NR_FLASH_LIGHTS 1;

////////////////////////////////////////// Lights
#define MAX_NR_LIGHTS 10
// Light types
#define DIR_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct LightProperties {
    bool isEnabled;
    int lightType;

    vec3 position;
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    // spotlight
    float spotOuterCosCutoff;
    float epsilon; // inner - outer CosCutoff

    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

///////////////////////////////////////// Material
struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

// In 
in vec3 exNormal;
in vec3 exPosition;
in vec2 exTexcoord;
in vec3 camPosition;

// out
out vec4 FragColor;

// uniform 
uniform Material material;
uniform LightProperties Lights[MAX_NR_LIGHTS];

uniform samplerCube Skybox;
uniform sampler2D texture1;

vec4 computeLight(LightProperties light, vec3 lightDir, vec3 normal, vec3 viewDir, bool attenuation, float intensity) {
    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(abs(dot(normalize(viewDir), normalize(reflectDir))), 0.0), material.shininess);
    // combine results w/ intensity
    vec4 ambient  = light.ambient  *        material.ambient   * intensity;
    vec4 diffuse  = light.diffuse  * diff * material.diffuse   * intensity;
    vec4 specular = light.specular * spec * material.specular  * intensity;
    // attenuation
    if (attenuation) {
        float distance = length(light.position - exPosition);
        float attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * distance + 
        light.quadraticAttenuation * (distance * distance));
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
    }
    vec4 result = ambient + 0.1 * diffuse + specular; // diffuse has little contribution in galssy materials
    result.a = ambient.a + specular.a; // average alpha
    return result;
}

vec4 CalcDirLight(LightProperties light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    return computeLight(light, lightDir, normal, viewDir, false, 1.0);
}

vec4 CalcPointLight(LightProperties light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - exPosition);
    return computeLight(light, lightDir, normal, viewDir, true, 1.0);
}

vec4 CalcSpotLight(LightProperties light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - exPosition);
    float theta = max(dot(lightDir, normalize(-light.direction)), 0.0);
    if (theta > light.spotOuterCosCutoff) {
        float intensity = clamp((theta - light.spotOuterCosCutoff) / light.epsilon, 0.0, 1.0);
         return computeLight(light, lightDir, normal, viewDir, true, intensity);
    }
    else
        return light.ambient;
}

vec4 CalcLight(vec3 normal, vec3 viewDir) {
    vec4 resultColor = vec4(0.0);
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
    resultColor.a = min(resultColor.a, 1.0);
    return resultColor;
}

void main()
{
    vec4 texColor = texture(texture1, exTexcoord);
    vec4 texSpecular = texture(texture1, exTexcoord);
    vec3 fragDir = exPosition - camPosition;

    vec4 resultColor = CalcLight(normalize(exNormal), normalize(-fragDir));
    
    // reflection
    vec3 I = normalize(fragDir);
    vec3 Reflection = reflect(I, exNormal);
    // if vecs coincide, reflection should be low. If they are close to parallel it should be 1
    float reflectionFactor = pow(max(1 - dot(-I, exNormal), 0.0), 2);
    
    vec4 reflection = vec4(texture(Skybox, Reflection).rgb, 0.0);
    reflection.r = min(pow(reflection.r, 1), 0.9);
    reflection.g = min(pow(reflection.g, 1), 0.9);
    reflection.b = min(pow(reflection.b, 1), 0.9);
    reflection.a = reflection.r + reflection.b + reflection.g / 3;

    FragColor =  resultColor +  reflection * reflectionFactor;

    // FragColor = vec4(camPosition, 1.0);
}