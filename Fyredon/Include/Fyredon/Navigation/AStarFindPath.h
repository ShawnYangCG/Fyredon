#pragma once
#ifndef ASTAR_FIND_PATH
#define ASTAR_FIND_PATH

#include <queue>
#include <map>
#include "AStarGridNode.h"

using namespace std;


class AStarMap {
private:
	map<Pos, bool> passableMsg;		// store the unpassable positions

	/// <summary>
	/// Determine if the position is out of bounds
	/// </summary>
	/// <param name="pos">position that need judge</param>
	/// <returns>Determine if the position is out of bounds</returns>
	inline bool isLegelPos(const Pos& pos) const {
		return (pos.y >= 0) && (!width > 0 || pos.y < width) && (pos.x >= 0) && (!height > 0 || pos.x < height);
	}

	/// <summary>
	/// Judge if this position is passable
	/// </summary>
	/// <param name="pos">position</param>
	/// <returns>if this position is passable</returns>
	inline bool isPassable(const Pos& pos) const {
		auto msg = passableMsg.find(pos);
		if (msg != passableMsg.end()) return !msg->second;
		else return true;
	}

public:
	int width;	// width of the map (It's infinite if it's not greater than 0)
	int height;	// height of the map (It's infinite if it's not greater than 0)
	
	/// <summary>
	/// Constructor
	/// </summary>
	AStarMap(int width = 0, int height = 0)
		: width(width)
		, height(height)
	{}

	/// <summary>
	/// Constructor
	/// </summary>
	/// <typeparam name="T">Iterator for the container that records the position of the wall</typeparam>
	/// <param name="wallVesselStart">Initial iterator for the container that records the wall position</param>
	/// <param name="wallVesselEnd">Terminating iterator for the container that records the wall position</param>
	/// <param name="width">The width of the map, 0 is infinite</param>
	/// <param name="height">The height of the map, 0 is infinite</param>
	template<typename T>
	AStarMap(const T& wallVesselStart, const T& wallVesselEnd, int width = 0, int height = 0)
		: width(width)
		, height(height)
	{
		for (T it = wallVesselStart; it != wallVesselEnd; ++it) {
			passableMsg[*it] = true;
		}
	}

	/// <summary>
	/// Add wall
	/// </summary>
	/// <param name="pos">position</param>
	void addWall(const Pos& pos) {
		setImPassable(pos, true);
	}

	/// <summary>
	/// Add multiple walls
	/// </summary>
	/// <typeparam name="T">Iterator for the container that records the position of the wall</typeparam>
	/// <param name="wallVesselStart">Initial iterator for the container that records the wall position</param>
	/// <param name="wallVesselEnd">Terminating iterator for the container that records the wall position</param>
	template<typename T>
	void addWall(const T& wallVesselStart, const T& wallVesselEnd) {
		for (T it = wallVesselStart; it != wallVesselEnd; ++it) {
			setImPassable(*it, true);
		}
	}

	/// <summary>
	/// Remove wall
	/// </summary>
	/// <param name="pos">position</param>
	void removeWall(const Pos& pos) {
		setImPassable(pos, false);
	}

	/// <summary>
	/// Remove multiple walls
	/// </summary>
	/// <typeparam name="T">Iterator for the container that records the position of the wall</typeparam>
	/// <param name="wallVesselStart">Initial iterator for the container that records the wall position</param>
	/// <param name="wallVesselEnd">Terminating iterator for the container that records the wall position</param>
	template<typename T>
	void removeWall(const T& wallVesselStart, const T& wallVesselEnd) {
		for (T it = wallVesselStart; it != wallVesselEnd; ++it) {
			setImPassable(*it, false);
		}
	}

	/// <summary>
	/// set the impassability of a position, the position is impassable when set the value as true
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="passable"></param>
	void setImPassable(const Pos& pos, const bool passable) {
		if (!isLegelPos(pos)) throw invalid_argument("the point is out of bounds£¡");
		passableMsg[pos] = passable;
	}

	/// <summary>
	/// A* find path
	/// </summary>
	/// <param name="start">start position</param>
	/// <param name="end">end position</param>
	/// <returns>a list of path</returns>
	list<Pos>* find(const Pos& start, const Pos& end) const {
		if (!isLegelPos(start)) throw invalid_argument("The start point is out of bounds£¡");
		if (!isLegelPos(end)) throw invalid_argument("The end point is out of bounds£¡");

		list<Node*> GC;		// Used to collect objects for garbage collection

		map<Pos, bool> close_map;												// Positions that have already been visited
		priority_queue<Node, vector<Node>, Node::isSecondPriority> open_set;	// Positions that accessible but have not been visited

		open_set.push(Node(start, end));

		while (!open_set.empty()) {

			// get the node with lowest cost
			auto* best = new Node(open_set.top());
			GC.push_back(best);		// Record the garbage object need to be collected

			// If reach the end, stop and return the path from the beginning to end point
			if (best->v == end) {
				auto list = Node::GetWay(*best);
				for (auto node : GC) delete node;	// collect garbage
				return list;
			}

			// Otherwise, remove it from the open_set and put it in the close_map
			close_map[best->v] = true;
			open_set.pop();
			
			// Add the points up, down, left or right that are not in the close_map to the open_set
			const auto up = best->v.GetUp();
			if (isLegelPos(up) && isPassable(up) && !close_map[up]) {
				open_set.push(Node(up, end, best));
			}

			const auto down = best->v.GetDown();
			if (isLegelPos(down) && isPassable(down) && !close_map[down]) {
				open_set.push(Node(down, end, best));
			}

			const auto left = best->v.GetLeft();
			if (isLegelPos(left) && isPassable(left) && !close_map[left]) {
				open_set.push(Node(left, end, best));
			}

			const auto right = best->v.GetRight();
			if (isLegelPos(right) && isPassable(right) && !close_map[right]) {
				open_set.push(Node(right, end, best));
			}
		}

		// traverse all of the possibile nodes, the end point cannot be reached
		return {};
	}
};

#endif // ASTAR_FIND_PATH
