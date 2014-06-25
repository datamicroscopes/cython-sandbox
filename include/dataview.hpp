#pragma once

#include <vector>
#include <cassert>

#include "type_helper.hpp"

class row_accessor {
public:
  row_accessor() : data_(), types_(), offsets_(), pos_() {}
  row_accessor(const uint8_t *data,
               const std::vector<runtime_type_info> *types,
               const std::vector<size_t> *offsets)
    : data_(data), types_(types), offsets_(offsets), cursor_(data), pos_()
  {
    assert(data);
    assert(types->size() == offsets->size());
  }

  inline size_t tell() const { return pos_; }
  inline size_t nfeatures() const { return types_->size(); }

  inline void
  seek(size_t pos)
  {
    assert(pos <= nfeatures());
    if (pos < nfeatures())
      cursor_ = data_ + (*offsets_)[pos];
    else
      cursor_ = nullptr;
    pos_ = pos;
  }

  template <typename T>
  const T &
  get() const
  {
    assert(cursor_);
    // XXX: implementing casting here
    return *reinterpret_cast<const T *>(cursor_);
  }

  inline void
  bump()
  {
    seek(pos_ + 1);
  }

  inline void
  reset()
  {
    seek(0);
  }

private:
  const uint8_t *data_;
  const std::vector<runtime_type_info> *types_;
  const std::vector<size_t> *offsets_;

  const uint8_t *cursor_;
  size_t pos_;
};

class dataview {
protected:
  dataview(size_t n, const std::vector<runtime_type_info> &types);

  inline const std::vector<size_t> & offsets() const { return offsets_; }
  inline size_t rowsize() const { return rowsize_; }

public:
  virtual ~dataview() {}

  // implementations need to provide the following API
  virtual row_accessor get() const = 0;
  virtual size_t index() const = 0;
  virtual void next() = 0;
  virtual void reset() = 0;
  virtual bool end() const = 0;

  inline size_t size() const { return n_; }
  inline const std::vector<runtime_type_info> & types() const { return types_; }

private:
  size_t n_;

  std::vector<runtime_type_info> types_;
  std::vector<size_t> offsets_;
  size_t rowsize_;
};

class row_major_dataview : public dataview {
public:
  row_major_dataview(const uint8_t *data,
                     size_t n, const std::vector<runtime_type_info> &types);
  row_accessor get() const override;
  size_t index() const override;
  void next() override;
  void reset() override;
  bool end() const override;

private:
  const uint8_t *data_;
  size_t pos_;
};
