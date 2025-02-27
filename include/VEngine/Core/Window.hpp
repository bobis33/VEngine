///
/// @file Window.hpp
/// @brief This file contains the Window class
/// @namespace ven
///

#pragma once


struct GLFWwindow;

namespace ven {

    ///
    /// @class Window
    /// @brief Class for window
    /// @namespace ven
    ///
    class Window {

        public:

            static constexpr uint16_t DEFAULT_WIDTH = 1920;
            static constexpr uint16_t DEFAULT_HEIGHT = 1080;

            explicit Window(const uint16_t width = DEFAULT_WIDTH, const uint16_t height = DEFAULT_HEIGHT) : m_window(createWindow(width, height, "VEngine")) { setWindowIcon("assets/icons/icon64x64.png"); }
            ~Window();

            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;
            Window(Window&&) = delete;
            Window& operator=(Window&&) = delete;

            void createWindowSurface(const VkInstance& instance, VkSurfaceKHR* surface) const;
            static void setFullscreen(bool fullscreen, uint16_t width, uint16_t height);
            [[nodiscard]] bool wasWindowResized() const { return m_frameBufferResized; }
            void resetWindowResizedFlag() { m_frameBufferResized = false; }
            [[nodiscard]] bool shouldClose() const;
            [[nodiscard]] bool isKeyPressed(int key) const;
            static void pollEvents();
            static void waitEvents();

            [[nodiscard]] VkExtent2D getExtent() const;
            [[nodiscard]] GLFWwindow* getGLFWWindow() const { return m_window; }
            void getFrameBufferSize(int& width, int& height) const;
            [[nodiscard]] static const char **getRequiredInstanceExtensions(uint32_t *count);

        private:

            [[nodiscard]] GLFWwindow* createWindow(uint16_t width, uint16_t height, const std::string& title);
            void setWindowIcon(const std::string& path) const;
            static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);

            GLFWwindow* m_window = nullptr;
            bool m_frameBufferResized = false;

    }; // class Window

} // namespace ven
