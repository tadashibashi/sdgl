#include "Glfw3Backend.h"

#include "Glfw3.h"
#include "Glfw3Keyboard.h"
#include "Glfw3Mouse.h"

#include "Glfw3ImGuiPlugin.h"

#include <iostream>
#include <sdgl/gl.h>

#include "Glfw3Gamepad.h"
#include "Glfw3Window.h"
#include "sdgl/logging.h"

namespace sdgl::backends {

    Backend *createBackend()
    {
        return new glfw3::Glfw3Backend();
    }
}

namespace sdgl::backends::glfw3 {

    struct Glfw3Backend::Impl {
        vector<Glfw3Window *> windows{};
        vector<Glfw3Gamepad> gamepads{};
    };

    // ===== Callbacks =====

    static void errorCallback(int error, const char *message)
    {
        std::cerr << "GLFW error " << error << ": " << message << '\n';
    }

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    Glfw3Backend::Glfw3Backend() : m(new Impl)
    {

    }

    Glfw3Backend::~Glfw3Backend()
    {
        delete m;
    }

    bool Glfw3Backend::init()
    {
#if defined(SDGL_PLATFORM_APPLE) && !defined(SDGL_PLATFORM_EMSCRIPTEN)
        glfwInitHint(GLFW_ANGLE_PLATFORM_TYPE, GLFW_ANGLE_PLATFORM_TYPE_METAL);
#endif
        glfwSetErrorCallback(errorCallback);

        // gamepads
        m->gamepads.emplace_back().init(GLFW_JOYSTICK_1);
        m->gamepads.emplace_back().init(GLFW_JOYSTICK_2);
        m->gamepads.emplace_back().init(GLFW_JOYSTICK_3);
        m->gamepads.emplace_back().init(GLFW_JOYSTICK_4);

        return glfwInit();
    }

    double Glfw3Backend::getAppTime() const
    {
        return glfwGetTime();
    }

    Window *Glfw3Backend::createWindow(const string &title, int width, int height,
        const WindowFlags::Enum flags, const PluginConfig &plugins)
    {
        // OpenGL version hints (GLES3 for Angle)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#if !defined(SDGL_PLATFORM_EMSCRIPTEN)

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
#endif

        // Window hints from flags
#if !defined(SDGL_PLATFORM_EMSCRIPTEN)
        glfwWindowHint(GLFW_MAXIMIZED, (flags & WindowFlags::Fullscreen) > 0);
        glfwWindowHint(GLFW_FLOATING, (flags & WindowFlags::AlwaysOnTop) > 0);
        glfwWindowHint(GLFW_DECORATED, !(flags & WindowFlags::Borderless));
        glfwWindowHint(GLFW_RESIZABLE, (flags & WindowFlags::Resizable) > 0);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, (flags & WindowFlags::Transparent) > 0);
#endif

        // create the window, setup callbacks / rendering attributes
        const auto window = glfwCreateWindow(
            width, height, title.c_str(), nullptr, nullptr);
        if (!window) {
            return nullptr;
        }

        glfwMakeContextCurrent(window);

        // Create window wrapper, add plugins
        auto backendWindow = new Glfw3Window(window, m->gamepads, title);
        glfwSetWindowUserPointer(window, backendWindow);

        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

#ifndef SDGL_PLATFORM_EMSCRIPTEN
        glfwSwapInterval(1);
#endif
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // set window viewport
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // add plugins to window
        if (plugins.imgui)
            backendWindow->plugins()->addPlugin(new Glfw3ImGuiPlugin(window));
        for (const auto plugin : plugins.plugins)
        {
            backendWindow->plugins()->addPlugin(plugin);
        }

        if (!backendWindow->plugins()->init())
        {
            // a plugin failed to init, fail fast
            destroyWindow(backendWindow);
            return nullptr;
        }

        // success
        m->windows.emplace_back(backendWindow);
        return backendWindow;
    }

    void Glfw3Backend::destroyWindow(Window *window)
    {
        window->plugins()->shutdown();

        const auto glfwWindow = static_cast<GLFWwindow *>(window->getHandle());
        glfwDestroyWindow(glfwWindow);

        // remove window from windows container
        for (auto it = m->windows.begin(); it != m->windows.end(); ++it)
        {
            if (*it == window)
            {
                m->windows.erase(it);
                break;
            }
        }

        delete window;
    }

    void Glfw3Backend::processInput()
    {
        for (const auto window : m->windows)
        {
            window->preProcessInput();
        }

        glfwPollEvents();

        for (auto &gp : m->gamepads)
            gp.processInput();

        for (const auto window : m->windows)
        {
            window->processInput();
        }
    }

    void Glfw3Backend::shutdown()
    {
        for (const auto window : m->windows)
        {
            window->plugins()->shutdown();
            glfwDestroyWindow(window->m_window);
            delete window;
        }
        m->windows.clear();
        glfwTerminate();
    }
}
