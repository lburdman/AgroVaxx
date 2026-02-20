#include "TagDedupFilter.h"

void TagDedupFilter::reset() {
  lastTag = "";
  lastReadTime = 0;
}

bool TagDedupFilter::processTag(const std::string &tag) {
  if (tag.empty())
    return false;

  uint64_t now = timeProvider->getTicksMs();

  if (tag != lastTag || (now - lastReadTime) >= timeoutMs) {
    lastTag = tag;
    lastReadTime = now;
    return true;
  }

  // Update the read time so the timeout is from the LAST time the tag was seen,
  // essentially latching it as long as it's present.
  lastReadTime = now;
  return false;
}
