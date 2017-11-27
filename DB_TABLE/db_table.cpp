//
//  main.cpp
//  DB_TABLE
//
//  Created by Galambos Mihály on 2017. 11. 20..
//  Copyright © 2017. Galambos Mihály. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "db_table.hpp"
#include "tableexception.hpp"
#include "test.hpp"

using namespace std;

//Ezt az internetről szereztem :))) 
template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}


//Függvények
void db_table::add_column(string name) {
    if(!stringIsValid(name) || name == "" || existingName(name)) throw(invalid_column_name());
    
    table[name] = vector<string>(numberOfRows, "");
}


vector<string> db_table::column_names() const {
    vector<string> names;
    
    for(const auto &i: table) {
        names.insert(names.begin(), i.first);
    }

    return names;
}


void db_table::add_row() {
    for(auto &i: table) {
        i.second.push_back("");
    }
    numberOfRows++;
}


size_t db_table::row_count() const {
    return numberOfRows;
}


void db_table::set(string col, size_t row, string value) {
    if(invalidIndex(row, col)) throw(invalid_index());
    
    size_t s = value.find('\0');
    if(s != string::npos) throw(invalid_value());
    
    table[col][row] = value;
}


string db_table::get( string col, size_t row) const {
    if(invalidIndex(row, col)) throw(invalid_index());
    
    return table.at(col)[row];
}


db_table db_table::join(db_table other, string cond) const {
    if(cond.find("=") == string::npos) throw(invalid_condition());
    
    vector<string> thisTable;
    vector<string> otherTable;
    
    size_t idx;                     //Egyenlőségjel indexe
    string sh;                      //Segéd string
    stringstream ss;                //Segéd stringstream
    ss<<cond;
    while(getline(ss, sh, ',')) {
        idx = sh.find("=");
        
        string A = sh.substr(0,idx);
        string B = sh.substr(idx+1);
        
        if(existingName(A)) {
            thisTable.push_back(A);
        } else if(other.existingName(B)) {
            otherTable.push_back(B);
        } else {
            throw (invalid_condition());
        }
    }
    db_table newTable;
    
    return newTable;
}


//Segéd függvények
bool db_table::invalidIndex(size_t row, string col) const {
    auto search = table.find(col);
    return (search == table.end() || row > numberOfRows);
}


bool db_table::stringIsValid(string &str) const {
    return find_if(str.begin(), str.end(), [](char c){ return !(isalnum(c));} ) == str.end();
}


bool db_table::existingName(string name) const {
    bool contain = false;
    for(unsigned i=0; i<column_names().size(); i++) {
        if(column_names()[i] == name) contain = true;
    }
    return contain;
}


//Unit test
int main(int argc, char *argv[]) {
    int test_no = 0;
    int counter = 0;
    int total = 0;
    srand(time(nullptr));
    if (argc == 2) {
        test_no = std::atoi(argv[1]);
    }
    
    using namespace std;
    
    TEST("create an empty table", 5) {
        db_table t;
    }
    
    TEST("add good columns", 10) {
        db_table t;
        vector<string> cn {"asd", "AsDf"};
        t.add_column("asd");
        t.add_column("AsDf");
        CHECK_EQ(cn, t.column_names());
    }
    
    TEST("add bad columns", 5) {
        db_table t;
        CHECK_EXC(invalid_column_name, t.add_column("i'm invalid"));
        CHECK_EXC(invalid_column_name, t.add_column("=lllll"));
        CHECK_EXC(invalid_column_name, t.add_column(",kkkk"));
        CHECK_EXC(invalid_column_name, t.add_column(""));
        t.add_column("a");
        CHECK_EXC(invalid_column_name, t.add_column("a"));
    }
    
    TEST("add rows", 5) {
        db_table t;
        t.add_row();
        t.add_row();
        CHECK_EQ(2, t.row_count());
        t.add_column("aaa");
        t.add_row();
        CHECK_EQ(3, t.row_count());
    }
    
    TEST("set/get values with good index", 10) {
        db_table t;
        t.add_column("col1");
        t.add_column("col2");
        t.add_row();
        t.add_row();
        t.set("col1", 0, "a");
        t.set("col2", 1, "a+b");
        CHECK_EQ("", t.get("col2", 0));
        CHECK_EQ("", t.get("col1", 1));
        CHECK_EQ("a", t.get("col1", 0));
        CHECK_EQ("a+b", t.get("col2", 1));
        t.set("col1", 0, "x");
        CHECK_EQ("x", t.get("col1", 0));
        t.add_column("col3");
        CHECK_EQ("", t.get("col3", 0));
        CHECK_EQ("", t.get("col3", 1));
    }
    
    TEST("set/get values with bad index", 5) {
        db_table t;
        t.add_column("col1");
        t.add_row();
        t.add_row();
        CHECK_EXC(invalid_index, t.set("col2", 0, "e"));
        CHECK_EXC(invalid_index, t.get("col2", 0));
        CHECK_EXC(invalid_index, t.set("col1", 10, "e"));
        CHECK_EXC(invalid_index, t.get("col1", 10));
        CHECK_EXC(invalid_index, t.set("col+", 0, "e"));
        CHECK_EXC(invalid_index, t.get("col+", 0));
    }
    
    TEST("set bad value", 5) {
        db_table t;
        t.add_column("col1");
        t.add_row();
        std::string str = "asd";
        str.push_back('\0');
        str += "asd";
        CHECK_EXC(invalid_value, t.set("col1", 0, str));
    }
    
    TEST("simple join", 15) {
        db_table t1;
        t1.add_column("1col1");
        t1.add_column("1col2");
        t1.add_row();
        t1.add_row();
        t1.set("1col1", 0, "a");
        t1.set("1col2", 0, "b");
        t1.set("1col1", 1, "c");
        t1.set("1col2", 1, "d");
        db_table t2;
        t2.add_column("2col1");
        t2.add_column("2col2");
        t2.add_row();
        t2.add_row();
        t2.set("2col1", 0, "b");
        t2.set("2col2", 0, "b");
        t2.set("2col1", 1, "y");
        t2.set("2col2", 1, "z");
        
        db_table t3 = t1.join(t2, "1col2=2col2");
        CHECK_EQ(4, t3.column_names().size());
        CHECK_EQ(1, t3.row_count());
        CHECK_EQ("a", t3.get("1col1", 0));
        CHECK_EQ("b", t3.get("1col2", 0));
        CHECK_EQ("b", t3.get("2col1", 0));
        CHECK_EQ("b", t3.get("2col2", 0));
        
        db_table t4 = t1.join(t2, "1col2=2col2,1col2=2col1");
        CHECK_EQ(4, t4.column_names().size());
        CHECK_EQ(1, t4.row_count());
        CHECK_EQ("a", t4.get("1col1", 0));
        CHECK_EQ("b", t4.get("1col2", 0));
        CHECK_EQ("b", t4.get("2col1", 0));
        CHECK_EQ("b", t4.get("2col2", 0));
        
        db_table t5 = t1.join(t2, "1col1=2col2,1col2=2col2,1col1=2col1");
        CHECK_EQ(4, t5.column_names().size());
        CHECK_EQ(0, t5.row_count());
    }
    
    TEST("more complex join", 20) {
        db_table a, b;
        a.add_column("a1");
        a.add_column("a2");
        a.add_column("a3");
        b.add_column("b1");
        b.add_column("b2");
        b.add_column("b3");
        a.add_row();
        a.add_row();
        a.add_row();
        a.add_row();
        b.add_row();
        b.add_row();
        b.add_row();
        b.add_row();
        a.set("a1", 0, "pata");
        a.set("a2", 0, "kapa");
        a.set("a3", 0, "papa");
        a.set("a1", 1, "pata");
        a.set("a2", 1, "pete");
        a.set("a3", 1, "teve");
        a.set("a1", 2, "teve");
        a.set("a2", 2, "tele");
        a.set("a3", 2, "fele");
        a.set("a1", 3, "tere");
        a.set("a2", 3, "fere");
        a.set("a3", 3, "fele");
        b.set("b1", 0, "pata");
        b.set("b2", 0, "totĂł");
        b.set("b3", 0, "kapa");
        b.set("b1", 1, "pata");
        b.set("b2", 1, "mohĂł");
        b.set("b3", 1, "kapa");
        b.set("b1", 2, "tere");
        b.set("b2", 2, "jojĂł");
        b.set("b3", 2, "fere");
        b.set("b1", 3, "hohĂł");
        b.set("b2", 3, "hihi");
        b.set("b3", 3, "hehe");
        
        db_table c = a.join(b, "a1=b1,a2=b3");
        CHECK_EQ(6, c.column_names().size());
        CHECK_EQ(3, c.row_count());
        bool ok1 = false, ok2 = false, ok3 = false;
        std::cout << "looking for row 1... ";
        for (size_t i = 0; i<3; ++i) {
            bool match = true;
            match &= "pata" == c.get("a1", i);
            match &= "kapa" == c.get("a2", i);
            match &= "papa" == c.get("a3", i);
            match &= "pata" == c.get("b1", i);
            match &= "totĂł" == c.get("b2", i);
            match &= "kapa" == c.get("b3", i);
            ok1 |= match;
        }
        CHECK_EQ(true, ok1);
        std::cout << "ok" << std::endl;
        std::cout << "looking for row 2... ";
        for (size_t i = 0; i<3; ++i) {
            bool match = true;
            match &= "pata" == c.get("a1", i);
            match &= "kapa" == c.get("a2", i);
            match &= "papa" == c.get("a3", i);
            match &= "pata" == c.get("b1", i);
            match &= "mohĂł" == c.get("b2", i);
            match &= "kapa" == c.get("b3", i);
            ok2 |= match;
        }
        CHECK_EQ(true, ok2);
        std::cout << "ok" << std::endl;
        std::cout << "looking for row 3... ";
        for (size_t i = 0; i<3; ++i) {
            bool match = true;
            match &= "tere" == c.get("a1", i);
            match &= "fere" == c.get("a2", i);
            match &= "fele" == c.get("a3", i);
            match &= "tere" == c.get("b1", i);
            match &= "jojĂł" == c.get("b2", i);
            match &= "fere" == c.get("b3", i);
            ok3 |= match;
        }
        CHECK_EQ(true, ok3);
        std::cout << "ok" << std::endl;
    }
    
    TEST("simple yet tricky join", 10) {
        db_table t1, t2;
        t1.add_column("a");
        t1.add_column("b");
        t1.add_row();
        t2.add_column("c");
        t2.add_column("d");
        t2.add_row();
        t1.set("a", 0, "q");
        t1.set("b", 0, "qq");
        t2.set("c", 0, "qq");
        t2.set("d", 0, "q");
        db_table t3 = t1.join(t2, "a=c,b=d");
        CHECK_EQ(0, t3.row_count());
    }
    
    TEST("bad join conditions", 10) {
        db_table t1, t2, t3;
        t1.add_column("a");
        t1.add_column("b");
        t2.add_column("c");
        t3.add_column("a");
        t3.add_column("d");
        CHECK_EXC(invalid_condition, t1.join(t2, "a=b,"));
        CHECK_EXC(invalid_condition, t1.join(t2, "a=d"));
        CHECK_EXC(invalid_condition, t1.join(t2, "c=a"));
        CHECK_EXC(invalid_condition, t1.join(t2, "a=c,b"));
        CHECK_EXC(invalid_condition, t1.join(t2, "a=c,,b=c"));
        CHECK_EXC(invalid_condition, t1.join(t2, "a==c"));
        CHECK_EXC(invalid_condition, t1.join(t2, "a,a=c"));
        CHECK_EXC(invalid_condition, t1.join(t3, "a=d,b=a"));
    }
    
    std::cout << ">>> TOTAL POINTS: " << total << std::endl;
    return 0;
}


