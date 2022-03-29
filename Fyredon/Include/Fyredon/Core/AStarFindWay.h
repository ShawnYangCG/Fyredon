#pragma once
#ifndef ASTAR_FIND_WAY
#define ASTAR_FIND_WAY

#include <vector>
#include <queue>
#include <iostream>

using namespace std;

/// <summary>
/// Find the shortest path through A*
/// </summary>
/// <typeparam name="T">data type of the grids£¬probably int or double</typeparam>
/// <param name="Grids">A 2D array which store the data of grids</param>
/// <param name="width">width of grids</param>
/// <param name="length">length if grids</param>
/// <param name="threshold">threshold value of the height, if the height bigger than this value, the grid will be initially set as unpassable</param>
/// <param name="startX">x cooridinate of start point</param>
/// <param name="startY">y cooridinate of start point</param>
/// <param name="endX">x cooridinate of end point</param>
/// <param name="endY">y cooridinate of end point</param>
/// <returns>A array of coordinates of the path points</returns>
template<typename T>
vector<vector<int>>* aStar(T** Grids, int width, int length, T threshold, int startX, int startY, int endX, int endY) {
	/// <summary>
	/// Coordinates object
	/// </summary>
	struct Point {
		int x;					//x value
		int y;					//y value
		double w_cost;			//cost from start point to current point
		const Point* parent;	//father point
		int cost;				//potential whole cost from start point to end point by Manhattan distance
		/// <summary>
		/// Contructor, give values directly
		/// </summary>
		/// <param name="x">x value</param>
		/// <param name="y">y value</param>
		/// <param name="w">w_cost</param>
		/// <param name="p">father</param>
		/// <param name="cost">whole cose</param>
		/// <returns>Coordinates object</returns>
		Point(int x, int y, double w, Point* p = nullptr, int cost = 0) : x(x), y(y), w_cost(w), parent(p), cost(cost) {}
		/// <summary>
		/// Coordinates object, the whole cost from start point to end point computed by Manhattan distance
		/// </summary>
		/// <param name="x">x value</param>
		/// <param name="y">y value</param>
		/// <param name="end">end point object</param>
		/// <param name="p">father point object</param>
		/// <returns>Coordinates object</returns>
		Point(int x, int y, const Point& end, const Point* p = nullptr) : x(x), y(y), parent(p) {
			cost = p->cost + 1;
			w_cost = *this - end + cost;
		}
		/// <summary>
		/// overloading operator "==" to judge whether points have same position
		/// </summary>
		/// <param name="p">point which needs to be compared</param>
		/// <returns>return True when they are same</returns>
		bool operator == (const Point& p) const { return this->x == p.x && this->y == p.y; }
		/// <summary>
		/// overloading operator "!=" to judge whether points have different position
		/// </summary>
		/// <param name="p">point which needs to be compared</param>
		/// <returns>return True when they are different</returns>
		bool operator != (const Point& p) const { return !(*this == p); }
		/// <summary>
		/// overloading operator "<" for priority queue
		/// </summary>
		/// <param name="p">point which needs to be compared</param>
		/// <returns>if the point p has higher priority</returns>
		bool operator < (const Point& p) const { return this->w_cost > p.w_cost; }
		/// <summary>
		/// overloading operator "-" to compute Manhattan distance
		/// </summary>
		/// <param name="p">point which needs to be compared</param>
		/// <returns>Manhattan distance between two points</returns>
		int operator - (const Point& p) const { return abs(this->x - p.x) + abs(this->y - p.y); }
		/// <summary>
		/// get the vector of path between start and end points
		/// </summary>
		/// <returns>the vector of path between start and end points</returns>
		vector<vector<int>>* getWay() const {
			if (!parent) return new vector<vector<int>>{ {x, y} };
			auto parentWay = parent->getWay();
			parentWay->push_back({ x, y });
			return parentWay;
		}
	};

	Point endP(endX, endY, 0);		// construct the end point
	vector<vector<bool>> close_map;	// a set of unpassable points
	priority_queue<Point> open_set;	// a set of potential points, use the priority_queue to get the point with highest priority
	// Initialize the close set
	for (int i = 0; i < length; i++) {
		vector<bool> close_line;
		vector<bool> open_line;
		for (int j = 0; j < width; j++) {
			if (Grids[i][j] < threshold) close_line.push_back(true);
			else close_line.push_back(false);
			open_line.push_back(false);
		}
		close_map.push_back(close_line);

	}
	// push start point to the open set
	open_set.push({ startX, startY, 0 });
	// start search
	while (!open_set.empty()) {
		// get the point with lowest cost
		auto* best = new Point(open_set.top());

		// if the point is end point, end loop, return the path
		if (*best == endP) return best->getWay();

		// if not, remove it from open set, push to close set
		close_map[best->x][best->y] = false;
		open_set.pop();

		// push the neighbor points which are not in close set to open set
		if (best->x - 1 > 0 && close_map[best->x - 1][best->y]) {
			open_set.push(Point(best->x - 1, best->y, endP, best));
		}
		if (best->x + 1 < length && close_map[best->x + 1][best->y]) {
			open_set.push(Point(best->x + 1, best->y, endP, best));
		}
		if (best->y - 1 > 0 && close_map[best->x][best->y - 1]) {
			open_set.push(Point(best->x, best->y - 1, endP, best));
		}
		if (best->y + 1 < width && close_map[best->x][best->y + 1]) {
			open_set.push(Point(best->x, best->y + 1, endP, best));
		}
	}
	// the end point is not available
	return {};
}

// Following code are written for testing

/// <summary>
/// Draw the path and map
/// </summary>
/// <typeparam name="T">data type of the grids£¬probably int or double</typeparam>
/// <param name="Grids">A 2D array which store the data of grids</param>
/// <param name="width">width of grids</param>
/// <param name="length">length if grids</param>
/// <param name="threshold">threshold value of the height, if the height bigger than this value, the grid will be initially set as unpassable</param>
/// <param name="startX">x cooridinate of start point</param>
/// <param name="startY">y cooridinate of start point</param>
/// <param name="endX">x cooridinate of end point</param>
/// <param name="endY">y cooridinate of end point</param>
/// <param name="way">The path finded by A*</param>
template<typename T>
void drawAStar(T** Grids, int width, int length, T threshold, int startX, int startY, int endX, int endY, vector<vector<int>>& way) {
	vector<vector<char>> image;
	for (int i = 0; i < length; i++) {
		vector<char> line;
		for (int j = 0; j < width; j++) {
			if (Grids[i][j] < threshold) line.push_back('.');
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

/// <summary>
/// Draw the path and map
/// </summary>
/// <typeparam name="T">data type of the grids£¬probably int or double</typeparam>
/// <param name="Grids">A 2D array which store the data of grids</param>
/// <param name="width">width of grids</param>
/// <param name="length">length if grids</param>
/// <param name="threshold">threshold value of the height, if the height bigger than this value, the grid will be initially set as unpassable</param>
/// <param name="startX">x cooridinate of start point</param>
/// <param name="startY">y cooridinate of start point</param>
/// <param name="endX">x cooridinate of end point</param>
/// <param name="endY">y cooridinate of end point</param>
template<typename T>
void drawAStar(T** Grids, int width, int length, T threshold, int startX, int startY, int endX, int endY) {
	drawAStar(Grids, width, length, threshold, startX, startY, endX, endY, *aStar(Grids, width, length, threshold, startX, startY, endX, endY));
}

#endif // ASTAR_FIND_WAY