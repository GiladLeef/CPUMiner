#ifndef SHA2_HELPERS_H
#define SHA2_HELPERS_H

/* SHA256 round constants */
static const uint32_t sha256_h[8] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

static const uint32_t sha256_k[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static inline uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
	return z ^ (x & (y ^ z));
}

static inline uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) | (z & (x | y));
}

static inline uint32_t ROTR(uint32_t x, uint32_t n) {
	return (x >> n) | (x << (32 - n));
}

static inline uint32_t S0(uint32_t x) {
	return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
}

static inline uint32_t S1(uint32_t x) {
	return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
}

static inline uint32_t s0(uint32_t x) {
	return ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3);
}

static inline uint32_t s1(uint32_t x) {
	return ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10);
}

static inline void RNDr(uint32_t *S, const uint32_t *W, uint32_t i) {
	uint32_t t1, t2;
	t1 = S[7] + S1(S[4]) + Ch(S[4], S[5], S[6]) + sha256_k[i] + W[i];
	t2 = S0(S[0]) + Maj(S[0], S[1], S[2]);
	S[7] = S[6];
	S[6] = S[5];
	S[5] = S[4];
	S[4] = S[3] + t1;
	S[3] = S[2];
	S[2] = S[1];
	S[1] = S[0];
	S[0] = t1 + t2;
}

#endif /* SHA2_HELPERS_H */ 