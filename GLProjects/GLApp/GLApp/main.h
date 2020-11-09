//-------------------------------------------------------------------------------------

#include <gl/glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\matrix_decompose.hpp>
#include <gtc\type_ptr.hpp>
#include <map>

//-------------------------------------------------------------------------------------

class Camera;
class Model;
class Shader;

struct GLFWwindow;
struct PostProcessSettings;

//-------------------------------------------------------------------------------------

//const GLint width = 1200, height = 800;
const GLint width = 1920, height = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void Tick();
void initialiseLights(Shader * lightShader);
void DrawLights(Shader * lampShader, Camera* Perspective);
void DrawSkybox(Shader * skyboxShaderRef, GLuint * facesRef, Camera* Perspective);
void DrawModel(Shader* modelShader, Model* InModel, glm::mat4 model, Camera* Perspective, GLuint * ShadowMap, float shine = 16);
void SetQuadUp(GLuint * quadVAO, GLuint * quadVBO);
void KeyCallback(GLFWwindow  * window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow * window, double xPos, double yPos);
void RenderDemo(Shader* InLampShader, Shader* InSkyboxShader, GLuint* InSkyboxTexture, Shader* InModelShader, Model* InModel, Shader* InUnlitShader, Model* InGizmo,  Camera* Perspective, GLuint * ShadowMap);

Camera* ourCamera;
Camera* LightingCamera;
GLfloat lastX = width / 2.0f;
GLfloat lastY = height / 2.0f;

GLfloat deltaTime, keyboardlockout, lastFrame = 0.0f, SecondCounter = 1.0f;
bool Keys[1024];
auto firstMouse = false, lightDirection = true;
PostProcessSettings* currentPostProcessSettings;
std::map<int, int> InputMap;

auto AliasingCount = 4, NumberofLights = 4;

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------