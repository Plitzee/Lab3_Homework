#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

struct Point {
    vector<double> coords;
    Point(vector<double> coordinates) : coords(move(coordinates)) {}
    double get(int index) const { return coords[index]; }
    int size() const { return coords.size(); }
};

struct KDNode {
    Point point;
    KDNode* left;
    KDNode* right;
    KDNode(Point pt) : point(pt), left(nullptr), right(nullptr) {}
};

class KDTree {
public:
    KDTree(const vector<Point>& points) {
        k = points[0].size();
        root = constructTree(points, 0);
    }

    Point findNearest(const Point& target) const {
        KDNode* nearestNode = nullptr;
        double closestDist = numeric_limits<double>::infinity();
        searchNearest(root, target, nearestNode, closestDist, 0);
        return nearestNode->point;
    }

private:
    KDNode* root;
    int k;

    KDNode* constructTree(vector<Point> points, int depth) {
        if (points.empty()) return nullptr;

        int axis = depth % k;
        nth_element(points.begin(), points.begin() + points.size() / 2, points.end(),
                    [axis](const Point& a, const Point& b) { return a.get(axis) < b.get(axis); });

        int medianIndex = points.size() / 2;
        KDNode* node = new KDNode(points[medianIndex]);

        vector<Point> leftPoints(points.begin(), points.begin() + medianIndex);
        vector<Point> rightPoints(points.begin() + medianIndex + 1, points.end());

        node->left = constructTree(leftPoints, depth + 1);
        node->right = constructTree(rightPoints, depth + 1);

        return node;
    }

    // Helper function to calculate squared Euclidean distance between two points
    double squaredDistance(const Point& a, const Point& b) const {
        double dist = 0.0;
        for (int i = 0; i < a.size(); ++i) {
            dist += (a.get(i) - b.get(i)) * (a.get(i) - b.get(i));
        }
        return dist;
    }

    // Recursive function to search for the nearest neighbor
    void searchNearest(KDNode* node, const Point& target, KDNode*& best, double& bestDist, int depth) const {
        if (node == nullptr) return;

        double dist = squaredDistance(node->point, target);
        if (dist < bestDist) {
            bestDist = dist;
            best = node;
        }

        int axis = depth % k;
        bool goLeft = target.get(axis) < node->point.get(axis);
        searchNearest(goLeft ? node->left : node->right, target, best, bestDist, depth + 1);

        double planeDist = fabs(target.get(axis) - node->point.get(axis));
        if (planeDist * planeDist < bestDist) {
            searchNearest(goLeft ? node->right : node->left, target, best, bestDist, depth + 1);
        }
    }
};

int main() {
    int numPoints, dimensions;
    cout << "Enter the number of points: ";
    cin >> numPoints;
    cout << "Enter the number of dimensions: ";
    cin >> dimensions;

    vector<Point> points;
    for (int i = 0; i < numPoints; ++i) {
        vector<double> coords(dimensions);
        cout << "Enter coordinates for point " << i + 1 << ": ";
        for (int j = 0; j < dimensions; ++j) {
            cin >> coords[j];
        }
        points.emplace_back(coords);
    }

    KDTree kdTree(points);

    vector<double> targetCoords(dimensions);
    cout << "Enter the coordinates of the target point: ";
    for (int i = 0; i < dimensions; ++i) {
        cin >> targetCoords[i];
    }
    Point target(targetCoords);

    Point nearest = kdTree.findNearest(target);
    cout << "Nearest neighbor to (";
    for (int i = 0; i < target.size(); ++i) {
        cout << target.get(i) << (i < target.size() - 1 ? ", " : "");
    }
    cout << ") is (";
    for (int i = 0; i < nearest.size(); ++i) {
        cout << nearest.get(i) << (i < nearest.size() - 1 ? ", " : "");
    }
    cout << ")" << endl;

    return 0;
}
