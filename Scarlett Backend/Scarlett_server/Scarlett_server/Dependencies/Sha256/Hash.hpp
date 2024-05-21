#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

class SHA256 {
private:
    static const uint32_t k[64];
    static const uint32_t initialHashValues[8];

    std::vector<uint8_t> data;
    uint32_t h[8];
    uint64_t nBits;

    static uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (~x & z);
    }

    static uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    static uint32_t ROTR(uint32_t x, int n) {
        return (x >> n) | (x << (32 - n));
    }

    static uint32_t Sigma0(uint32_t x) {
        return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
    }

    static uint32_t Sigma1(uint32_t x) {
        return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
    }

    static uint32_t sigma0(uint32_t x) {
        return ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3);
    }

    static uint32_t sigma1(uint32_t x) {
        return ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10);
    }

    void processBlock(const uint8_t* block) {
        uint32_t w[64];
        uint32_t a, b, c, d, e, f, g, h_temp, T1, T2;

        for (int i = 0, j = 0; i < 16; i++, j += 4) {
            w[i] = (block[j] << 24) | (block[j + 1] << 16) | (block[j + 2] << 8) | (block[j + 3]);
        }

        for (int i = 16; i < 64; i++) {
            w[i] = sigma1(w[i - 2]) + w[i - 7] + sigma0(w[i - 15]) + w[i - 16];
        }

        a = h[0];
        b = h[1];
        c = h[2];
        d = h[3];
        e = h[4];
        f = h[5];
        g = h[6];
        h_temp = h[7];

        for (int i = 0; i < 64; i++) {
            T1 = h_temp + Sigma1(e) + Ch(e, f, g) + k[i] + w[i];
            T2 = Sigma0(a) + Maj(a, b, c);
            h_temp = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += h_temp;
    }

public:
    SHA256() {
        reset();
    }

    void reset() {
        h[0] = initialHashValues[0];
        h[1] = initialHashValues[1];
        h[2] = initialHashValues[2];
        h[3] = initialHashValues[3];
        h[4] = initialHashValues[4];
        h[5] = initialHashValues[5];
        h[6] = initialHashValues[6];
        h[7] = initialHashValues[7];
        nBits = 0;
        data.clear();
    }

    void update(const uint8_t* bytes, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            data.push_back(bytes[i]);
            nBits += 8;
            if (data.size() == 64) {
                processBlock(data.data());
                data.clear();
            }
        }
    }

    std::string final() {
        size_t totalSize = data.size();

        // Padding
        data.push_back(0x80);  // Append 1 bit and 7 zeros
        while (data.size() != 56) {
            data.push_back(0x00);
        }

        // Append original length
        uint64_t bits = nBits;
        for (int i = 7; i >= 0; --i) {
            data.push_back((bits >> (8 * i)) & 0xFF);
        }

        processBlock(data.data());

        std::stringstream ss;
        for (int i = 0; i < 8; ++i) {
            ss << std::hex << std::setw(8) << std::setfill('0') << h[i];
        }
        reset();  // Prepare for next hash
        return ss.str();
    }
};

const uint32_t SHA256::initialHashValues[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
const uint32_t SHA256::k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};