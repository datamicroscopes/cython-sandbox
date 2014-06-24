#pragma once

#include "type_info.h"
#include <vector>
#include <iostream>

template <typename T>
struct _info { static const size_t size = sizeof(T); };

class runtime_type_traits {
public:
  static inline size_t
  TypeSize(runtime_type_info t)
  {
    return TypeSizes_[t];
  }
private:
  static const size_t TypeSizes_[TYPE_INFO_NELEMS];
};
