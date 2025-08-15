#include "crypto/md5.hpp"
#include <optional>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>

namespace crypto
{
    using namespace CryptoPP;
    std::optional<std::string> md5_digest(const std::span<const uint8_t> data)
    {
        std::array<CryptoPP::byte, CryptoPP::Weak::MD5::DIGESTSIZE> digest {};
        Weak::MD5 hash;
        hash.CalculateDigest(&digest.front(), data.data(), data.size());
        HexEncoder encoder { nullptr, false };
        encoder.Put(digest.data(), digest.size());
        encoder.MessageEnd();

        const auto size { encoder.MaxRetrievable() };
        if (size != 0U)
        {
            std::string encoded {};
            encoded.resize(encoder.MaxRetrievable());
            encoder.Get(reinterpret_cast<byte*>(&encoded.front()), encoded.size());
            return std::make_optional(encoded);
        }
        return std::nullopt;
    }
} // namespace crypto
