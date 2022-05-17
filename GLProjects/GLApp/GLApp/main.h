//-------------------------------------------------------------------------------------

#include <gl/glew.h>
#include <glm.hpp>
#include <gtx\matrix_decompose.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <map>
#include <memory>

//-------------------------------------------------------------------------------------

class Renderer;
struct GLFWwindow;

//-------------------------------------------------------------------------------------

const GLint width = 1920, height = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void Tick();
void KeyCallback(GLFWwindow  * window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow * window, double xPos, double yPos);

float lastX = width / 2.0f;
float lastY = height / 2.0f;

float deltaTime, keyboardlockout, lastFrame = 0.0f, SecondCounter = 1.0f;
bool Keys[1024];
auto firstMouse = false, lightDirection = true;
std::map<int, int> InputMap;

std::unique_ptr<Renderer> MainRenderer;

auto AliasingCount = 4, NumberofLights = 3;

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------