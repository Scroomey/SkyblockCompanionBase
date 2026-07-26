#include "skyblock/EliteContestProvider.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using json = nlohmann::json;

namespace
{
    class CurlRuntime
    {
    public:
        CurlRuntime()
        {
            initialized_ =
                curl_global_init(
                    CURL_GLOBAL_DEFAULT
                ) == CURLE_OK;
        }

        ~CurlRuntime()
        {
            if (initialized_)
            {
                curl_global_cleanup();
            }
        }

        [[nodiscard]] bool initialized() const
        {
            return initialized_;
        }

    private:
        bool initialized_ = false;
    };

    CurlRuntime& curlRuntime()
    {
        static CurlRuntime runtime;
        return runtime;
    }

    std::size_t writeCallback(
        char* contents,
        std::size_t size,
        std::size_t count,
        void* output
    )
    {
        const std::size_t totalBytes = size * count;

        auto* response =
            static_cast<std::string*>(output);

        response->append(
            contents,
            totalBytes
        );

        return totalBytes;
    }

    std::string httpGet(const std::string& url)
    {
        if (!curlRuntime().initialized())
        {
            throw std::runtime_error(
                "Could not initialize libcurl."
            );
        }

        CURL* curl = curl_easy_init();

        if (curl == nullptr)
        {
            throw std::runtime_error(
                "Could not create an HTTP request."
            );
        }

        std::string response;

        curl_easy_setopt(
            curl,
            CURLOPT_URL,
            url.c_str()
        );

        curl_easy_setopt(
            curl,
            CURLOPT_FOLLOWLOCATION,
            1L
        );

        curl_easy_setopt(
            curl,
            CURLOPT_USERAGENT,
            "SkyblockCompanion/0.3"
        );

        curl_easy_setopt(
            curl,
            CURLOPT_ACCEPT_ENCODING,
            ""
        );

        curl_easy_setopt(
            curl,
            CURLOPT_WRITEFUNCTION,
            writeCallback
        );

        curl_easy_setopt(
            curl,
            CURLOPT_WRITEDATA,
            &response
        );

        curl_easy_setopt(
            curl,
            CURLOPT_CONNECTTIMEOUT,
            10L
        );

        curl_easy_setopt(
            curl,
            CURLOPT_TIMEOUT,
            20L
        );

        const CURLcode requestResult =
            curl_easy_perform(curl);

        if (requestResult != CURLE_OK)
        {
            const std::string error =
                curl_easy_strerror(requestResult);

            curl_easy_cleanup(curl);

            throw std::runtime_error(
                "Request failed: " + error
            );
        }

        long statusCode = 0;

        curl_easy_getinfo(
            curl,
            CURLINFO_RESPONSE_CODE,
            &statusCode
        );

        curl_easy_cleanup(curl);

        if (
            statusCode < 200 ||
            statusCode >= 300
        )
        {
            throw std::runtime_error(
                "API returned HTTP " +
                std::to_string(statusCode)
            );
        }

        return response;
    }

    void addContest(
        std::vector<skyblock::JacobContest>& contests,
        const std::string& timestampText,
        const json& contestValue
    )
    {
        skyblock::JacobContest contest;

        try
        {
            contest.startTimestamp =
                std::stoll(timestampText);
        }
        catch (const std::exception&)
        {
            return;
        }

        if (contest.startTimestamp > 100000000000LL)
        {
            contest.startTimestamp /= 1000;
        }

        const json* crops = nullptr;

        if (contestValue.is_array())
        {
            crops = &contestValue;
        }
        else if (
            contestValue.is_object() &&
            contestValue.contains("crops") &&
            contestValue.at("crops").is_array()
        )
        {
            crops = &contestValue.at("crops");
        }

        if (crops == nullptr)
        {
            return;
        }

        for (const json& crop : *crops)
        {
            if (crop.is_string())
            {
                contest.crops.push_back(
                    crop.get<std::string>()
                );
            }
        }

        if (!contest.crops.empty())
        {
            contests.push_back(
                std::move(contest)
            );
        }
    }
}

namespace skyblock
{
    EliteContestProvider::EliteContestProvider(
        std::string endpoint
    )
        : endpoint_(std::move(endpoint))
    {
    }

    ContestFetchResult EliteContestProvider::fetchCurrentContests()
    {
        ContestFetchResult result;

        try
        {
            const json data =
                json::parse(
                    httpGet(endpoint_)
                );

            if (
                !data.contains("contests") ||
                !data.at("contests").is_object()
            )
            {
                throw std::runtime_error(
                    "The API response does not contain "
                    "a contests object."
                );
            }

            result.skyBlockYear =
                data.value("year", 0);

            result.reportedCount =
                data.value("count", 0);

            result.complete =
                data.value("complete", false);

            const json& contestObject =
                data.at("contests");

            for (
                auto entry = contestObject.begin();
                entry != contestObject.end();
                ++entry
            )
            {
                addContest(
                    result.contests,
                    entry.key(),
                    entry.value()
                );
            }

            std::sort(
                result.contests.begin(),
                result.contests.end(),
                [](
                    const JacobContest& first,
                    const JacobContest& second
                )
                {
                    return
                        first.startTimestamp <
                        second.startTimestamp;
                }
            );

            if (result.contests.empty())
            {
                throw std::runtime_error(
                    "The API returned no readable "
                    "contest entries."
                );
            }

            result.success = true;
        }
        catch (const json::parse_error&)
        {
            result.error =
                "The API returned invalid JSON.";
        }
        catch (const std::exception& error)
        {
            result.error = error.what();
        }

        return result;
    }
}
