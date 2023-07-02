#include "number.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>

uint2022_t from_uint(uint32_t i) {
  uint2022_t result;
  if (i == 0) {
    result.now_size++;
  }
  while (i > 0) {
    if (result.now_size%9 == 0 && result.now_size > 1) {
      result.now_ind--;
    }
    result.value[result.now_ind] += (i%10*pow(10, (result.now_size)%9));
    i = i/10;
    result.now_size++;
  }
  return result;
}

uint2022_t from_string(const char* buff) {
  uint2022_t result;
  int32_t i = strlen(buff) - 1;
  while (i >= 0) {
    if (result.now_size%9 == 0 && result.now_size > 1) {
      result.now_ind--;
    }
    result.value[result.now_ind] += ((buff[i] - '0')*pow(10, (result.now_size)%9));
    i--;
    result.now_size++;
  }
  return result;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t result;
  uint32_t mx_now_size;
  uint32_t mx_now_ind;
  if (lhs.now_size > rhs.now_size) {
    mx_now_size = lhs.now_size;
    mx_now_ind = lhs.now_ind;
  } else {
    mx_now_size = rhs.now_size;
    mx_now_ind = rhs.now_ind;
  }
  for (int i = uint2022_t::kMaxSize - 1; i >= mx_now_ind; --i) {
    result.value[i] += ((lhs.value[i]%uint2022_t::kBase) + (rhs.value[i]%uint2022_t::kBase))%uint2022_t::kBase;
    if ((lhs.value[i] + rhs.value[i]) >= uint2022_t::kBase) {
      result.value[i - 1]++;
    }
  }
  result.update_size();
  return result;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t result;
  uint32_t credit = 0;

  for (int i = uint2022_t::kMaxSize - 1; i >= lhs.now_ind; --i) {
    if (lhs.value[i] >= (rhs.value[i] + credit)) {
      result.value[i] = lhs.value[i] - rhs.value[i] - credit;
      credit = 0;
    } else {
      result.value[i] = pow(10, 9) - (rhs.value[i] - lhs.value[i]) - credit;
      credit = 1;
    }
  }
  result.update_size();
  return result;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t result;
  uint64_t result_long[uint2022_t::kMaxSize];
  uint64_t lhs_long[uint2022_t::kMaxSize];
  uint64_t rhs_long[uint2022_t::kMaxSize];
  for (int i = 0; i < uint2022_t::kMaxSize; i++) {
    lhs_long[i] = lhs.value[uint2022_t::kMaxSize - 1 - i];
    rhs_long[i] = rhs.value[uint2022_t::kMaxSize - 1 - i];
    result_long[i] = 0;
  }
  for (int i = 0; i < uint2022_t::kMaxSize; i++) {
    for (int j = 0; j < uint2022_t::kMaxSize - i; j++) {
      result_long[i + j] += lhs_long[i]*rhs_long[j];
    }
  }

  for (int i = 0; i < uint2022_t::kMaxSize - 1; i++) {
    result_long[i + 1] += result_long[i]/uint2022_t::kBase;
    result_long[i] %= uint2022_t::kBase;
  }
  for (int i = 0; i < uint2022_t::kMaxSize; i++) {
    result.value[uint2022_t::kMaxSize - 1 - i] = result_long[i];
  }
  result.update_size();
  return result;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t result;
  uint2022_t now_value;
  uint32_t el_to_add = lhs.now_ind;
  while (el_to_add < uint2022_t::kMaxSize) {
    while (now_value < rhs) {
      now_value.shift();
      now_value.value[uint2022_t::kMaxSize - 1] = lhs.value[el_to_add];
      el_to_add++;
    }
    uint2022_t current;
    uint32_t closest = 0;
    uint32_t left = 0;
    uint32_t right = uint2022_t::kBase;
    while (left <= right) {
      int mid = (left + right)/2;
      current = rhs*from_uint(mid);
      if (current < now_value || current == now_value) {
        closest = mid;
        left = mid + 1;
      } else
        right = mid - 1;
    }
    now_value = now_value - (from_uint(closest)*rhs);
    result.shift();
    result.value[uint2022_t::kMaxSize - 1] = closest;
  }
  result.update_size();
  return result;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
  for (int i = lhs.now_ind; i < uint2022_t::kMaxSize; ++i) {
    if (lhs.value[i] != rhs.value[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
  return !(lhs == rhs);
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
  for (int i = 0; i < uint2022_t::kMaxSize; ++i) {
    if (lhs.value[i] != rhs.value[i])
      return lhs.value[i] > rhs.value[i];
  }
  return false;
}
bool operator<(const uint2022_t& lhs, const uint2022_t& rhs) {
  return !(lhs > rhs || lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
  bool flag_for_null = false;
  for (int i = 0; i < uint2022_t::kMaxSize; ++i) {
    if (value.value[i] != 0) {
      flag_for_null = true;
    }
    if (flag_for_null) {
      stream << std::setw(9) << std::setfill('0') << value.value[i];
    }
  }
  if (!flag_for_null) {
    stream << 0;
  }
  return stream;
}