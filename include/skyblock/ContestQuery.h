#pragma once

#include "skyblock/ContestFilter.h"
#include "skyblock/JacobContest.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace skyblock
{
    class ContestQuery
    {
    public:
        [[nodiscard]] static std::vector<const JacobContest*> upcoming(
            const std::vector<JacobContest>& contests,
            std::int64_t now,
            const ContestFilter& filter,
            std::size_t offset = 0,
            std::size_t limit = 10
        );

        [[nodiscard]] static std::vector<const JacobContest*> allUpcoming(
            const std::vector<JacobContest>& contests,
            std::int64_t now,
            const ContestFilter& filter
        );

        [[nodiscard]] static const JacobContest* next(
            const std::vector<JacobContest>& contests,
            std::int64_t now,
            const ContestFilter& filter
        );
    };
}
