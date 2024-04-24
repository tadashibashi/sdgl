#pragma once

namespace sdgl {
    /// A plugin that can be used by a Window or App
    class IPlugin {
    public:
        virtual ~IPlugin() = default;
        virtual bool init() = 0;
        virtual void shutdown() = 0;
        /** Higher occurs earlier, lower occurs later */
        [[nodiscard]]
        virtual int initPriority() const { return 0; }
    };

    class IFramePlugin {
    public:
        virtual ~IFramePlugin() = default;
        virtual void startFrame() = 0;
        virtual void endFrame() = 0;
        /** Higher occurs earlier, lower occurs later */
        [[nodiscard]]
        virtual int framePriority() const { return 0; }
    };

    class IRenderPlugin {
    public:
        virtual ~IRenderPlugin() = default;
        virtual void render() = 0;
        /** Higher occurs earlier, lower occurs later */
        [[nodiscard]]
        virtual int renderPriority() const { return 0; }
    };

    class IEventPlugin {
    public:
        virtual ~IEventPlugin() = default;
        virtual void processEvent(const void *event) = 0;
        /** Higher occurs earlier, lower occurs later */
        [[nodiscard]]
        virtual int eventPriority() const { return 0; }
    };
}
