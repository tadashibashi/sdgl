#pragma once

namespace sdgl {
    class SceneRunner;

    class Scene
    {
    public:
        Scene() = default;
        virtual ~Scene() = default;
    protected:

        virtual void init() = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render(float deltaTime) = 0;
        virtual void paused() = 0;
        virtual void resumed() = 0;
        virtual void shutdown() = 0;

        [[nodiscard]]
        SceneRunner *getManager() const { return m_manager; }
    private:
        friend class SceneRunner;
        SceneRunner *m_manager{};
    };

} // sdgl
