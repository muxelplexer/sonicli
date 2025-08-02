#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>


namespace crypto
{
    std::optional<std::string> md5_digest(const std::span<const uint8_t> data);
}
