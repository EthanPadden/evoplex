/**
 *  This file is part of Evoplex.
 *
 *  Evoplex is a multi-agent system for networks.
 *  Copyright (C) 2018 - Marcos Cardinot <marcos@cardinot.net>
 *  Copyright (C) 2018 - Ethan Padden <e.padden1@nuigalway.ie>
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

#include <memory>
#include <QtTest>
#include <core/attrsgenerator.h>

namespace evoplex {
class TestAttrsGenerator: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {}
    void cleanupTestCase() {}
    void tst_parseStarCmd();
    void tst_parseHashCmd_min();
    void tst_parseHashCmd_max();
    void tst_parseHashCmd_rand();
    void tst_parseHashCmd_setValue();
    void tst_parseHashCmd_mixedFunc();
    void _tst_attrs(const SetOfAttributes& res, const Attributes& attrs,  const bool testValues);
    void _tst_mode(const SetOfAttributes& res, const QString& mode, const AttributesScope& ascope, const int numOfAttrRges);
    void _tst_parseStarCmd(const QString& mode);
    AttributesScope _newAttrsScope(const QStringList& names, const QStringList& attrRges);
};

void TestAttrsGenerator::_tst_attrs(const SetOfAttributes& res, const Attributes& attrs,  const bool testValues)
{
    for (int i = 0; i < res.size(); ++i) {
        QCOMPARE(res.at(i).names(), attrs.names());
        QCOMPARE(res.at(i).size(), attrs.size());

        if (testValues) {
            QCOMPARE(res.at(i).values(), attrs.values());
            for (int j = 0; j < attrs.size(); ++j) {
                QCOMPARE(res.at(i).value(j), attrs.value(j));
            }
        }
    }
}

void TestAttrsGenerator::_tst_mode(const SetOfAttributes& res, const QString& mode, const AttributesScope& ascope, const int numOfAttrRges)
{
   if (mode == "min") {
        for (int i = 0; i < res.size(); ++i) {
            for (int j = 0; j < numOfAttrRges; ++j) {
                QCOMPARE(res.at(i).value(j), ascope.value(res.at(i).name(j))->min());
            }
        }
    } else if (mode == "max") {
        for (int i = 0; i < res.size(); ++i) {
            for (int j = 0; j < numOfAttrRges; ++j) {
                QCOMPARE(res.at(i).value(j), ascope.value(res.at(i).name(j))->max());
            }
        }
    } else if (mode == "rand") {
        for (int i = 0; i < res.size(); ++i) {
            for (int j = 0; j < numOfAttrRges; ++j) {
                QVERIFY(res.at(i).value(j) >= ascope.value(res.at(i).name(j))->min());
                QVERIFY(res.at(i).value(j) <= ascope.value(res.at(i).name(j))->max());
            }
        }
    }
}

AttributesScope TestAttrsGenerator::_newAttrsScope(const QStringList& names, const QStringList& attrRges)
{
    Q_ASSERT(names.size() == attrRges.size());
    AttributesScope attrsScope;
    attrsScope.reserve(names.size());
    for (int i = 0; i  < names.size(); ++i) {
        AttributeRange* a = AttributeRange::parse(i, names.at(i), attrRges.at(i));
        attrsScope.insert(a->attrName(), a);
    }
    return attrsScope;
}

void TestAttrsGenerator::_tst_parseStarCmd(QString& mode)
{
    AttributesScope attrsScope;
    AttrsGenerator* agen;
    SetOfAttributes res;
    Attributes attrs;
    const int sizeOfAgen = 3;
    const QStringList names = {"test0", "test1", "test2"};
    const QStringList attrRgeStrs = {"int[0,2]", "double[2.3,7.8]", "int{-2,0,2,4,6}"};
    const QString seed("_123");
    QString error, cmd;

    // Valid * command with empty attrScope
    if (!strcmp(Value(mode).toString(), "rand")) {
        cmd = QString("*%1;%2%3").arg(Value(sizeOfAgen).toQString()).arg(mode).arg(seed);
    } else {
        cmd = QString("*%1;%2").arg(Value(sizeOfAgen).toQString()).arg(mode);
    }

    agen = AttrsGenerator::parse(attrsScope, cmd, error);

    QCOMPARE(agen->command(), cmd);
    QCOMPARE(agen->size(), sizeOfAgen);

    res = agen->create();

    _tst_attrs(res, attrs, true);

    // Valid * command with non-empty attrScope
    attrsScope = _newAttrsScope(names, attrRgeStrs);
    agen = AttrsGenerator::parse(attrsScope, cmd, error);

    attrs.resize(3);
    for (int i = 0; i < attrs.size(); ++i) {
        attrs.replace(i, names.at(i), NULL);
    }

    QCOMPARE(agen->command(), cmd);
    QCOMPARE(agen->size(), sizeOfAgen);

    res = agen->create();

    _tst_attrs(res, attrs, false);
    _tst_mode(res, mode, attrsScope, 3);
}

void TestAttrsGenerator::tst_parseStarCmd()
{
    _tst_parseStarCmd("min");
    _tst_parseStarCmd("max");
    _tst_parseStarCmd("rand");
}

void TestAttrsGenerator::tst_parseHashCmd_min()
{
    AttrsGenerator* agen;
    SetOfAttributes res;
    Attributes attrs;
    const int sizeOfAgen = 3;
    const QStringList names = {"test0", "test1", "test2", "test3"};
    const QStringList attrRgeStrs = {"int[0,2]", "double[2.3,7.8]", "int{-2,0,2,4,6}", "double{-2.2, -1.1, 0, 2.3}"};
    QString error;

    // To shorten line:
    const QStringList cmdList = {
        Value(sizeOfAgen).toQString(),
        QString("%1_min").arg(names.at(0)),
        QString("%1_min").arg(names.at(1)),
        QString("%1_min").arg(names.at(2)),
        QString("%1_min").arg(names.at(3))
    };
    const QString cmd = QString("#%1;%2;%3;%4;%5").arg(cmdList.at(0)).arg(cmdList.at(1)).arg(cmdList.at(2)).arg(cmdList.at(3)).arg(cmdList.at(4));

    // Valid # command with non-empty attrScope
    const AttributesScope attrsScope = _newAttrsScope(names, attrRgeStrs);

    agen = AttrsGenerator::parse(attrsScope, cmd, error);

    attrs.resize(4);
    for (int i = 0; i < attrs.size(); ++i) {
        attrs.replace(i, names.at(i), attrsScope.value(names.at(i))->min());
    }

    QCOMPARE(agen->command(), cmd);
    QCOMPARE(agen->size(), sizeOfAgen);

    res = agen->create();

    _tst_attrs(res, attrs, true);
}

void TestAttrsGenerator::tst_parseHashCmd_max()
{
    AttrsGenerator* agen;
    SetOfAttributes res;
    Attributes attrs;
    const int sizeOfAgen = 3;
    const QStringList names = {"test0", "test1", "test2", "test3"};
    const QStringList attrRgeStrs = {"int[0,2]", "double[2.3,7.8]", "int{-2,0,2,4,6}", "double{-2.2, -1.1, 0, 2.3}"};
    QString error;

    // To shorten line:
    const QStringList cmdList = {
        Value(sizeOfAgen).toQString(),
        QString("%1_max").arg(names.at(0)),
        QString("%1_max").arg(names.at(1)),
        QString("%1_max").arg(names.at(2)),
        QString("%1_max").arg(names.at(3))
    };
    const QString cmd = QString("#%1;%2;%3;%4;%5").arg(cmdList.at(0)).arg(cmdList.at(1)).arg(cmdList.at(2)).arg(cmdList.at(3)).arg(cmdList.at(4));

    // Valid # command with non-empty attrScope
    const AttributesScope attrsScope = _newAttrsScope(names, attrRgeStrs);

    agen = AttrsGenerator::parse(attrsScope, cmd, error);

    attrs.resize(4);
    for (int i = 0; i < attrs.size(); ++i) {
        attrs.replace(i, names.at(i), attrsScope.value(names.at(i))->max());
    }

    QCOMPARE(agen->command(), cmd);
    QCOMPARE(agen->size(), sizeOfAgen);

    res = agen->create();

    _tst_attrs(res, attrs, true);
}

void TestAttrsGenerator::tst_parseHashCmd_rand()
{
    AttrsGenerator* agen;
    SetOfAttributes res;
    Attributes attrs;
    const int sizeOfAgen = 3;
    const QStringList names = {"test0", "test1", "test2", "test3"};
    const QStringList attrRgeStrs = {"int[0,2]", "double[2.3,7.8]", "int{-2,0,2,4,6}", "double{-2.2, -1.1, 0, 2.3}"};
    QString error;

    // To shorten line:
    const QStringList cmdList = {
        Value(sizeOfAgen).toQString(),
        QString("%1_rand_123").arg(names.at(0)),
        QString("%1_rand_123").arg(names.at(1)),
        QString("%1_rand_123").arg(names.at(2)),
        QString("%1_rand_123").arg(names.at(3))
    };
    const QString cmd = QString("#%1;%2;%3;%4;%5").arg(cmdList.at(0)).arg(cmdList.at(1)).arg(cmdList.at(2)).arg(cmdList.at(3)).arg(cmdList.at(4));

    // Valid # command with non-empty attrScope
    AttributesScope attrsScope = _newAttrsScope(names, attrRgeStrs);

    attrs.resize(4);
    agen = AttrsGenerator::parse(attrsScope, cmd, error);

    for (int i = 0; i < attrs.size(); ++i) {
        attrs.replace(i, names.at(i), NULL);
    }

    QCOMPARE(agen->command(), cmd);
    QCOMPARE(agen->size(), sizeOfAgen);

    res = agen->create();

    _tst_attrs(res, attrs, false);
    _tst_mode(res, "rand", attrsScope, 4);
}

void TestAttrsGenerator::tst_parseHashCmd_setValue()
{
    AttrsGenerator* agen;
    SetOfAttributes res;
    Attributes attrs;
    const Values values = {Value(1), Value(3.6), Value(4), Value(-1.1)};
    const int sizeOfAgen = 3;
    const QStringList names = {"test0", "test1", "test2", "test3"};
    const QStringList attrRgeStrs = {"int[0,2]", "double[2.3,7.8]", "int{-2,0,2,4,6}", "double{-2.2, -1.1, 0, 2.3}"};
    QString error;

    // To shorten line:
    const QStringList cmdList = {
        Value(sizeOfAgen).toQString(),
        QString("%1_value_%2").arg(names.at(0)).arg(values[0].toQString()),
        QString("%1_value_%2").arg(names.at(1)).arg(values[1].toQString()),
        QString("%1_value_%2").arg(names.at(2)).arg(values[2].toQString()),
        QString("%1_value_%2").arg(names.at(3)).arg(values[3].toQString())
    };
    const QString cmd = QString("#%1;%2;%3;%4;%5").arg(cmdList.at(0)).arg(cmdList.at(1)).arg(cmdList.at(2)).arg(cmdList.at(3)).arg(cmdList.at(4));

    // Valid # command with non-empty attrScope
    AttributesScope attrsScope = _newAttrsScope(names, attrRgeStrs);

    agen = AttrsGenerator::parse(attrsScope, cmd, error);

    attrs.resize(4);
    for (int i = 0; i < attrs.size(); ++i) {
        attrs.replace(i, names.at(i), values[i]);
    }

    QCOMPARE(agen->command(), cmd);
    QCOMPARE(agen->size(), sizeOfAgen);

    res = agen->create();

    _tst_attrs(res, attrs, true);
}

void TestAttrsGenerator::tst_parseHashCmd_mixedFunc()
{
    AttrsGenerator* agen;
    SetOfAttributes res;
    Attributes attrs;
    const Value value = Value(-1.1);
    const int sizeOfAgen = 3;
    const QStringList names = {"test0", "test1", "test2", "test3"};
    const QStringList attrRgeStrs = {"int[0,2]", "double[2.3,7.8]", "int{-2,0,2,4,6}", "double{-2.2, -1.1, 0, 2.3}"};
    QString error;

    // To shorten line:
    QStringList cmdList = {
        Value(sizeOfAgen).toQString(),
        QString("%1_min").arg(names.at(0)),
        QString("%1_max").arg(names.at(1)),
        QString("%1_rand_123").arg(names.at(2)),
        QString("%1_value_%2").arg(names.at(3)).arg(value.toQString())
    };
    const QString cmd = QString("#%1;%2;%3;%4;%5").arg(cmdList.at(0)).arg(cmdList.at(1)).arg(cmdList.at(2)).arg(cmdList.at(3)).arg(cmdList.at(4));

    // Valid # command with non-empty attrScope
    AttributesScope attrsScope = _newAttrsScope(names, attrRgeStrs);

    agen = AttrsGenerator::parse(attrsScope, cmd, error);

    attrs.resize(4);
    for (int i = 0; i < (attrs.size()-1); ++i) {
        attrs.replace(i, names.at(i), NULL);
    }
     attrs.replace(3, names.at(3), value);

    QCOMPARE(agen->command(), cmd);
    QCOMPARE(agen->size(), sizeOfAgen);

    res = agen->create();

    _tst_attrs(res, attrs, false);

    for (int i = 0; i < res.size(); ++i) {
        QCOMPARE(res.at(i).value(0), attrsScope.value(names.at(0))->min());
        QCOMPARE(res.at(i).value(1), attrsScope.value(names.at(1))->max());
        QVERIFY(res.at(i).value(2) <= attrsScope.value(names.at(2))->max());
        QVERIFY(res.at(i).value(2) >= attrsScope.value(names.at(2))->min());
        QCOMPARE(res.at(i).value(3), value);
    }
}

} // evoplex
QTEST_MAIN(evoplex::TestAttrsGenerator)
#include "tst_attrsgenerator.moc"
