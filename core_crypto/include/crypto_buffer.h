#ifndef EMBEDDED_SECURITY_CRYPTO_BUFFER_H
#define EMBEDDED_SECURITY_CRYPTO_BUFFER_H

#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <algorithm>

namespace security {
    class CryptoBuffer {
    public:
        //Constructors
        CryptoBuffer() = default;
        explicit CryptoBuffer(std::size_t size, uint8_t initial_value = 0);
        CryptoBuffer(const uint8_t* data, std::size_t size);

        //Destructor
        ~CryptoBuffer();

        //Move Semantics
        CryptoBuffer(const CryptoBuffer& other) = default;
        CryptoBuffer& operator=(const CryptoBuffer& other) = default;

        CryptoBuffer(CryptoBuffer&& other) noexcept;
        CryptoBuffer& operator=(CryptoBuffer&& other) noexcept;


        void xor_transform(uint8_t key);
        void wipe();

        [[nodiscard]] size_t size() const noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] const std::vector<uint8_t>& data() const noexcept;

        [[nodiscard]] uint8_t at(size_t index) const;

    private:
        std::vector<uint8_t> m_buffer;
    };
}

#endif //EMBEDDED_SECURITY_CRYPTO_BUFFER_H
