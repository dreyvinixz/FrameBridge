#pragma once

#include <optional>

namespace framebridge
{
template <typename T> class AppliedOptionsCache
{
  public:
    // The caller records an option set only after the external API reports success.
    bool needs_update(const T& value) const { return !_applied.has_value() || _applied.value() != value; }

    void mark_applied(const T& value) { _applied = value; }

    // Reinitialization, deactivation, or a failed API call invalidates the cache.
    void invalidate() { _applied.reset(); }

  private:
    std::optional<T> _applied;
};
} // namespace framebridge
