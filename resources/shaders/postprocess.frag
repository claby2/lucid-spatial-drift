#version 330 core

in vec2 UV;

uniform sampler2D sampler;
uniform bool perPixelFilter;
uniform bool kernelBasedFilter;
uniform int screenWidth;
uniform int screenHeight;

out vec4 fragColor;

void main() {
    fragColor = texture(sampler, UV);

    if (kernelBasedFilter) {
        float factor = 1.0 / 25.0;

        vec4 color = vec4(0);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                vec2 offset = vec2(i - 2, j - 2) / vec2(screenWidth, screenHeight);
                vec4 sample = texture(sampler, UV + offset);
                color += sample;
            }
        }
        fragColor = color * factor;
    }

    if (perPixelFilter) fragColor = vec4(1) - fragColor;
}
