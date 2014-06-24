#include "dataview.hpp"
#include <cassert>

using namespace std;

dataview::dataview(size_t n, const vector<runtime_type_info> &types)
  : n_(n), types_(types)
{
  size_t acc = 0;
  for (auto t : types) {
    offsets_.push_back(acc);
    acc += runtime_type_traits::TypeSize(t);
  }
}


row_accessor
row_major_dataview::get() const
{
  const uint8_t *cursor = data_ + rowsize() * pos_;
  return row_accessor(cursor, &types(), &offsets());
}

size_t
row_major_dataview::index() const
{
  return pos_;
}

void
row_major_dataview::next()
{
  assert(!end());
  pos_++;
}

void
row_major_dataview::reset()
{
  pos_ = 0;
}

bool
row_major_dataview::end() const
{
  return pos_ == size();
}
