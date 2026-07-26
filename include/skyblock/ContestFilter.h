#pragma once

#include "skyblock/JacobContest.h"

#include <string>
#include <vector>

namespace skyblock
{
    enum class MatchMode
    {
        Any,
        All
    };

    struct ContestFilter
    {
        std::vector<std::string> selectedCrops;
        MatchMode matchMode = MatchMode::Any;

        [[nodiscard]] bool matches(const JacobContest& contest) const;
    };

    [[nodiscard]] std::string normalizeCropName(const std::string& cropName);
}
