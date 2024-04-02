#pragma once

namespace sdgl {
    /**
     * App-level plugin
     */
    class IPlugin {
    public:
        virtual ~IPlugin() = default;
        virtual bool init() = 0;
        virtual void shutdown() = 0;
        virtual int initPriority() { return 0; }
    };

    class IFramePlugin {
    public:
        virtual ~IFramePlugin() = default;
        virtual void startFrame() = 0;
        virtual void endFrame() = 0;
        /** Higher occurs earlier, lower occurs later */
        virtual int framePriority() { return 0; }
    };

    class IRenderPlugin {
    public:
        virtual ~IRenderPlugin() = default;
        virtual void render() = 0;
        virtual int renderPriority() { return 0; }
    };

    class IEventPlugin {
    public:
        virtual ~IEventPlugin() = default;
        virtual void processEvent(const void *event) = 0;
        virtual int eventPriority() { return 0; }
    };
}