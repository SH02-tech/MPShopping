/* 
 * File:   Index.cpp
 * @author MP-Team DECSAI
 * @warning To be implemented by students 
 */
#include <string>
#include <iostream>
#include <fstream>
#include "Index.h"

using namespace std;

//Index::Index() {
//    _entries = nullptr;
//    _nEntries = 0;
//    _onBrand = 0;
//    _capacity = 0;
//}

Index::Index(int onBrand) {
    _entries = nullptr;
    _nEntries = 0;
    _onBrand = 0;
    _capacity = 0;
    setIOnWhich(onBrand);
}

Index::Index(const Index &orig) {
    _entries = nullptr;
    _nEntries = _capacity = 0;
    _onBrand = orig.getIOnWhich();
    
    copy(orig);
}

void Index::setIOnWhich(int val) {
    _onBrand = ((0<=val && val <=2) ? val : 1);
}

void Index::copy(const Index &orig) {
    deallocate();
    reallocate(orig._capacity);
    
    _onBrand = orig.getIOnWhich();
    
    for (int i=0; i<orig.size(); i++)
        add(orig.at(i));
}

void Index::copy(Pair *dest) {
    if (dest != nullptr)
        for (int i=0; i<_nEntries; i++)
            dest[i] = _entries[i];
}

void Index::reallocate(int size) {
    int newCapacity = _capacity + size;
    
    if (newCapacity > 0) {
        Pair *newEntries = nullptr;
        newEntries = new Pair[newCapacity];
        
        copy(newEntries);
        
        int nEntries = _nEntries;
        int onBrand = _onBrand;
        
        deallocate();
        _entries = newEntries;
        _nEntries = nEntries;
        _capacity = newCapacity;
        _onBrand = onBrand;
    }
}

void Index::deallocate() {
    if (_entries != nullptr) {
        delete[] _entries;
        _entries = nullptr;
        _nEntries = 0;
        _capacity = 0;
        _onBrand = 0;
    }
}

int Index::size() const {
    return _nEntries;
}

void Index::clear() {
    deallocate();
}

int Index::getIOnWhich() const {
    return _onBrand;
}

int Index::add(const Pair & pair) {   
    
    if (_nEntries == _capacity)
        reallocate(2*_capacity+1);
    
    int successful = 1;
    
    int pos = upper_bound(pair.getKey());
    
    for (int i=lower_bound(pair.getKey()); i<pos && successful == 1; i++)
        if (_entries[i].to_string().compare(pair.to_string()) == 0)
            successful = 0;
    
    if (successful == 1) {
        for (int i=_nEntries; i>pos; i--)
            _entries[i] = _entries[i-1];

        _entries[pos] = pair;
        _nEntries++;
    }
    
    return successful;
}

void Index::build(const EventSet & evSet, int onBrand) {
    clear();
    
    setIOnWhich(onBrand);
    string field;
    
    if (onBrand == 2)
        field = "Session";
    else if (onBrand == 0)
        field = "UserID";
    else
        field = "Brand";
    
    
    for (int pos=0; pos<evSet.size(); pos++) {
        Pair one(evSet.at(pos).getField(field), pos);
        
        if (field == "UserID" || field == "Session")
            add(one);
        else if (!evSet.at(pos).getBrand().empty())
            add(one);
            
    }
}

int Index::lower_bound(const string & key) const {
    int pos=0;

    while (at(pos).getKey().compare(key) < 0 && pos < size())
        pos++;
    
    return pos;
}

int Index::upper_bound(const string & key) const {
    int pos = lower_bound(key);
    
    while (at(pos).getKey().compare(key) == 0 && pos < _nEntries)
        pos++;
    
    return pos;
}

const Pair & Index::at(int pos) const {
    return (0 <= pos && pos < _nEntries ? _entries[pos] : NULL_PAIR);
}

Pair & Index::at(int pos) {
    return (0 <= pos && pos < _nEntries ? _entries[pos] : NULL_PAIR);
}

void Index::print()const {
    for (int i=0; i<_nEntries; i++)
        cout << _entries[i].to_string() << endl;
}

Index& Index::operator=(const Index &one) {
    
    if (this != &one) {
        copy(one);
    }
    
    return *this;
}

Index::~Index() {
    clear();
}

Event getEvent(const EventSet & evSet, const Index & indx, int pos) {
    return evSet.at(indx.at(pos).getPos());
}

void write(ofstream &os, const EventSet & evSet, const Index & indx) {
    for (int pos=0; pos<indx.size(); pos++)
        os << evSet.at(indx.at(pos).getPos()).to_string();
}

Index rawFilterIndex(const EventSet & evSet, const Index & indx, const std::string &field, const std::string &value) {
    Index index(indx.getIOnWhich());
    
    if (field == "DateTime") {
        for (int i=0; i<indx.size(); i++) {
            DateTime date = getEvent(evSet, indx,i).getDateTime();
            
            string evfield = to_string(date.year()) + "-" + 
                    to_string(date.month()) + "-" + to_string(date.day());

            if (date.sameDay(DateTime(value)))
                index.add(Pair(evfield, indx.at(i).getPos()));
        }
    } 
    else {
        for (int pos=0; pos<indx.size(); pos++) {
            string evfield = getEvent(evSet, indx, pos).getField(field);
            
            if (evfield == value)
                index.add(indx.at(pos));
        }
    }
        
    
    return index;
}

float sumPrice(const EventSet & evSet, const Index & indx) {
    float sum=0;
    
    for (int pos=0; pos<indx.size(); pos++) {
        float price = evSet.at(indx.at(pos).getPos()).getPrice();
        
        if (price > 0)
            sum += price;
    }
    
    return sum;
}