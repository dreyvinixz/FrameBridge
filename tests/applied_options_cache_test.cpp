// Platform-independent smoke test; build with:
//   c++ -std=c++20 -Wall -Wextra -Werror tests/applied_options_cache_test.cpp -o cache_test
#include "../OptiScaler/runtime/AppliedOptionsCache.h"

#include <cassert>
#include <tuple>
#include <utility>

int main()
{
    using DlssgKey = std::tuple<int, unsigned int, int, float>;
    framebridge::AppliedOptionsCache<DlssgKey> dlssg;

    const DlssgKey initial { 1, 1, 0, 0.0f };
    const DlssgKey moreFrames { 1, 3, 0, 0.0f };
    const DlssgKey dynamic { 2, 3, 0, 120.0f };

    assert(dlssg.needs_update(initial));
    dlssg.mark_applied(initial);
    assert(!dlssg.needs_update(initial));
    assert(dlssg.needs_update(moreFrames));
    // Failure: do not call mark_applied; the same candidate must be retried.
    assert(dlssg.needs_update(moreFrames));
    dlssg.mark_applied(moreFrames);
    assert(!dlssg.needs_update(moreFrames));
    assert(dlssg.needs_update(dynamic));
    dlssg.mark_applied(dynamic);
    dlssg.invalidate(); // A new swapchain or reactivation must reapply options.
    assert(dlssg.needs_update(dynamic));

    framebridge::AppliedOptionsCache<std::pair<int, bool>> reflex;
    assert(reflex.needs_update({ 1, false }));
    reflex.mark_applied({ 1, false });
    assert(!reflex.needs_update({ 1, false }));
    assert(reflex.needs_update({ 1, true })); // Marker availability changed.
    reflex.mark_applied({ 1, true });
    assert(!reflex.needs_update({ 1, true }));
    reflex.invalidate();
    assert(reflex.needs_update({ 1, true }));

    return 0;
}
