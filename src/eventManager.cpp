#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "VEngine/EventManager.hpp"

bool ven::EventManager::isKeyJustPressed(GLFWwindow* window, const int key, std::unordered_map<int, bool>& keyStates)
{
    const bool isPressed = glfwGetKey(window, key) == GLFW_PRESS;
    const bool wasPressed = keyStates[key];

    keyStates[key] = isPressed;

    return isPressed && !wasPressed;
}

template<typename Iterator>
void ven::EventManager::processKeyActions(GLFWwindow* window, Iterator begin, Iterator end)
{
    for (auto it = begin; it != end; ++it) {
        if (glfwGetKey(window, it->key) == GLFW_PRESS) {
            *it->dir += it->value;
        }
    }
}

void ven::EventManager::moveCamera(GLFWwindow* window, Camera& camera, const float dt)
{
    glm::vec3 rotate{0};
    glm::vec3 moveDir{0.F};
    static constexpr glm::vec3 upDir{0.F, -1.F, 0.F};
    const float yaw = camera.transform.rotation.y;
    const glm::vec3 forwardDir{std::sin(yaw), 0.F, std::cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.F, -forwardDir.x};
    const std::array<KeyAction, 10> moveActions = {{
        {.key=DEFAULT_KEY_MAPPINGS.lookLeft, .dir=&rotate, .value={0.F, -1.F, 0.F}},
        {.key=DEFAULT_KEY_MAPPINGS.lookRight, .dir=&rotate, .value={0.F, 1.F, 0.F}},
        {.key=DEFAULT_KEY_MAPPINGS.lookUp, .dir=&rotate, .value={1.F, 0.F, 0.F}},
        {.key=DEFAULT_KEY_MAPPINGS.lookDown, .dir=&rotate, .value={-1.F, 0.F, 0.F}},
        {.key=DEFAULT_KEY_MAPPINGS.moveForward, .dir=&moveDir, .value=forwardDir},
        {.key=DEFAULT_KEY_MAPPINGS.moveBackward, .dir=&moveDir, .value=-forwardDir},
        {.key=DEFAULT_KEY_MAPPINGS.moveRight, .dir=&moveDir, .value=rightDir},
        {.key=DEFAULT_KEY_MAPPINGS.moveLeft, .dir=&moveDir, .value=-rightDir},
        {.key=DEFAULT_KEY_MAPPINGS.moveUp, .dir=&moveDir, .value=upDir},
        {.key=DEFAULT_KEY_MAPPINGS.moveDown, .dir=&moveDir, .value=-upDir}
    }};

    processKeyActions(window, moveActions.begin(), moveActions.end());

    if (length2(rotate) > EPSILON) {
        camera.transform.rotation += camera.getLookSpeed() * dt * normalize(rotate);
    }
    if (length2(moveDir) > EPSILON) {
        camera.transform.translation += camera.getMoveSpeed() * dt * normalize(moveDir);
    }

    camera.transform.rotation.x = glm::clamp(camera.transform.rotation.x, -1.5F, 1.5F);
    camera.transform.rotation.y = glm::mod(camera.transform.rotation.y, glm::two_pi<float>());
}

void ven::EventManager::handleEvents(GLFWwindow *window, ENGINE_STATE *engineState, Camera& camera, Gui& gui, const float dt) const
{
    glfwPollEvents();
    if (glfwWindowShouldClose(window) == GLFW_TRUE) {
        updateEngineState(engineState, EXIT);
    }
    if (isKeyJustPressed(window, DEFAULT_KEY_MAPPINGS.toggleGui, m_keyState)) {
        gui.getState() == HIDDEN ? gui.setState(VISIBLE) : gui.setState(HIDDEN);
    }
    moveCamera(window, camera, dt);
}
