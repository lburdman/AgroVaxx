#ifndef TAG_DEDUP_FILTER_H
#define TAG_DEDUP_FILTER_H

#include "Interfaces.h"
#include <cstdint>
#include <string>

class TagDedupFilter {
private:
  ITimeProvider *timeProvider;
  std::string lastTag;
  uint64_t lastReadTime;
  uint64_t timeoutMs;

public:
  TagDedupFilter(ITimeProvider *tProvider, uint64_t timeout_ms = 3000)
      : timeProvider(tProvider), lastTag(""), lastReadTime(0),
        timeoutMs(timeout_ms) {}

  void reset();

  // Returns true if this is considered a "new" tag event that should be
  // processed.
  bool processTag(const std::string &tag);
};

#endif // TAG_DEDUP_FILTER_H
