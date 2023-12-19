#include <iostream>
#include <cstring>
#include <cmath>

template <typename _Ty, bool is_const = false, bool is_reverse = false>
class vector_iterator {
public:
    using value_type = std::conditional_t<is_const, const _Ty, _Ty>;
    using pointer = std::conditional_t<is_const, const _Ty*, _Ty*>;
    using reference = std::conditional_t<is_const, const _Ty&, _Ty&>;
public:
    explicit vector_iterator(pointer ptr);
    reference operator*() const;
    pointer operator->() const;
    vector_iterator& operator++();
    vector_iterator operator++(int);
    vector_iterator& operator--();
    vector_iterator operator--(int);
    bool operator==(const vector_iterator& other) const;
    bool operator!=(const vector_iterator& other) const;
private:
    pointer _ptr;
};

template <typename _Ty, bool is_const, bool is_reverse>
vector_iterator<_Ty, is_const, is_reverse>::vector_iterator(typename vector_iterator<_Ty, is_const, is_reverse>::pointer ptr) 
    : _ptr(ptr) {}

template <typename _Ty, bool is_const, bool is_reverse>
typename vector_iterator<_Ty, is_const, is_reverse>::reference vector_iterator<_Ty, is_const, is_reverse>::operator*() const {
    return *_ptr;
}

template <typename _Ty, bool is_const, bool is_reverse>
typename vector_iterator<_Ty, is_const, is_reverse>::pointer vector_iterator<_Ty, is_const, is_reverse>::operator->() const {
    return _ptr;
}

template <typename _Ty, bool is_const, bool is_reverse>
vector_iterator<_Ty, is_const, is_reverse>& vector_iterator<_Ty, is_const, is_reverse>::operator++() {
    if (is_reverse)
        --_ptr;
    else
        ++_ptr;
    return *this;
}

template <typename _Ty, bool is_const, bool is_reverse>
vector_iterator<_Ty, is_const, is_reverse> vector_iterator<_Ty, is_const, is_reverse>::operator++(int) {
    vector_iterator<_Ty, is_const, is_reverse> tmp = *this;
    ++(*this);
    return tmp;
}

template <typename _Ty, bool is_const, bool is_reverse>
vector_iterator<_Ty, is_const, is_reverse>& vector_iterator<_Ty, is_const, is_reverse>::operator--() {
    if (is_reverse)
        ++_ptr;
    else
        --_ptr;
    return *this;
}

template <typename _Ty, bool is_const, bool is_reverse>
vector_iterator<_Ty, is_const, is_reverse> vector_iterator<_Ty, is_const, is_reverse>::operator--(int) {
    vector_iterator<_Ty, is_const, is_reverse> tmp = *this;
    --(*this);
    return tmp;
}

template <typename _Ty, bool is_const, bool is_reverse>
bool vector_iterator<_Ty, is_const, is_reverse>::operator==(const vector_iterator<_Ty, is_const, is_reverse>& other) const {
    return _ptr == other._ptr;
}

template <typename _Ty, bool is_const, bool is_reverse>
bool vector_iterator<_Ty, is_const, is_reverse>::operator!=(const vector_iterator<_Ty, is_const, is_reverse>& other) const {
    return !(*this == other);
}

template <typename _Ty>
class vector {
public:
    using reference = _Ty&;
    using const_reference = const _Ty&;
    using iterator = vector_iterator<_Ty>;
    using const_iterator = vector_iterator<_Ty, true>;
    using reverse_iterator = vector_iterator<_Ty, false, true>;
    using const_reverse_iterator = vector_iterator<_Ty, true, true>;
public:
    vector();
    vector(size_t size);
    vector(size_t size, _Ty v);
    ~vector();
public: // element access
    reference at(size_t index);
    const_reference at(size_t index) const;
    reference operator[](size_t index);
    const_reference operator[](size_t index) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    _Ty* data();
    const _Ty* data() const;
public: // capaity
    bool empty() const;
    size_t size() const;
    void reserve(size_t size);
    size_t capacity() const;
public: // modifiers    
    void clear();
    void insert(size_t index, size_t count, const _Ty& value);
    void push_back(const _Ty& value);
    void push_back(_Ty&& value);
    template <class... Args>
    void emplace_back(Args&&... args);
    void pop_back();
    void resize(size_t count);
    void resize(size_t count, const _Ty& value);
public: //iterators
    iterator begin();
    const_iterator cbegin();
    iterator end();
    const_iterator cend();
    reverse_iterator rbegin();
    const_reverse_iterator crbegin();
    reverse_iterator rend();
    const_reverse_iterator crend();

private:
    _Ty *_data;
    size_t _size;
    size_t _capacity;
};

template <typename _Ty>
vector<_Ty>::vector() : _size(0), _capacity(8) {
    _data = new _Ty[_capacity];
    memset(_data, 0, _capacity * sizeof(_Ty));
}

template <typename _Ty>
vector<_Ty>::vector(size_t size) : _size(size) {
    _capacity = pow(2, ceil(log2(size)));
    _data = new _Ty[_capacity];
    memset(_data, 0, _capacity * sizeof(_Ty));
}

template <typename _Ty>
vector<_Ty>::vector(size_t size, _Ty value) : _size(size) {
    _capacity = pow(2, ceil(log2(size)));
    _data = new _Ty[_capacity];
    memset(_data, 0, _capacity * sizeof(_Ty));
    for (size_t i = 0; i < size; ++i)
        _data[i] = value;
}

template <typename _Ty>
vector<_Ty>::~vector() {
    delete _data;
    _data = nullptr;
}

template <typename _Ty>
typename vector<_Ty>::reference vector<_Ty>::at(size_t index) {
    if (index >= _size) {
        throw std::out_of_range("operator[] out of range");
    }
    return _data[index];
}

template <typename _Ty>
typename vector<_Ty>::const_reference vector<_Ty>::at(size_t index) const {
    if (index >= _size) {
        throw std::out_of_range("operator[] out of range");
    }
    return _data[index];
}

template <typename _Ty>
typename vector<_Ty>::reference vector<_Ty>::operator[](size_t index) {
    if (index >= _size) {
        throw std::out_of_range("operator[] out of range");
    }
    return _data[index];
}

template <typename _Ty>
typename vector<_Ty>::const_reference vector<_Ty>::operator[](size_t index) const {
    if (index >= _size) {
        throw std::out_of_range("operator[] out of range");
    }
    return _data[index];
}

template <typename _Ty>
typename vector<_Ty>::reference vector<_Ty>::front() {
    if (_size == 0) {
        throw std::out_of_range("front() on empty vector");
    }
    return _data[0];
}

template <typename _Ty>
typename vector<_Ty>::const_reference vector<_Ty>::front() const {
    if (_size == 0) {
        throw std::out_of_range("front() on empty vector");
    }
    return _data[0];
}

template <typename _Ty>
typename vector<_Ty>::reference vector<_Ty>::back() {
    if (_size == 0) {
        throw std::out_of_range("back() on empty vector");
    }
    return _data[_size - 1];
}

template <typename _Ty>
typename vector<_Ty>::const_reference vector<_Ty>::back() const {
    if (_size == 0) {
        throw std::out_of_range("back() on empty vector");
    }
    return _data[_size - 1];
}

template <typename _Ty>
_Ty* vector<_Ty>::data() {
    return _data;
}

template <typename _Ty>
const _Ty* vector<_Ty>::data() const {
    return _data;
}

template <typename _Ty>
bool vector<_Ty>::empty() const {
    return _size == 0;
}

template <typename _Ty>
size_t vector<_Ty>::size() const {
    return _size;
}

template <typename _Ty>
void vector<_Ty>::reserve(size_t size) {
    _capacity = size;
    _Ty* new_buffer = new _Ty[size];
    memset(new_buffer, 0, sizeof(_Ty) * size);
    memcpy(new_buffer, _data, sizeof(_Ty) * _size);
    delete _data;
    _data = new_buffer;
}

template <typename _Ty>
size_t vector<_Ty>::capacity() const {
    return _capacity;
}

template <typename _Ty>
void vector<_Ty>::clear() {
    delete _data;
    _size = 0;
    _capacity = 8;
    _data = new _Ty[_capacity];
    memset(_data, 0, sizeof(_Ty) * _capacity);
}

template <typename _Ty>
void vector<_Ty>::insert(size_t index, size_t count, const _Ty& value) {
    if (_size + count > _capacity) {
        reserve(_capacity * 2);
        _capacity *= 2;
    }
    memcpy(_data + index + count, _data + index, sizeof(_Ty) * (_size - index));
    for (size_t i = index; i < index + count; ++i)
        _data[i] = value;
    _size += count;
}

template <typename _Ty>
void vector<_Ty>::push_back(const _Ty& value) {
    if (_size == _capacity) {
        reserve(_capacity * 2);
        _capacity *= 2;
    }
    _data[_size++] = value;
}

template <typename _Ty>
void vector<_Ty>::push_back(_Ty&& value) {
    if (_size == _capacity) {
        reserve(_capacity * 2);
        _capacity *= 2;
    }
    _data[_size++] = std::move(value);
}

template <typename _Ty>
template <class... Args>
void vector<_Ty>::emplace_back(Args&&... args) {
    if (_size == _capacity) {
        reserve(_capacity * 2);
        _capacity *= 2;
    }
    _data[_size++] = _Ty(std::forward<Args>(args)...);
}

template <typename _Ty>
void vector<_Ty>::pop_back() {
    _size--;
}

template <typename _Ty>
void vector<_Ty>::resize(size_t count) {
    if (count > _capacity) {
        reserve(count);
        _capacity = count;
    }
    memset(_data + _size, 0, (_capacity - count) * sizeof(_Ty));
    _size = count;
}

template <typename _Ty>
void vector<_Ty>::resize(size_t count, const _Ty& value) {
    if (count > _capacity) {
        reserve(count);
        _capacity = count;
    }
    memset(_data + _size, 0, (_capacity - count) * sizeof(_Ty));
    for (size_t index = _size; index < count; ++index)
        _data[index] = value;
    _size = count;
}

template <typename _Ty>
typename vector<_Ty>::iterator vector<_Ty>::begin() {
    return iterator(_data);
}

template <typename _Ty>
typename vector<_Ty>::const_iterator vector<_Ty>::cbegin() {
    return const_iterator(_data);
}

template <typename _Ty>
typename vector<_Ty>::iterator vector<_Ty>::end() {
    return iterator(_data + _size);
}

template <typename _Ty>
typename vector<_Ty>::const_iterator vector<_Ty>::cend() {
    return const_iterator(_data + _size);
}

template <typename _Ty>
typename vector<_Ty>::reverse_iterator vector<_Ty>::rbegin() {
    return reverse_iterator(_data + _size - 1);
}

template <typename _Ty>
typename vector<_Ty>::const_reverse_iterator vector<_Ty>::crbegin() {
    return const_reverse_iterator(_data + _size - 1);
}

template <typename _Ty>
typename vector<_Ty>::reverse_iterator vector<_Ty>::rend() {
    return reverse_iterator(_data - 1);
}

template <typename _Ty>
typename vector<_Ty>::const_reverse_iterator vector<_Ty>::crend() {
    return const_reverse_iterator(_data - 1);
}
