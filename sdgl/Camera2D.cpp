#include "Camera2D.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>


namespace sdgl {
    struct Camera2D::Impl
    {
        Impl() : scale(1, 1), position(0, 0), origin(.5f, .5f), rotation(0),
            matrix(), invertedMatrix(), worldBounds(), isDirty()  { }

        Vector2 scale, position, origin;
        float rotation;

        glm::mat4 matrix;
        glm::mat4 invertedMatrix;
        std::optional<FRectangle> worldBounds;
        std::shared_ptr<Rectangle> viewport;

        bool isDirty;

        void updateMatrix()
        {
            if (!isDirty) return;
            auto temp = glm::mat4x4(1.0f);
            temp = glm::translate(temp, {-position.x, -position.y, 0});
            temp = glm::scale(temp, {scale.x, scale.y, 1.f});
            temp = glm::rotate(temp, mathf::toRadians(rotation), {0, 0, 1.f});

            if (viewport)
                temp = glm::translate(temp, {origin.x * (float)viewport->w, origin.y * (float)viewport->h, 0});

            matrix = temp;
            invertedMatrix = glm::inverse(temp);

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
        return m->viewport ? *m->viewport : Rectangle(); // return a "null" rectangle, if no viewport attached
    }

    Camera2D & Camera2D::setViewport(const std::shared_ptr<Rectangle> &value)
    {
        if (value != m->viewport)
        {
            m->viewport = value;
            m->isDirty = true;
        }

        return *this;
    }

    Vector2 Camera2D::getOrigin() const
    {
        return m->origin;
    }

    Camera2D & Camera2D::setOrigin(Vector2 value)
    {
        if (value != m->origin)
        {
            m->origin = value;
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

    Vector2 Camera2D::viewToWorld(const Vector2 viewPosition) const
    {
        m->updateMatrix();
        auto v = glm::vec4{viewPosition.x, viewPosition.y, 0, 1} * m->matrix;
        return {v.x, v.y};
    }

    Vector2 Camera2D::worldToView(const Vector2 worldPosition) const
    {
        m->updateMatrix();
        auto v = glm::vec4{worldPosition.x, worldPosition.y, 0, 1} * m->invertedMatrix;
        return {v.x, v.y};
    }

    FRectangle Camera2D::getWorldBounds() const
    {
        if (!m->worldBounds)
        {
            auto worldPos = viewToWorld(Vector2::Zero);
            auto worldSize = m->viewport ? viewToWorld({(float)m->viewport->w, (float)m->viewport->h}) : Vector2::Zero;
            m->worldBounds = FRectangle(worldPos.x, worldPos.y, worldSize.x, worldSize.y);
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
            m->rotation = value;
            m->isDirty = true;
        }

        return *this;
    }

    float Camera2D::getRotation() const
    {
        return m->rotation;
    }




}
