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

uniform vec4 whiteColor;
uniform vec4 darkColor;

uniform samplerCube Skybox;
uniform sampler2D texture1;

uniform samplerCube DiffuseTexture;
uniform samplerCube SpecularTexture;

vec4 computeLight(LightProperties light, vec3 lightDir, vec3 normal, vec3 viewDir, bool attenuation, float intensity) {
    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    // specular
    vec3 halfVec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normalize(halfVec), normalize(normal)), 0.0), material.shininess);
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
    vec4 result = ambient + diffuse + specular;
    result.a = ambient.a + diffuse.a + specular.a / 3; // average alpha
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
        return vec4(light.ambient.rgb, 0.0);
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
    resultColor.a = max(resultColor.a, 1.0);
    return resultColor;
}

void main()
{
    vec4 texColor = texture(texture1, exTexcoord);
    vec4 texSpecular = texture(texture1, exTexcoord);
    vec3 fragDir = exPosition - camPosition;

    vec4 resultColor = CalcLight(normalize(exNormal), normalize(-fragDir));
    
    // Diffuse Environment lighting
    vec4 diffuseEnv = vec4(texture(DiffuseTexture, exNormal).rgb, 1.0);
    // Specular Environment lighting
    vec3 I = normalize(fragDir);
    vec3 Reflection = reflect(I, exNormal);
    vec4 specEnv = vec4(texture(SpecularTexture, Reflection).rgb, 1.0);
    float specEnvFactor = pow(max(dot(normalize(-fragDir), normalize(exNormal)), 1.0), material.shininess);

    vec4 color = mix(whiteColor, darkColor, texColor.r);
    FragColor = resultColor * color +  diffuseEnv * 0.1 + specEnv * specEnvFactor * 0.1;
}