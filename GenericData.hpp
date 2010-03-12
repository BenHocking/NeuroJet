#if !defined(GENERICDATA_HPP)
#define GENERICDATA_HPP

#include <iostream>
#include <cstring>

template <typename T>
class GenericData {
public:
  GenericData(const T& data): _data(data) { }
  ~GenericData() {
  }
  template <typename S>
  friend std::ostream& operator<<(std::ostream& out, const GenericData<S>& gd);
  template <typename S>
  friend std::istream& operator>>(std::istream& in, GenericData<S>& gd);
  char* serialize() {
    return (char*)&_data;
  }
  void deserialize(char* data) {
    _data = *((T *) data);
  }
  T get() { return _data; }
  void set(T data) { _data = data; }
private:
  T _data;
};

template <typename S>
std::ostream& operator<<(std::ostream& out, const GenericData<S>& gd) {
  return (out << gd._data);
}

template <typename S>
std::istream& operator>>(std::istream& in, GenericData<S>& gd) {
  return (in >> gd._data);
}

#endif // GENERICDATA_HPP
