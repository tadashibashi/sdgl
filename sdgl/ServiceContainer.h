#pragma once

#include <sdgl/sdglib.h>
#include <typeindex>

namespace sdgl {
    class ServiceContainer {
    public:
        ServiceContainer() : m_services() {}

        /// Provide a service to the container
        /// @param service service to provide
        /// @returns reference to this container for convenient chaining
        template <typename T>
        ServiceContainer &provide(T *service)
        {
            m_services[std::type_index(typeid(T))] = service;
            return *this;
        }

        /// Get a service from the container
        /// @returns service pointer of type `T`, or `nullptr` if it does not own one of this type yet.
        template <typename T>
        [[nodiscard]]
        T *getService() const
        {
            return static_cast<T *>(getService(std::type_index(typeid(T))));
        }

        /// Remove a service from the container
        /// @returns whether removal was successful (where the container owned a service of
        ///          type `T` and removed it from the container)
        template <typename T>
        bool remove()
        {
            auto it = m_services.find(typeid(T));
            if (it != m_services.end())
            {
                m_services.erase(it);
                return true;
            }

            return false;
        }

        [[nodiscard]]
        auto size() const { return m_services.size(); }

        [[nodiscard]]
        auto empty() const { return m_services.empty(); }
    private:
        [[nodiscard]]
        void *getService(std::type_index typeIndex) const;

        map<std::type_index, void *> m_services;
    };
}
