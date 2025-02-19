#include "stdint.h"

// encryption routine
void encrypt(static unsigned long  *v, static unsigned long *k) {
    unsigned long v0 = v[0], v1 = v[1], sum = 0, i;
    unsigned long delta = 0x9e3779b9;
    unsigned long k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
    for (i = 0; i < 32; i++) {
        sum += delta;
        v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
        v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
    }
    v[0] = v0;
    v[1] = v1;
}

// decryption routine
void decrypt(static unsigned long *v, static unsigned long *k) {
    unsigned long v0 = v[0], v1 = v[1], sum = 0xC6EF3720, i;
    unsigned long delta = 0x9e3779b9;
    unsigned long k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
    for (i = 0; i < 32; i++) {
        v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
        v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
        sum -= delta;
    }
    v[0] = v0;
    v[1] = v1;
}