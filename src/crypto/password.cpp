#include "crypto/password.hpp"
#include <cstdint>
#include <span>
#include <stdexcept>

#include "crypto/md5.hpp"
#include <chrono>

namespace crypto
{
    password::password(const std::string_view pass_str)
        : mHash(this->hash_password(pass_str))
    {

    }

    std::string password::hash_password(const std::string_view pass_str) const
    {
        const std::string pass{std::string(pass_str) + mSalt};
        const std::span<const uint8_t> pass_span{reinterpret_cast<const uint8_t*>(pass.data()), pass.size()};
        const auto val{crypto::md5_digest(pass_span)};
        if (!val)
        {
            throw std::runtime_error("Could not hash password");
        }
        return *val;
    }

    std::string password::generate_salt() const
    {
        const auto now{std::chrono::steady_clock::now()};
        return std::to_string(now.time_since_epoch().count());
    }
}
