#version 330 core

in vec3 worldSpaceNormal;
in vec3 worldSpacePosition;

struct GlobalData {
    float ka;
    float kd;
    float ks;
};

struct Light {
    int type;

    vec4 color;
    vec4 dir;
    vec4 pos;
    vec3 function;

    float angle;
    float penumbra;
};

struct Material {
    vec4 cAmbient;
    vec4 cDiffuse;
    vec4 cSpecular;
    float shininess;

    vec4 cReflective;
};

uniform Material material;
uniform GlobalData globalData;
uniform Light lights[8];
uniform int lightCount;
uniform vec4 worldSpaceCamera;

out vec4 fragColor;

vec3 calculateLightDirection(Light light, vec3 pos) {
    switch (light.type) {
        case 0:
        case 2:
            return normalize(vec3(light.pos) - pos);
        case 1:
        default:
            return normalize(-vec3(light.dir));
    }
}

float calculateAttenuation(Light light, float distance) {
    float c1 = light.function.x;
    float c2 = light.function.y;
    float c3 = light.function.z;
    return min(1.0f, 1.0f / (c1 + c2 * distance + c3 * distance * distance));
}

float calculateSpotLightFactor(Light light, vec3 lightDirection) {
  float x = acos(dot(normalize(vec3(-light.dir)),
                               normalize(lightDirection)));
  float outer = light.angle;
  float inner = outer - light.penumbra;
  if (x <= inner) {
    return 1.0f;
  } else if (x > inner && x <= outer) {
    float factor = (x - inner) / (outer - inner);
    return 1.0f - (-2.0f * pow(factor, 3) + 3.0f * pow(factor, 2));
  } else {
    return 0.0f;
  }
}


void main() {
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);

    // Ambient
    fragColor += vec4(globalData.ka * vec3(material.cAmbient), 0.0);

    for (int i = 0; i < lightCount; i++) {
        Light light = lights[i];

        float distance = length(vec3(light.pos) - worldSpacePosition);

        float fAtt = 1.0;
        if (light.type == 0 || light.type == 2) { // Point or spot light
            fAtt = calculateAttenuation(light, distance);
        }

        vec3 lightDirection = calculateLightDirection(light, worldSpacePosition);

        float spotFactor = 1.0;
        if (light.type == 2) { // Spot light
            spotFactor = calculateSpotLightFactor(light, lightDirection);
        }

        // Diffuse
        float nl = max(dot(normalize(worldSpaceNormal), lightDirection), 0.0);
        fragColor += spotFactor * fAtt * globalData.kd * nl * material.cDiffuse * light.color;

        // Specular
        vec3 r = reflect(-lightDirection, normalize(worldSpaceNormal));
        vec3 e = normalize(vec3(worldSpaceCamera) - worldSpacePosition);
        float re = clamp(dot(r, e), 0.0, 1.0);
        if (re == 0.0 && material.shininess <= 0.0) continue;
        float reShininess = pow(re, material.shininess);
        fragColor += spotFactor * fAtt * vec4(globalData.ks * reShininess * vec3(material.cSpecular) * vec3(light.color), 0.0);
    }

    fragColor = clamp(fragColor, 0.0, 1.0);
}
