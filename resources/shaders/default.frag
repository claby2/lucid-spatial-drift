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

in float currentTime;

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
    fragColor += vec4((globalData.ka * worldSpacePosition / 180.0), 0.0);

    // Individual block term
    fragColor += vec4(globalData.ka * (vec3(mod(floor(worldSpacePosition.x + 0.5), 7) / 9.0 + mod(floor(worldSpacePosition.y + 0.5), 8) / 26.0 + mod(floor(worldSpacePosition.z + 0.5), 9) / 31.0 - mod(floor(worldSpacePosition.x + 4.5), 11) / 21.0,
                                            mod(floor(worldSpacePosition.x + 0.5), 7) / 21.0 + mod(floor(worldSpacePosition.y + 0.5), 8) / 10.0 + mod(floor(worldSpacePosition.z + 0.5), 9) / 31.0 - mod(floor(worldSpacePosition.y + 3.5), 13) / 27.0,
                                            mod(floor(worldSpacePosition.x + 0.5), 7) / 21.0 + mod(floor(worldSpacePosition.y + 0.5), 8) / 26.0 + mod(floor(worldSpacePosition.z + 0.5), 9) / 11.0 - mod(floor(worldSpacePosition.z + 2.5), 10) / 19.0)),
                                       0.0);

    // Time-dependent term
    fragColor += vec4(globalData.ka * (vec3(sin(3.1 * currentTime + mod(worldSpacePosition.x, 3.9) / 4.0 + worldSpacePosition.z / 2.9) / 6.0 + sin(0.15 * mod(worldSpacePosition.y, 2.4) * currentTime - 0.72) / 6.7,
                                            sin(1.53 * currentTime + mod(worldSpacePosition.y + 1, 5.1) / 3.9 + worldSpacePosition.x / 2.3) / 5.5 - sin(0.14 * mod(worldSpacePosition.z, 2.1) * currentTime) / 8.0,
                                            sin(2.5 * currentTime + 0.2 + mod(worldSpacePosition.x - 2, 3.6) / 4.4 + worldSpacePosition.y / 1.6) / 5.0 + sin(0.13 * mod(worldSpacePosition.x, 2.0) * currentTime + 0.9) / 6.9)),
                                       0.0);

    // Glowing
    fragColor += vec4(globalData.ka * (vec3(pow(mod(pow(pow(worldSpacePosition.x - worldSpacePosition.y, 2) + pow(worldSpacePosition.z, 2), 0.5), 6), (5 + sin(3.95 * currentTime))) / 26000,
                                           pow(mod(pow(pow(worldSpacePosition.x, 2) + pow(worldSpacePosition.z - worldSpacePosition.y, 2), 0.5), 6), (5 + sin(3.6 * currentTime))) / 29000,
                                           pow(mod(pow(pow(worldSpacePosition.z - worldSpacePosition.y, 2) + pow(worldSpacePosition.z, 2), 0.5), 6), (5 + sin(3.3 * currentTime))) / 24000)),
                                           0.0);

    fragColor += vec4(globalData.ka * (vec3(pow(mod(pow(pow(worldSpacePosition.x, 2) + pow(worldSpacePosition.z - worldSpacePosition.y, 2), 0.5), 6), (5 + sin(3.6 * currentTime))) / 80000,
                                           pow(mod(pow(pow(worldSpacePosition.z - worldSpacePosition.y, 2) + pow(worldSpacePosition.z, 2), 0.5), 6), (5 + sin(3.3 * currentTime))) / 100000,
                                            pow(mod(pow(pow(worldSpacePosition.x - worldSpacePosition.y, 2) + pow(worldSpacePosition.z, 2), 0.5), 6), (5 + sin(3.95 * currentTime))) / 70000)),
                                           0.0);

    fragColor += vec4(globalData.ka * (vec3(pow(mod(pow(pow(worldSpacePosition.z - worldSpacePosition.y, 2) + pow(worldSpacePosition.z, 2), 0.5), 6), (5 + sin(3.3 * currentTime))) / 90000,
                                            pow(mod(pow(pow(worldSpacePosition.x - worldSpacePosition.y, 2) + pow(worldSpacePosition.z, 2), 0.5), 6), (5 + sin(3.95 * currentTime))) / 90000,
                                            pow(mod(pow(pow(worldSpacePosition.x, 2) + pow(worldSpacePosition.z - worldSpacePosition.y, 2), 0.5), 6), (5 + sin(3.6 * currentTime))) / 170000)),
                                           0.0);
    // Lasers

    fragColor += vec4(globalData.ka * (vec3(pow(mod(worldSpacePosition.x + currentTime, 5), 8) / 250000,
                                           pow(mod(worldSpacePosition.x + currentTime, 5), 8) / 250000,
                                            pow(mod(worldSpacePosition.x + currentTime, 5), 8) / 250000)),
                                           0.0);

    fragColor += vec4(globalData.ka * (vec3(pow(mod(worldSpacePosition.y + 1.5 * currentTime, 5), 7) / 70000,
                                           pow(mod(worldSpacePosition.y + 1.5 * currentTime, 5), 7) / 170000,
                                            pow(mod(worldSpacePosition.y + 1.5 * currentTime, 5), 7) / 750000)),
                                           0.0);

    fragColor += vec4(globalData.ka * (vec3(pow(mod(-worldSpacePosition.y + 1.4 * currentTime, 5), 7) / 1000000,
                                           pow(mod(-worldSpacePosition.y + 1.4 * currentTime, 5), 7) / 120000,
                                            pow(mod(-worldSpacePosition.y + 1.4 * currentTime, 5), 7) / 80000)),
                                           0.0);

    // Brightness/darkness filter

    // fragColor += 0.2 * fragColor * (sin(currentTime * 0.5) - 1);


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
