#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <functional>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

template <class KeyClass, class ValueClass, class HashFunc>
class HashTable
{
public:
    HashTable();
    HashTable(HashFunc func);
    HashTable(const HashTable& other);
    void swap(HashTable& other);
    HashTable operator=(const HashTable& other);
    ~HashTable();

    void clear();
    ValueClass& at(const KeyClass&) const;
    template<class KeysArrayType, class ValuesArrayType>
    ValuesArrayType at(const KeysArrayType&) const;
    size_t size() const;

    bool operator==(const HashTable<KeyClass, ValueClass, HashFunc>& other) const;
    bool operator!=(const HashTable<KeyClass, ValueClass, HashFunc>& other) const;
    void add(const KeyClass&,const  ValueClass&);
    void pop(const KeyClass&);
    ValueClass& operator[](const KeyClass&);
    bool has(const KeyClass&) const;

    void fromFile(std::string);
    void toFile(std::string);

    vector<pair<KeyClass, ValueClass>> getAllValues();

private:
    HashFunc _f;
    size_t _capacity;
    size_t _size;
    bool* _has_value;
    KeyClass* _keys_values;
    ValueClass* _values;
    void _resize(size_t);
};

#include "hashtable.cpp"
#endif // HASHTABLE_H
