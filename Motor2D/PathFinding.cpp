#include "PathFinding.h"
#include "Input.h"
#include "Maps.h"
#include "App.h"
#include "Render.h"
#include "p2Log.h"

// Two interesting links about PathFinding
// Introduction: http://www.raywenderlich.com/4946/introduction-to-a-pathfinding
// Detailed: http://www.redblobgames.com/pathfinding/a-star/introduction.html

#define INVALID_WALKABILTY_CODE 255

pathNode::pathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL) { }
pathNode::pathNode(int g_score, int h_score, iPoint position, const pathNode *parent_node) : g(g_score), h(h_score), pos(position), parent(parent_node) { }
pathNode::pathNode(const pathNode &node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)  { }

uint pathNode::findWalkableAdjacents(pathList& list_to_fill) const
{
	uint items_before = list_to_fill.list.count();
	iPoint new_pos;
	uint items_added = 0;

	//Nodes-- > North, West, South, East
	new_pos.set(pos.x, pos.y - 1);
	// North;
	if (app->path->isWalkable(new_pos))
	{
		items_added++;
		list_to_fill.list.add(pathNode(-1, -1, new_pos, this));
	}

	new_pos.set(pos.x - 1, pos.y);
	// West;
	if (app->path->isWalkable(new_pos))
	{
		items_added++;
		list_to_fill.list.add(pathNode(-1, -1, new_pos, this));
	}

	new_pos.set(pos.x, pos.y + 1);
	// South
	if (app->path->isWalkable(new_pos))
	{
		items_added++;
		list_to_fill.list.add(pathNode(-1, -1, new_pos, this));
	}

	// East
	new_pos.set(pos.x + 1, pos.y);
	if (app->path->isWalkable(new_pos))
	{
		items_added++;
		list_to_fill.list.add(pathNode(-1, -1, new_pos, this));
	}

	return items_added - items_before;
}

int pathNode::score() const
{
	return g + h;
}

int pathNode::calculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = abs(destination.x - pos.x) + abs(destination.y - pos.y); // pos.distanceTo(destination);
	return g + h;
}

doubleNode<pathNode> *pathList::find(const iPoint& point) const
{
	doubleNode<pathNode> *item = list.getFirst();
	while (item != NULL)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return item;
}

doubleNode<pathNode> *pathList::getNodeLowestScore() const
{
	doubleNode<pathNode> *item = list.getFirst();
	doubleNode<pathNode> *item_lowest_score = NULL;
	int score = 65355;

	while (item != NULL)
	{
		if (item->data.score() < score)
		{
			score = item->data.score();
			item_lowest_score = item;
		}
		item = item->next;
	}

	return item_lowest_score;
}

PathFinding::PathFinding()
{
	name.create("path_finding");
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

void PathFinding::walkableAreaFrom(SDL_Rect &starting_area, pathList &close_list)
{
	iPoint left_top_corner = app->map->worldToMap(starting_area.x, starting_area.y);
	iPoint right_bottom_corner = app->map->worldToMap(starting_area.x + starting_area.w, starting_area.y + starting_area.h);

	pathList open_list;

	for (int i = left_top_corner.x; i <= right_bottom_corner.x; i++)
	{
		for (int j = left_top_corner.y; j <= right_bottom_corner.y; j++)
		{
			iPoint new_pos = { i, j };
			if(isWalkable(new_pos))
				open_list.list.add(pathNode(-1, -1, new_pos, NULL));
		}
	}

	while (open_list.list.count() > 0)
	{
		doubleNode<pathNode> *pnode = open_list.list.getFirst();
		pathList candidate_nodes;
		pnode->data.findWalkableAdjacents(candidate_nodes);
		doubleNode<pathNode> *item = candidate_nodes.list.getFirst();

		close_list.list.add(pnode->data);
		open_list.list.del(pnode);

		while (item)
		{
			if (open_list.find(item->data.pos) == NULL && close_list.find(item->data.pos) == NULL)
				open_list.list.add(item->data);
			item = item->next;
		}
	}
}

int PathFinding::createPath(const iPoint& origin, const iPoint& destination)
{
	// Origin and destination are walkable?
	if (!isWalkable(origin) || !isWalkable(destination))
		return -1;

	path_found.clear();

	// Open and close list
	pathList open_list, close_list;

	open_list.list.add(pathNode(0, 0, origin, NULL));

	while (open_list.list.count() > 0)
	{		
		doubleNode<pathNode> *pnode = open_list.getNodeLowestScore();
		close_list.list.add(pnode->data);
		iPoint pos = pnode->data.pos;
		open_list.list.del(pnode);
		pnode = close_list.find(pos);

		if (pnode->data.pos == destination)
		{
			close_list.list.add(pnode->data);
			break;
		}

		pathList candidate_nodes;		
		int items_added = pnode->data.findWalkableAdjacents(candidate_nodes);
		doubleNode<pathNode> *item = candidate_nodes.list.getLast();

		for (int i = 0; i < items_added; i++)
		{
			if (close_list.find(item->data.pos))
			{
				item = item->previous;
				continue;
			}
			else if (open_list.find(item->data.pos))
			{
				doubleNode<pathNode> *to_compare = open_list.find(item->data.pos);
				if (item->data.calculateF(destination) < to_compare->data.score())
				{
					to_compare->data.parent = item->data.parent;
					to_compare->data.calculateF(destination);
				}
			}
			else
			{
				item->data.calculateF(destination);
				open_list.list.add(item->data);
			}
			item = item->previous;
		}
	}

	const pathNode *item = &(close_list.list.getLast()->data);
	while (item != NULL)
	{
		path_found.pushBack(item->pos);
		item = item->parent;
	}

	path_found.flip();

	return path_found.getNumElements();
}

float PathFinding::costOfPath(const iPoint& origin, const iPoint& destination)
{
	// Open and close list
	pathList open_list, close_list;

	open_list.list.add(pathNode(0, 0, origin, NULL));

	while (open_list.list.count() > 0)
	{
		doubleNode<pathNode> *pnode = open_list.getNodeLowestScore();
		close_list.list.add(pnode->data);
		iPoint pos = pnode->data.pos;
		open_list.list.del(pnode);
		pnode = close_list.find(pos);

		if (pnode->data.pos == destination)
		{
			close_list.list.add(pnode->data);
			break;
		}

		pathList candidate_nodes;
		int items_added = pnode->data.findWalkableAdjacents(candidate_nodes);
		doubleNode<pathNode> *item = candidate_nodes.list.getLast();

		for (int i = 0; i < items_added; i++)
		{
			if (close_list.find(item->data.pos))
			{
				item = item->previous;
				continue;
			}
			else if (open_list.find(item->data.pos))
			{
				doubleNode<pathNode> *to_compare = open_list.find(item->data.pos);
				if (item->data.calculateF(destination) < to_compare->data.score())
				{
					to_compare->data.parent = item->data.parent;
					to_compare->data.calculateF(destination);
				}
			}
			else
			{
				item->data.calculateF(destination);
				open_list.list.add(item->data);
			}
			item = item->previous;
		}
	}

	float cost = close_list.list.getLast()->data.g;
	return cost;
}


const DynArray<iPoint> *PathFinding::getLastPath() const
{
	return &path_found;
}

bool PathFinding::checkBoundaries(const iPoint& pos) const
{
	if (pos.x >= 0 &&
		pos.x < width &&
		pos.y >= 0 &&
		pos.y < height)
		return true;
	return false;
}

bool PathFinding::isWalkable(const iPoint& pos) const
{
	if (checkBoundaries(pos) && map_data[pos.y * width + pos.x] != 0)
		return true;
	return false;
}

uchar PathFinding::getTileAt(const iPoint& pos) const
{
	if (checkBoundaries(pos))
		return map_data[pos.y * width + pos.x];
	return INVALID_WALKABILTY_CODE;
}