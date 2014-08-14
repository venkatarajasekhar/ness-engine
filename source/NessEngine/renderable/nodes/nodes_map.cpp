#include "../../renderer/renderer.h"
#include "nodes_map.h"

namespace Ness
{
	const SRenderTransformations& NodesMap::get_absolute_transformations()
	{
		// if don't have a parent, return self transformations
		if (!m_parent)
			return m_transformations;

		// calculate this transformations with parent transformations
		m_absolute_transformations = m_transformations;
		m_absolute_transformations.add_transformations(m_parent->get_absolute_transformations());
		return m_absolute_transformations;
	}

	NodesMap::NodesMap(Renderer* renderer, const Sizei& mapSize, const Size& nodesSize, const Size& nodesDistance, TCreateNodes createNodesFunction)
		: NodeAPI(renderer), m_size(mapSize), m_nodes_distance(nodesDistance),  m_node_size(nodesSize)
	{
		if (m_nodes_distance == Size::ZERO)
			m_nodes_distance = m_node_size;

		// create the sprites grid
		Pointi index;
		m_nodes.resize(m_size.x);
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			m_nodes[index.x].resize(m_size.y);
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				// create the sprite
				NodeAPIPtr NewNode;
				if (createNodesFunction == nullptr)
				{
					NewNode = ness_make_ptr<Node>(this->m_renderer);
				}
				else
				{
					NewNode = createNodesFunction(index);
				}
				NewNode->__change_parent(this);
				
				// arrange current tile in the grid
				arrange_node(NewNode, index);

				// add to matrix of tiles
				m_nodes[index.x][index.y] = NewNode;
			}
		}
	}

	// arrange a single tile sprite during creation
	void NodesMap::arrange_node(const NodeAPIPtr& node, const Ness::Pointi& index)
	{				
		// set position
		node->set_position(Point(
			(float)(index.x * m_nodes_distance.x) + (m_node_size.x * 0.5f), 
			(float)(index.y * m_nodes_distance.y) + m_node_size.y));

		// set z-index
		node->set_zindex(node->get_position().y - m_nodes_distance.y);
	}

	void NodesMap::apply_to_all(TExecuteOnNodes func)
	{
		Sizei index;
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				func(index, m_nodes[index.x][index.y]);
			}
		}
	}

	void NodesMap::transformations_update()
	{
		Sizei index;
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				m_nodes[index.x][index.y]->transformations_update();
			}
		}
	}

	void NodesMap::__get_visible_entities(Vector<RenderableAPI*>& out_list, const CameraPtr& camera)
	{
		Rectangle range = get_nodes_in_screen(camera);
		for (int i = range.x; i < range.w; i++)
		{
			for (int j = range.y; j < range.h; j++)
			{
				out_list.push_back(m_nodes[i][j].get());
			}
		}
	}
	
	void NodesMap::__get_all_entities(Vector<RenderableAPI*>& out_list, bool breakGroups)
	{
		for (int i = 0; i < m_size.x; i++)
		{
			for (int j = 0; j < m_size.y; j++)
			{
				out_list.push_back(m_nodes[i][j].get());
			}
		}
	}

	Rectangle NodesMap::get_nodes_in_screen(const CameraPtr& camera) 
	{
		Rectangle ret;
		SRenderTransformations trans = get_absolute_transformations(); 

		Point pos = trans.position;
		if (camera)
		{
			pos.x -= camera->position.x;
			pos.y -= camera->position.y;
		}

		// get first and last tiles to render
		int startI = pos.x < 0 ? (int)(-pos.x / m_nodes_distance.x) : 0;
		if (startI >= m_size.x) startI = m_size.x - 1;
		int startJ = pos.y < 0 ? (int)(((-pos.y - m_node_size.y) / m_nodes_distance.y) - 1) : 0;
		if (startJ >= m_size.y) startJ = m_size.y - 1;
		if (startJ < 0) startJ = 0;
		int endI = startI + (int)((m_renderer->get_target_size().x + m_node_size.x) / m_nodes_distance.x) + 1;
		int endJ = startJ + (int)((m_renderer->get_target_size().y + m_node_size.y * 2) / m_nodes_distance.y) + 2;
		if (endI >= m_size.x) endI = m_size.x - 1;
		if (endJ >= m_size.y) endJ = m_size.y - 1;
		ret.x = startI;
		ret.y = startJ;
		ret.w = endI;
		ret.h = endJ;
		return ret;
	}

	Pointi NodesMap::get_index_from_position(const Point& position)
	{
		Pointi index;
		index.x = (int)(position.x / m_nodes_distance.x);
		index.y = (int)((position.y) / m_nodes_distance.y);
		return index;
	}

	NodeAPIPtr& NodesMap::get_node_by_position_any(const Point& position)
	{
		static NodeAPIPtr empty;
		Pointi index = get_index_from_position(position);
		if (index.x < 0 || index.y < 0 || index.x >= m_size.x || index.y >= m_size.y)
		{
			return empty;
		}
		return get_node_any(index);
	}

	bool NodesMap::is_really_visible(const CameraPtr& camera)
	{
		if (!m_visible)
			return false;

		SRenderTransformations trans = get_absolute_transformations(); 
		if (trans.color.a <= 0.0f)
			return false;

		Point pos = trans.position;
		pos.x -= camera->position.x;
		pos.y -= camera->position.y;

		int startI = pos.x < 0 ? (int)(-pos.x / m_nodes_distance.x) : 0;
		if (startI >= m_size.x)
			return false;
		
		int startJ = pos.y < 0 ? (int)(((-pos.y - m_node_size.y) / m_nodes_distance.y) - 1) : 0;
		if (startJ >= m_size.y)
			return false;
		
		int endI = startI + (int)((m_renderer->get_target_size().x + m_node_size.x) / m_nodes_distance.x) + 1;
		if (endI <= 0)
			return false;
		
		int endJ = startJ + (int)((m_renderer->get_target_size().y + m_node_size.y * 2) / m_nodes_distance.y) + 2;
		if (endJ <= 0)
			return false;

		return true;
	}

	void NodesMap::render(const CameraPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
			return;

		Rectangle range = get_nodes_in_screen(camera);

		// render all visible tiles
		for (int i = range.x; i < range.w; i++)
		{
			for (int j = range.y; j < range.h; j++)
			{
				m_nodes[i][j]->render(camera);
			}
		}
	}

};