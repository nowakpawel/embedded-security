#include "crypto_buffer.h"

namespace security {
    CryptoBuffer::CryptoBuffer(std::size_t size, uint8_t initial_value)
        : m_buffer(size, initial_value) {}

    CryptoBuffer::CryptoBuffer(const uint8_t* data, std::size_t size) {
        if (data != nullptr && size > 0) {
            m_buffer.assign(data, data + size);
        }
    }

    CryptoBuffer::~CryptoBuffer() {
        wipe();
    }

    CryptoBuffer::CryptoBuffer(CryptoBuffer&& other) noexcept
        : m_buffer(std::move(other.m_buffer)) {}

    CryptoBuffer& CryptoBuffer::operator=(CryptoBuffer&& other) noexcept {
        if (this != &other) {
            wipe(); // clear any sensitive data before taking new ownership
            m_buffer = std::move(other.m_buffer);
        }

        return *this;
    }

    void CryptoBuffer::xor_transform(uint8_t key) {
        std::transform(m_buffer.begin(), m_buffer.end(),
            m_buffer.begin(), [key](uint8_t byte) {return byte ^ key;});
    }

    void CryptoBuffer::wipe() {
        //prevent RAM scraping
        std::fill(m_buffer.begin(), m_buffer.end(), 0);
        m_buffer.clear();
    }

    std::size_t CryptoBuffer::size() const noexcept {
        return m_buffer.size();
    }

    bool CryptoBuffer::empty() const noexcept {
        return m_buffer.empty();
    }

    const std::vector<uint8_t>& CryptoBuffer::data() const noexcept {
        return m_buffer;
    }

    uint8_t CryptoBuffer::at(size_t index) const {
        return m_buffer.at(index);
    }
}