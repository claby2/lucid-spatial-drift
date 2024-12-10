#version 330 core
uniform sampler2D sampler;
in vec2 uv;

out vec4 fragColor;

void main(void)
{
    fragColor = texture(sampler, uv);
}
