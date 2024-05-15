#pragma once
#include <sdgl/assert.h>
#include <sdgl/sdglib.h>

#include <typeinfo>
#include <type_traits>

namespace sdgl {
    /// Common vertex types
    struct GLType
    {
        enum Enum
        {
            Byte = 0,
            Ubyte,
            Short,
            Ushort,
            Int,
            Uint,
            Float,
        };
    };

    /// Conveniently creates vertex attribute pointer info for shader programs
    class ShaderAttribs {
        struct Attribute
        {
            int count = 1;
            bool normalized = false;
            int type = 0;
            uint stride = 0;
            uint offset = 0;
        };

        /// Get the pointer offset of a member within a struct
        template <typename Member, typename T>
        static constexpr size_t offset(Member T::* p, T sample = T()) {
            // using a constructed sample may cause unintended side-effects in the ctor/dtor,
            // but for primatives should be ok
            return (uintptr_t)((void *)&(sample.*p)) - (uintptr_t)((void *)&sample);
        }

    public:
        /// Add an attribute from a vertex class
        template <typename T, typename Member> requires (!std::is_pointer_v<Member>)
        ShaderAttribs &attrib(Member T::*vertexMember, bool normalized = false)
        {
            size_t hash;
            int count;
            if (std::is_array_v<Member>)
            {
                using underlying = std::remove_all_extents_t<Member>;
                hash = typeid(underlying).hash_code();
                count = sizeof(Member) / sizeof(underlying);
            }
            else
            {
                hash = typeid(Member).hash_code();
                count = 1;
            }

            if (m_attribs.empty())
                m_sizeofVertex = sizeof(T);
            else
            {
                SDGL_ASSERT(m_sizeofVertex == sizeof(T));
            }

            if (!m_vertexTypeHash)
                m_vertexTypeHash = typeid(T).hash_code();
            return attrib(hash, count, normalized, sizeof(T), offset(vertexMember));
        }

        /// Add an attribute from a vertex class, pointer overload. Add the count manually.
        template <typename T, typename Member> requires std::is_pointer_v<Member>
        ShaderAttribs &attrib(Member T::*vertexMember, int count, bool normlized = false)
        {
            using underlying = decltype(*Member());
            const auto hash = typeid(underlying).hash_code();

            if (m_attribs.empty())
                m_sizeofVertex = sizeof(T);
            else
            {
                SDGL_ASSERT(m_sizeofVertex == sizeof(T));
            }

            if (!m_vertexTypeHash)
                m_vertexTypeHash = typeid(T).hash_code();
            return attrib(hash, count, normlized, sizeof(T), offset(vertexMember));
        }

        /// For custom packed vector classes e.g. glm::vec2, etc. Manually indicate types and count.
        template<typename T, typename Member> requires (!std::is_pointer_v<T> && !std::is_array_v<T>)
        ShaderAttribs &attrib(Member T::*member, const GLType::Enum type, const int count,
          const bool normalized = false)
        {
            if (!m_vertexTypeHash)
                m_vertexTypeHash = typeid(T).hash_code();

            if (m_attribs.empty())
                m_sizeofVertex = sizeof(T);
            else
            {
                SDGL_ASSERT(m_sizeofVertex == sizeof(T));
            }
            return attrib(type, count, normalized, sizeof(T), offset(member));
        }

        [[nodiscard]]
        auto begin() { return m_attribs.begin(); }
        [[nodiscard]]
        auto end() { return m_attribs.end(); }

        [[nodiscard]]
        auto begin() const { return m_attribs.cbegin(); }
        [[nodiscard]]
        auto end() const { return m_attribs.cend(); }

        [[nodiscard]]
        bool empty() const { return m_attribs.empty(); }
        [[nodiscard]]
        auto size() const { return m_attribs.size(); }

        /// The byte size of one vertex
        [[nodiscard]]
        auto sizeofVertex() const { return m_sizeofVertex; }
        [[nodiscard]]
        auto vertexTypeHash() const { return m_vertexTypeHash; }

    private:
        ShaderAttribs &attrib(size_t hash, int count, bool normalized, uint stride, uint offset);
        ShaderAttribs &attrib(GLType::Enum type, int count, bool normalized, uint stride, uint offset);
        vector<Attribute> m_attribs{};
        size_t m_sizeofVertex{0};
        size_t m_vertexTypeHash{0};
    };


}
