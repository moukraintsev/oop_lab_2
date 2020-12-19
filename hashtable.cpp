#ifndef HASHTABLE_CPP
#define HASHTABLE_CPP
#include <algorithm>
#include "hashtable.h"
#include <exception>
#include <fstream>
#include <utility>
#include <QDebug>

using namespace std;

template <class KeyClass, class ValueClass, class HashFunc>
HashTable<KeyClass, ValueClass, HashFunc>::HashTable()
{
    _f = HashFunc();
    _capacity = 2;
    _has_value = new bool [_capacity];
    for (int i = 0; i<_capacity; i++){
        _has_value[i] = false;
    }
    _keys_values = new KeyClass[_capacity];
    _values = new ValueClass[_capacity];
    _size = 0;
}


template <class KeyClass, class ValueClass, class HashFunc>
HashTable<KeyClass, ValueClass, HashFunc>::HashTable(HashFunc func)
{
    _f = func();
    _capacity = 2;
    _has_value = new bool [_capacity];
    for (auto i = 0; i<_capacity; i++){
        _has_value[i] = false;
    }
    _keys_values = new KeyClass[_capacity];
    _values = new ValueClass[_capacity];
    _size = 0;
}

template <class KeyClass, class ValueClass, class HashFunc>
HashTable<KeyClass, ValueClass, HashFunc>::HashTable(const HashTable& other)
{
    _f = other._f;
    _capacity = other._capacity;
    _size = other._size;
    _has_value = new bool [_capacity];
    for (auto i = 0; i<_capacity; i++){
        _has_value[i] = false;
    }
    _keys_values = new KeyClass[_capacity];
    _values = new ValueClass[_capacity];
    for (int i=0; i < _capacity; i++){
        if (other._has_value[i]){
            _has_value[i] = other._has_value[i];
            _keys_values[i] = other._keys_values[i];
            _values[i] = other._values[i];
        }
    }
}

template <class KeyClass, class ValueClass, class HashFunc>
void HashTable<KeyClass, ValueClass, HashFunc>::swap(HashTable& other)
{
    std::swap(_f, other._f);
    std::swap(_capacity, other._capacity);
    std::swap(_has_value, other._has_value);
    std::swap(_keys_values, other._keys_values);
    std::swap(_values, other._values);
    std::swap(_size, other._size);
}

template <class KeyClass, class ValueClass, class HashFunc>
HashTable<KeyClass, ValueClass, HashFunc> HashTable<KeyClass, ValueClass, HashFunc>::operator=(const HashTable<KeyClass, ValueClass, HashFunc>& other)
{
    if (this != &other)
        HashTable<KeyClass, ValueClass, HashFunc>(other).swap(*this);
    return *this;
}

template <class KeyClass, class ValueClass, class HashFunc>
HashTable<KeyClass, ValueClass, HashFunc>::~HashTable()
{
    clear();
}

template <class KeyClass, class ValueClass, class HashFunc>
void HashTable<KeyClass, ValueClass, HashFunc>::clear()
{
    if (_capacity >= 1){
        delete [] _has_value;
        delete [] _keys_values;
        delete [] _values;
    }
    _size = 0;
    _capacity = 0;
}

template <class KeyClass, class ValueClass, class HashFunc>
ValueClass& HashTable<KeyClass, ValueClass, HashFunc>::at(const KeyClass& key) const
{
    int idx = _f(key) % _capacity;
    while (_has_value[idx] and idx <= _capacity){
        if (_keys_values[idx] == key){
            return _values[idx];
        }
        idx++;
    }
    throw std::invalid_argument("There is no such key in HashTable");
}


template <class KeyClass, class ValueClass, class HashFunc>
template<class KeysArrayType, class ValuesArrayType>
ValuesArrayType HashTable<KeyClass, ValueClass, HashFunc>::at(const KeysArrayType& keys) const
{
    ValuesArrayType result;
    for (auto el: keys){
        if (!has(el)) throw std::invalid_argument("There is no such key in HashTable");
        else result.push_back(at(el));
    }
    return result;
}


template <class KeyClass, class ValueClass, class HashFunc>
size_t HashTable<KeyClass, ValueClass, HashFunc>::size() const
{
    return _size;
}

template <class KeyClass, class ValueClass, class HashFunc>
bool HashTable<KeyClass, ValueClass, HashFunc>::operator==(const HashTable<KeyClass, ValueClass, HashFunc>& other) const
{
    int size1 = _size, size2 = other._size;
    if (size1 != size2) return false;
    for (int i = 0; i < _capacity; i++){
        if (!_has_value[i]) continue;
        auto cur_key = _keys_values[i];
        auto cur_value = _values[i];
        int idx = other._f(cur_key) % other._capacity;
        while (other._has_value[idx] && other._keys_values[idx] != cur_key && idx < other._capacity) idx++;
        if (idx >= other._capacity or !other._has_value[idx]) return false;
        if (other._keys_values[idx] != cur_key or other._values[idx] != cur_value) return false;
    }
    return true;
}

template <class KeyClass, class ValueClass, class HashFunc>
void HashTable<KeyClass, ValueClass, HashFunc>::add(const KeyClass& key,const  ValueClass& val)
{
    int idx = _f(key) % _capacity;
    while (_has_value[idx] and idx < _capacity){
        if (_keys_values[idx] == key){
            _values[idx] = val;
            return;
        }
        idx++;
    }
    if (idx>=_capacity){
        _resize(_capacity*2);
        add(key, val);
        return;
    }
    _keys_values[idx] = key;
    _values[idx] = val;
    _has_value[idx] = true;
    _size++;
}

template <class KeyClass, class ValueClass, class HashFunc>
void HashTable<KeyClass, ValueClass, HashFunc>::pop(const KeyClass& key)
{
    int idx = _f(key) % _capacity;
    while (_has_value[idx] and idx <= _capacity){
        if (_keys_values[idx] == key){
            _has_value[idx] = false;
            _size--;
            return;
        }
        idx++;
    }
}

template <class KeyClass, class ValueClass, class HashFunc>
ValueClass& HashTable<KeyClass, ValueClass, HashFunc>::operator[](const KeyClass& key)
{
    int idx = _f(key) % _capacity;
    while (_has_value[idx] and idx <= _capacity){
        if (_keys_values[idx] == key){
            return _values[idx];
        }
        idx++;
    }
    throw std::invalid_argument("There is no such key in HashTable");
}

template <class KeyClass, class ValueClass, class HashFunc>
bool HashTable<KeyClass, ValueClass, HashFunc>::has(const KeyClass& key) const
{
    if (_size == 0) return false;
    int idx = _f(key) % _capacity;
    while (_has_value[idx] and idx <= _capacity){
        if (_keys_values[idx] == key){
            return true;
        }
        idx++;
    }
    return false;
}

template <class KeyClass, class ValueClass, class HashFunc>
void HashTable<KeyClass, ValueClass, HashFunc>::_resize(size_t new_capacity)
{
    auto new_has_value = new bool [new_capacity];
    auto new_keys_values = new KeyClass [new_capacity];
    auto new_values = new ValueClass [new_capacity];
    for (int i = 0; i<new_capacity; i++){
        new_has_value[i] = false;
    }
    for (int i = 0; i < _capacity; i++){
        if (!_has_value[i]) continue;
        int idx = _f(_keys_values[i]) % new_capacity;
        while (new_has_value[idx] and idx <= new_capacity){
            idx++;
        }
        new_has_value[idx] = true;
        new_keys_values[idx] = _keys_values[i];
        new_values[idx] = _values[i];
    }
    auto new_size = _size;
    clear();
    _size = new_size;
    _values = new_values;
    _keys_values = new_keys_values;
    _has_value = new_has_value;
    _capacity = new_capacity;
}

template <class KeyClass, class ValueClass, class HashFunc>
bool HashTable<KeyClass, ValueClass, HashFunc>::operator!=(const HashTable<KeyClass, ValueClass, HashFunc>& other) const
{
    return not((*this) == other);
}

template <class KeyClass, class ValueClass, class HashFunc>
void HashTable<KeyClass, ValueClass, HashFunc>::fromFile(string fileName){
    ifstream in;
    in.open(fileName);
    if (in.good()){
        clear();
        _capacity = 2;
        _has_value = new bool [_capacity];
        for (int i = 0; i<_capacity; i++){
            _has_value[i] = false;
        }
        _keys_values = new KeyClass[_capacity];
        _values = new ValueClass[_capacity];
        _size = 0;
        while(!in.eof()){
            KeyClass key;
            ValueClass val;
            in>>key;
            if (in.eof()) break;
            in>>val;
            add(key, val);
        }
    }
    in.close();
}

template <class KeyClass, class ValueClass, class HashFunc>
void HashTable<KeyClass, ValueClass, HashFunc>::toFile(string fileName){
    ofstream out;
    out.open(fileName);
    if (out.good()){
        for (int i = 0; i < _capacity; i++){
            if (!_has_value[i]) continue;
            auto a = _keys_values[i];
            auto b = _values[i];
            out<<a<<endl;
            out<<b<<endl;
        }
    }
    out.close();
}

template <class KeyClass, class ValueClass, class HashFunc>
vector<pair<KeyClass, ValueClass>> HashTable<KeyClass, ValueClass, HashFunc>::getAllValues()
{
    vector<pair<KeyClass, ValueClass>> vec;
    for (int i = 0; i < _capacity; i++){
        if (_has_value[i]){
            vec.push_back(make_pair(_values[i], _keys_values[i]));
        }
    }
    return vec;
}



#endif // HASHTABLE_CPP
