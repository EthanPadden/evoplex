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

#ifndef BASE_PLUGIN_H
#define BASE_PLUGIN_H

#include <QString>

#include "attributes.h"
#include "prg.h"
#include "value.h"

namespace evoplex {

class BasePlugin
{
public:
    inline PRG* prg() const;
    inline const Attributes* attrs() const;

    inline const QString& attrName(int attrId) const;

    inline const Value& attr(int attrId) const;
    inline const Value& attr(const char* name) const;
    inline const Value& attr(const QString& name) const;
    inline const Value& attr(const char* name, const Value& defaultValue) const;
    inline const Value& attr(const QString& name, const Value& defaultValue) const;

    inline bool attrExists(const char* name) const;
    inline bool attrExists(const QString& name) const;

protected:
    PRG* m_prg;

    explicit BasePlugin() : m_prg(nullptr), m_attrs(nullptr) {}
    virtual ~BasePlugin() {}

    // takes the ownership of the PRG
    // cannot be called twice
    virtual inline bool setup(PRG* prg, const Attributes* attrs);

private:
    const Attributes* m_attrs;
};

/************************************************************************
   BasePlugin: Inline member functions
 ************************************************************************/

inline PRG* BasePlugin::prg() const
{ return m_prg; }

inline const Attributes* BasePlugin::attrs() const
{ return m_attrs; }

inline const QString& BasePlugin::attrName(int attrId) const
{ return m_attrs->name(attrId); }

inline const Value& BasePlugin::attr(int attrId) const
{ return m_attrs->value(attrId);  }

inline const Value& BasePlugin::attr(const char* name) const
{ return m_attrs->value(name); }

inline const Value& BasePlugin::attr(const QString& name) const
{ return m_attrs->value(name); }

inline const Value& BasePlugin::attr(const char* name, const Value& defaultValue) const
{ return m_attrs->value(name, defaultValue); }

inline const Value& BasePlugin::attr(const QString& name, const Value& defaultValue) const
{ return m_attrs->value(name, defaultValue); }

inline bool BasePlugin::attrExists(const char* name) const
{ return m_attrs->contains(name); }

inline bool BasePlugin::attrExists(const QString& name) const
{ return m_attrs->contains(name); }

inline bool BasePlugin::setup(PRG* prg, const Attributes* attrs) {
    Q_ASSERT_X(prg, "BasePlugin::setup", "PRG must not be null");
    Q_ASSERT_X(!m_prg, "BasePlugin::setup", "tried to setup a plugin twice");
    m_prg = prg;
    m_attrs = attrs;
    return m_prg != nullptr;
}


} // evoplex
#endif // BASE_PLUGIN_H
