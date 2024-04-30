#pragma once
#include <algorithm>
#include <any>
#include <functional>
#include <vector>

#include "sdglib.h"

namespace sdgl
{
    namespace detail
    {
        template <typename ...Args>
        class CallbackComparer
        {
        public:
            template <typename T>
            CallbackComparer(T *object, void (T::*func)(Args...))
                : object(object), functionPtr(func), function(
                    [func, object](Args...args)
                    {
                        (object->*func)(args...);
                    })
            { }

            explicit CallbackComparer(void (*func)(Args...))
                : object(nullptr), functionPtr(func), function(func)
            { }

            void operator()(Args...args)
            {
                if (function)
                {
                    function(args...);
                }
            }

            template <typename T>
            [[nodiscard]]
            bool matches(T *object, std::any functionPtr)
            {
                return this->object == object && this->functionPtr.type() == functionPtr.type() &&
                    std::any_cast<void(T::*)(Args...)>(this->functionPtr) == std::any_cast<void(T::*)(Args...)>(functionPtr);
            }

            [[nodiscard]]
            bool matches(void(*func)(Args...))
            {
                return this->object == nullptr && this->functionPtr.type() == typeid(func) &&
                    std::any_cast<void(*)(Args...)>(this->functionPtr) == func;
            }

        private:
            void *object;
            std::any functionPtr;
            std::function<void(Args...)> function;
        };
    }


    /**
     * Used to construct callbacks that can be registered with `Delegate`
     */
    template<typename T, typename ...Args>
    struct Callback
    {
        Callback(T* object, void (T:: *function)(Args...)) : object(object), function(function) {}
        T *object;
        void (T:: *function)(Args...);
    };

    template <typename ...Args>
    class Delegate {
        struct CallbackData {
            explicit CallbackData(detail::CallbackComparer<Args...> callback) : callback(callback), shouldRemove() { }
            detail::CallbackComparer<Args...> callback;
            bool shouldRemove;
        };
    public:
        Delegate(): m_callbacks(), m_isDirty(false)
        {}

        [[nodiscard]]
        size_t size() const { return m_callbacks.size(); }

        [[nodiscard]]
        bool empty() const { return m_callbacks.empty(); }

        void clear() const { m_callbacks.clear(); }

        template <typename T>
        Delegate &operator+=(Callback<T, Args...> callback)
        {
            m_callbacks.emplace_back(CallbackData{CallbackComparer(callback.object, callback.function)});
            return *this;
        }

        Delegate &operator+=(void (*func)(Args...))
        {
            m_callbacks.emplace_back({
                .callback=CallbackComparer(func),
                .shouldRemove=false
            });

            return *this;
        }

        template <typename T>
        Delegate &operator-=(Callback<T, Args...> callback)
        {
            for (auto &data : m_callbacks)
            {
                if (data.callback.matches(callback.object, callback.function))
                {
                    data.shouldRemove = true;
                    m_isDirty = true;
                    break;
                }
            }

            return *this;
        }

        Delegate &operator-=(void (* func)(Args...))
        {
            for (auto &data : m_callbacks)
            {
                if (data.callback.matches(func))
                {
                    data.shouldRemove = true;
                    m_isDirty = true;
                    break;
                }
            }

            return *this;
        }

        void operator()(Args...args)
        {
            if (m_isDirty)
            {
                processRemovals();
                m_isDirty = false;
            }

            // Use predefined size limit to prevent calling callbacks added this frame
            for (auto i = 0, size = m_callbacks.size(); i < size; ++i)
            {
                m_callbacks[i](std::forward<Args>(args)...);
            }
        }
    private:
        
        void processRemovals()
        {
            m_callbacks.erase(std::remove_if(m_callbacks.begin(), m_callbacks.end(),
                [](const CallbackData &data) { return data.shouldRemove;}));
        }

        vector<CallbackData> m_callbacks;
        bool m_isDirty;
    };
}
