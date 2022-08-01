#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
#include <chrono>
#include <ctime>
#include <time.h>
#include<sstream>
#include<algorithm>
#include<stdio.h>
#include<bits/stdc++.h>
#include <chrono>
#include <ctime>
using namespace std;
class DesiredData {
public:
    string name, author, publisher;
    int month, year;
    float rating;
    DesiredData(string name, string author, string publisher, int month, int year, float rating) {
        this->name = name;
        this->author = author;
        this->publisher = publisher;
        this->month = month;
        this->year = year;
        this->rating = rating;
    }
};
struct BookInfo {
    long long int Id, pageno, PbMonth, PbDay, PbYear, CountOfReviews, CountOfTextReviews;
    string name, author, publisher, language, Rd1, Rd2, Rd3, Rd4, Rd5, RdTotal, ISBN, Description;
    float rating;
    struct BookInfo* leftchild;
    struct BookInfo* rightchild;
    BookInfo() {
        leftchild = NULL;
        rightchild = NULL;
        author = "Anonymous";
        publisher = "Anonymous";
        Description = "Not Available";
        language = "Not Mentioned";
        CountOfTextReviews = 0;
    }
};
class BookDataSet {
public:
    struct BookInfo* root;
    long long int countRecords;
    BookDataSet() {
        root = NULL;
        countRecords = 0;
    }

    long long int height(BookInfo* t) {
        long long int h = 0;
        if (t != NULL) {
            long long int l_height = height(t->leftchild);
            long long int r_height = height(t->rightchild);
            long long int max_height = max(l_height, r_height);
            h = max_height + 1;
        }
        return h;
    }
    BookInfo* insert(BookInfo* r, int id, int page, int month, int day, int year, int countrev, int countTrev, string na, string aut, string pub, string lan, string t1, string t2, string t3, string t4, string t5, string tt, string isbn, string dsp, float rat) {
        if (r == NULL) {
            r = new BookInfo;
            r->name = na;
            r->author = aut;
            r->CountOfReviews = countrev;
            r->CountOfTextReviews = countTrev;
            r->Description = dsp;
            r->Id = id;
            r->ISBN = isbn;
            r->language = lan;
            r->pageno = page;
            r->PbDay = day;
            r->PbMonth = month;
            r->PbYear = year;
            r->publisher = pub;
            r->rating = rat;
            r->Rd1 = t1;
            r->Rd2 = t2;
            r->Rd3 = t3;
            r->Rd4 = t4;
            r->Rd5 = t5;
            r->RdTotal = tt;
            countRecords++;
            return r;
        }
        else if (strcasecmp(na.c_str(), r->name.c_str()) < 0) {
            r->leftchild = insert(r->leftchild, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
        }
        else if (strcasecmp(na.c_str(), r->name.c_str()) > 0) {
            r->rightchild = insert(r->rightchild, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
        }
        return r;
    }
    void LoadDesiredData(BookInfo* r1, string s, vector<DesiredData>& temp) {
        if (r1 == NULL) {
            return;
        }
        LoadDesiredData(r1->leftchild, s, temp);
        if (r1->name.find(s) >= 0 && r1->name.find(s) < r1->name.length()) {
            temp.push_back(DesiredData(r1->name, r1->author, r1->publisher, r1->PbMonth, r1->PbYear, r1->rating));
        }
        LoadDesiredData(r1->rightchild, s, temp);
    }
    long long int countnode(BookInfo* r) {
        if (r == NULL) {
            return 0;
        }
        long long int l = countnode(r->leftchild);
        long long int r1 = countnode(r->rightchild);
        return l + r1 + 1;
    }
};
class InvalidRecord {
public:
    string name;
    bool pageno;
    bool publishingMonth;
    bool Isbn;
    bool publishingYear;
    InvalidRecord* next;
public:
    InvalidRecord(string name, bool pageno, bool month, bool Isbn, bool year) {
        this->pageno = pageno;
        this->publishingMonth = month;
        this->Isbn = Isbn;
        this->publishingYear = year;
        this->name = name;
        next = NULL;
    }
};
class InvalidList {
public:
    InvalidRecord* head;
public:
    long long int size;
    InvalidList() {
        head = NULL;
        size = 0;
    }
    void insert(string name, bool pageno, bool month, bool Isbn, bool year) {
        if (head == NULL) {
            InvalidRecord* temp = new InvalidRecord(name, pageno, month, Isbn, year);
            head = temp;
            size++;
        }
        else {
            InvalidRecord* newRecord = new InvalidRecord(name, pageno, month, Isbn, year);
            InvalidRecord* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newRecord;
            size++;
        }
    }
    void show() {
        InvalidRecord* temp = head;
        while (temp != NULL) {
            cout << temp->name << " " << temp->publishingMonth << " " << temp->Isbn << " " << temp->publishingYear << " " << temp->pageno << " " << endl;
            temp = temp->next;
        }
    }
};

class LoadDataSet {
public:
    void ReadFile1(BookDataSet& rec, InvalidList& l) {
        fstream fin1;
        fin1.open("book1-100k.csv", ios::in);
        if (fin1) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin1, use);
            long int i = 0;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i < 58292) {
                row.clear();
                getline(fin1, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[2];
                page = atoi(row[3].c_str());
                t4 = row[4];
                tt = row[5];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[9].c_str());
                year = atoi(row[10].c_str());
                if (row[11] != "") {
                    lan = row[11];
                }
                if (row[12] != "") {
                    aut = row[12];
                }
                rat = atof(row[13].c_str());
                t2 = row[14];
                t5 = row[15];
                isbn = row[16];
                t3 = row[17];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin1.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile2(BookDataSet& rec, InvalidList& l) {
        fstream fin2;
        fin2.open("book200k-300k.csv", ios::in);
        if (fin2) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin2, use);
            long int i = 0;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i < 56182) {
                row.clear();
                getline(fin2, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[9].c_str());
                row[4].erase(remove(row[4].begin(), row[4].end(), '\"'), row[4].end());
                na = row[4];
                t1 = row[11];
                page = atoi(row[6].c_str());
                t4 = row[10];
                tt = row[1];
                month = atoi(row[8].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[3].c_str());
                if (row[0] != "") {
                    pub = row[0];
                }
                countrev = atoi(row[14].c_str());
                year = atoi(row[17].c_str());
                if (row[7] != "") {
                    lan = row[7];
                }
                if (row[15] != "") {
                    aut = row[15];
                }
                rat = atof(row[5].c_str());
                t2 = row[13];
                t5 = row[2];
                isbn = row[12];
                t3 = row[16];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin2.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile3(BookDataSet& rec, InvalidList& l) {
        fstream fin3;
        fin3.open("book300k-400k.csv", ios::in);
        if (fin3) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin3, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 56587) {
                row.clear();
                getline(fin3, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[4].c_str());
                row[8].erase(remove(row[8].begin(), row[8].end(), '\"'), row[8].end());
                na = row[8];
                t1 = row[1];
                page = atoi(row[5].c_str());
                t4 = row[0];
                tt = row[15];
                month = atoi(row[12].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[14].c_str());
                if (row[17] != "") {
                    pub = row[17];
                }
                countrev = atoi(row[10].c_str());
                year = atoi(row[9].c_str());
                if (row[6] != "") {
                    lan = row[6];
                }
                if (row[3] != "") {
                    aut = row[3];
                }
                rat = atof(row[16].c_str());
                t2 = row[13];
                t5 = row[11];
                isbn = row[2];
                t3 = row[7];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin3.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile5(BookDataSet& rec, InvalidList& l) {
        fstream fin2;
        fin2.open("book500k-600k.csv", ios::in);
        if (fin2) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin2, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 54860) {
                row.clear();
                getline(fin2, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[11];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin2.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile7(BookDataSet& rec, InvalidList& l) {
        fstream fin7;
        fin7.open("book600k-700k.csv", ios::in);
        if (fin7) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin7, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i < 55157) {
                row.clear();
                getline(fin7, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin7.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile8(BookDataSet& rec, InvalidList& l) {
        fstream fin8;
        fin8.open("book700k-800k.csv", ios::in);
        if (fin8) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin8, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i < 15) {
                row.clear();
                getline(fin8, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                if (row[18] != "") {
                    dsp = row[18];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                countTrev = atoi(row[19].c_str());
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin8.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile9(BookDataSet& rec, InvalidList& l) {
        fstream fin9;
        fin9.open("book800k-900k.csv", ios::in);
        if (fin9) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin9, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 49844) {
                row.clear();
                getline(fin9, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                if (row[18] != "") {
                    countTrev = atoi(row[18].c_str());
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin9.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile10(BookDataSet& rec, InvalidList& l) {
        fstream fin10;
        fin10.open("book900k-1000k.csv", ios::in);
        if (fin10) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin10, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 40891) {
                row.clear();
                getline(fin10, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                if (row[18] != "") {
                    countTrev = atoi(row[18].c_str());
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin10.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile11(BookDataSet& rec, InvalidList& l) {
        fstream fin11;
        fin11.open("book1100k-1200k.csv", ios::in);
        if (fin11) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin11, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 41893) {
                row.clear();
                getline(fin11, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                if (row[18] != "") {
                    countTrev = atoi(row[18].c_str());
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin11.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile12(BookDataSet& rec, InvalidList& l) {
        fstream fin12;
        fin12.open("book1200k-1300k.csv", ios::in);
        if (fin12) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin12, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 43623) {
                row.clear();
                getline(fin12, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                if (row[18] != "") {
                    countTrev = atoi(row[18].c_str());
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin12.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile13(BookDataSet& rec, InvalidList& l) {
        fstream fin13;
        fin13.open("book1300k-1400k.csv", ios::in);
        if (fin13) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin13, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 38289) {
                row.clear();
                getline(fin13, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                if (row[18] != "") {
                    countTrev = atoi(row[18].c_str());
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin13.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile14(BookDataSet& rec, InvalidList& l) {
        fstream fin14;
        fin14.open("book1400k-1500k.csv", ios::in);
        if (fin14) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin14, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 38289) {
                row.clear();
                getline(fin14, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                if (row[18] != "") {
                    countTrev = atoi(row[18].c_str());
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin14.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile15(BookDataSet& rec, InvalidList& l) {
        fstream fin15;
        fin15.open("book1500k-1600k.csv", ios::in);
        if (fin15) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin15, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 33440) {
                row.clear();
                getline(fin15, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                if (row[18] != "") {
                    countTrev = atoi(row[18].c_str());
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin15.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile16(BookDataSet& rec, InvalidList& l) {
        fstream fin16;
        fin16.open("book1600k-1700k.csv", ios::in);
        if (fin16) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin16, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 32988) {
                row.clear();
                getline(fin16, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                if (row[18] != "") {
                    countTrev = atoi(row[18].c_str());
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin16.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile17(BookDataSet& rec, InvalidList& l) {
        fstream fin17;
        fin17.open("book1800k-1900k.csv", ios::in);
        if (fin17) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin17, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 38864) {
                row.clear();
                getline(fin17, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin17.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile18(BookDataSet& rec, InvalidList& l) {
        fstream fin18;
        fin18.open("book1900k-2000k.csv", ios::in);
        if (fin18) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin18, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 43562) {
                row.clear();
                getline(fin18, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin18.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile19(BookDataSet& rec, InvalidList& l) {
        fstream fin19;
        fin19.open("book2000k-3000k.csv", ios::in);
        if (fin19) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin19, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 395958) {
                row.clear();
                getline(fin19, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin19.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile20(BookDataSet& rec, InvalidList& l) {
        fstream fin20;
        fin20.open("book3000k-4000k.csv", ios::in);
        if (fin20) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin20, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 256596) {
                row.clear();
                getline(fin20, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin20.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile21(BookDataSet& rec, InvalidList& l) {
        fstream fin21;
        fin21.open("book4000k-5000k.csv", ios::in);
        if (fin21) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin21, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 280257) {
                row.clear();
                getline(fin21, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[0].c_str());
                row[1].erase(remove(row[1].begin(), row[1].end(), '\"'), row[1].end());
                na = row[1];
                t1 = row[13];
                page = atoi(row[17].c_str());
                t4 = row[10];
                tt = row[14];
                month = atoi(row[6].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[7].c_str());
                if (row[8] != "") {
                    pub = row[8];
                }
                countrev = atoi(row[15].c_str());
                year = atoi(row[5].c_str());
                if (row[16] != "") {
                    lan = row[16];
                }
                if (row[2] != "") {
                    aut = row[2];
                }
                rat = atof(row[4].c_str());
                t2 = row[12];
                t5 = row[9];
                isbn = row[3];
                t3 = row[11];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin21.close();
        }
        else {
            cout << "File not opened";
        }
    }
    void ReadFile22(BookDataSet& rec, InvalidList& l) {
        fstream fin22;
        fin22.open("book1700k-1800k.csv", ios::in);
        if (fin22) {
            vector<string> row;
            string line, word, temp, use;
            getline(fin22, use);
            long int i = 2;
            int id, page, month, day, year, countrev, countTrev = 0;
            string na, aut = "Anonymous", pub = "Anonymous", lan = "Not Mentioned", t1, t2, t3, t4, t5, tt, isbn, dsp = "Not mentioned";
            float rat;
            bool pno, mon, Isb, y;
            while (i <= 32106) {
                row.clear();
                getline(fin22, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    row.push_back(word);
                }
                id = atoi(row[3].c_str());
                row[5].erase(remove(row[5].begin(), row[5].end(), '\"'), row[5].end());
                na = row[5];
                t1 = row[11];
                page = atoi(row[17].c_str());
                t4 = row[14];
                tt = row[16];
                month = atoi(row[7].c_str());
                if (month > 12 && month <= 25) {
                    month = 5;
                }
                day = atoi(row[6].c_str());
                if (row[9] != "") {
                    pub = row[9];
                }
                countrev = atoi(row[1].c_str());
                year = atoi(row[8].c_str());
                if (row[4] != "") {
                    lan = row[4];
                }
                if (row[0] != "") {
                    aut = row[0];
                }
                rat = atof(row[10].c_str());
                t2 = row[12];
                t5 = row[15];
                isbn = row[2];
                t3 = row[13];
                if (month > 25 || isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length()) || page <= 0 || year > 2022) {
                    if (month > 25) {
                        mon = true;
                    }
                    else {
                        mon = false;
                    }
                    if (page <= 0) {
                        pno = true;
                    }
                    else {
                        pno = false;
                    }
                    if (isbn == "" || (isbn.find('X') >= 0 && isbn.find('X') < isbn.length())) {
                        Isb = true;
                    }
                    else {
                        Isb = false;
                    }
                    if (year > 2022) {
                        y = true;
                    }
                    else {
                        y = false;
                    }
                    l.insert(na, pno, mon, Isb, y);
                }
                else {
                    rec.root = rec.insert(rec.root, id, page, month, day, year, countrev, countTrev, na, aut, pub, lan, t1, t2, t3, t4, t5, tt, isbn, dsp, rat);
                }
                i++;
            }
            fin22.close();
        }
        else {
            cout << "File not opened";
        }
    }
};
void MainMenu(BookDataSet& V1, InvalidList& L1);
void InvalidData(InvalidList& L1);
void UsefulData(BookDataSet& V1);
void FilteringWrsMonth(vector<DesiredData>& record);
void FilteringWrsYear(vector<DesiredData>& record);
void FilteringWrsRating(vector<DesiredData>& record);
bool compareRating(const DesiredData& b1, const DesiredData& b2) {
    return b1.rating < b2.rating;
}
bool compareMonth(const DesiredData& b1, const DesiredData& b2) {
    return b1.month < b2.month;
}
bool compareYear(const DesiredData& b1, const DesiredData& b2) {
    return b1.year < b2.year;
}
int main() {
    LoadDataSet Loader;
    BookDataSet Set;
    InvalidList Inv;
    chrono::time_point<std::chrono::system_clock> start, end, temp_start, temp_end;
    system("cls");
    cout << "\n\nDATA SET LOADING\n\n";
    start = std::chrono::system_clock::now();
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile1(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 1: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;
    cout << "Data of file 1 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile2(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 2: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;
    cout << "Data of file 2 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile3(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 3: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;
    cout << "Data of file 3 loaded perfectly\n";

    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile5(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 5: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;
    cout << "Data of file 5 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile7(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 7: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;
    cout << "Data of file 7 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile8(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 8: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 8 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile9(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 9: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 9 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile10(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 10: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 10 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile11(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 11: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 11 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();

    Loader.ReadFile12(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 12: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 12 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();

    Loader.ReadFile13(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 13: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "Data of file 13 loaded perfectly\n";
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;


    Loader.ReadFile14(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 14: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 14 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile15(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 15: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 15 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile16(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 16: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;


    cout << "Data of file 16 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile17(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 17: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;


    cout << "Data of file 17 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile18(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 18: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 18 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile19(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 19: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 19 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile20(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 20: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 20 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();
    Loader.ReadFile21(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 21: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 21 loaded perfectly\n";
    temp_start = std::chrono::system_clock::now();

    Loader.ReadFile22(Set, Inv);
    temp_end = std::chrono::system_clock::now();
    cout << "\n\nTime taken to load file 22: " << std::chrono::duration_cast<std::chrono::minutes>(temp_end - temp_start).count() << " minutes" << endl;
    cout << "time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(temp_end - temp_start).count() << endl;

    cout << "Data of file 22 loaded perfectly\n";

    end = std::chrono::system_clock::now();
    cout << "\n\n\n\n\tALL DataSet loaded\n";
    chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    cout << "\n\n\t\tfinished computation at " << std::ctime(&end_time);
    cout << "\n\nTime taken to load all file : " << std::chrono::duration_cast<std::chrono::minutes>(end - start).count() << " minutes" << endl << "Total elapsed time in seconds: " << elapsed_seconds.count() << "s ";
    cout << "\n\n\nTotal nodes: " << Set.countnode(Set.root) << endl;
    cout << "\n\nTotal Garbage Size: " << Inv.size;
    cout << "\n\nTotal Valid Data by inv-size: " << Set.countRecords;
    cout << "\n\nHeight of the tree: " << Set.height(Set.root);

    // Inv.show();garbage data print
    cout << "\n\n\t\t\nPress any key to continue\n";
    getchar();
    system("cls");
    while (1) {
        MainMenu(Set, Inv);
        system("cls");
    }
}
void MainMenu(BookDataSet& V1, InvalidList& L1) {
    int n;
    cout << endl << endl << "        What do you want  ???" << endl << endl;
    cout << "  (1)       Have a look on Useful Data(After Cleaned) " << endl;
    cout << "  (2)       Invalid  Records(Garbage) " << endl;
    cout << "  (3)       Exit \n" << endl;
    cout << "       Enter your choice :";
    cin >> n;
    if (n == 1) {
        UsefulData(V1);
    }
    else if (n == 2) {
        InvalidData(L1);
    }
    else if (n == 3) {
        exit(0);
    }
    else {
        cout << "Invalid Choice" << endl;
        cout << "Press any key to continue";
        getchar();
        system("cls");
        MainMenu(V1, L1);
    }
}
void InvalidData(InvalidList& L1) {
    cout << endl;
    system("pause");
    system("cls");
    int c;
    cout << endl << endl << endl << "     (1)     Books With Invalid ISBN " << endl;
    cout << "     (2)     Books with Invalid Month " << endl;
    cout << "     (3)     Books with Invalid Page No" << endl << endl;
    cout << endl << "       Enter your choice :";
    cin >> c;
    cout << endl;
    system("pause");
    system("cls");
    if (c == 1) {
        cout << endl << endl;
        int count = 0, i = 0;
        InvalidRecord* temp = L1.head;

        clock_t start;
        start = clock();
        while (temp != NULL) {

            if (temp->Isbn == true) {
                if (count < 7) {
                    cout << "    Name :" << temp->name << endl;
                    count++;
                }
                i++;
            }
            temp = temp->next;
        }
        if (i == 0) {
            cout << "No such record found " << endl;
        }
        if (i > 7) {
            cout << endl << "........." << i - 7 << " more books";
        }
        start = clock() - start;
        cout << "\nTime taken for searching in Linked List: " << (float)start / CLOCKS_PER_SEC << " seconds\n";
    }
    else if (c == 3) {
        cout << endl << endl;
        int count = 0, i = 0;
        InvalidRecord* temp = L1.head;
        clock_t start;
        start = clock();
        while (temp != NULL) {

            if (temp->pageno == true) {
                if (count < 7) {
                    cout << "    Name :" << temp->name << endl;
                    count++;
                }
                i++;
            }
            temp = temp->next;
        }
        if (i == 0) {
            cout << "No such record found " << endl;
        }
        if (i > 7) {

            cout << endl << "........." << i - 7 << " more books";

            start = clock() - start;
            cout << "\nTime taken for searching in Linked List: " << (float)start / CLOCKS_PER_SEC << " seconds\n";

        }
    }
    else if (c == 2) {
        cout << endl << endl << endl;
        int count = 0, i = 0;
        InvalidRecord* temp = L1.head;

        clock_t start;
        start = clock();
        while (temp != NULL) {

            if (temp->publishingMonth == true) {
                if (count < 7) {
                    cout << "    Name :" << temp->name << endl;
                    count++;
                }
                i++;
            }
            temp = temp->next;
        }
        if (i == 0) {
            cout << "No such record found " << endl;
        }
        if (i > 7) {
            cout << endl << "........." << i - 7 << " more books";
            start = clock() - start;
            cout << "\nTime taken for searching in Linked List: " << (float)start / CLOCKS_PER_SEC << " seconds\n";

        }
    }
    cout << endl << endl << endl;;
    system("pause");
}
void UsefulData(BookDataSet& V1) {
    chrono::time_point<std::chrono::system_clock> start, end, temp_start, temp_end;

    vector<DesiredData> record;
    string n;
    int i;
    system("cls");
    cout << endl << "   Entered Book to be searched :";
    cin >> n;
    cout << endl;
    clock_t start1;
    start1 = clock();

    V1.LoadDesiredData(V1.root, n, record);
    if (record.size() != 0) {

        cout << "Suggestions:" << endl << endl;
        for (i = 0; i < record.size(); i++) {
            if (i < 10) {
                cout << i + 1 << ") " << record[i].name << endl;
            }
            else {
                break;
            }
        }
        if (i != record.size()) {
            cout << endl << endl << ".........." << record.size() - i << " more books" << endl << endl;
        }
        cout << endl;
        start1 = clock() - start1;
        cout << "\nTime taken for Suggestions:  " << (float)start1 / CLOCKS_PER_SEC << " seconds\n";

        system("pause");
        system("cls");
        cout << endl << endl << "          Need much more filterating in data ???????" << endl << endl;
        cout << "    In which aspect ??" << endl << endl;
        int c;
        cout << "   1) Publishing Month " << endl;
        cout << "   2) Publishing Year  " << endl;
        cout << "   3) Rating " << endl;
        cout << endl << endl << "Enter your choice :";
        cin >> c;
        if (c == 1) {
            cout << endl << endl;
            system("pause");
            FilteringWrsMonth(record);
        }
        else if (c == 2) {
            cout << endl << endl;
            system("pause");
            FilteringWrsYear(record);
        }
        else {
            cout << endl << endl;
            system("pause");
            FilteringWrsRating(record);
        }
    }
    else {
        cout << endl << "No record Found " << endl << endl;
        return;
        system("pause");
        system("cls");
    }
}
void FilteringWrsMonth(vector<DesiredData>& record) {
    int i;
    system("cls");
    cout << endl << endl << "    (1)  Most Recent Book " << endl;
    cout << endl << "    (2)  Required Publishing Month" << endl;
    cout << endl << "    (3)  Month_wise Sorting" << endl << endl;
    cout << "    Enter your choice :";
    cin >> i;
    system("cls");
    if (i == 2) {
        int z, i;
        int m;
        cout << endl << endl << "       Enter month :";
        cin >> m;

        clock_t start;
        start = clock();
        cout << endl << endl;
        int counter = 0;
        for (int z = 0; z < record.size(); z++) {
            if (z < 5) {
                if (record[z].month == m) {
                    cout << z + 1 << ") Name:" << record[z].name << endl;
                    cout << "       Month:" << record[z].month << endl << endl;
                    counter++;
                }
            }
            else {
                break;
            }
        }
        if (counter == 0) {
            cout << endl << "No such Record" << endl << endl;
        }
        if (counter > 5) {
            cout << endl << ".........." << counter - 5 << " more books" << endl << endl;
        }
        start = clock() - start;
        cout << "\nTime taken for searching: " << (float)start / CLOCKS_PER_SEC << " seconds\n";
    }
    else if (i == 1) {
        string na;
        int month;

        clock_t start;
        start = clock();
        na = record[0].name;
        month = record[0].month;
        for (int i = 0; i < record.size(); i++) {
            if (month < record[i].month) {
                na = record[i].name;
                month = record[i].month;
            }
        }
        cout << endl << endl << endl << "    Name: " << na << endl << endl;
        cout << "    Month: " << month;
        start = clock() - start;
        cout << "\nTime taken for searching: " << (float)start / CLOCKS_PER_SEC << " seconds\n";
    }
    else {

        long long int s = record.size();

        sort(record.begin(), record.end(), &compareMonth);
        int i;

        clock_t start;
        start = clock();
        cout << endl << endl << endl;
        for (int i = 0; i < record.size(); i++) {
            if (i < 10) {
                cout << "   Name: " << record[i].name << endl;
                cout << "   Month: " << record[i].month << endl;
            }
            else {
                break;
            }
        }
        if (i != record.size()) {
            cout << endl << endl << ".........." << record.size() - i << " more books" << endl << endl;
        }
        cout << endl;
        start = clock() - start;
        cout << "\nTime taken for sorting: " << (float)start / CLOCKS_PER_SEC << " seconds\n";
        if (i != record.size()) {

            cout << endl << ".........." << s - i << " more books " << endl;

        }
    }


    cout << endl;
    system("pause");
    return;
}
void FilteringWrsYear(vector<DesiredData>& record) {
    int i;
    system("cls");
    cout << endl << endl << "    (1)  Most Recent Book " << endl;
    cout << endl << "    (2)  Required Publishing Year" << endl << endl;
    cout << endl << "    (3)  Year-Wise Sorting " << endl << endl;
    cout << "Enter your choice :";
    cin >> i;
    cout << endl;
    system("pause");
    system("cls");
    if (i == 2) {
        int z, i;
        int m;
        cout << endl << endl << "    Enter year :";
        cin >> m;

        clock_t start;
        start = clock();
        cout << endl;
        int counter = 0;
        for (int z = 0; z < record.size(); z++) {
            if (z < 5) {
                if (record[z].year == m) {
                    cout << z + 1 << ") Name:" << record[z].name << endl;
                    cout << "       Month:" << record[z].year << endl << endl;
                    counter++;
                }
            }

            else {

                break;

            }

        }

        if (counter == 0) {
            cout << endl << "No such Record" << endl << endl;
        }
        if (counter > 5) {
            cout << endl << ".........." << counter - 5 << " more books" << endl << endl;
        }
        start = clock() - start;
        cout << "\nTime taken for searching: " << (float)start / CLOCKS_PER_SEC << " seconds\n";

    }
    else if (i == 1) {
        string na;
        int year;

        na = record[0].name;
        year = record[0].year;
        for (int i = 0; i < record.size(); i++) {
            if (year < record[i].year) {
                na = record[i].name;
                year = record[i].year;
            }
        }
        cout << endl << endl << "    Name: " << na << endl << endl;
        cout << "    Year: " << year;
    }
    else {
        long int s = record.size();
        sort(record.begin(), record.end(), &compareYear);
        long int i;
        cout << endl << endl;
        for (int i = 0; i < record.size(); i++) {
            if (i < 10) {
                cout << "   Name: " << record[i].name << endl;
                cout << "   year: " << record[i].year << endl;
            }
            else {
                break;
            }
        }
        if (i != record.size()) {
            cout << endl << ".........." << s - i << " more books " << endl;
        }
    }
    cout << endl;
    system("pause");
    return;
}
void FilteringWrsRating(vector<DesiredData>& record) {
    system("cls");
    int c;
    system("cls");
    cout << endl << endl << endl << "     (1)  Highest Rating Book " << endl;
    cout << "     (2)  Lowest Rating Book  " << endl;
    cout << "     (3)  Book Of Specific Rating  " << endl << endl;
    cout << "    Enter your Choice :";
    cin >> c;
    sort(record.begin(), record.end(), &compareRating);
    if (c == 1) {
        cout << "    Name: " << record[record.size() - 1].name << endl;
        cout << "    Rating " << record[record.size() - 1].rating << endl << endl;
    }
    else if (c == 2) {
        cout << "    Name: " << record[0].name << endl;
        cout << "    Rating " << record[0].rating << endl << endl;
    }
    else {
        float f;
        cout << "Enter rating :";
        cin >> f;
        int first = 0, middle = 0, last = record.size() - 1;
        while (first <= last) {
            middle = (first + last) / 2;
            if (record[middle].rating < f) {
                first = middle + 1;
            }
            else if (record[middle].rating == f) {
                cout << endl << "   Name: " << record[middle].name << endl;
                cout << "    Rating:" << record[middle].rating << endl;
                break;
            }
            else {
                last = middle - 1;
            }
        }
        if (first > last) {
            cout << endl << "    No such Record " << endl;
        }
    }
    system("pause");
    return;
}
