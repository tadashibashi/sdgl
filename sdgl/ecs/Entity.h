#pragma once
#include <entt/entt.hpp>
#include <sdgl/sdglib.h>

namespace sdgl::ecs {

    /// Thin wrapper around an entt::entity
    class Entity
    {
    public:
        Entity(entt::entity id, entt::registry &registry) : m_id(id), m_registry(registry)
        {

        }

        /// Add a component to the entity
        /// @tparam T type of component to add (required)
        /// @tparam TArgs types of the args to be passed to the constructor (inferred by the compiler)
        /// @param args the arguments used to construct the component, may be omitted if default constructor available
        /// @returns a reference to this entity for convenient chaining
        template <typename T, typename ...TArgs>
        Entity &add(TArgs&&...args)
        {
            m_registry.emplace<T, TArgs...>(m_id, std::forward<TArgs>(args)...);
            return *this;
        }

        /// Get a series of components from the Entity
        /// @tparam T a list of component types to query; component pointers will
        ///           appear in the returned tuple in this list's order
        /// @returns a tuple containing the components as pointers; if the entity
        ///          does not hold a particular component, it will be null
        template <typename ...T>
        decltype(auto) get()
        {
            return m_registry.try_get<T...>(m_id);
        }

        /// Get a series of components from the entity
        /// @tparam T a list of component types to query; component pointers will
        ///           appear in the returned tuple in this list's order
        /// @returns a tuple containing the components as pointers; if the entity
        ///          does not hold a particular component, it will be null
        template <typename ...T>
        decltype(auto) get() const
        {
            return m_registry.try_get<T...>(m_id);
        }

        template <typename T>
        Entity &remove()
        {
            m_registry.erase<T>(m_id);
            return *this;
        }

        [[nodiscard]]
        entt::entity id() const { return m_id; }

        [[nodiscard]]
        bool isValid() const { return m_registry.valid(m_id); }
    private:
        entt::entity m_id;
        entt::registry &m_registry;
    };

}

