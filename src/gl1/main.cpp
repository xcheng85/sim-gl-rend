#include <cmath>
#include <numbers>
#include <array>
#include <iostream>
#include <initializer_list>
#include <functional>
#include <memory>
#include <cassert>
#include <format>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector.h>
#include <matrix.h>
#include <camera.h>
#include <fp.h>
#include <pipeline.h>
#include <quaternion.h>
#include <texture.h>
#include <shader.h>

#include <common.h>

// forward declaration
bool checkExtensitions();
void update();
void render();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

constexpr uint16_t SCR_WIDTH = 1920;
constexpr uint16_t SCR_HEIGHT = 1080;

// synthetic data
// world position of center of each cube
// local space
// pos + color + tex
constexpr float vertices[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

constexpr static int NUM_CUBES = 10;
vec3f cubePositions[NUM_CUBES] = {
    vec3f(std::array{0.0f, 0.0f, 0.0f}),
    vec3f(std::array{2.0f, 5.0f, -15.0f}),
    vec3f(std::array{-1.5f, -2.2f, -2.5f}),
    vec3f(std::array{-3.8f, -2.0f, -12.3f}),
    vec3f(std::array{2.4f, -0.4f, -3.5f}),
    vec3f(std::array{-1.7f, 3.0f, -7.5f}),
    vec3f(std::array{1.3f, -2.0f, -2.5f}),
    vec3f(std::array{1.5f, 2.0f, -2.5f}),
    vec3f(std::array{1.5f, 0.2f, -1.5f}),
    vec3f(std::array{-1.3f, 1.0f, -1.5f})};

// GLuint shaderProgramHandle;
std::unique_ptr<Pipeline> g_ShaderProgramPipeline;
std::unique_ptr<Shader> g_VertexShader;
std::unique_ptr<Shader> g_FragmentShader;

std::unique_ptr<Texture> g_texture2d_1;
GLuint g_bindlessHandleUniformBufferHandle;

// vertex data related
GLuint g_VBO;
GLuint g_VAO;

// camera controller
Camera camera(
    vec3f(std::array{0.0f, 0.0f, 20.0f}), // pos
    vec3f(std::array{0.0f, 0.0f, -1.0f}), // target -z
    vec3f(std::array{0.0f, 1.0f, 0.0f}),  // initial world up
    0.0f,                                 // initial pitch
    -90.f                                 // initial yaw
);

struct PerFrameData
{
    mat4x4f mvp;
};
// avoid mapping unmapping per frame
PerFrameData *g_perFramePersistentPtr{nullptr};
// uniform Block is backed up by buffer
GLuint g_perFrameDataUniformBlockHandle;
GLuint g_perFrameDataBufferHandle;
// measure in bytes
GLint g_perFrameDataUniformBlockSize;
// this cpu-side measurement is not precise if the glsl compiler does optim
const GLsizeiptr g_perFrameDataBufferSize = sizeof(vec3f) * NUM_CUBES;

GLuint g_instancingOffsetUniformBlockHandle;
GLuint g_instancingOffsetBufferHandle;
GLint g_instancingOffsetUniformBlockSize;
const GLsizeiptr g_instancingOffsetBufferSize = sizeof(PerFrameData);

// sub-routine
GLuint g_subroutineTextureScalerUniformHandle;
GLuint g_TextureUpscaler;
GLuint g_TextureDownscaler;

// indirect-draw buffer handle
GLuint g_indirectDrawBufferHandle;
struct DrawArraysIndirectCommand
{
    GLuint count;
    GLuint instanceCount;
    GLuint first;
    GLuint baseInstance;
};

// occultion query against depth/stencil test
GLuint g_query;

// camera controller
float g_dt{0.0f};
float g_lastFrameTime{0.0f};
float g_firstMouseCursor{true};
float g_lastMouseCursorX, g_lastMouseCursorY;

static void errorCB(int error, const char *description)
{
    std::cerr << std::format("Error: {}\n", description);
}

static void keyCB(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    // if (key == GLFW_KEY_W && action == GLFW_PRESS)
    // {
    //     camera.handleKeyboardEvent(Camera::CameraActionType::FORWARD, g_dt);
    // }
    // if (key == GLFW_KEY_S && action == GLFW_PRESS)
    // {
    //     camera.handleKeyboardEvent(Camera::CameraActionType::BACKWARD, g_dt);
    // }
    // if (key == GLFW_KEY_A && action == GLFW_PRESS)
    // {
    //     camera.handleKeyboardEvent(Camera::CameraActionType::LEFT, g_dt);
    // }
    // if (key == GLFW_KEY_D && action == GLFW_PRESS)
    // {
    //     camera.handleKeyboardEvent(Camera::CameraActionType::RIGHT, g_dt);
    // }
}

// relative to the left-upper corner
static void mouseCursorCB(GLFWwindow *window, double newXPos, double newYPos)
{
    float x = static_cast<float>(newXPos);
    float y = static_cast<float>(newYPos);

    if (g_firstMouseCursor)
    {
        g_lastMouseCursorX = x;
        g_lastMouseCursorY = y;
        g_firstMouseCursor = false;
    }

    float dx = x - g_lastMouseCursorX;
    float dy = g_lastMouseCursorY - y; // screen space is opposite to camera space

    g_lastMouseCursorX = x;
    g_lastMouseCursorY = y;

    camera.handleMouseCursorEvent(dx, dy);
}

// Callback function for printing debug statements
void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     const GLchar *msg, const void *data)
{
    const char *_source;
    const char *_type;
    const char *_severity;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }

    // ignore notification severity (you can add your own ignores)
    // + Adds __debugbreak if _DEBUG is defined (automatic in visual studio)
    // note: __debugbreak is specific for MSVC, won't work with gcc/clang
    // -> in that case remove it and manually set breakpoints
    if (_severity != "NOTIFICATION")
    {
        printf("OpenGL error [%d]: %s of %s severity, raised from %s: %s\n",
               id, _type, _severity, _source, msg);
    }
}

bool checkExtensitions()
{
    std::vector<std::string> exts{
        "GL_ARB_bindless_texture",
        // "GL_NV_bindless_texture",
    };

    // const auto checkExtension = [](const std::string& ext)
    // {
    //     GLint ExtensionCount = 0;
    //     glGetIntegerv(GL_NUM_EXTENSIONS, &ExtensionCount);
    //     for (GLint i = 0; i < ExtensionCount; ++i)
    //         if (std::string((char const *)glGetStringi(GL_EXTENSIONS, i)) == ext)
    //             return true;
    //     return false;
    // };

    for (const auto &ext : exts)
    {
        if (!glfwExtensionSupported(ext.c_str()))
        {
            std::cerr << std::format("{} is not supported\n", ext);
            return false;
        }
    }
    return true;
}
// continous
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.handleKeyboardEvent(Camera::CameraActionType::FORWARD, g_dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.handleKeyboardEvent(Camera::CameraActionType::BACKWARD, g_dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.handleKeyboardEvent(Camera::CameraActionType::LEFT, g_dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.handleKeyboardEvent(Camera::CameraActionType::RIGHT, g_dt);
}

// resize ?
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void update()
{
}

void initQuery()
{
    // after 4.5
    glCreateQueries(GL_ANY_SAMPLES_PASSED_CONSERVATIVE, 1, &g_query);
}

// about the data
void initBuffer()
{
    glGenBuffers(1, &g_VBO);
    // glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

// about the data layout, how gpu inteprete it
void initVAO()
{
    // VAO includes all the vertex attribute buffer (<=16) + element buffer
    glGenVertexArrays(1, &g_VAO);
    glBindVertexArray(g_VAO);
    // interleave
    // two vertex attributes
    // position attribute: slot 0
    // stride = 3 + 3 + 2
    // to be more precise, instead of hard-coded 0
    const auto posLoc = glGetAttribLocation(g_VertexShader->programHandle(), "aPos");
    // assert(posLoc >= 0);
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(posLoc);
    // color attribute: slot 1
    const auto colorLoc = glGetAttribLocation(g_VertexShader->programHandle(), "aColor");
    // assert(colorLoc >= 0);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(colorLoc);
    // texture coord attribute: slot2, uv
    const auto texCoordLoc = glGetAttribLocation(g_VertexShader->programHandle(), "aTexCoord");
    // assert(texCoordLoc >= 0);
    glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(texCoordLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // instanced rendering a separate buffer, count is the instance count
}

void render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClearBufferiv for the attachment to the fbo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 0: swapchain buffer fbo
    glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    g_ShaderProgramPipeline->bind();
    mat4x4f identity(1.0f);
    // // +20.f, move away, zoom out
    // auto view1 = MatrixMultiply4x4(MatrixTranslation4x4(0.0f, 0.0f, 20.f), identity);
    // cout << view1 << "\n";

    // vec3f cameraPos(std::array{0.0f, 0.0f, 20.f});
    // vec3f cameraLookatTarget(std::array{0.0f, 0.0f, 0.f});
    // vec3f cameraWorldUp(std::array{1.0f, 0.0f, 0.f});
    // auto view = ViewTransformLH4x4(cameraPos, cameraLookatTarget, cameraWorldUp);
    // cout << view << "\n";

    auto view = camera.viewTransformLH();
    auto persPrj = PerspectiveProjectionTransformLH(0.1f, 100.0f, 0.5f, (float)SCR_WIDTH / (float)SCR_HEIGHT);

    // left-hand
    auto vp = MatrixMultiply4x4(view, persPrj);
    auto mvp = MatrixMultiply4x4(identity, vp);

    // bind vao
    glBindVertexArray(g_VAO);
    // bind uniform buffers
    // buffer is genric, here bind it to unifor buffer, tell driver the usage of this buffer.
    // ub slot: 0
    // here basically bind the full range a buffer, glBindBufferRange is not useful
    // glBindBufferRange(GL_UNIFORM_BUFFER, 0, g_perFrameDataBufferHandle, 0, g_perFrameDataBufferSize);
    // inefficient way to update uniform buffer
    // glBindBufferBase(GL_UNIFORM_BUFFER, 0, g_perFrameDataBufferHandle);
    PerFrameData perFrameData{.mvp = mvp};
    // glNamedBufferSubData(g_perFrameDataBufferHandle, 0, g_perFrameDataBufferSize, &perFrameData);

    // persistent mapped, update using mapping pointer
    *g_perFramePersistentPtr = perFrameData;

    // this is for rendering
    glBindBufferBase(GL_UNIFORM_BUFFER, PER_FRAME_DATA, g_perFrameDataBufferHandle);
    // bindless texture handle
    // ub slot: 1
    glBindBufferBase(GL_UNIFORM_BUFFER, MATERIAL, g_bindlessHandleUniformBufferHandle);
    // cube position buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, INSTANCING, g_instancingOffsetBufferHandle);

    // select subroutine fp: texture scaler
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &g_TextureDownscaler);

    // glDrawArraysInstanced(GL_TRIANGLES, 0, 36, NUM_CUBES);
    // glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 36, NUM_CUBES, 0);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, g_indirectDrawBufferHandle);
    glBeginQuery(GL_ANY_SAMPLES_PASSED_CONSERVATIVE, g_query);
    glDrawArraysIndirect(GL_TRIANGLES, 0);
    glEndQuery(GL_ANY_SAMPLES_PASSED_CONSERVATIVE);

    GLuint64 samplePassed;
    glGetQueryObjectui64v(g_query, GL_QUERY_RESULT, &samplePassed);
    cout << "samplePassed: " << samplePassed << endl;

    // for (unsigned int i = 0; i < NUM_CUBES; ++i)
    // {
    //     auto t = MatrixMultiply4x4(MatrixTranslation4x4(
    //                                    cubePositions[i][COMPONENT::X],
    //                                    cubePositions[i][COMPONENT::Y],
    //                                    cubePositions[i][COMPONENT::Z]),
    //                                identity);
    //     float angle = 0.24f * i;
    //     auto r = MatrixRotationAxis4x4(vec3f(std::array{1.0f, 0.3f, 0.5f}), angle);
    //     auto model = MatrixMultiply4x4(r, t);
    //     // left-hand
    //     auto mvp = MatrixMultiply4x4(model, vp);
    //     cout << mvp << "\n";
    //     PerFrameData perFrameData{.mvp = mvp};
    //     glNamedBufferSubData(g_perFrameDataBufferHandle, 0, g_perFrameDataBufferSize, &perFrameData);
    //     // 6 * 6
    //     glDrawArrays(GL_TRIANGLES, 0, 36);
    // }

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
}
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(errorCB);
    glfwSetKeyCallback(window, keyCB);
    glfwSetCursorPosCallback(window, mouseCursorCB);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    assert(checkExtensitions());
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugMessageCallback, nullptr);

    g_VertexShader = make_unique<Shader>("assets/shaders/test.vert");
    g_FragmentShader = make_unique<Shader>("assets/shaders/test.frag");
    const auto vsHandle = g_VertexShader->programHandle();
    const auto fsHandle = g_FragmentShader->programHandle();

    // uniform block
    g_perFrameDataBufferHandle = glGetUniformBlockIndex(vsHandle, "perFrameData");
    g_instancingOffsetUniformBlockHandle = glGetUniformBlockIndex(vsHandle, "instancingData");

    glGetActiveUniformBlockiv(vsHandle, g_perFrameDataBufferHandle, GL_UNIFORM_BLOCK_DATA_SIZE,
                              &g_perFrameDataUniformBlockSize);
    glGetActiveUniformBlockiv(vsHandle, g_instancingOffsetUniformBlockHandle, GL_UNIFORM_BLOCK_DATA_SIZE,
                              &g_instancingOffsetUniformBlockSize);
    // subroutine (function pointer)
    g_subroutineTextureScalerUniformHandle = glGetSubroutineUniformLocation(fsHandle, GL_FRAGMENT_SHADER, "TextureScaler");
    g_TextureUpscaler = glGetSubroutineIndex(fsHandle, GL_FRAGMENT_SHADER, "scale4Up");
    g_TextureDownscaler = glGetSubroutineIndex(fsHandle, GL_FRAGMENT_SHADER, "scale4Down");
    assert(g_subroutineTextureScalerUniformHandle >= 0);
    assert(g_TextureUpscaler >= 0);
    assert(g_TextureDownscaler >= 0);

    g_ShaderProgramPipeline = make_unique<Pipeline>(unordered_map<GLenum, GLuint>{
        {GL_VERTEX_SHADER_BIT, vsHandle},
        {GL_FRAGMENT_SHADER_BIT, fsHandle},
    });

    initBuffer();

    initVAO();

    initQuery();

    // uniform buffer
    // requires opengl 4.5
    // glCreateBuffers is the equivalent of glGenBuffers + glBindBuffer(the initialization part)
    glCreateBuffers(1, &g_perFrameDataBufferHandle);

    // no data to copy during init
    // glNamedBufferStorage(g_perFrameDataBufferHandle, g_perFrameDataBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(g_perFrameDataBufferHandle, g_perFrameDataUniformBlockSize, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    // bind is needed
    glBindBuffer(GL_UNIFORM_BUFFER, g_perFrameDataBufferHandle);
    g_perFramePersistentPtr = (PerFrameData *)glMapBufferRange(
        GL_UNIFORM_BUFFER, 0, sizeof(PerFrameData),
        GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glCreateBuffers(1, &g_instancingOffsetBufferHandle);
    // static and immutable
    glNamedBufferStorage(g_instancingOffsetBufferHandle,
                         g_instancingOffsetUniformBlockSize, &cubePositions[0], 0);

    // // textures
    g_texture2d_1 = make_unique<Texture>(
        GL_TEXTURE_2D,
        "assets/textures/dame.jpeg");

    // bindless uint64_t handle
    const auto bindlessTextureHandle = g_texture2d_1->getBindlessHandle();
    glCreateBuffers(1, &g_bindlessHandleUniformBufferHandle);
    // static buffer: set flags​ to 0 and use data​ as the initial upload
    //  immutable storage
    glNamedBufferStorage(g_bindlessHandleUniformBufferHandle,
                         sizeof(bindlessTextureHandle),
                         &bindlessTextureHandle, 0);

    // indirect draw
    // static and immutable
    // reference: glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 36, NUM_CUBES, 0);
    DrawArraysIndirectCommand drawNoIndex{
        .count = 36,
        .instanceCount = NUM_CUBES,
        .first = 0,
        .baseInstance = 0,
    };
    glCreateBuffers(1, &g_indirectDrawBufferHandle);
    glNamedBufferStorage(g_indirectDrawBufferHandle,
                         sizeof(DrawArraysIndirectCommand), &drawNoIndex, 0);

    vec3f pod{std::array<float, 3>{0.f, 1.f, 1.f}};
    cout << pod << "\n";
    pod.normalize();
    cout << pod << "\n";
    // vector arithmatic
    pod = pod - std::array<float, 3>{-1.f, 1.f, 0.f};
    cout << pod << "\n";
    // {
    //     // testing fp
    //     const auto cosVector = functor1<vec, float, 3, 16>::call(cos, pod);
    //     cout << cosVector << "\n";

    //     // testing fp with lamda
    //     const auto t = functor1_lamda<vec, float, 2, 8>::call([](auto v)
    //                                                           {
    //         cout << v << "\n";
    //         return v; },
    //                                                           vec2f(std::array{
    //                                                               1.0f, 2.0f}));
    //     cout << t << "\n";
    //     cout << rad(vec3f(std::array{0.0f, 90.0f, 180.f})) << "\n";
    //     cout << rad(-90.f) << "\n";
    // }
    {
        // testing quaternion
        quatf quat = QuaternionFromAxisAngle(vec3f(std::array<float, 3>{0.f, 1.f, 0.f}), 0.588f);
        const auto axis = RotationAxisFromQuaternion(quat);
        const auto angle = RotationAngleFromQuaternion(quat);
        cout << axis << "\n";
        cout << angle << "\n";
    }

    mat4x4f identity(std::array<float, 4 * 4>{
        1.f,
        0.f,
        0.f,
        0.f,
        0.f,
        1.f,
        0.f,
        0.f,
        0.f,
        0.f,
        1.f,
        0.f,
        0.f,
        0.f,
        0.f,
        1.f,
    });
    cout << identity << "\n";
    // auto type deduction
    auto m2 = MatrixMultiply4x4(identity, MatrixScale4x4(2.f, 3.f, 4.f));
    cout << m2 << "\n";

    // rotation on canonical basis
    auto rotx = MatrixMultiply4x4(MatrixRotationX4x4(0.5f), MatrixRotationX4x4(-0.5f));
    cout << rotx << "\n";
    auto roty = MatrixMultiply4x4(MatrixRotationY4x4(0.5f), MatrixRotationY4x4(-0.5f));
    cout << roty << "\n";
    auto rotz = MatrixMultiply4x4(MatrixRotationZ4x4(0.5f), MatrixRotationZ4x4(-0.5f));
    cout << rotz << "\n";

    {
        // validate rotation x
        auto mat1 = MatrixRotationAxis4x4(vec3f(std::array<float, 3>{1.f, 0, 0}),
                                          (float)std::numbers::pi / 4);
        auto mat2 = MatrixRotationX4x4(std::numbers::pi / 4);

        cout << "mat1: \n"
             << mat1 << "\n";
        cout << "mat2: \n"
             << mat2 << "\n";
    }

    // cout <<  std::invoke(std::cos, 0.f) << "\n";
    // cout <<  cos(0.f) << "\n";
    // [](){ print_num(42) };

    while (!glfwWindowShouldClose(window))
    {
        float t = static_cast<float>(glfwGetTime());
        g_dt = t - g_lastFrameTime;
        g_lastFrameTime = t;
        processInput(window);
        update();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &g_VAO);
    glDeleteBuffers(1, &g_VBO);
    if (g_perFramePersistentPtr)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, g_perFrameDataBufferHandle);
        glUnmapBuffer(GL_UNIFORM_BUFFER);
        g_perFramePersistentPtr = nullptr;
    }
    glDeleteBuffers(1, &g_perFrameDataBufferHandle);
    glDeleteBuffers(1, &g_bindlessHandleUniformBufferHandle);
    glDeleteBuffers(1, &g_instancingOffsetBufferHandle);

    glfwTerminate();
    return 0;
}
