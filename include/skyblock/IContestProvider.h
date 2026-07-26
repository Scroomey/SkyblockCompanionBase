#pragma once

#include "skyblock/JacobContest.h"

#include <string>
#include <vector>

namespace skyblock
{
    struct ContestFetchResult
    {
        bool success = false;
        std::vector<JacobContest> contests;
        int skyBlockYear = 0;
        int reportedCount = 0;
        bool complete = false;
        std::string error;
    };

    class IContestProvider
    {
    public:
        virtual ~IContestProvider() = default;

        [[nodiscard]] virtual ContestFetchResult fetchCurrentContests() = 0;
    };
}
