#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>
#include <initializer_list>

class vector final
{
private:
    double *arr;
    size_t _capacity, _size;

    void _resize(size_t new_capacity)
    {
        double *resize;
        resize = new double[new_capacity];
        auto count = std::min(_size, new_capacity);
        std::copy(arr, arr + count, resize);
        delete[] arr;
        arr = resize;
        _capacity = new_capacity;
        _size = count;
    }

public:
    class iterator final
    {
    private:
        double *_ptr;

    public:
        iterator(double *ptr) : _ptr{ptr} {}
        double const &operator*() const { return *_ptr; }
        double &operator*() { return *_ptr; }

        iterator &operator++()
        {
            _ptr++;
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator &operator--()
        {
            _ptr--;
            return *this;
        }

        iterator operator--(int)
        {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        iterator operator+(size_t shift) const { return iterator(_ptr + shift); }
        iterator &operator+=(size_t shift)
        {
            _ptr += shift;
            return *this;
        }

        iterator operator-(size_t shift) const { return iterator(_ptr - shift); }
        iterator &operator-=(size_t shift)
        {
            _ptr -= shift;
            return *this;
        }

        double *operator->() { return _ptr; }
        double const *operator->() const { return _ptr; }
        double &operator[](size_t shift)
        {
            return *(_ptr + shift);
        }
        double operator[](size_t shift) const
        {
            return *(_ptr + shift);
        }

        size_t operator-(const iterator &iter) const { return _ptr - iter._ptr; }
        bool operator==(const iterator &iter) const { return _ptr == iter._ptr; };
        bool operator!=(const iterator &iter) const { return _ptr != iter._ptr; };
        bool operator>(const iterator &iter) const { return _ptr > iter._ptr; };
        bool operator>=(const iterator &iter) const { return _ptr >= iter._ptr; };
        bool operator<(const iterator &iter) const { return _ptr < iter._ptr; };
        bool operator<=(const iterator &iter) const { return _ptr <= iter._ptr; };
    };

    vector(size_t count_elements, double default_value) : arr{new double[count_elements]},
                                                          _size(count_elements),
                                                          _capacity(count_elements)

    {
        std::fill_n(arr, count_elements, default_value);
    }

    vector(size_t count_elements) : vector(count_elements, 0.0) {}

    template <typename Iterator>
    vector(Iterator const begin, Iterator const end)
    {
        auto distance = end - begin;
        arr = new double[distance];
        _capacity = _size = distance;
        std::copy(begin, end, arr);
    }

    vector(std::initializer_list<double> init_list) : _capacity{init_list.size()}, _size(init_list.size()),
                                                      arr{new double[init_list.size()]}
    {
        std::copy(init_list.begin(), init_list.end(), arr);
    }

    vector(const vector &v) : vector(v.begin(), v.end()) {};

    double &at(size_t index)
    {
        if (index < 0 || index >= _size)
            throw std::range_error("Out of range");

        return arr[index];
    }

    double const &at(size_t index) const
    {
        if (index < 0 || index >= _size)
            throw std::range_error("Out of range");

        return arr[index];
    }

    double &front()
    {
        if (_size == 0)
            throw std::range_error("Out of range");

        return arr[0];
    }

    double const &front() const
    {
        if (_size == 0)
            throw std::range_error("Out of range");

        return arr[0];
    }

    double &back()
    {
        if (_size == 0)
            throw std::range_error("Out of range");

        return arr[_size - 1];
    }

    double const &back() const
    {
        if (_size == 0)
            throw std::range_error("Out of range");

        return arr[_size - 1];
    }

    double *data()
    {
        return arr;
    }

    double const *data() const
    {
        return arr;
    }

    bool empty() const { return !_size; }

    void reserve(size_t num)
    {
        if (num <= _capacity)
            return;

        _resize(num);
    }

    size_t capacity() const { return _capacity; }
    size_t size() const { return _size; }

    void shrink_to_fit()
    {
        if (_capacity <= _size)
            return;

        _resize(_size);
    }

    void clear() { _size = 0; }

    void push_back(double elem)
    {
        if (_capacity == _size)
        {
            _resize(_capacity * 2);
        }
        arr[_size++] = elem;
    }

    void pop_back()
    {
        if (_size > 0)
            --_size;
    }

    void resize(size_t size, double elem)
    {
        size_t old_size = _size;
        _resize(size);
        if (size > old_size)
        {
            std::fill(arr + old_size, arr + size, elem);
        }
        _size = size;
    }

    void erase(size_t index)
    {
        if (index >= _size)
            return;

        for (size_t i = index; i < _size - 1; ++i)
        {
            arr[i] = arr[i + 1];
        }
        --_size;
    }

    void insert(size_t index, double elem)
    {
        if (index > _size)
        {
            throw std::range_error("Out of range");
        }
        if (_size == _capacity)
        {
            _resize(_capacity * 2);
        }
        ++_size;

        for (size_t i = _size - 1; i > index; --i)
        {
            arr[i] = arr[i - 1];
        }
        arr[index] = elem;
    }

    bool operator==(vector const &v) const
    {
        if (_size != v._size)
            return false;

        for (size_t i = 0; i < _size; ++i)
        {
            if (arr[i] != v.arr[i])
                return false;
        }
        return true;
    }

    int operator<=>(vector const &v) const
    {
        for (size_t i = 0; i < _size && i < v._size; ++i)
        {
            if (arr[i] - v.arr[i] < -__DBL_EPSILON__)
                return -1;
            if (arr[i] - v.arr[i] > __DBL_EPSILON__)
                return 1;
        }
        if (_size > v._size)
        {
            return 1;
        }
        if (_size < v._size)
            return -1;
        return 0;
    }

    vector &operator=(vector const &v)
    {
        if (this != &v)
        {
            vector tmp(v.begin(), v.end());
            std::swap(tmp.arr, this->arr);
            std::swap(tmp._capacity, this->_capacity);
            std::swap(tmp._size, this->_size);
        }
        return *this;
    }

    iterator begin() { return iterator(arr); }
    iterator end() { return iterator(arr + size()); }

    const iterator begin() const { return iterator(arr); }
    const iterator end() const { return iterator(arr + size()); }

    ~vector()
    {
        delete[] arr;
    }
};

int main()
{
    double arr[3] = {1, 2, 3};
    std::vector<double> v1(5, 10);
    v1.insert(v1.cbegin() + 5, 15);
    vector a{1, 2, 3, 4, 5};
    a.insert(2, 10);
    vector b(10, 0.0);

    for (auto elem : a)
    {
        std::cout << elem << std::endl;
    }
    const vector edds{9., 2., 3., 4., 5.};
    const double &aboba = edds.at(2);
    double &abobs = a.at(3);
    return 0;
}