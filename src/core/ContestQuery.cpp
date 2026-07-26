#include "skyblock/ContestQuery.h"

namespace skyblock
{
    std::vector<const JacobContest*> ContestQuery::allUpcoming(
        const std::vector<JacobContest>& contests,
        std::int64_t now,
        const ContestFilter& filter
    )
    {
        std::vector<const JacobContest*> results;

        for (const JacobContest& contest : contests)
        {
            if (contest.hasEndedAt(now))
            {
                continue;
            }

            if (!filter.matches(contest))
            {
                continue;
            }

            results.push_back(&contest);
        }

        return results;
    }

    std::vector<const JacobContest*> ContestQuery::upcoming(
        const std::vector<JacobContest>& contests,
        std::int64_t now,
        const ContestFilter& filter,
        std::size_t offset,
        std::size_t limit
    )
    {
        std::vector<const JacobContest*> results;
        std::size_t matchedIndex = 0;

        for (const JacobContest& contest : contests)
        {
            if (contest.hasEndedAt(now))
            {
                continue;
            }

            if (!filter.matches(contest))
            {
                continue;
            }

            if (matchedIndex++ < offset)
            {
                continue;
            }

            results.push_back(&contest);

            if (results.size() >= limit)
            {
                break;
            }
        }

        return results;
    }

    const JacobContest* ContestQuery::next(
        const std::vector<JacobContest>& contests,
        std::int64_t now,
        const ContestFilter& filter
    )
    {
        for (const JacobContest& contest : contests)
        {
            if (
                !contest.hasEndedAt(now) &&
                filter.matches(contest)
            )
            {
                return &contest;
            }
        }

        return nullptr;
    }
}
