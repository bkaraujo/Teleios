#include "teleios/teleios.h"
#include "teleios/platform.h"

struct TLUlid { char ulid[27]; };

#define ULID_RELAXED   (1 << 0)
#define ULID_PARANOID  (1 << 1)
#define ULID_SECURE    (1 << 2)

typedef struct  {
    unsigned char last[16];
    unsigned long long last_ts;
    int flags;
    unsigned char i, j;
    unsigned char s[256];
} TLUlidGenerator;

TLUlidGenerator generator = { 0 };

void tl_ulid_encode(char str[27], const unsigned char ulid[16]) {
    TLDPUSH;
    static const char set[256] = {
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x48, 0x4a, 0x4b, 0x4d, 0x4e, 0x50, 0x51,
        0x52, 0x53, 0x54, 0x56, 0x57, 0x58, 0x59, 0x5a,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x48, 0x4a, 0x4b, 0x4d, 0x4e, 0x50, 0x51,
        0x52, 0x53, 0x54, 0x56, 0x57, 0x58, 0x59, 0x5a,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x48, 0x4a, 0x4b, 0x4d, 0x4e, 0x50, 0x51,
        0x52, 0x53, 0x54, 0x56, 0x57, 0x58, 0x59, 0x5a,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x48, 0x4a, 0x4b, 0x4d, 0x4e, 0x50, 0x51,
        0x52, 0x53, 0x54, 0x56, 0x57, 0x58, 0x59, 0x5a,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x48, 0x4a, 0x4b, 0x4d, 0x4e, 0x50, 0x51,
        0x52, 0x53, 0x54, 0x56, 0x57, 0x58, 0x59, 0x5a,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x48, 0x4a, 0x4b, 0x4d, 0x4e, 0x50, 0x51,
        0x52, 0x53, 0x54, 0x56, 0x57, 0x58, 0x59, 0x5a,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x48, 0x4a, 0x4b, 0x4d, 0x4e, 0x50, 0x51,
        0x52, 0x53, 0x54, 0x56, 0x57, 0x58, 0x59, 0x5a,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x48, 0x4a, 0x4b, 0x4d, 0x4e, 0x50, 0x51,
        0x52, 0x53, 0x54, 0x56, 0x57, 0x58, 0x59, 0x5a
    };
    str[ 0] = set[ ulid[ 0] >> 5];
    str[ 1] = set[ ulid[ 0] >> 0];
    str[ 2] = set[ ulid[ 1] >> 3];
    str[ 3] = set[(ulid[ 1] << 2 | ulid[ 2] >> 6) & 0x1f];
    str[ 4] = set[ ulid[ 2] >> 1];
    str[ 5] = set[(ulid[ 2] << 4 | ulid[ 3] >> 4) & 0x1f];
    str[ 6] = set[(ulid[ 3] << 1 | ulid[ 4] >> 7) & 0x1f];
    str[ 7] = set[ ulid[ 4] >> 2];
    str[ 8] = set[(ulid[ 4] << 3 | ulid[ 5] >> 5) & 0x1f];
    str[ 9] = set[ ulid[ 5] >> 0];
    str[10] = set[ ulid[ 6] >> 3];
    str[11] = set[(ulid[ 6] << 2 | ulid[ 7] >> 6) & 0x1f];
    str[12] = set[ ulid[ 7] >> 1];
    str[13] = set[(ulid[ 7] << 4 | ulid[ 8] >> 4) & 0x1f];
    str[14] = set[(ulid[ 8] << 1 | ulid[ 9] >> 7) & 0x1f];
    str[15] = set[ ulid[ 9] >> 2];
    str[16] = set[(ulid[ 9] << 3 | ulid[10] >> 5) & 0x1f];
    str[17] = set[ ulid[10] >> 0];
    str[18] = set[ ulid[11] >> 3];
    str[19] = set[(ulid[11] << 2 | ulid[12] >> 6) & 0x1f];
    str[20] = set[ ulid[12] >> 1];
    str[21] = set[(ulid[12] << 4 | ulid[13] >> 4) & 0x1f];
    str[22] = set[(ulid[13] << 1 | ulid[14] >> 7) & 0x1f];
    str[23] = set[ ulid[14] >> 2];
    str[24] = set[(ulid[14] << 3 | ulid[15] >> 5) & 0x1f];
    str[25] = set[ ulid[15] >> 0];
    str[26] = 0;
    TLDRE;
}

i32 tl_ulid_decode(unsigned char ulid[16], const char *s) {
    TLDPUSH;
    static const signed char v[] = {
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09,   -1,   -1,   -1,   -1,   -1,   -1,
          -1, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
        0x11, 0x01, 0x12, 0x13, 0x01, 0x14, 0x15, 0x00,
        0x16, 0x17, 0x18, 0x19, 0x1a,   -1, 0x1b, 0x1c,
        0x1d, 0x1e, 0x1f,   -1,   -1,   -1,   -1,   -1,
          -1, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
        0x11, 0x01, 0x12, 0x13, 0x01, 0x14, 0x15, 0x00,
        0x16, 0x17, 0x18, 0x19, 0x1a,   -1, 0x1b, 0x1c,
        0x1d, 0x1e, 0x1f,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
          -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1
    };
    
    if (v[(int)s[0]] > 7) {
        TLDRV(1);
    }
    
    for (int i = 0; i < 26; i++) {
        if (v[(int)s[i]] == -1) {
            TLDRV(2);
        }
    }

    ulid[ 0] = v[(int)s[ 0]] << 5 | v[(int)s[ 1]] >> 0;
    ulid[ 1] = v[(int)s[ 2]] << 3 | v[(int)s[ 3]] >> 2;
    ulid[ 2] = v[(int)s[ 3]] << 6 | v[(int)s[ 4]] << 1 | v[(int)s[ 5]] >> 4;
    ulid[ 3] = v[(int)s[ 5]] << 4 | v[(int)s[ 6]] >> 1;
    ulid[ 4] = v[(int)s[ 6]] << 7 | v[(int)s[ 7]] << 2 | v[(int)s[ 8]] >> 3;
    ulid[ 5] = v[(int)s[ 8]] << 5 | v[(int)s[ 9]] >> 0;
    ulid[ 6] = v[(int)s[10]] << 3 | v[(int)s[11]] >> 2;
    ulid[ 7] = v[(int)s[11]] << 6 | v[(int)s[12]] << 1 | v[(int)s[13]] >> 4;
    ulid[ 8] = v[(int)s[13]] << 4 | v[(int)s[14]] >> 1;
    ulid[ 9] = v[(int)s[14]] << 7 | v[(int)s[15]] << 2 | v[(int)s[16]] >> 3;
    ulid[10] = v[(int)s[16]] << 5 | v[(int)s[17]] >> 0;
    ulid[11] = v[(int)s[18]] << 3 | v[(int)s[19]] >> 2;
    ulid[12] = v[(int)s[19]] << 6 | v[(int)s[20]] << 1 | v[(int)s[21]] >> 4;
    ulid[13] = v[(int)s[21]] << 4 | v[(int)s[22]] >> 1;
    ulid[14] = v[(int)s[22]] << 7 | v[(int)s[23]] << 2 | v[(int)s[24]] >> 3;
    ulid[15] = v[(int)s[24]] << 5 | v[(int)s[25]] >> 0;

    TLDRV(0);
}

TLUlid* tl_ulid_wrap(const char* desired) {
    TLDPUSH;
    
    TLUlid* ulid = tl_memory_alloc(TL_MEMORY_ULID, sizeof(TLUlid));
    if (tl_string_length(desired) != 27) TLFATAL("String length missmatch, expected 27 got %u", tl_string_length(desired));

    tl_memory_copy((void*)desired, sizeof(TLUlid), (void*)&ulid);
    
    TLDRV(ulid);
}

TLUlid* tl_ulid_generate(void) {
    TLDPUSH;

    // ===================================
    // Initialize the TLUlidGenerator
    // ===================================
    generator.last_ts = 0;
    generator.flags = ULID_SECURE;
    generator.i = generator.j = 0;

    for (int i = 0; i < 256; i++)
        generator.s[i] = i;

    /* RC4 is used to fill the random segment of ULIDs. It's tiny,
     * simple, perfectly sufficient for the task (assuming it's seeded
     * properly), and doesn't require fixed-width integers. It's not the
     * fastest option, but it's plenty fast for the task.
     *
     * Besides, when we're in a serious hurry in normal operation (not
     * in "relaxed" mode), we're incrementing the random field much more
     * often than generating fresh random bytes.
     */

    // int initstyle = 1;
    unsigned char key[256] = {0};
    if (!tl_platform_entropy(key, 256)) {
        /* Mix entropy into the RC4 state. */
        for (int i = 0, j = 0; i < 256; i++) {
            j = (j + generator.s[i] + key[i]) & 0xff;
            int tmp = generator.s[i];
            generator.s[i] = generator.s[j];
            generator.s[j] = tmp;
        }
        // initstyle = 0;
    } else if (!(generator.flags & ULID_SECURE)) {
        /* Failed to read entropy from OS, so generate some. */
        u32 n = 0;
        u64 now;
        u64 start = tl_chrono_time_epoch_micros();
        do {
            struct {
                // clock_t clk;
                u64 ts;
                i64 n;
                void* stackgap;
            } noise;
            noise.ts = now = tl_chrono_time_epoch_micros();
            // noise.clk = clock();
            noise.stackgap = &noise;
            noise.n = n;
            
            u8 *k = (u8*)&noise;
            for (int i = 0, j = 0; i < 256; i++) {
                j = (j + generator.s[i] + k[i % sizeof(noise)]) & 0xff;
                int tmp = generator.s[i];
                generator.s[i] = generator.s[j];
                generator.s[j] = tmp;
            }
        } while (n++ < 1UL << 16 || now - start < 500000ULL);
    }

    // ===================================
    // Generate
    // ===================================
    u64 ts = tl_chrono_time_epoch_millis();

    /* Fill out timestamp */
    generator.last_ts = ts;
    generator.last[0] = ts >> 40;
    generator.last[1] = ts >> 32;
    generator.last[2] = ts >> 24;
    generator.last[3] = ts >> 16;
    generator.last[4] = ts >>  8;
    generator.last[5] = ts >>  0;

    /* Fill out random section */
    for (int k = 0; k < 10; k++) {
        generator.i = (generator.i + 1) & 0xff;
        generator.j = (generator.j + generator.s[generator.i]) & 0xff;
        int tmp = generator.s[generator.i];
        generator.s[generator.i] = generator.s[generator.j];
        generator.s[generator.j] = tmp;
        generator.last[6 + k] = generator.s[(generator.s[generator.i] + generator.s[generator.j]) & 0xff];
    }

    if (generator.flags & ULID_PARANOID)
        generator.last[6] &= 0x7f;
 
    TLUlid* ulid = (TLUlid*) tl_memory_alloc(TL_MEMORY_ULID, sizeof(TLUlid));
    tl_ulid_encode(ulid->ulid, generator.last);

    TLDRV(ulid);
}

b8 tl_ulid_equals(TLUlid* first, TLUlid* second) {
    TLDPUSH;

    if (first == NULL || second == NULL) TLDRV(false);
    b8 equals = tl_string_equals(first->ulid, second->ulid);

    TLDRV(equals);
}

void tl_ulid_destroy(TLUlid* ulid) {
    TLDPUSH;
    tl_memory_free(TL_MEMORY_ULID, sizeof(TLUlid), (void*) ulid);
    TLDRE;
}