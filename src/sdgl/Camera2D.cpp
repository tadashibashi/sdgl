#include "Camera2D.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace sdgl {
    struct Camera2D::Impl
    {
        Impl() : scale(1, 1), position(0, 0), origin(.5f, .5f), rotation(0), ortho(),
            matrix(), invertedMatrix(), worldBounds(), isDirty()  { }

        Vector2 scale, position, origin;
        float rotation;

        glm::mat4 ortho;
        glm::mat4 matrix;
        glm::mat4 invertedMatrix;
        std::optional<FRectangle> worldBounds;
        Rectangle viewport;

        bool isDirty;

        void updateMatrix()
        {
            if (!isDirty) return;

            matrix =
                ortho *
                glm::translate(glm::mat4(1.f), {origin.x * (float)viewport.w, origin.y * (float)viewport.h, 0}) *
                glm::rotate(glm::mat4(1.f), mathf::toRadians(rotation), {0.f, 0.f, 1.f}) *
                glm::scale(glm::mat4(1.f), {scale.x, scale.y, 1.f}) *
                glm::translate(glm::mat4(1.f), {-position.x, -position.y, 0});

            invertedMatrix = glm::inverse(matrix);

            isDirty = false;
            worldBounds.reset();
        }
    };

    Camera2D::Camera2D() : m(new Impl) { }
    Camera2D::~Camera2D()
    {
        delete m;
    }

    Rectangle Camera2D::getViewport() const
    {
        return m->viewport;
    }

    Camera2D &Camera2D::setViewport(Rectangle value)
    {
        if (value != m->viewport)
        {
            m->viewport = value;

            // This flips the y axis
            m->ortho = glm::ortho<float>(
                static_cast<float>(value.x),   ///< left
                static_cast<float>(value.w),   ///< right
                static_cast<float>(value.h),   ///< top
                static_cast<float>(value.y));  ///< bottom
            m->isDirty = true;
        }

        return *this;
    }

    Vector2 Camera2D::getOrigin(const bool normalized) const
    {
        return normalized ? m->origin : m->origin * m->viewport.size();
    }

    Camera2D &Camera2D::setOrigin(const Vector2 value, const bool normalized)
    {
        if (value != m->origin)
        {
            m->origin = normalized ? value : value / (Vector2)m->viewport.size();
            m->isDirty = true;
        }

        return *this;
    }

    Camera2D &Camera2D::setPosition(const Vector2 value)
    {
        if (value != m->position)
        {
            m->position = value;
            m->isDirty = true;
        }

        return *this;
    }

    const float *Camera2D::getMatrix() const
    {
        m->updateMatrix();
        return &m->matrix[0][0];
    }

    Vector2 Camera2D::viewToWorld(const Vector2 viewCoords) const
    {
        m->updateMatrix();
        const auto screenCoords = Vector2{
            (viewCoords.x + (float)m->viewport.x) / (float)m->viewport.w * 2.f - 1.f,
            (1.f - (viewCoords.y + (float)m->viewport.y) / (float)m->viewport.h) * 2.f - 1.f,
        };

        auto worldCoords = m->invertedMatrix * glm::vec4{screenCoords.x, screenCoords.y, 0, 1};
        return {worldCoords.x, worldCoords.y};
    }

    Vector2 Camera2D::worldToView(const Vector2 worldCoords) const
    {
        m->updateMatrix();
        auto screenCoords = m->matrix * glm::vec4{worldCoords.x, worldCoords.y, 0, 1};

        return { // view coordinates
            (screenCoords.x + 1.f) * .5f * (float)m->viewport.w - (float)m->viewport.x,
            (1.f - (screenCoords.y + 1.f) * .5f) * (float)m->viewport.h - (float)m->viewport.y
        };
    }

    FRectangle Camera2D::getWorldBounds() const
    {
        if (!m->worldBounds)
        {
            auto worldPos = viewToWorld(Vector2::Zero);
            auto worldSize = viewToWorld({(float)m->viewport.w, (float)m->viewport.h});;
            m->worldBounds = FRectangle(worldPos.x, worldPos.y, worldSize.x - worldPos.x, worldSize.y - worldPos.y);
        }

        return m->worldBounds.value();
    }

    Vector2 Camera2D::getPosition() const
    {
        return m->position;
    }

    Camera2D &Camera2D::setScale(const Vector2 value)
    {
        if (value != m->scale)
        {
            m->scale = value;
            m->isDirty = true;
        }

        return *this;
    }

    Vector2 Camera2D::getScale() const
    {
        return m->scale;
    }


    Camera2D &Camera2D::setRotation(const float value)
    {
        if (value != m->rotation)
        {
            m->rotation = mathf::wrap(value, 0, 360.f);
            m->isDirty = true;
        }

        return *this;
    }

    float Camera2D::getRotation() const
    {
        return m->rotation;
    }




}
