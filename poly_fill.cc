#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#include "bitmap.h"
#include "poly_fill.h"

#define MAX_SIZE 410

int cross(const Point &O, const Point &A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

Aet::Aet(double x, double dx, int y_max) : x(x), dx(dx), y_max(y_max), next(nullptr) {}

Aet::~Aet() {
    delete next;
}

EdgeTable::EdgeTable(int width, int height, const std::string& filename) : image(width, height, filename), active(nullptr) {
    net.resize(MAX_SIZE, nullptr);
}

EdgeTable::~EdgeTable() {
    for (auto& edge : net) {
        delete edge;
    }
    delete active;
}

void EdgeTable::putEdge(int n, Point Points[]) {
    for (int i = 0; i < n; i++) {
        Point p1 = Points[i];
        Point p2 = Points[(i + 1) % n];

        low = std::min(low, p1.y);
        high = std::max(high, p1.y);
        low = std::min(low, p2.y);
        high = std::max(high, p2.y);

        if (p1.y == p2.y) continue;

        int y_min = std::min(p1.y, p2.y);
        Aet* aet = new Aet((p1.y < p2.y) ? p1.x : p2.x, 1.0 * (p2.x - p1.x) / (p2.y - p1.y), (p1.y < p2.y) ? p2.y : p1.y);


        //append to the end of net[y_min]
        Aet* tmp = net[y_min];
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next = aet;
    }
}

void EdgeTable::insertEdge(Aet*& active, Aet* edge) {
    Aet* ep = edge->next;
    while (ep) {
        Aet* ap = active;
        while (ap->next && ep->x > ap->next->x) {
            ap = ap->next;
        }
        Aet* newNode = new Aet(ep->x, ep->dx, ep->y_max);
        Aet* tmp = ap->next;

        newNode->next = ap->next;
        ap->next = newNode;

        if (tmp && std::fabs(newNode->x - tmp->x) < 0.01 && newNode->dx > tmp->dx) {

           std::swap(newNode->dx, tmp->dx);
           std::swap(newNode->y_max, tmp->y_max);
		}

        ep = ep->next;
    }

}

void EdgeTable::scanFill(int y, pixel color) {
    Aet* ap = active;
    while (ap && ap->next && ap->next->next) {
        ap = ap->next;
        for (int x = static_cast<int>(ap->x); x <= static_cast<int>(ap->next->x); x++) {
            image.set_pixel(x, y, color);
        }
        ap = ap->next;
    }
}

void EdgeTable::updateAet(int y) {
    Aet* ap = active;
    while (ap && ap->next) {
        if (ap->next->y_max <= y) {
            Aet* temp = ap->next;
            ap->next = ap->next->next;
            temp->next = nullptr;
            delete temp;
        } else {
            ap->next->x += ap->next->dx;
            ap = ap->next;
        }
    }
}

void EdgeTable::polyFill(int n, Point Points[], pixel color) {
    for (int i = 0; i < n; i++) {
        net[Points[i].y] = new Aet();
    }
    active = new Aet();
    putEdge(n, Points);

    for (int y = low; y <= high; y++) {
        scanFill(y, color);
        if (net[y]) insertEdge(active, net[y]);
        updateAet(y);
    }

    image.write();
}
