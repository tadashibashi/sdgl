#include "ServiceContainer.h"

namespace sdgl {
    void *ServiceContainer::getService(std::type_index typeIndex) const
    {
        auto it = m_services.find(typeIndex);
        return it == m_services.end() ? nullptr : it->second;
    }
}
