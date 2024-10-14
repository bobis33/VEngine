#include <glm/gtx/norm.hpp>
#include "VEngine/EventManager.hpp"

void ven::EventManager::handleEvents(GLFWwindow *window, ENGINE_STATE *engineState, Camera& camera, Gui& gui, const float dt) const
{
    if (glfwWindowShouldClose(window) == GLFW_TRUE) {
        updateEngineState(engineState, EXIT);
    }
    moveCamera(window, camera, gui, dt);
}

bool ven::EventManager::isKeyJustPressed(GLFWwindow* window, const int key, std::unordered_map<int, bool>& keyStates)
{
    const bool isPressed = glfwGetKey(window, key) == GLFW_PRESS;
    const bool wasPressed = keyStates[key];

    keyStates[key] = isPressed;

    return isPressed && !wasPressed;
}

void ven::EventManager::moveCamera(GLFWwindow* window, Camera& camera, Gui& gui, const float dt) const
{
    glm::vec3 rotate{0};
    glm::vec3 moveDir{0.F};
    const float yaw = camera.transform3D.rotation.y;
    const glm::vec3 forwardDir{std::sin(yaw), 0.F, std::cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.F, -forwardDir.x};
    constexpr glm::vec3 upDir{0.F, -1.F, 0.F};
    const std::array<KeyAction, 4> rotationActions = {{
        {.key=m_keys.lookLeft, .dir=&rotate, .value={0.F, -1.F, 0.F}},
        {.key=m_keys.lookRight, .dir=&rotate, .value={0.F, 1.F, 0.F}},
        {.key=m_keys.lookUp, .dir=&rotate, .value={1.F, 0.F, 0.F}},
        {.key=m_keys.lookDown, .dir=&rotate, .value={-1.F, 0.F, 0.F}}
    }};
    const std::array<KeyAction, 6> moveActions = {{
        {.key=m_keys.moveForward, .dir=&moveDir, .value=forwardDir},
        {.key=m_keys.moveBackward, .dir=&moveDir, .value=-forwardDir},
        {.key=m_keys.moveRight, .dir=&moveDir, .value=rightDir},
        {.key=m_keys.moveLeft, .dir=&moveDir, .value=-rightDir},
        {.key=m_keys.moveUp, .dir=&moveDir, .value=upDir},
        {.key=m_keys.moveDown, .dir=&moveDir, .value=-upDir}
    }};

    for (const auto&[key, dir, value] : rotationActions) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            *dir += value;
        }
    }

    for (const auto&[key, dir, value] : moveActions) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            *dir += value;
        }
    }

    if (glm::length2(rotate) > std::numeric_limits<float>::epsilon()) {
        camera.transform3D.rotation += camera.getLookSpeed() * dt * glm::normalize(rotate);
    }
    if (glm::length2(moveDir) > std::numeric_limits<float>::epsilon()) {
        camera.transform3D.translation += camera.getMoveSpeed() * dt * glm::normalize(moveDir);
    }

    camera.transform3D.rotation.x = glm::clamp(camera.transform3D.rotation.x, -1.5F, 1.5F);
    camera.transform3D.rotation.y = glm::mod(camera.transform3D.rotation.y, glm::two_pi<float>());

    if (isKeyJustPressed(window, m_keys.toggleGui, m_keyState)) {
        gui.getState() == HIDDEN ? gui.setState(VISIBLE) : gui.setState(HIDDEN);
    }
}
