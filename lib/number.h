#pragma once
#include <cinttypes>
#include <iostream>

struct uint2022_t {
    static const uint32_t kMaxSize = 68;
    static const uint32_t kBase = 1000000000;
    uint32_t value[kMaxSize] = {0};
    uint32_t now_ind = kMaxSize - 1;
    uint32_t now_size = 0;
    void update_size(){
        for(int i = 0; i < kMaxSize; ++i){
            if(value[i] != 0){
                now_ind = i;
                uint32_t tmp_value = value[i];
                uint32_t digits_counter = 0;
                while(tmp_value > 0){
                    digits_counter++;
                    tmp_value = tmp_value / 10;
                }
                now_size = digits_counter + 9 * (kMaxSize - now_ind - 1);
                break;
            }
        }
    }
    void shift() {
        for(int i = now_ind; i < kMaxSize; ++i) {
            value[i - 1] = value[i];
        }
        now_ind--;
    }
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);

uint2022_t from_string(const char* buff);

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);
