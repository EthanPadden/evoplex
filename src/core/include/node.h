/**
 *  This file is part of Evoplex.
 *
 *  Evoplex is a multi-agent system for networks.
 *  Copyright (C) 2016 - Marcos Cardinot <marcos@cardinot.net>
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

#ifndef NODE_H
#define NODE_H

#include <memory>

#include "attributes.h"
#include "edges.h"
#include "prg.h"

namespace evoplex {

class Node;
typedef std::shared_ptr<Node> NodePtr;

class NodeInterface
{
    friend class BaseGraph;
public:
    virtual NodePtr clone() const = 0;
    virtual const Edges& inEdges() const = 0;
    virtual const Edges& outEdges() const = 0;
    virtual int degree() const = 0;
    virtual int inDegree() const = 0;
    virtual int outDegree() const = 0;

private:
    virtual void addInEdge(const EdgePtr& inEdge) = 0;
    virtual void addOutEdge(const EdgePtr& outEdge) = 0;
    virtual void removeInEdge(const int edgeId) = 0;
    virtual void removeOutEdge(const int edgeId) = 0;
    virtual void clearInEdges() = 0;
    virtual void clearOutEdges() = 0;
};

class Node : public NodeInterface
{
public:
    inline const Attributes& attrs() const;
    inline const Value& attr(const char* name) const;
    inline const Value& attr(const int id) const;
    inline void setAttr(const int id, const Value& value);

    inline int id() const;

    inline int x() const;
    inline void setX(int x);
    inline int y() const;
    inline void setY(int y);
    inline void setCoords(int x, int y);

    inline const NodePtr& randNeighbour(PRG* prg) const;

protected:
    Edges m_outEdges;

    explicit Node(int id, Attributes attrs, int x, int y)
        : m_id(id), m_attrs(attrs), m_x(x), m_y(y) {}

    explicit Node(int id, Attributes attr)
        : Node(id, attr, 0, id) {}

    virtual ~Node() {}

private:
    const int m_id;
    Attributes m_attrs;
    int m_x;
    int m_y;
};

class UNode : public Node
{
public:
    explicit UNode(int id, Attributes attrs, int x, int y) : Node(id, attrs, x, y) {}
    explicit UNode(int id, Attributes attrs) : Node(id, attrs) {}
    virtual ~UNode() {}

    inline NodePtr clone() const override;
    inline const Edges& inEdges() const override;
    inline const Edges& outEdges() const override;
    inline int degree() const override;
    inline int inDegree() const override;
    inline int outDegree() const override;

private:
    inline void addInEdge(const EdgePtr& inEdge) override;
    inline void addOutEdge(const EdgePtr& outEdge) override;
    inline void removeInEdge(const int edgeId) override;
    inline void removeOutEdge(const int edgeId) override;
    inline void clearInEdges() override;
    inline void clearOutEdges() override;
};

class DNode : public Node
{
public:
    explicit DNode(int id, Attributes attrs, int x, int y) : Node(id, attrs, x, y) {}
    explicit DNode(int id, Attributes attrs) : Node(id, attrs) {}
    virtual ~DNode() {}

    inline NodePtr clone() const override;
    inline const Edges& inEdges() const override;
    inline const Edges& outEdges() const override;
    inline int degree() const override;
    inline int inDegree() const override;
    inline int outDegree() const override;

private:
    Edges m_inEdges;

    inline void addInEdge(const EdgePtr& inEdge) override;
    inline void addOutEdge(const EdgePtr& outEdge) override;
    inline void removeInEdge(const int edgeId) override;
    inline void removeOutEdge(const int edgeId) override;
    inline void clearInEdges() override;
    inline void clearOutEdges() override;
};

/************************************************************************
   Node: Inline member functions
 ************************************************************************/

inline const Attributes& Node::attrs() const
{ return m_attrs; }

inline const Value& Node::attr(const char* name) const
{ return m_attrs.value(name); }

inline const Value& Node::attr(const int id) const
{ return m_attrs.value(id); }

inline void Node::setAttr(const int id, const Value& value)
{ m_attrs.setValue(id, value); }

inline int Node::id() const
{ return m_id; }

inline int Node::x() const
{ return m_x; }

inline void Node::setX(int x)
{ m_x = x; }

inline int Node::y() const
{ return m_y; }

inline void Node::setY(int y)
{ m_y = y; }

inline void Node::setCoords(int x, int y)
{ setX(x); setY(y); }

inline const NodePtr& Node::randNeighbour(PRG* prg) const
{ return (*std::next(m_outEdges.cbegin(), prg->randI(m_outEdges.size()-1))).second->neighbour(); }

/************************************************************************
   UNode: Inline member functions
 ************************************************************************/

inline NodePtr UNode::clone() const
{ return std::make_shared<UNode>(id(), attrs(), x(), y()); }

inline const Edges& UNode::inEdges() const
{ return m_outEdges; }

inline const Edges& UNode::outEdges() const
{ return m_outEdges; }

inline int UNode::degree() const
{ return static_cast<int>(m_outEdges.size()); }

inline int UNode::inDegree() const
{ return degree(); }

inline int UNode::outDegree() const
{ return degree(); }

inline void UNode::addInEdge(const EdgePtr& inEdge)
{ addOutEdge(inEdge); }

inline void UNode::addOutEdge(const EdgePtr& outEdge)
{ m_outEdges.insert({outEdge->id(), outEdge}); }

inline void UNode::removeInEdge(const int edgeId)
{ removeOutEdge(edgeId); }

inline void UNode::removeOutEdge(const int edgeId)
{ m_outEdges.erase(edgeId); }

inline void UNode::clearInEdges()
{ clearOutEdges(); }

inline void UNode::clearOutEdges()
{ m_outEdges.clear(); }

/************************************************************************
   DNode: Inline member functions
 ************************************************************************/

inline NodePtr DNode::clone() const
{ return std::make_shared<DNode>(id(), attrs(), x(), y()); }

inline const Edges& DNode::inEdges() const
{ return m_inEdges; }

inline const Edges& DNode::outEdges() const
{ return m_outEdges; }

inline int DNode::degree() const
{ return inDegree() + outDegree(); }

inline int DNode::inDegree() const
{ return static_cast<int>(m_inEdges.size()); }

inline int DNode::outDegree() const
{ return static_cast<int>(m_outEdges.size()); }

inline void DNode::addInEdge(const EdgePtr& inEdge)
{ m_inEdges.insert({inEdge->id(), inEdge}); }

inline void DNode::addOutEdge(const EdgePtr& outEdge)
{ m_outEdges.insert({outEdge->id(), outEdge}); }

inline void DNode::removeInEdge(const int edgeId)
{ m_inEdges.erase(edgeId); }

inline void DNode::removeOutEdge(const int edgeId)
{ m_outEdges.erase(edgeId); }

inline void DNode::clearInEdges()
{ m_inEdges.clear(); }

inline void DNode::clearOutEdges()
{ m_outEdges.clear(); }

} // evoplex
#endif // NODE_H
