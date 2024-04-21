#include "bitmap.h"
#include "poly_fill.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib> 
#include <ctime> 
#include <unistd.h>

#define MAX_WIDTH 400
#define MAX_HEIGHT 400

// Andrew's algorithm for Convex Hull
std::vector<Point> convexHull(std::vector<Point> points) {
    int n = points.size(), k = 0;
    if (n <= 3) return points;
    std::vector<Point> hull(2 * n);

    std::sort(points.begin(), points.end());

    // Build lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], points[i]) <= 0) k--;
        hull[k++] = points[i];
    }

    // Build upper hull
    for (int i = n - 1, t = k + 1; i > 0; --i) {
        while (k >= t && cross(hull[k - 2], hull[k - 1], points[i - 1]) <= 0) k--;
        hull[k++] = points[i - 1];
    }

    hull.resize(k - 1);
    return hull;
}


std::vector<Point> generateRandomPoints(int count, int width, int height) {
    std::vector<Point> points;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < count; ++i) {
        Point p;
        p.x = std::rand() % width;
        p.y = std::rand() % height;
        points.push_back(p);
    }

    return points;
}

void drawPoints(bitmap& bmp, const std::vector<Point>& points, const pixel& color) {
    for (const auto& point : points) {
        // Center point
        bmp.set_pixel(point.x, point.y, color);

        int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

        for (int i = 0; i < 4; i ++) {
            int x = point.x + dx[i], y = point.y + dy[i];
            if (x < 0 || x > MAX_HEIGHT) continue;
            if (y < 0 || y > MAX_HEIGHT) continue;
            bmp.set_pixel(x, y, color);
        }
    }
}

int main() {
    int width = 400;
    int height = 400;
    std::string filename = "output.bmp";
    EdgeTable et(width, height, filename);      

    bitmap& bmp = et.image;
	pixel background{127, 127, 127}; // Gray background
	bmp.set_all(background);
	pixel cyan{255, 255, 25};
	pixel red{0, 0, 255};

    std::vector<Point> points = generateRandomPoints(50, width, height);
	std::vector<Point> hull = convexHull(points);

    et.polyFill(hull.size(), hull.data(), cyan);

	drawPoints(bmp, points, red);
    bmp.write();

}

