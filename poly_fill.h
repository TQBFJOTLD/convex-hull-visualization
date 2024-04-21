#ifndef POLY_FILL_H
#define POLY_FILL_H

#include "bitmap.h"
#include <vector>
#include <string>

struct Point {
    int x, y;
    bool operator<(const Point &t) const {
        return x != t.x ? x < t.x : y < t.y;
    }
};

int cross(const Point &O, const Point &A, const Point &B);

class Aet {
public:
    double x;
    double dx;
    int y_max;
    Aet* next;

    Aet(double x = 0, double dx = 0, int y_max = 0);
    ~Aet();
};

class EdgeTable {
private:
    std::vector<Aet*> net;
    Aet* active;
    int low = 400, high = 0;
    
public:
    bitmap image;
    EdgeTable(int width, int height, const std::string& filename);
    ~EdgeTable();
    void polyFill(int n, Point Points[], pixel color);

private:
    void putEdge(int n, Point Points[]);
    void insertEdge(Aet*& active, Aet* edge);
    void scanFill(int y, pixel color);
    void updateAet(int scan);
};

#endif
