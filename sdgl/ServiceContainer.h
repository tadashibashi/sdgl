#pragma once

#include <sdgl/sdglib.h>
#include <typeindex>

namespace sdgl {
    class ServiceContainer {
    public:
        ServiceContainer() : m_container() {}

        template <typename T>
        ServiceContainer &provide(T *service)
        {
            m_container[std::type_index(typeid(T))] = service;
            return *this;
        }

        template <typename T>
        [[nodiscard]]
        T *getService() const
        {
            return static_cast<T *>(getService(std::type_index(typeid(T))));
        }


    private:
        [[nodiscard]]
        void *getService(std::type_index typeIndex) const;

    private:
        map<std::type_index, void *> m_container;
    };
}
