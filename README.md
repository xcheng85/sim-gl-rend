# sim-gl-rend

## Passing data through uniform block

### 1. multiple uniform block backed up by one buffer object

in the rendering loop, use glBindBufferRange

### 2. each uniform block backed up by different buffer object

glBindBufferBase

### 3. deal with uniformblockbinding (vulkan bind descriptor set)

in glsl, layout(binding = MATERIAL) uniform material

This could avoid the following: 
glGetUniformBlockIndex and then glUniformBlockBinding

### 4. uniformblock size needed for backed-up buffer
glGetActiveUniformBlockiv is needed due to glsl compiler optimization


## SSBO

### SSBO vs Uniform Buffer
SSBO: write,

Syntax: 
GL_SHADER_STORAGE_BUFFER vs GL_UNIFORM_BUFFER

SSBO: std430 layout
Uniform: std140/std430


## glsl coding style
interface block


## shader compilation vs c++ compilation
shaderObject: .obj in cpp
shaderPrograme: link multiple .obj

### compilation and link (old-way)

glShaderSource,
glCompileShader
glCreateProgram
glAttachShader
glLinkProgram

### compilation and link (new-way, > 4.1)

glCreateShaderProgramv
glCreateProgramPipelines
glUseProgramStages


## subroutine vs c++ function pointer

subroutine uniform is like function pointer passed in

glGetSubroutineUniformLocation: uniform