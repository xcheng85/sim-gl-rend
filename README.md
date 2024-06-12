# sim-gl-rend

## Math

homogeneous coordinates

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

### compilation and link (new-way, > 4.1), separate shader object

glCreateShaderProgramv
glCreateProgramPipelines
glUseProgramStages

gen-bind-delete for pipeline as well

glCreateProgramPipelines
glBindProgramPipeline
glDeleteProgramPipelines

caution: 
// to use separate shader object, the following is needed
// ARB_separate_shader_objects requires built-in block gl_PerVertex to be redeclared before accessing its members
out gl_PerVertex
{
    vec4 gl_Position;
};

// bindless for uniform
glProgramUniform* vs bind version glUniform*


## binary shader
https://www.khronos.org/opengl/wiki/Shader_Compilation
### gl style 

### spir-v + glsl
glslang, 

specialization constant: better alternative to macron (remember what I did in cgg ? )

compilation
IP: obfuscation

## subroutine vs c++ function pointer

subroutine uniform is like function pointer passed in

glGetSubroutineUniformLocation: uniform

## point cloud rendering
vs:
gl_PointSize
GL_PROGRAM_POINT_SIZE

fs:
gl_PointCoord

fix function: glPointSiez()

winding: cw / ccw

## DSA (direct state access, no bind, gen)


## Rendering pre-requisite


## Buffer object

create
storage
bind
upload data

copy between buffers
read data from buffer (if unmapped)

### mapping buffer (without copy, data --> to gpu)
mapped i/o in linux
gpu memory address mapped to the host

### buffer + multi-threading + async i/o

glMapNamedBufferRange

glUnmap*

1. can be a persistent mapping, only unmap at teardown 
2. use case in gl320-buffer-update: (this may be combined with async io and multi threading)
cpu: data-transfer + invalidate old data (let opengl know)
gpu: may have command rely on this buffer (ongoing, pending command, etc)

glFlushMappedBufferRange: data to gpu

use case: read huge amount of vertices/point cloud

A large buffer for all the vertices 

each threads mapped different subrange
and invalidate, then flush

metadata of this buffer should be used as indirect draw

glInvalidateBufferSubData: if no pending command uses this sub-range, tell opengl to optimize it. 

application must coordinate the draw command with different subranges of this mega-buffer


## VAO: data layout

### VAO: interleave
pos, normal, color, tex, pos, normal, tex, ...

### VAO: separate
pos buffer,
normal buffer
color buffer
...

### VAO in the instance rendering setup
buffer1: interleave for pos, normal, 
buffer2: instance-based matrix, 

vao->query->set->enable/disable

vertex specs

vertex buffer + vertex atributes array + enable the va for opengl to understand how to read the buffer

mat4 vertex attribute is complicated: treat it as 4 vec4


## Draw animation frame use case
animation of same model, only vb different, index buffer the same

packing K frames of vb into one big buffer
glDrawElementBaseVertex(..., ba) //ba will be ith frame * number of vertices 


## indirect draw
for non-index
for index

## multi-draw

draw multiple different objects with one command

## multi-indirect draw

## use case: draw multiple different object with one call, one buffer
need index-based draw

1. triangle strips with primitive restart index (mark)
2. multi-indirect draw, a lot of meta data maintainance

0xFFFF: pow(2, n) - 1

## instancing

draw multiple same object

gl_InstanceID

### Fetching per-instance data method1: vao instance attributes

### Fetching per-instance data method2: ssbo, texture buffer + gl_InstanceID

texture buffer: SampleBuffer
ssbo: buffer



## fbo

n fbo
fbo: k attachment

### fbo in vulkan vs opengl
vk: create default fbo mannually from the imageView of swapchain 3 images

opengl: default framebuffer is given

### clear attachment of fbo (bind fbo first)
glBindFramebuffer(GL_FRAMEBUFFER, handle);
glClearBufferiv 
glBindFramebuffer(GL_FRAMEBUFFER, 0);

### fbo and texture (texture view in vk)
glFramebufferTexture == in vulkan, texture view bind to framebuffer

### non-swapchain fbo
glBindFramebuffer(GL_FRAMEBUFFER, handle);

fbo bind multiple textures
glFramebufferTexture

### swapchain fbo
glBindFramebuffer(GL_FRAMEBUFFER, 0);

glColorMaski(attachment): mask how fragment shader write to an attachment of a fbo

### how a fragment is discarded
window clipp: scissor test
stencil test
failed depth test

blending
logic or

### MRT: (multiple render target)
configure fragment pre-test per attachment

### Direct State Access + FBO

### multi-sample
gl_NumSamples;
gl_SamplePosition;
gl_SampleID
gl_SampleMask

1. multi-sample texture
2. bind texture to the attachment of fbo
3. regular texture
4. bind regular texture to the resolve fbo
5. blitFramebuffer from ms to resolve

## z-fighting 
glPolygonOffset 

summry:
1. polygon parallel to the near-far clipping plane, small value
2. polygon large slop to the nf clipping plane, high value
3. polygon closer to the near clipping plane, high value
4. polygon farway to the near clipping plane, small value

## blending
source blending factor: fragment shader
dest: existing fbo attachment

## logic op on two fbo


## Occulusion Query & bounding volume against depth/stencil test
1. rendering bounding volume

disable color writes, disable blending due to no contribution for occultion result

2. use occulustion querie

3. if all occuleded, no need to renderer 

### before 4.5
glGenQueires
### after 4.5
glCreateQueries

## Occulustion Query & Conditional rendering



## Transformation

eye space: mv, for lighting

clip space: after mvp * pos, [x, y, z, w], gl_Posistion



opengl do the w-divide ---> clipspace to NDC; [-1, 1] * [-1, 1] * [0, 1]

fustrum clipping

### 1. Why homogenous coordinates
1. make translation to be a linear transformation (turn affine transformation back to linear transformation)
2. for perspective 

## Cull and clip for performance optimization
view fustrum
scissor
back/front face cull, cw, ccw

### 2. tranform normal vectors

normal vector won't be affected by translation, so only need the 3X3 concatenation matrix of rotation and scaling

### 3. z fighting
near plane more precise than far plane
soluton: near and far plane close 

### 4. extra culling 
gl_CullDistance[]
gl_ClipDistance[]: less than 0 will be culled waway
glEnable(GL_CLIP_DISTANCE0)

the transpose of inverse of M (3 * 3)


## Transform feedback 
another pattern of gpu-driven rendering

vs + transform feedback ---> ssbo

ssbo --> rendering 

### Old style
gen: glGenTransformFeedback

bind: glBindTransformFeedback

bind buffer (to be written) to the binding point of transformfeedback
the buffer could be Vertex Array buffer


### DSA style
glCreateTransformFeedbackBuffer

// bind a buffer handle to the binding point of a transform feedback handle
glTransformFeedbackBufferBase()


### 1. what is the minimum pipeline ? 
vs + transform feedback to a buffer