#pragma once

#if SDGL_DEBUG
#include <cassert>

#define IMPL_SDGL_ASSERT_1(condition) (assert(condition))
#define IMPL_SDGL_ASSERT_2(condition, comment) (assert((condition) && comment))

#define IMPL_SDGL_GET_ASSERT_MACRO(_1, _2, NAME, ...) NAME

/// Assert a value (or noop in release mode)
/// @param condition boolean statement to assert - this should not include any code with runtime side-effects since it
///                  will not get called in release mode
/// @param message c-string describing assertion (optional)
#define SDGL_ASSERT(...) IMPL_SDGL_GET_ASSERT_MACRO(__VA_ARGS__, IMPL_SDGL_ASSERT_2, IMPL_SDGL_ASSERT_1)(__VA_ARGS__)
#else

#    ifdef _MSVER
#       define SDGL_ASSERT(...) __noop
#    else
#        define SDGL_ASSERT(...) do {} while(0)
#    endif
#endif