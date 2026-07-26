#pragma once

#include "skyblock/IContestProvider.h"

#include <string>

namespace skyblock
{
    class EliteContestProvider final : public IContestProvider
    {
    public:
        explicit EliteContestProvider(
            std::string endpoint =
                "https://api.elitebot.dev/contests/at/now"
        );

        [[nodiscard]] ContestFetchResult fetchCurrentContests() override;

    private:
        std::string endpoint_;
    };
}
