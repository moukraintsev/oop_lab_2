#include "mainwindow.h"
#include <QApplication>

#include "hashtable.h"
#include "hashtable.cpp"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    HashTable<string, int, std::hash<string>> h;
    assert(h.size() == 0);
    h.add("1", 1);
    assert(h.size() == 1);
    h.add("1", 2);
    assert(h.size() == 1);
    assert(h.has("1"));
    assert(h["1"] == 2);
    assert(h.at("1") == 2);
    h.add("3", 3);
    h.pop("1");
    assert(h.size() == 1);
    assert(!h.has("1"));
    h.pop("3");
    HashTable<string, int, std::hash<string>> h1;
    assert(h == h1);
    h.add("1", 1);
    h.add("3", 3);
    HashTable<string, int, std::hash<string>> h2(h);
    assert(h2 == h);
    h["1"] = 10;
    assert(h2 != h);
    vector<string> s = {"1", "3"};
    auto p = h.at<vector<string>, vector<int>>(s);
    assert(p[0] == 10 and p[1] == 3);
    std::string path = "/home/feodosii/1.txt";
    h.toFile(path);
    h1.clear();
    h1.fromFile(path);
    assert(h == h1);
    h.clear();
    h.clear();
    h.clear();
    h.fromFile(path);
    std::cout<<"All tests are passed"<<std::endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
