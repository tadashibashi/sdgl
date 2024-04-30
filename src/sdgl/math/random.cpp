#include <random>
#include "random.h"

float sdgl::mathf::rand()
{
    static std::random_device s_device;
    static std::mt19937 s_mt(s_device());
    static std::uniform_real_distribution<float> s_dist(0.0, 1.0);

    return s_dist(s_mt);
}

float sdgl::mathf::rand(const float n)
{
    return rand() * n;
}

float sdgl::mathf::rand(const float low, const float high)
{
    return rand() * (high - low) + low;
}

int sdgl::mathf::irand(const int n)
{
    return static_cast<int>(
      rand(static_cast<float>(n))
    );
}

int sdgl::mathf::irand(const int low, const int high)
{
    return irand(high - low) + low;
}

bool sdgl::mathf::chance(const float n, const float outof)
{
    return rand(outof) < n;
}

bool sdgl::mathf::chance(const float normPercent)
{
    return rand() < normPercent;
}


