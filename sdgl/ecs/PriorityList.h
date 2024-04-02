#pragma once
#include <sdgl/sdglib.h>
#include <sdgl/logging.h>
#include <algorithm>

namespace sdgl {
    template <typename T> requires std::equality_comparable<T> && std::is_copy_constructible_v<T>
    class PriorityList
    {
    public:
        explicit PriorityList(func<float(const T &)> getPriority, func<void(T &)> onDelete, const vector<T> &objects = {}) :
            m_v(objects), m_getPriority(std::move(getPriority)), m_onErase(onDelete)
        {
            SDGL_ASSERT(getPriority);
            if (m_v.size() > 1)
                sort();
        }

        void sort()
        {
            auto &getPriority = m_getPriority;
            std::sort(m_v.begin(), m_v.end(), [&getPriority](const T &a, const T &b) {
                return getPriority(a) > getPriority(b);
            });
        }

        void add(T &obj)
        {
            auto &getPriority = m_getPriority;
            auto it = std::lower_bound(m_v.rbegin(), m_v.rend(), obj, [&getPriority](const T &a, const T &b) {
                return getPriority(a) < getPriority(b);
            });

            if (it == m_v.rbegin())
            {
                m_v.emplace_back(obj);
            }
            else
            {
                m_v.insert((it).base(), obj);
            }
        }

        void add(const vector<T> &objs)
        {
            if (objs.empty()) return;

            for (auto &obj : objs)
            {
                add(obj, false);
            }

            sort();
        }

        /** Returns if item was erased */
        bool eraseItem(const T &obj)
        {
            auto it = std::find(m_v.begin(), m_v.end(), obj);
            if (it != m_v.end())
            {
                if (m_onErase)
                    m_onErase(*it);
                m_v.erase(it);
                return true;
            }

            return false;
        }

        /** Returns if anything in the container was erased */
        bool eraseIf(const func<bool(const T &)> &pred)
        {
            return eraseUntilEnd(std::ranges::remove_if(m_v, pred));
        }

        auto begin() const { return m_v.begin(); }
        auto end() const { return m_v.end(); }

        auto &at(int index) { return m_v.at(index); }
        const auto &at(int index) const { return m_v.at(index); }

        [[nodiscard]]
        auto &operator[](int index) { return m_v[index]; }
        [[nodiscard]]
        const auto &operator[](int index) const { return m_v[index]; }

        auto size() const { return m_v.size(); }
        auto empty() const { return m_v.empty(); }
    private:

        /** Returns true if anyting was erased */
        bool eraseUntilEnd(const typename vector<T>::iterator from)
        {
            if (from == end()) return false;
            auto startSize = m_v.size();

            for (auto it = from; it != end(); ++it)
            {
                if (m_onErase)
                    m_onErase(*it);
            }

            m_v.erase(from, m_v.end());
            return true;
        }

        vector<T> m_v;
        func<float(const T &)> m_getPriority;
        func<void(T &)> m_onErase;
    };
}
