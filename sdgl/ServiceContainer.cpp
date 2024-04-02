#include "ServiceContainer.h"

namespace sdgl {
    void *ServiceContainer::getService(std::type_index typeIndex) const
    {
        auto it = m_container.find(typeIndex);
        return it == m_container.end() ? nullptr : it->second;
    }
}
