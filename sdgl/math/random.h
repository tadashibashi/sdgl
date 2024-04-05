#pragma once

namespace sdgl::mathf {
    /// Generate a random floating point number from 0 - 1 (not including 1)
    [[nodiscard]]
    float rand();

    /// Generate a random floating point number from 0 to `n` (not including `n`)
    /// @param n max of exlusive range (not including `n`)
    [[nodiscard]]
    float rand(float n);

    /// Generate a random floating point number from `low` to `high` (not including `high`)
    /// @param low minimum of range (inclusive)
    /// @param high max of range (exclusive)
    [[nodiscard]]
    float rand(float low, float high);

    /// Generate a random integer from 0 to `n`, not including `n`
    [[nodiscard]]
    int irand(int n);

    /// Generate a random integer from `low` to `high`, not including `high`
    [[nodiscard]]
    int irand(int low, int high);

    /// Perform a "coin toss", where the chances are `n` out of the second argument.
    /// @return whether coin landed on "heads"
    [[nodiscard]]
    bool chance(float n, float outof);

    /// Perform a "coin toss", where chances are a normalized percent (default: .5f => 50%)
    /// @return whether coin landed on "heads"
    [[nodiscard]]
    bool chance(float normPercent = .5f);
}
