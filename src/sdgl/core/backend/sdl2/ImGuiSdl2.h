#pragma once
#include "../../IPlugin.h"

#include <SDL2/SDL.h>

namespace sdgl {
    class ImGuiSdl2 final :  public IPlugin, public IFramePlugin, public IRenderPlugin, public IEventPlugin {
    public:
        ImGuiSdl2(SDL_Window *window, SDL_GLContext context) : m_window(window), m_context(context) { }

        bool init() override;
        void processEvent(const void *event) override;
        void startFrame() override;
        void endFrame() override;
        void render() override;
        void shutdown() override;

    private:
        SDL_Window *m_window;
        SDL_GLContext m_context;
    };
}
