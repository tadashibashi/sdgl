#pragma once
#include "ISystem.h"
#include "PriorityList.h"

namespace sdgl::ecs {

    /// Simple RAII container that manages a list of ECS systems
    class SystemManager
    {
    public:
        SystemManager();
        ~SystemManager();

        /// Initialize all systems that were added. Should only be called once until `shutdown` is called.
        /// Any systems added after `init` will be automatically initialized.
        bool init();

        /// Shutdown and clean up all systems - call to shutdown happens in reverse order of initOrder for the sake
        /// of encapsulation scope.
        void shutdown();

        void update(double deltaTime);

        template <typename T, typename ...TArgs> requires std::is_base_of_v<ISystem, T>
        void add(TArgs &&...args)
        {
            // Create the system
            auto sys = new T(std::forward<TArgs>(args)...);

            // Add it
            m_systems.add(sys);
            m_updateList.add(sys);

            // Initialize the system during runtime (post `init`)
            if (m_wasInit)
                sys->init();
        }

        /// Erase all systems that inherit from type `T`. Calls shutdown on them, order is not defined.
        template<typename T>
        bool erase()
        {
            // Try erasing systems inheriting from type `T`
            if (m_systems.eraseIf([](const ISystem *sys) { return dynamic_cast<T *>(sys); }))
            {
                // Also remove it from the update list
                SDGL_ASSERT(m_updateList.eraseIf([](const ISystem *sys) { return dynamic_cast<T *>(sys); }),
                    "Update list should also remove system, if main list ");
                return true;
            }

            // Nothing was erased
            return false;
        }

    private:
        /// Master list of systems
        PriorityList<ISystem *> m_systems;
        /// List of systems in update order
        PriorityList<ISystem *> m_updateList;
        /// Flag set when init is called, and unset on shutdown
        bool m_wasInit;
    };
}
