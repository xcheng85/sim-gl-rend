#include <assets/shaders/shadeCommon.h>
#include <assets/shaders/common.h>

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout(std140, column_major, binding = PER_FRAME_DATA) uniform perFrameData
{
	uniform mat4 mvp;
} PerFrameData;

layout(std140, binding = INSTANCING) uniform instancingData {
    uniform vec3 offsets[10];
} InstancingData;

// interface block
out block
{
    layout(location=1) vec2 TexCoord;
} Out;

// to use separate shader object, the following is needed
// ARB_separate_shader_objects requires built-in block gl_PerVertex to be redeclared before accessing its members
out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    Out.TexCoord = aTexCoord;
    // mat4 transform1 = mat4(2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    // left-hand
    gl_Position = PerFrameData.mvp * vec4(aPos + InstancingData.offsets[gl_InstanceID], 1.0f);
}