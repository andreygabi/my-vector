#pragma once
#include <iostream>

template <class T>
class Vector {
  T* vec_pointer_ = nullptr;
  size_t vec_size_ = 0;
  size_t vec_cap_ = 0;

 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  Vector() = default;

  explicit Vector(size_t new_size) {
    vec_size_ = new_size;
    vec_cap_ = vec_size_;
    if (new_size) {
      vec_pointer_ = new T[new_size];
    } else {
      vec_pointer_ = nullptr;
    }
  }

  Vector(size_t new_size, const T& t) {
    vec_size_ = new_size;
    vec_cap_ = new_size;
    if (vec_size_) {
      vec_pointer_ = new T[new_size];
      try {
        for (size_t i = 0; i < new_size; i++) {
          vec_pointer_[i] = t;
        }
      } catch (...) {
        delete[] vec_pointer_;
        throw;
      }
    } else {
      vec_pointer_ = nullptr;
    }
  }

  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator begin, Iterator end) {
    vec_size_ = std::distance(begin, end);
    vec_cap_ = vec_size_;
    if (!vec_size_) {
      vec_pointer_ = nullptr;
    } else {
      vec_pointer_ = new T[vec_size_];
      try {
        for (size_t i = 1; i <= vec_size_; i++) {
          vec_pointer_[i - 1] = *(begin + i - 1);
        }
      } catch (...) {
        delete[] vec_pointer_;
        throw;
      }
    }
  }

  Vector(std::initializer_list<T> initializer) : Vector(initializer.begin(), initializer.end()) {
  }

  Vector(const Vector& other) {
    vec_size_ = other.vec_size_;
    vec_cap_ = other.vec_cap_;
    if (!vec_size_) {
      vec_pointer_ = nullptr;
      vec_cap_ = 0;
      return;
    }
    vec_pointer_ = new T[other.vec_cap_];
    try {
      for (size_t i = 0; i < vec_size_; i++) {
        vec_pointer_[i] = other.vec_pointer_[i];
      }
    } catch (...) {
      delete[] vec_pointer_;
      throw;
    }
  }

  Vector(Vector&& other) noexcept {
    vec_size_ = other.vec_size_;
    vec_cap_ = other.vec_cap_;
    other.vec_size_ = 0;
    other.vec_cap_ = 0;
    if (!vec_size_) {
      vec_cap_ = 0;
      vec_pointer_ = nullptr;
    } else {
      vec_pointer_ = std::move(other.vec_pointer_);
    }
    other.vec_pointer_ = nullptr;
  }

  Vector& operator=(const Vector& other) {
    T* tmp = nullptr;
    try {
      if (vec_pointer_ == other.vec_pointer_) {
        return *this;
      }
      tmp = (!other.vec_size_ ? nullptr : new T[other.vec_cap_]);
      vec_cap_ = other.vec_cap_;
      vec_size_ = other.vec_size_;
      for (size_t i = 0; i < other.vec_size_; ++i) {
        tmp[i] = other.vec_pointer_[i];
      }
    } catch (...) {
      delete[] tmp;
      throw;
    }
    delete[] vec_pointer_;
    vec_pointer_ = std::move(tmp);
    return *this;
  }

  Vector& operator=(Vector&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    delete[] vec_pointer_;
    vec_size_ = other.vec_size_;
    other.vec_size_ = 0;
    vec_pointer_ = other.vec_pointer_;
    other.vec_pointer_ = nullptr;
    vec_cap_ = other.vec_cap_;
    other.vec_cap_ = 0;
    return *this;
  }

  size_t Size() const noexcept {
    return vec_size_;
  }

  bool Empty() const noexcept {
    return (vec_size_ == 0);
  }

  size_t Capacity() const noexcept {
    return vec_cap_;
  }

  T& operator[](size_t i) noexcept {
    return vec_pointer_[i];
  }

  const T& operator[](size_t i) const noexcept {
    return vec_pointer_[i];
  }

  T& At(size_t number) {
    if (number < vec_size_) {
      return vec_pointer_[number];
    }
    throw std::out_of_range("Out of range");
  }

  const T& At(size_t number) const {
    if (number < vec_size_) {
      return vec_pointer_[number];
    }
    throw std::out_of_range("Out of range");
  }

  T& Front() {
    return vec_pointer_[0];
  }

  const T& Front() const {
    return vec_pointer_[0];
  }

  T& Back() {
    return vec_pointer_[vec_size_ - 1];
  }

  const T& Back() const {
    return vec_pointer_[vec_size_ - 1];
  }

  T* Data() {
    return vec_pointer_;
  }

  const T* Data() const {
    return vec_pointer_;
  }

  void Swap(Vector& other) {
    std::swap(other, *this);
  }

  void Resize(size_t new_size) {
    if (new_size > vec_cap_) {
      auto* tmp = new T[new_size];
      for (size_t i = 0; i < vec_size_; i++) {
        tmp[i] = std::move(vec_pointer_[i]);
      }
      delete[] vec_pointer_;
      vec_pointer_ = tmp;
      vec_cap_ = new_size;
    }
    vec_size_ = new_size;
  }

  void Resize(size_t new_size, const T& t) {
    if (new_size > vec_size_) {
      if (new_size > vec_cap_) {
        Reserve(new_size * 2);
      }
      for (size_t i = vec_size_; i < new_size; ++i) {
        vec_pointer_[i] = t;
      }
      vec_cap_ = new_size * 2;
    }
    vec_size_ = new_size;
  }

  void Reserve(size_t new_cap) {
    if (vec_cap_ < new_cap) {
      auto* tmp = new T[new_cap];
      for (size_t i = 0; i < vec_size_; i++) {
        tmp[i] = std::move(vec_pointer_[i]);
      }
      delete[] vec_pointer_;
      vec_pointer_ = tmp;
    }
    vec_cap_ = new_cap;
  }

  void ShrinkToFit() {
    if (vec_size_) {
      auto* tmp = new T[vec_size_];
      for (size_t i = 0; i < vec_size_; i++) {
        tmp[i] = std::move(vec_pointer_[i]);
      }
      delete[] vec_pointer_;
      vec_cap_ = vec_size_;
      vec_pointer_ = tmp;
      return;
    }
    delete[] vec_pointer_;
    vec_cap_ = 0;
    vec_pointer_ = nullptr;
  }

  void Clear() noexcept {
    vec_size_ = 0;
  }

  void PushBack(const T& t) {
    if (vec_size_ < vec_cap_) {
      vec_pointer_[vec_size_] = std::move(t);
      ++vec_size_;
      return;
    }
    if (vec_size_) {
      auto* tmp = new T[vec_cap_ * 2];
      try {
        for (size_t i = 0; i < vec_size_; i++) {
          tmp[i] = std::move(vec_pointer_[i]);
        }
        tmp[vec_size_] = std::move(t);
      } catch (...) {
        delete[] tmp;
        throw;
      }
      delete[] vec_pointer_;
      vec_pointer_ = tmp;
      vec_cap_ *= 2;
      ++vec_size_;
    } else {
      auto* tmp = new T[1];
      try {
        tmp[0] = std::move(t);
      } catch (...) {
        delete[] tmp;
        throw;
      }
      delete[] vec_pointer_;
      vec_pointer_ = tmp;
      vec_size_ = 1;
      vec_cap_ = 1;
    }
  }

  void PushBack(T&& t) {
    if (vec_size_ >= vec_cap_) {
      Reserve(std::max<size_t>(vec_cap_ * 2, 1));
    }
    vec_pointer_[vec_size_++] = std::move(t);
  }

  void PopBack() noexcept {
    if (vec_size_) {
      --vec_size_;
    }
  }

  ConstIterator begin() const {  // NOLINT
    return vec_pointer_;
  }

  Iterator begin() {  // NOLINT
    return vec_pointer_;
  }

  ConstIterator end() const {  // NOLINT
    return vec_pointer_ + vec_size_;
  }

  Iterator end() {  // NOLINT
    return vec_pointer_ + vec_size_;
  }

  ConstIterator cbegin() const {  // NOLINT
    return vec_pointer_;
  }

  ConstIterator cend() const {  // NOLINT
    return vec_pointer_ + vec_size_;
  }

  std::reverse_iterator<ConstIterator> rbegin() const {  // NOLINT
    return std::make_reverse_iterator(vec_pointer_ + vec_size_);
  }

  std::reverse_iterator<Iterator> rbegin() {  // NOLINT
    return std::make_reverse_iterator(vec_pointer_ + vec_size_);
  }

  std::reverse_iterator<ConstIterator> crbegin() const {  // NOLINT
    return std::make_reverse_iterator(vec_pointer_ + vec_size_);
  }

  std::reverse_iterator<ConstIterator> rend() const {  // NOLINT
    return std::make_reverse_iterator(vec_pointer_);
  }

  std::reverse_iterator<Iterator> rend() {  // NOLINT
    return std::make_reverse_iterator(vec_pointer_);
  }

  std::reverse_iterator<ConstIterator> crend() const {  // NOLINT
    return std::make_reverse_iterator(vec_pointer_);
  }

  ~Vector() {
    delete[] vec_pointer_;
    vec_size_ = 0;
    vec_cap_ = 0;
  }

  friend bool operator<(const Vector<T>& first_vec, const Vector<T>& second_vec) {
    if (first_vec.vec_size_ < second_vec.vec_size_) {
      for (size_t i = 0; i < first_vec.vec_size_; i++) {
        if (first_vec.vec_pointer_[i] != second_vec.vec_pointer_[i]) {
          return first_vec.vec_pointer_[i] < second_vec.vec_pointer_[i];
        }
      }
      return first_vec.vec_size_ < second_vec.vec_size_;
    }
    for (size_t i = 0; i < second_vec.vec_size_; i++) {
      if (first_vec.vec_pointer_[i] != second_vec.vec_pointer_[i]) {
        return first_vec.vec_pointer_[i] < second_vec.vec_pointer_[i];
      }
    }
    return first_vec.vec_size_ < second_vec.vec_size_;
  }
  friend bool operator>=(const Vector<T>& first_vec, const Vector<T>& second_vec) {
    return !(first_vec < second_vec);
  }
  friend bool operator>(const Vector<T>& first_vec, const Vector<T>& second_vec) {
    return second_vec < first_vec;
  }
  friend bool operator<=(const Vector<T>& first_vec, const Vector<T>& second_vec) {
    return !(first_vec > second_vec);
  }
  friend bool operator==(const Vector<T>& first_vec, const Vector<T>& second_vec) {
    if (first_vec.vec_size_ != second_vec.vec_size_) {
      return false;
    }
    for (size_t i = 0; i < first_vec.vec_size_; i++) {
      if (first_vec.vec_pointer_[i] != second_vec.vec_pointer_[i]) {
        return false;
      }
    }
    return true;
  }
  friend bool operator!=(const Vector<T>& first_vec, const Vector<T>& second_vec) {
    return !(first_vec == second_vec);
  }
};