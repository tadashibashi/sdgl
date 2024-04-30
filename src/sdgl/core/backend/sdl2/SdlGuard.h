#pragma once

namespace sdgl {
    class SdlGuard {
    public:
        SdlGuard();
        ~SdlGuard();

        [[nodiscard]]
        static bool wasInit();
    private:
        bool m_wasInit;
    };
}