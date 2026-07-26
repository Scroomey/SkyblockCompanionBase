#include "skyblock/ContestFilter.h"

#include <algorithm>
#include <cctype>
#include <unordered_set>

namespace skyblock
{
    std::string normalizeCropName(const std::string& cropName)
    {
        std::string normalized;
        normalized.reserve(cropName.size());

        for (unsigned char character : cropName)
        {
            if (std::isalnum(character))
            {
                normalized.push_back(
                    static_cast<char>(
                        std::tolower(character)
                    )
                );
            }
        }

        return normalized;
    }

    bool ContestFilter::matches(const JacobContest& contest) const
    {
        if (selectedCrops.empty())
        {
            return true;
        }

        std::unordered_set<std::string> contestCrops;

        for (const std::string& crop : contest.crops)
        {
            contestCrops.insert(
                normalizeCropName(crop)
            );
        }

        if (matchMode == MatchMode::Any)
        {
            return std::any_of(
                selectedCrops.begin(),
                selectedCrops.end(),
                [&contestCrops](const std::string& selectedCrop)
                {
                    return contestCrops.contains(
                        normalizeCropName(selectedCrop)
                    );
                }
            );
        }

        return std::all_of(
            selectedCrops.begin(),
            selectedCrops.end(),
            [&contestCrops](const std::string& selectedCrop)
            {
                return contestCrops.contains(
                    normalizeCropName(selectedCrop)
                );
            }
        );
    }
}
