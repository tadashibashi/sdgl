#pragma once

namespace sdgl::ecs {
    /**
     * System for a component
     */
    class IComponentSystem {
    public:
        virtual ~IComponentSystem() = default;
        virtual bool init() = 0;
        virtual void shutdown() = 0;
        virtual int initPriority() { return 0; }
    };

    class IFrameSystem {
    public:
        virtual ~IFrameSystem() = default;
        virtual void startFrame() = 0;
        virtual void endFrame() = 0;
        /** Higher occurs earlier, lower occurs later */
        virtual int framePriority() { return 0; }
    };

    class IRenderSystem {
    public:
        virtual ~IRenderSystem() = default;
        virtual void render() = 0;
        virtual int renderPriority() { return 0; }
    };
}