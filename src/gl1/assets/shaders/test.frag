#include <assets/shaders/shadeCommon.h>
#include <assets/shaders/common.h>

// interface block
in block
{
    layout(location=1) vec2 TexCoord;
} In;

// bindless texture
// uvec2 for uint64_t handle
layout(binding = MATERIAL) uniform material
{
	uvec2 Diffuse;
} Material;

layout(location = FRAG_COLOR, index = 0) out vec4 FragColor;

// glsl forward declaration
// function pointer: scale4
subroutine vec4 scale4();
subroutine uniform scale4 TextureScaler;

subroutine(scale4)
vec4 scale4Up() {
    return vec4(1.0, 1.0, 0.0, 1.0);
}

subroutine(scale4)
vec4 scale4Down() {
    return vec4(0.0, 0.0, 1.0, 1.0);
}

void main()
{
    FragColor = texture(sampler2D(Material.Diffuse), In.TexCoord) * TextureScaler();

    // FragColor = mix(texture(inTexture0, outTexCoord), texture(inTexture1, outTexCoord), 1);
} 