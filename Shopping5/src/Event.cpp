/**
 * @file Event.cpp
 * @author DECSAI
 * @note To be implemented by students 
 * 
 */

#include <fstream>
#include <string>
#include "Event.h"
using namespace std;

const char SEPARATOR = ',';

Event::Event() {
    initDefault();
}

void Event::initDefault() {
    set(EVENT_DEFAULT);
}

Event::Event(const string &line) {    
    set(line);
}

DateTime Event::getDateTime() const {
    return _dateTime;
}

string Event::getType() const {
    return _type;
}

string Event::getProductID() const {
    return _prod_id;
}

string Event::getCategoryID() const {
    return _cat_id;
}

string Event::getCategoryCode() const {
    return _cat_cod;
}

string Event::getBrand() const {
    return _brand;
}

double Event::getPrice() const {
    return _price;
}

string Event::getUserID() const {
    return _user_id;
}

string Event::getSession() const {
    return _session;
}

void Event::setDateTime(const std::string &time) {
    _dateTime = DateTime(time);
}

void Event::setType(const std::string &type) {
    const int NUM_VALID_TYPES = 4;
    
    bool is_valid = false;
    int pos = 0;
    
    while (pos < NUM_VALID_TYPES & !is_valid)
        if (type == VALID_TYPES[pos])
            is_valid = true;
        else
            pos++;
    
    if (is_valid)
        _type = type;
}

void Event::setProductID(const std::string &prod_id) {
    if (!prod_id.empty())
        _prod_id = prod_id;
    else
        _prod_id = EMPTY_FIELD;
 }

void Event::setCategoryID(const std::string &cat_id) {
    _cat_id = cat_id;
}

void Event::setCategoryCode(const std::string &cat_cod) {
    _cat_cod = cat_cod;
}

void Event::setBrand(const std::string &brand) {
    _brand = brand;
}

void Event::setPrice(double price) {
    if (price >= 0)
        _price = price;
    else
        _price = -1;
}

void Event::setUserID(const std::string &user_id) {
    if (!user_id.empty())
        _user_id = user_id;
    else
        _user_id = EMPTY_FIELD;
}

void Event::setSession(const std::string &session) {
    if (!session.empty())
        _session = session;
    else
        _session = EMPTY_FIELD;
}

void Event::set(const std::string &line) {
    
    int pos_ini = 0;
    int pos_fin = 0;
    
    // In order to find each data, I use the consecutive separators and
    // I consider the characters between them.
    
    pos_fin = line.find(SEPARATOR, pos_ini+1);
    string date_time = line.substr(pos_ini, (pos_fin-pos_ini));
    pos_ini = pos_fin;
    
    pos_fin = line.find(SEPARATOR, pos_ini+1);
    string type = line.substr(pos_ini+1, (pos_fin-pos_ini)-1);
    pos_ini = pos_fin;
    
    pos_fin = line.find(SEPARATOR, pos_ini+1);
    string prod_id = line.substr(pos_ini+1, (pos_fin-pos_ini)-1);
    pos_ini = pos_fin;
    
    pos_fin = line.find(SEPARATOR, pos_ini+1);
    string cat_id = line.substr(pos_ini+1, (pos_fin-pos_ini)-1);
    pos_ini = pos_fin;
    
    pos_fin = line.find(SEPARATOR, pos_ini+1);
    string cat_cod = line.substr(pos_ini+1, (pos_fin-pos_ini)-1);
    pos_ini = pos_fin;
    
    pos_fin = line.find(SEPARATOR, pos_ini+1);
    string brand = line.substr(pos_ini+1, (pos_fin-pos_ini)-1);
    pos_ini = pos_fin;
    
    pos_fin = line.find(SEPARATOR, pos_ini+1);
    string priceString = line.substr(pos_ini+1, (pos_fin-pos_ini)-1);
    double price = stod(priceString);
    pos_ini = pos_fin;
    
    pos_fin = line.find(SEPARATOR, pos_ini+1);
    string user_id = line.substr(pos_ini+1, (pos_fin-pos_ini)-1);
    pos_ini = pos_fin;
    
    string session = line.substr(pos_ini+1, line.size()-1);
        
    // Introduction of data.
    
    _dateTime = DateTime(date_time);
    _type = type;
    _prod_id = (!prod_id.empty() ? prod_id : EMPTY_FIELD);
    _cat_id = cat_id;
    _cat_cod = cat_cod;
    _brand = brand;
    _price = (price >= 0 ? price : -1.0);
    _user_id = (!user_id.empty() ? user_id : EMPTY_FIELD);
    _session = (!session.empty() ? session : EMPTY_FIELD);
}

bool Event::isEmpty() const {
    
    bool prod_id_empty = _prod_id == EMPTY_FIELD;
    bool price_empty = _price == -1;
    bool user_id_empty = _user_id == EMPTY_FIELD;
    bool session_empty = _session == EMPTY_FIELD;
    
    return (prod_id_empty || price_empty || user_id_empty || session_empty);
}

string Event::to_string() const {
    string salida;
    salida += _dateTime.to_string();
    salida += "," + _type;
    salida += "," + _prod_id;
    salida += "," + _cat_id;
    salida += "," + _cat_cod;
    salida += "," + _brand;
    salida += "," + std::to_string(_price);
    salida += "," + _user_id;
    salida += "," + _session;
    return salida;
}

std::string Event::getField(const std::string &field) const {
    string data;
    
    if (field == "DateTime")
        data = _dateTime.to_string();
    else if (field == "Type")
        data = _type;
    else if (field == "ProductID")
        data = _prod_id;
    else if (field == "CategoryID")
        data = _cat_id;
    else if (field == "CategoryCode")
        data = _cat_cod;
    else if (field == "Brand")
        data = _brand;
    else if (field == "Price")
        data = std::to_string(_price);
    else if (field == "UserID")
        data = _user_id;
    else
        data = _session;
    
    return data;
}


void Event::write(std::ostream &os) const {
    os << to_string();
}

void Event::read(std::istream &is) {
    string line;
    getline(is, line);
    
    set(line);
}