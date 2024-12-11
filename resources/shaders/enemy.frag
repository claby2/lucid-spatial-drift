#version 330 core
uniform sampler2D sampler;
in vec2 uv;
uniform vec4 topLeftColor;
uniform vec4 bottomLeftColor;
uniform vec4 topRightColor;
uniform vec4 bottomRightColor;

out vec4 fragColor;

void main(void)
{
    float u = uv[0];
    float v = uv[1];
    vec4 filterColor = 
        u * v * topLeftColor + 
        u * (1-v) * bottomLeftColor + 
        (1-u) * v * topRightColor +
        (1-u) * (1-v) * bottomRightColor;
    vec4 c = texture(sampler, uv);
    if (c[3] == 0) {
        fragColor = c;
    } else {
        fragColor = texture(sampler, uv) + 0.5f * filterColor;
    }
}
