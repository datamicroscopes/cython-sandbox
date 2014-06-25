#include "dataview.hpp"
#include <cassert>
#include <iostream>

using namespace std;

dataview::dataview(size_t n, const vector<runtime_type_info> &types)
  : n_(n), types_(types), rowsize_()
{
  size_t acc = 0;
  for (auto t : types) {
    offsets_.push_back(acc);
    acc += runtime_type_traits::TypeSize(t);
  }
  rowsize_ = acc;
}

row_major_dataview::row_major_dataview(
    const uint8_t *data, size_t n, const vector<runtime_type_info> &types)
    : dataview(n, types), data_(data), pos_()
{
    //cout << "types:" << endl;
    //for (auto t : types)
    //    cout << "  " << t << endl;
    //cout << "offsets:" << endl;
    //for (auto off : offsets())
    //    cout << "  " << off << endl;
    //cout << "rowsize:" << rowsize() << endl;
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
