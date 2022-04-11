#pragma once
#ifndef A_STAR_GRID_NODE
#define A_STAR_GRID_NODE

#include <list>
#include <iostream>

using namespace std;

struct Pos {
	int x;
	int y;

	Pos(int x, int y)
		: x(x)
		, y(y)
	{}

	/// <summary>
	/// Get the up position of the current position
	/// </summary>
	/// <returns>Get the up position of the current position</returns>
	inline Pos GetUp() const {
		return Pos(x - 1, y);
	}

	/// <summary>
	/// Get the down position of the current position
	/// </summary>
	/// <returns>Get the down position of the current position</returns>
	inline Pos GetDown() const {
		return Pos(x + 1, y);
	}

	/// <summary>
	/// Get the left position of the current position
	/// </summary>
	/// <returns>Get the left position of the current position</returns>
	inline Pos GetLeft() const {
		return Pos(x, y - 1);
	}

	/// <summary>
	/// Get the right position of the current position
	/// </summary>
	/// <returns>Get the right position of the current position</returns>
	inline Pos GetRight() const {
		return Pos(x, y + 1);
	}

	/// <summary>
	/// Judge if the position same with current position
	/// </summary>
	/// <param name="p">another position</param>
	/// <returns>Judge if the position same with current position</returns>
	inline bool operator ==(const Pos& p) const {
		return this->x == p.x && this->y == p.y;
	}

	/// <summary>
	/// Make Pos type partial order for map use
	/// </summary>
	inline bool operator <(const Pos& p) const {
		return this->x == p.x ? this->y < p.y : this->x < p.x;
	}

	/// <summary>
	/// Compute Manhattan Distance
	/// </summary>
	/// <param name="p1">first position</param>
	/// <param name="p2">second position</param>
	/// <returns>Manhattan Distance</returns>
	inline static int GetManhattanDistance(const Pos& p1, const Pos& p2) {
		return abs(p1.x - p2.x) + abs(p1.y - p2.y);
	}
};

/// <summary>
/// grid node
/// </summary>
struct Node {
	Pos v;			// position
	int G;			// cost from start point to current
	int H;			// potential cost from current point to end point
	Node* parent;	// parent node

	/// <summary>
	/// A functor for the comparator of the priority queue
	/// </summary>
	struct isSecondPriority {
		/// <summary>
		/// Judge if the second point have higher priority in A*
		/// </summary>
		/// <param name="p1">first point position</param>
		/// <param name="p2">second point position</param>
		/// <returns>if the second point have higher priority</returns>
		inline bool operator()(const Node& p1, const Node& p2) {
			return p1.getF() > p2.getF();
		}
	};

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="pos">Position of point</param>
	/// <param name="end">end point</param>
	/// <param name="parent">pointer of father node</param>
	Node(const Pos& pos, const Pos& end, Node* const parent = nullptr)
		: v(pos)
		, G(parent ? parent->G + 1 : 0)
		, H(Pos::GetManhattanDistance(pos, end))
		, parent(parent)
	{}

	/// <summary>
	/// whole cost
	/// </summary>
	/// <returns>whole cost</returns>
	inline int getF() const {
		return G + H;
	}

	/// <summary>
	/// A list which store the path from start to current points
	/// </summary>
	/// <param name="p">end point</param>
	/// <returns>A list which store the path from start to current pointslist</returns>
	static list<Pos>* GetWay(const Node& p) {
		if (!p.parent) return new list<Pos>{ p.v };
		auto parentWay = GetWay(*p.parent);
		parentWay->push_back(p.v);
		return parentWay;
	}
};

#endif // A_STAR_GRID_NODE