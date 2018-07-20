/**
 *  This file is part of Evoplex.
 *
 *  Evoplex is a multi-agent system for networks.
 *  Copyright (C) 2018 - Marcos Cardinot <marcos@cardinot.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "node.h"

// we need this cpp file to avoi weak-vtable issues
namespace evoplex {

Node::Node(const constructor_key&, int id, Attributes attrs, int x, int y)
    : m_id(id),
      m_attrs(attrs),
      m_x(x),
      m_y(y)
{
}

Node::Node(const constructor_key& k, int id, Attributes attr)
    : Node(k, id, attr, 0, id) {}

Node::~Node()
{
}

/*******************/

UNode::UNode(const constructor_key& k, int id, Attributes attrs, int x, int y)
    : Node(k, id, attrs, x, y)
{
}

UNode::UNode(const constructor_key& k, int id, Attributes attrs)
    : Node(k, id, attrs)
{
}

/*******************/

DNode::DNode(const constructor_key& k, int id, Attributes attrs, int x, int y)
    : Node(k, id, attrs, x, y)
{
}

DNode::DNode(const constructor_key& k, int id, Attributes attrs)
    : Node(k, id, attrs)
{
}

} // evoplex
