#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

struct Point {
    double x, y;
};

//  compare points based on x-coordinate
bool compareX(const Point& a, const Point& b) {
    return a.x < b.x;
}

//  compare points based on y-coordinate
bool compareY(const Point& a, const Point& b) {
    return a.y < b.y;
}

//  calculate the Euclidean distance between two points
double distance(const Point& p1, const Point& p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// Brute-force method to find the smallest distance between points in a small subset
double bruteForce(Point points[], int n) {
    double minDist = numeric_limits<double>::infinity();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dist = distance(points[i], points[j]);
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

//  find the minimum distance for points close to the line between two halves
double stripClosest(Point strip[], int size, double d) {
    double minDist = d;
    sort(strip, strip + size, compareY);  // Sort strip by y-coordinate

    for (int i = 0; i < size; ++i) {
        // Compare points within a small range in y-coordinates
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < minDist; ++j) {
            double dist = distance(strip[i], strip[j]);
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

double closestUtil(Point points[], int n) {
    // Use brute force if number of points is small
    if (n <= 3) {
        return bruteForce(points, n);
    }

    // Divide the points into left and right halves
    int mid = n / 2;
    Point midPoint = points[mid];

    // find the smallest distance in the left and right halves
    double dl = closestUtil(points, mid);
    double dr = closestUtil(points + mid, n - mid);

    double d = min(dl, dr);

    // Create a strip that contains points close to the dividing line
    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(points[i].x - midPoint.x) < d) {
            strip[j++] = points[i];
        }
    }

    // Find the minimum distance within the strip and return the smallest distance
    return min(d, stripClosest(strip, j, d));
}

// Main  find the closest pair of points
double closest(Point points[], int n) {
    sort(points, points + n, compareX);  // Sort points by x-coordinate
    return closestUtil(points, n);
}

int main() {
    int n;
    cout << "Enter the number of points: ";
    cin >> n;

    Point points[n];
    cout << "Enter the coordinates of the points (x y):" << endl;
    for (int i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;
    }

    double minDistance = closest(points, n);
    cout << "The smallest distance between two points is: " << minDistance << endl;

    return 0;
}
