/* 
 * File:   EventSet.cpp
 * @author MP-Team DECSAI
 * @note To be implemented by students 
 */

#include <fstream>
#include <unordered_map>
#include "EventSet.h"

using namespace std;

Event NULL_EVENT=Event();


/**
 * @brief Give string[] and a string, it searches for string within string[]
 * @param array     The set of strings to llok for
 * @param n_array   The size of string[]
 * @param name      The string that is being seeked
 * @return          The position of the first occurrence of string within string[]
 *  -1 if the string does not belong to string[]
 */
int findString(const string array[], int n_array, const string & name) {
    int pos = 0;
    
    while (!(array[pos] == name) && pos < n_array)
        pos++;
        
    if (pos == n_array)
        pos = -1;
    
    return pos;
}

void EventSet::reallocate(unsigned size) {
    
    // We shall consider size as the new positions to reallocate.
    
    unsigned totalCapacity = _nEvents + size;
    
    if (totalCapacity > 0) {
        Event *newSet = nullptr;
        newSet = new Event[totalCapacity];

        copy(newSet);
        
        int nEvents = _nEvents;
        
        deallocate();
        _events = newSet;
        _nEvents = nEvents;
        _capacity = totalCapacity;
        
        newSet = nullptr;
    }
}

void EventSet::deallocate() {
    // Free memory when necessary.
    if (_events != nullptr) {
        delete[] _events;
        _events = nullptr;
        _nEvents = _capacity = 0;
    }
}

void EventSet::copy(Event *dest) const {
    if (dest != nullptr) {
        for (int i=0; i<size(); i++)
            dest[i] = at(i);
    }
        
}

void EventSet::copy(const EventSet &orig) {
    deallocate();
    reallocate(orig._capacity);
    
    for (int i=0; i<orig.size(); i++)
        add(orig.at(i));
}

std::string EventSet::to_string() const {
    string result;
    
    result = std::to_string(_nEvents) + " ";
    
    for (int i = 0; i < _nEvents; i++)
        result += _events[i].to_string() + "\n";
    
    return result;
}

EventSet::EventSet() {
    _events = nullptr;
    _nEvents = 0;
    _capacity = 0;
}

EventSet::EventSet(const EventSet &orig) {
    this->_events = nullptr;
    this->_nEvents = this->_capacity = 0;
    copy(orig);
}

EventSet::~EventSet() {
    clear();
}

EventSet&  EventSet::operator=(const EventSet &one) {
    if (&one != this) {
        this->copy(one);
    }    
    
    return *this;
}

int EventSet::size() const {
    return _nEvents;
}

void EventSet::clear() {
    deallocate();
}

int EventSet::add(const Event & e) {
    
    if (_nEvents == _capacity)
        reallocate(2*_capacity+1);
        
    _events[_nEvents] = e;
    
    _nEvents++;
    
    return 1;
}

int EventSet::add(const std::string& line) {
    return (add(Event(line)));
}

const Event & EventSet::at(int pos) const {
    
    if (0 <= pos && pos < _nEvents)
        return _events[pos];
    else
        return NULL_EVENT;
}


EventSet rawFilterEvents(const EventSet& es, const std::string &field, const std::string &value) {
    EventSet matchedEvents;
    
    if (field != "DateTime")
        for (int i=0; i<es.size(); i++)
            if (es.at(i).getField(field) == value)
                matchedEvents.add(es.at(i));
            else {
                
            }
    else
        for (int i=0; i<es.size(); i++)
            if (es.at(i).getDateTime().sameDay(DateTime(value)))
                matchedEvents.add(es.at(i));
    
    return matchedEvents;
}

void EventSet::write(ofstream &os) const {
    string result;
    
    for (int i = 0; i < _nEvents; i++)
        result += _events[i].to_string() + "\n";
    
    os << result << endl;
}

bool EventSet::read(ifstream &is, int nelements) {
    bool isCorrect = true;
    
    clear();
    reallocate(nelements);
    
    int l2Read = 0;
    
    while (l2Read < nelements && isCorrect) {
        Event e;
        
        e.read(is);
        
        if (is) {
            add(e);
            l2Read++;
        }
        else
            isCorrect = false;
    }
    
    
    return isCorrect;
}	

void findUnique(const EventSet& es, const std::string &field, std::string values[], int & n_values) {
    EventSet auxES;
    
    
    if (field == "DateTime") {
        for (int i=0; i<es.size(); i++) {
            Event one = es.at(i);
            DateTime date = one.getDateTime();
            string line;
        
            line += to_string(date.year()) + "-" + to_string(date.month());
            line += "-" + to_string(date.day()) + " 00:00:00 UTC";
            
            one.setDateTime(line);

            auxES.add(one);
        }
    } else {
        auxES = es;
    }
    
    n_values = 0;

    for (int i=0; i<auxES.size(); i++) {
        std::string data = auxES.at(i).getField(field);

        if (findString(values, n_values, data) == -1) {
            values[n_values] = data;
            n_values++;
        }
    }
}

float sumPrice(const EventSet & evSet) {
    float total = 0;
    
    for (int i=0; i<evSet.size(); i++) {
        float price = evSet.at(i).getPrice();
        
        if (price > 0)
            total += price;
    }
    
    return total;
}

