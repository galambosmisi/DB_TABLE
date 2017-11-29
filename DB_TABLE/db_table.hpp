//
//  db_table.hpp
//  DB_TABLE
//
//  Created by Galambos Mihály on 2017. 11. 20..
//  Copyright © 2017. Galambos Mihály. All rights reserved.
//

#ifndef db_table_h
#define db_table_h

#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

///Tábla
class db_table {
private:
    map<string, vector<string>> table;                           //Adattároló
    size_t numberOfRows;                                         //Sorok száma
    
public:
    //Konstruktor
    db_table() : numberOfRows(0) {}                              //Sorok számának inicializálása
    
    //Függvények
    void add_column(string name);                                //Új (üres) oszlop hozzáadása
    void add_row();                                              //Új (üres) sor hozzáadása
    void set(string col, size_t row, string value);              //Értékadás
    size_t row_count() const;                                    //Visszaadja a sorok számát
    string get(const string col, const size_t row) const;        //Vissszadja az indexen lévő értéket
    vector<string> column_names() const;                         //Visszaadja az oszlopok neveit
    db_table join(db_table other, string cond) const;            //Összefűzi a sorokat a feltétel szerint
    
    //Segéd függvények
    bool invalidIndex(size_t, string) const;                     //Megvizsgálja, hogy létezik-e az index
    bool stringIsValid(string&) const;                           //Megvizsgálja, hogy alfanumerikus
    bool existingName(string) const;                             //Megvizsgálja, hogy létezik-e már ilyen nevü oszlop
    void addLine(size_t, db_table, db_table&) const;
};


#endif /* db_table_h */
