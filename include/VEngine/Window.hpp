/*
** @file Window.hpp
** @brief This file contains the Window class
** @namespace ven
*/

#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace ven {

    class Window {

        public:

            Window(int width, int height, const std::string &title) : m_window(createWindow(width, height, title)) {};
            ~Window() = default;

            static GLFWwindow* createWindow(int width, int height, const std::string &title);
            void destroyWindow();

            [[nodiscard]] GLFWwindow* getGLFWindow() const { return m_window; };

        private:

            GLFWwindow* m_window{nullptr};

    }; // class Window

} // namespace ven
