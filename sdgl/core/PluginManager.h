#pragma once
#include <ranges>

#include "IPlugin.h"
#include "../sdglib.h"

namespace sdgl {
    /**
     * Manager of plugins (and a plugin itself!)
     */
    class PluginManager : public IPlugin, public IFramePlugin, public IRenderPlugin, public IEventPlugin {
    public:
        PluginManager() = default;

        /**
         * @param plugins list of plugins to add during initialization
         */
        explicit PluginManager(const vector<IPlugin *> &plugins);

        /**
         * Call this once all desired plugins have been added to initialize each in order.
         * Can only be called once, and will simply return true otherwise.
         * But, if PluginManager::shutdown is called, this function can be called again.
         */
        bool init() override;

        /**
         * Add a plugin. Hands off ownership to this manager.
         * IPlugin::init will automatically be called if this manager was already initailized.
         * @param plug plugin to add
         */
        void addPlugin(IPlugin *plug);

        /**
         * Add a list of plugins. Hands off ownership to this manager.
         * IPlugin::init will automatically be called if this manager was already initailized.
         * @param plugs plugins to add
         */
        void addPlugins(const vector<IPlugin *> &plugs);

        /**
         * Remove plugins by class type, calling shutdown on all that inherit from it.
         * (This also deletes the matching plugins).
         */
        template <typename T> requires std::is_base_of_v<IPlugin, T>
        void removePlugin()
        {
            erasePlugins(std::ranges::remove_if(m_plugs, [](IPlugin *plugin) {
                return dynamic_cast<T *>(plugin);
            }));
        }

        /**
         * Remove plugin from the manager (this also deletes it).
         * @param plugin plugin to remove and delete
         */
        void removePlugin(IPlugin *plugin);

        void startFrame() override;
        void endFrame() override;
        void render() override;

        void processEvent(const void *event) override;

        /**
         * Clean up and shutdown all plugins
         */
        void shutdown() override;


        [[nodiscard]]
        size_t size() const { return m_plugs.size(); }
        [[nodiscard]]
        bool empty() const { return m_plugs.empty(); }
    private:
        /** the "erase" part of the remove-erase idiom */
        void erasePlugins(vector<IPlugin *>::iterator begin);

        vector<IPlugin *> m_plugs{};
        vector<IFramePlugin *> m_framePlugs{};
        vector<IRenderPlugin *> m_renderPlugs{};
        vector<IEventPlugin *> m_eventPlugs{};
        bool m_wasInit{};
    };
}
