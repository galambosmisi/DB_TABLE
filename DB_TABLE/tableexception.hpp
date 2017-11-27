//
//  tableexception.hpp
//  DB_TABLE
//
//  Created by Galambos Mihály on 2017. 11. 27..
//  Copyright © 2017. Galambos Mihály. All rights reserved.
//

#ifndef tableexception_h
#define tableexception_h

#include <exception>

class invalid_index : public exception
{
public:
    const char* what() const throw() { return "Nincs ilyen oszlop!"; }
};

class invalid_value : public exception
{
public:
    const char* what() const throw() { return "Tartalmaz '\0' karakter!"; }
};

class invalid_column_name : public exception
{
public:
    const char* what() const throw() { return "Már létezik ilyen név, vagy tartalmazza '\0' karaktert, vagy üres"; }
};

class invalid_condition : public exception
{
public:
    const char* what() const throw() { return "Rosszul formázott feltétel, vagy nem létező oszlopnévre hivatkozik, vagy a két tábla tartalmaz azonos nevű oszlopot"; }
};

#endif /* tableexception_h */
