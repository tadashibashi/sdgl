#include "Circle.h"
#include "mathf.h"

namespace sdgl {
    float Circle::area() const { return radius * radius * mathf::Pi; }

    float Circle::circumference() const { return 2.f * mathf::Pi * radius; }
}