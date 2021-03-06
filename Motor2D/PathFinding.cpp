#include "App.h"
#include "Point2d.h"
#include "PathFinding.h"
#include "EntityManager.h"

// CRZ for Rect Line, delete if this doesn't work
#include "Vector2D.h"
#include "p2Log.h"

#include "Brofiler.h"


// Two interesting links about PathFinding
// Introduction: http://www.raywenderlich.com/4946/introduction-to-a-pathfinding
// Detailed: http://www.redblobgames.com/pathfinding/a-star/introduction.html

#define INVALID_WALKABILTY_CODE 255

pathNode::pathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL) { }
pathNode::pathNode(int g_score, int h_score, iPoint position, const pathNode *parent_node) : g(g_score), h(h_score), pos(position), parent(parent_node) { }
pathNode::pathNode(const pathNode &node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)  { }


pathToFind::pathToFind() : id(-1), destination(iPoint(-1, -1)){}
pathToFind::pathToFind(uint _id, iPoint &_destination) : id(_id), destination(_destination) {}

uint pathNode::findAdjacents(pathList& list_to_fill, bool diagonals) const
{
	uint items_before = list_to_fill.list_of_nodes.size();
	iPoint new_pos;
	uint items_added = 0;

	// North
	new_pos.set(pos.x, pos.y - 1);
	items_added++;
	list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));

	//North-West
	if (diagonals)
	{
		new_pos.set(pos.x - 1, pos.y - 1);
		items_added++;
		list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));
	}	

	// West
	new_pos.set(pos.x - 1, pos.y);
	items_added++;
	list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));

	//South-West
	if (diagonals)
	{
		new_pos.set(pos.x - 1, pos.y + 1);
		items_added++;
		list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));
	}
	
	// South
	new_pos.set(pos.x, pos.y + 1);
	items_added++;
	list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));

	//South-East
	if (diagonals)
	{
		new_pos.set(pos.x + 1, pos.y + 1);
		items_added++;
		list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));
	}	

	// East
	new_pos.set(pos.x + 1, pos.y);
	items_added++;
	list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));

	//North-East
	if (diagonals)
	{
		new_pos.set(pos.x + 1, pos.y - 1);
		items_added++;
		list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));
	}	

	return items_added - items_before;
}

uint pathNode::findWalkableAdjacents(pathList& list_to_fill, bool diagonals) const
{ BROFILER_CATEGORY("Pathfinding::findWalkableAdjacents", Profiler::Color::Blue)
	uint items_before = list_to_fill.list_of_nodes.size();
	iPoint new_pos;
	uint items_added = 0;
	
	// North
	new_pos.set(pos.x, pos.y - 1);
	if (app->path->isWalkable(new_pos))
	{
		items_added++;
		list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));

		new_pos.set(pos.x - 1, pos.y);
		if (diagonals && app->path->isWalkable(new_pos))
		{
			//North-West
			new_pos.set(pos.x - 1, pos.y - 1);
			if (app->path->isWalkable(new_pos))
			{
				items_added++;
				list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));
			}
		}
	}

	// West
	new_pos.set(pos.x - 1, pos.y);
	if (app->path->isWalkable(new_pos))
	{
		items_added++;
		list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));

		new_pos.set(pos.x, pos.y + 1);
		if (diagonals && app->path->isWalkable(new_pos))
		{
			//South-West
			new_pos.set(pos.x - 1, pos.y + 1);
			if (app->path->isWalkable(new_pos))
			{
				items_added++;
				list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));
			}
		}
	}

	// South
	new_pos.set(pos.x, pos.y + 1);
	if (app->path->isWalkable(new_pos))
	{
		items_added++;
		list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));

		new_pos.set(pos.x + 1, pos.y);
		if (diagonals && app->path->isWalkable(new_pos))
		{
			//South-East
			new_pos.set(pos.x + 1, pos.y + 1);
			if (app->path->isWalkable(new_pos))
			{
				items_added++;
				list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));
			}
		}
	}

	// East
	new_pos.set(pos.x + 1, pos.y);
	if (app->path->isWalkable(new_pos))
	{
		items_added++;
		list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));

		new_pos.set(pos.x, pos.y - 1);
		if (diagonals && app->path->isWalkable(new_pos))
		{
			//North-East
			new_pos.set(pos.x + 1, pos.y - 1);
			if (app->path->isWalkable(new_pos))
			{
				items_added++;
				list_to_fill.list_of_nodes.push_back(pathNode(-1, -1, new_pos, this));
			}
		}
	}

	return items_added - items_before;
}

int pathNode::score() const
{
	return g + h;
}

int pathNode::calculateF(const iPoint& destination)
{ BROFILER_CATEGORY("Pathfinding::calculateF", Profiler::Color::Blue)
	g = parent->g + 1;
	h = pos.distanceNoSqrt(destination);
	return g + h;
}

int pathNode::calculateG()
{
	g = parent->g + 1;
	return g;
}

list<pathNode>::iterator pathList::find(const iPoint& point)
{
	list<pathNode>::iterator it = list_of_nodes.begin();
	while (it != list_of_nodes.end())
	{
		if (it->pos == point)
			return it;
		++it;
	}

	return it;
}

list<pathNode>::iterator pathList::getNodeLowestScore()
{
	int score = 2147483647;
	list<pathNode>::iterator it = list_of_nodes.begin();
	list<pathNode>::iterator lowest_score = list_of_nodes.end();
	while (it != list_of_nodes.end())
	{
		if (it->score() < score)
		{
			lowest_score = it;
			score = it->score();
		}			
		++it;
	}

	return lowest_score;
}

PathFinding::PathFinding(bool enabled) : Module(enabled)
{
	name.assign("path_finding");
}

PathFinding::~PathFinding()
{ 
	RELEASE_ARRAY(map_data);
}

bool PathFinding::setMap(const uint &width, const uint &height, uchar *data)
{
	this->width = width;
	this->height = height;

	if (data != NULL)
	{
		RELEASE_ARRAY(map_data);
		map_data = new uchar[width * height];
		memcpy(map_data, data, width*height);
	}

	return true;
}

int PathFinding::createPath(const iPoint& origin, const iPoint& destination, uint id)
{
	// Origin are walkable?
	if (!isWalkable(origin))
		return -1;

	iPoint new_dest = findNearestWalkableTile(destination, origin, 5);
	iPoint tile_error = { -1, -1 };
	if (new_dest == tile_error)
		return -1;

	iPoint _origin = origin;

	erase(id);

	pathToFind p(id, new_dest);
	paths_to_find.push_back(p);
	paths_to_find.back().open_list.list_of_nodes.push_back(pathNode(0, 0, origin, NULL));
	return 1;
}

bool PathFinding::postUpdate()
{ BROFILER_CATEGORY("Pathfinding::postUpdate", Profiler::Color::Blue)
	if (paths_to_find.size() > 0)
	{
		time_to_search.start();
		float time = ((1 / 60.0f) - (app->getDt() / 1000.0f)) / paths_to_find.size(); //((1sec / 60 frames) - current dt) / size of paths to find
		int x = 1;

		list<pathToFind>::iterator it = paths_to_find.begin();
		for (; it != paths_to_find.end();)
		{
			path_found.clear();
			do
			{
				if (it->open_list.list_of_nodes.size() == 0)
					break;
				list<pathNode>::iterator pnode = it->open_list.getNodeLowestScore();
				it->close_list.list_of_nodes.push_back(*pnode);
				iPoint pos = pnode->pos;
				it->open_list.list_of_nodes.erase(pnode);
				pnode = it->close_list.find(pos);

				if (pnode->pos == it->destination)
				{
					it->close_list.list_of_nodes.push_back(*pnode);
					break;
				}

				pathList candidate_nodes;
				int items_added = pnode->findWalkableAdjacents(candidate_nodes, true);

				if (items_added > 0)
				{
					list<pathNode>::iterator item = candidate_nodes.list_of_nodes.begin();
					while (item != candidate_nodes.list_of_nodes.end())
					{
						if (it->close_list.find(item->pos) != it->close_list.list_of_nodes.end())
						{
							++item;
							continue;
						}
						else if (it->open_list.find(item->pos) != it->open_list.list_of_nodes.end())
						{
							list<pathNode>::iterator to_compare = it->open_list.find(item->pos);
							if (item->parent->g < to_compare->parent->g)
							{
								to_compare->parent = item->parent;
								to_compare->g = item->calculateG();
							}
						}
						else
						{
							item->calculateF(it->destination);
							it->open_list.list_of_nodes.push_back(*item);
						}
						++item;
					}
				}
			} while ((time * x) > time_to_search.readSec());
			if (it->open_list.list_of_nodes.size() == 0 || (it->close_list.list_of_nodes.size() > 0 && it->close_list.list_of_nodes.back().pos == it->destination))
			{
				const pathNode *final_path = &it->close_list.list_of_nodes.back();
				vector<iPoint> tmp;
				while (final_path != NULL)
				{
					tmp.push_back(final_path->pos);
					final_path = final_path->parent;
				}

				vector<iPoint>* path_found_now = NULL;
				for (vector<iPoint>::reverse_iterator rit = tmp.rbegin(); rit != tmp.rend(); ++rit)
				{
					path_found.push_back(*rit);
				}

				path_found.erase(path_found.begin()); //we don't need the tile which is stepping

				path_found_now = new vector<iPoint>(path_found);

				paths_found.insert(pair<uint, vector<iPoint>*>(it->id, path_found_now));

				it = paths_to_find.erase(it);
			}
			else
				it++;

			x++;
		}
	}
	return true;
}

int PathFinding::createPathNow(const iPoint& origin, const iPoint& destination)
{ BROFILER_CATEGORY("Pathfinding::createPathNow", Profiler::Color::Blue)
	// Origin are walkable?
	if (!isWalkable(origin))
		return -1;

	iPoint new_dest = findNearestWalkableTile(destination, origin, 5);
	iPoint tile_error = { -1, -1 };
	if (new_dest == tile_error)
		return -1;

	path_found.clear();
	// Open and close list
	pathList open_list, close_list;

	open_list.list_of_nodes.push_back(pathNode(0, 0, origin, NULL));
	while (open_list.list_of_nodes.size() > 0)
	{
		list<pathNode>::iterator pnode = open_list.getNodeLowestScore();
		close_list.list_of_nodes.push_back(*pnode);
		iPoint pos = pnode->pos;
		open_list.list_of_nodes.erase(pnode);
		pnode = close_list.find(pos);

		if (pnode->pos == new_dest)
		{
			close_list.list_of_nodes.push_back(*pnode);
			break;
		}

		pathList candidate_nodes;
		int items_added = pnode->findWalkableAdjacents(candidate_nodes, true);

		if (items_added > 0)
		{
			list<pathNode>::iterator item = candidate_nodes.list_of_nodes.begin();
			while (item != candidate_nodes.list_of_nodes.end())
			{
				if (close_list.find(item->pos) != close_list.list_of_nodes.end())
				{
					++item;
					continue;
				}
				else if (open_list.find(item->pos) != open_list.list_of_nodes.end())
				{
					list<pathNode>::iterator to_compare = open_list.find(item->pos);
					if (item->parent->g < to_compare->parent->g)
					{
						to_compare->parent = item->parent;
						to_compare->g = item->calculateG();
					}
				}
				else
				{
					item->calculateF(new_dest);
					open_list.list_of_nodes.push_back(*item);
				}
				++item;
			}
		}
	}


	const pathNode *final_path = &close_list.list_of_nodes.back();
	vector<iPoint> tmp;
	while (final_path != NULL)
	{
		tmp.push_back(final_path->pos);
		final_path = final_path->parent;
	}

	for (vector<iPoint>::reverse_iterator rit = tmp.rbegin(); rit != tmp.rend(); ++rit)
	{
		path_found.push_back(*rit);
	}


	path_found.erase(path_found.begin()); //we don't need the tile which is stepping

	return path_found.size();
}

int PathFinding::createPathToAdjacent(const iPoint& origin, uint distance)
{ BROFILER_CATEGORY("Pathfinding::createPathToAdjacent", Profiler::Color::Blue)
	// Origin are walkable?
	if (!isWalkable(origin))
		return -1;

	pathNode pnode(0, 0, origin, NULL);
	pathList candidate_nodes;
	int items_added = pnode.findWalkableAdjacents(candidate_nodes, true);

	if (items_added > 0)
	{
		uint direction = rand() % items_added;

		list<pathNode>::iterator target = candidate_nodes.list_of_nodes.begin();
		for (uint i = 0; i < direction; i++)
			target++;
		iPoint it = target->pos - origin;
		it.x *= distance;
		it.y *= distance;
		target->pos += it;

		return createPathNow(origin, target->pos);
	}
	return -1;
}

iPoint PathFinding::findNearestWalkableTile(const iPoint &origin, const iPoint &destination, uint radius) const
{ BROFILER_CATEGORY("Pathfinding::findNearestWalkableTile", Profiler::Color::Blue)
	pathList open_list;
	open_list.list_of_nodes.push_back(pathNode(0, 0, origin, NULL));
	list<pathNode>::iterator pnode = open_list.list_of_nodes.begin();

	while (pnode != open_list.list_of_nodes.end())
	{
		pathList candidate_nodes;
		int items_added = pnode->findAdjacents(candidate_nodes);

		if (items_added > 0)
		{
			list<pathNode>::iterator item = candidate_nodes.list_of_nodes.begin();
			while (item != candidate_nodes.list_of_nodes.end())
			{
				if (isWalkable(item->pos))
					return item->pos;

				if (radius != 0 && abs(origin.x - item->pos.x) <= radius && abs(origin.y - item->pos.y) <= radius)
				{
					if (open_list.find(item->pos) == open_list.list_of_nodes.end())
						open_list.list_of_nodes.push_back(*item);
				}
				++item;
			}
		}
		++pnode;
	}

	pathList close_list;
	open_list.list_of_nodes.clear();

	open_list.list_of_nodes.push_back(pathNode(0, 0, origin, NULL));
	while (open_list.list_of_nodes.size() > 0)
	{
		list<pathNode>::iterator pnode = open_list.getNodeLowestScore();
		close_list.list_of_nodes.push_back(*pnode);
		iPoint pos = pnode->pos;
		open_list.list_of_nodes.erase(pnode);
		pnode = close_list.find(pos);

		if (app->path->isWalkable(pnode->pos))
		{
			return pnode->pos;
			break;
		}
		else if (pnode->pos == destination)
			return{ -1, -1 };

		pathList candidate_nodes;
		int items_added = pnode->findAdjacents(candidate_nodes);

		if (items_added > 0)
		{
			list<pathNode>::iterator item = candidate_nodes.list_of_nodes.begin();
			while (item != candidate_nodes.list_of_nodes.end())
			{
				if (close_list.find(item->pos) != close_list.list_of_nodes.end())
				{
					++item;
					continue;
				}
				else if (open_list.find(item->pos) != open_list.list_of_nodes.end())
				{
					list<pathNode>::iterator to_compare = open_list.find(item->pos);
					if (item->parent->g < to_compare->parent->g)
					{
						to_compare->parent = item->parent;
						to_compare->g = item->calculateG();
					}
				}
				else
				{
					item->calculateF(destination);
					open_list.list_of_nodes.push_back(*item);
				}
				++item;
			}
		}
	}

	return { -1, -1 };
}

const vector<iPoint> &PathFinding::getLastPath() const
{
	return path_found;
}

bool PathFinding::getPathFound(uint id, vector<iPoint> &path)
{
	map<uint, vector<iPoint>*>::iterator it = paths_found.begin();
	for (; it != paths_found.end();)
	{
		if (it->first == id)
		{
			path = *it->second;
			RELEASE(it->second);
			it = paths_found.erase(it);
			return true;
		}
		else
			it++;
	}
	return false;
}

bool PathFinding::checkBoundaries(const iPoint& pos) const
{
	if (pos.x >= 0 &&
		pos.x <= width &&
		pos.y >= 0 &&
		pos.y <= height)
		return true;
	return false;
}

bool PathFinding::isWalkable(const iPoint& pos) const
{
	if (checkBoundaries(pos) && map_data[pos.y * width + pos.x] != 2)
		return true;
	return false;
}

uchar PathFinding::getTileAt(const iPoint& pos) const
{
	if (checkBoundaries(pos))
		return map_data[pos.y * width + pos.x];
	return INVALID_WALKABILTY_CODE;
}

bool PathFinding::recalculatePath(const iPoint &origin, uint id)
{
	for (list<pathToFind>::iterator it = paths_to_find.begin(); it != paths_to_find.end();)
	{
		if (it->id == id)
		{
			if(createPath(origin, it->destination, id) != -1)
				return true;
			else
				return false;
		}
		else
			it++;
	}
	return false;
}

void PathFinding::erase(uint id)
{
	for (list<pathToFind>::iterator it = paths_to_find.begin(); it != paths_to_find.end();)
	{
		if (it->id == id)
		{
			it = paths_to_find.erase(it);
			return;
		}
		else
			it++;
	}

	for (map<uint, vector<iPoint>*>::iterator it = paths_found.begin(); it != paths_found.end();)
	{
		if (it->first == id)
		{
			RELEASE(it->second);
			it = paths_found.erase(it);
			return;
		}
		else
			it++;
	}
}

bool PathFinding::cleanUp()
{
	LOG("Cleaning PathFinding");
	
	for (list<pathToFind>::iterator it = paths_to_find.begin(); it != paths_to_find.end();)
	{
		it = paths_to_find.erase(it);
	}

	for (map<uint, vector<iPoint>*>::iterator it = paths_found.begin(); it != paths_found.end();)
	{
		RELEASE(it->second);
		it = paths_found.erase(it);
	}

	return true;
}