#include "Window.h"

namespace sdgl {
    void Window::startFrame()
    {
        m_plugins.startFrame();
    }

    void Window::endFrame()
    {
        m_plugins.endFrame();
    }

    void Window::render()
    {
        makeCurrent();
        m_plugins.render();
    }
}
