#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace skyblock
{
    struct JacobContest
    {
        std::int64_t startTimestamp = 0;
        std::vector<std::string> crops;

        [[nodiscard]] std::int64_t endTimestamp() const
        {
            return startTimestamp + 20 * 60;
        }

        [[nodiscard]] bool isActiveAt(std::int64_t timestamp) const
        {
            return
                timestamp >= startTimestamp &&
                timestamp < endTimestamp();
        }

        [[nodiscard]] bool hasEndedAt(std::int64_t timestamp) const
        {
            return timestamp >= endTimestamp();
        }
    };
}
