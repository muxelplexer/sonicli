#pragma once

#include <string>

namespace crypto
{
    class password
    {
    public:
        explicit password(const std::string_view pass_str);

        [[nodiscard]] const std::string& hash() const { return mHash; }
        [[nodiscard]] const std::string& salt() const { return mSalt; }

        [[nodiscard]] static std::string generate_salt();

    private:
        [[nodiscard]] std::string hash_password(const std::string_view pass_str) const;

        std::string mSalt { this->generate_salt() };
        std::string mHash;
    };
} // namespace crypto
