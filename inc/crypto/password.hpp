#pragma once

#include <string>

namespace crypto
{
    class password
    {
    public:
        password(const std::string_view pass_str);

        inline const std::string& hash() const
        {
            return mHash;
        }

        inline const std::string& salt() const
        {
            return mSalt;
        }

    private:
        std::string hash_password(const std::string_view pass_str) const;
        std::string generate_salt() const;

        std::string mSalt{this->generate_salt()};
        std::string mHash{};
    };
}
