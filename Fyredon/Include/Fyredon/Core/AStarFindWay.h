#pragma once
#ifndef ASTAR_FIND_WAY
#define ASTAR_FIND_WAY

#include <vector>
#include <queue>
#include <iostream>

using namespace std;

template<typename T>
vector<vector<int>>* aStar(T** grade, int width, int height, T threshold, int startX, int startY, int endX, int endY) {
	struct Point {
		int x;
		int y;
		int cost;//current cost
		double w_cost;//whole cost
		const Point* parent;

		Point(int x, int y, double w, Point* p = nullptr, int cost = 0) : x(x), y(y), w_cost(w), parent(p), cost(cost) {}
		Point(int x, int y, const Point& end, const Point* p = nullptr) : x(x), y(y), parent(p) { cost = p->cost + 1; w_cost = *this - end + cost; }
		bool operator == (const Point& p) const { return this->x == p.x && this->y == p.y; }
		bool operator != (const Point& p) const { return !(*this == p); }
		bool operator < (const Point& p) const { return this->w_cost > p.w_cost; }
		int operator - (const Point& p) const { return abs(this->x - p.x) + abs(this->y - p.y); }
		vector<vector<int>>* getWay() const {
			if (!parent) return new vector<vector<int>>{ {x, y} };
			auto parentWay = parent->getWay();
			parentWay->push_back({ x, y });
			return parentWay;
		}
	};

	Point endP(endX, endY, 0);
	vector<vector<bool>> close_map;
	vector<vector<bool>> open_map;
	for (int i = 0; i < height; i++) {
		vector<bool> close_line;
		vector<bool> open_line;
		for (int j = 0; j < width; j++) {
			if (grade[i][j] < threshold) close_line.push_back(true);
			else close_line.push_back(false);
			open_line.push_back(false);
		}
		close_map.push_back(close_line);
		open_map.push_back(open_line);
	}

	priority_queue<Point> open_set;
	open_set.push({ startX, startY, 0 });
	open_map[startX][startY] = true;
	while (!open_set.empty()) {
		auto* best = new Point(open_set.top());
		if (*best == endP) return best->getWay();
		close_map[best->x][best->y] = false;
		open_set.pop();
		if (best->x - 1 > 0 && close_map[best->x - 1][best->y]) {
			open_set.push(Point(best->x - 1, best->y, endP, best));
		}
		if (best->x + 1 < height && close_map[best->x + 1][best->y]) {
			open_set.push(Point(best->x + 1, best->y, endP, best));
		}
		if (best->y - 1 > 0 && close_map[best->x][best->y - 1]) {
			open_set.push(Point(best->x, best->y - 1, endP, best));
		}
		if (best->y + 1 < width && close_map[best->x][best->y + 1]) {
			open_set.push(Point(best->x, best->y + 1, endP, best));
		}
	}
	return {};
}

template<typename T>
void drawAStar(T** grade, int width, int height, T threshold, int startX, int startY, int endX, int endY, vector<vector<int>>& way) {
	vector<vector<char>> image;
	for (int i = 0; i < height; i++) {
		vector<char> line;
		for (int j = 0; j < width; j++) {
			if (grade[i][j] < threshold) line.push_back('.');
			else line.push_back('#');
		}
		image.push_back(line);
	}
	for (auto p : way) image[p[0]][p[1]] = '+';
	image[startX][startY] = '@';
	image[endX][endY] = '$';
	for (auto line : image) {
		for (auto c : line) cout << c;
		cout << endl;
	}
}

template<typename T>
void drawAStar(T** grade, int width, int height, T threshold, int startX, int startY, int endX, int endY) {
	drawAStar(grade, width, height, threshold, startX, startY, endX, endY, *aStar(grade, width, height, threshold, startX, startY, endX, endY));
}

#endif // ASTAR_FIND_WAY