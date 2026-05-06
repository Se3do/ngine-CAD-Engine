#pragma once

#include <atomic>
#include <cmath>
#include <cstdint>

namespace ngine {

using Real = double;
using EntityId = std::uint64_t;

struct Tolerance {
    static constexpr Real absolute = 1e-10;
    static constexpr Real relative = 1e-8;
};

[[nodiscard]] inline bool nearly_equal(Real a, Real b, Real eps = Tolerance::absolute) noexcept {
    return std::abs(a - b) <= eps;
}

[[nodiscard]] inline bool nearly_zero(Real a, Real eps = Tolerance::absolute) noexcept {
    return std::abs(a) <= eps;
}

class IdGenerator {
   public:
    [[nodiscard]] static EntityId next() noexcept {
        static std::atomic<EntityId> counter{0};
        return ++counter;
    }
};

}  // namespace ngine
