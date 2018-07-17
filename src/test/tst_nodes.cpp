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

#include <QtTest>
#include <QDir>
#include <QStringList>

#include <attributerange.h>
#include <nodes.h>

namespace evoplex {
class TestNodes: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {}
    void cleanupTestCase() {}

    void tst_fromCmd();

    // saving a set of nodes without attributes
    void tst_saveToFile_no_attrs();
    // saving a set of nodes with attributes
    void tst_saveToFile_with_attrs();

    // file with valid attributes, file without 2d coordinates
    void tst_fromFile_nodes_no_xy();
    // valid attributes AND both xy coordinates
    void tst_fromFile_nodes_with_xy();
    // valid attributes AND x coordinates (only)
    void tst_fromFile_nodes_with_x();
    // valid attributes AND y coordinates (only)
    void tst_fromFile_nodes_with_y();
    // invalid attributes
    void tst_fromFile_nodes_invalid_attrs();
    // invalid file
    void tst_fromFile_nodes_invalid_file();
private:
    // checks if sets of nodes have the same content
    void _compare_nodes(const Nodes& a, const Nodes& b) const;
    void _tst_empty_nodes(const Nodes& a, int size);
    void _tst_Node(Node *node, Attributes attrs);
    void _tst_attrs(NodePtr node, Attributes attrs);
};



void TestNodes::_tst_empty_nodes(const Nodes& a, int size){
    QCOMPARE(a.size(), size);
    for(int i = 0; i < a.size(); i++){
    QVERIFY(a.at(i)->attrs().isEmpty());
    QCOMPARE(a.at(i)->degree(), 0);
    QCOMPARE(a.at(i)->inDegree(), 0);
    QCOMPARE(a.at(i)->outDegree(), 0);

//    // Nodes are created at the origin
//    QCOMPARE(nodes.at(i)->x(), 0);
//    QCOMPARE(nodes.at(i)->y(), 1);
}

}
void TestNodes::_tst_attrs(NodePtr node, Attributes attrs)
{
    QCOMPARE(node->attrs().names(), attrs.names());
//    QCOMPARE(node->attrs().values(), attrs.values());


    QCOMPARE(node->attrs().size(), attrs.size());
}

//void TestNodes::_tst_Node(NodePtr node, Attributes attrs)
//{
//    int id = 0, x = 123, y = 456;

//    QCOMPARE(node->id(), id);
//    _tst_attrs(node, attrs);
//    QCOMPARE(node->x(), x);
//    QCOMPARE(node->y(), y);
//}

// Empty commands should fail
void TestNodes::tst_fromCmd()
{
    QString errorMsg;
    NodePtr node;
    const QStringList names = { "test0", "test1", "test2" };
    const Value values[] = { Value(123), Value(234), Value(456) };

    // Empty commands
    // Undirected with empty attrsScope
    QString cmd = "";
    AttributesScope attrsScope;
    GraphType graphType = GraphType::Undirected;
    Nodes nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    _tst_empty_nodes(nodes, 0);

    // Undirected with non-empty attrsScope
    AttributeRange* col0 = AttributeRange::parse(0, names[0], "int[0,1000]");
    attrsScope.insert(col0->attrName(), col0);
    AttributeRange* col1 = AttributeRange::parse(1, names[1], "int[0,1000]");
    attrsScope.insert(col1->attrName(), col1);
    AttributeRange* col2 = AttributeRange::parse(2, names[2], "int[0,1000]");
    attrsScope.insert(col2->attrName(), col2);

    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    QCOMPARE(nodes.size(), 0);

    // Directed with empty attrsScope
    graphType = GraphType::Directed;
    attrsScope.clear();
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    _tst_empty_nodes(nodes, 0);

    // Directed with non-empty attrsScope
    col0 = AttributeRange::parse(0, names[0], "int[0,1000]");
    attrsScope.insert(col0->attrName(), col0);
    col1 = AttributeRange::parse(1, names[1], "int[0,1000]");
    attrsScope.insert(col1->attrName(), col1);
    col2 = AttributeRange::parse(2, names[2], "int[0,1000]");
    attrsScope.insert(col2->attrName(), col2);

    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    QCOMPARE(nodes.size(), 0);

    // Valid * command cases
    // Undirected with empty attrsScope
    attrsScope.clear();
    cmd = "*3;min";
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    _tst_empty_nodes(nodes, 3);

    // Undirected with non-empty attrsScope
    col0 = AttributeRange::parse(0, names[0], "int[0,1000]");
    attrsScope.insert(col0->attrName(), col0);
    col1 = AttributeRange::parse(1, names[1], "int[0,1000]");
    attrsScope.insert(col1->attrName(), col1);
    col2 = AttributeRange::parse(2, names[2], "int[0,1000]");
    attrsScope.insert(col2->attrName(), col2);

    Attributes attrs(3);
    for(int i = 0; i < 3; i++){
       attrs.replace(i, names[i], values[i]);
    }

    cmd = "*3;min";
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);

    for(int i = 0; i < 3; i++){
        node = nodes.at(i);
        _tst_attrs(node, attrs);
    }

    // Directed with empty attrsScope
    graphType = GraphType::Directed;
    attrsScope.clear();

    cmd = "*3;min";
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    _tst_empty_nodes(nodes, 3);

    // Directed with non-empty attrsScope
    col0 = AttributeRange::parse(0, names[0], "int[0,1000]");
    attrsScope.insert(col0->attrName(), col0);
    col1 = AttributeRange::parse(1, names[1], "int[0,1000]");
    attrsScope.insert(col1->attrName(), col1);
    col2 = AttributeRange::parse(2, names[2], "int[0,1000]");
    attrsScope.insert(col2->attrName(), col2);

    for(int i = 0; i < 3; i++){
       attrs.replace(i, names[i], values[i]);
    }

    cmd = "*3;min";
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);

    for(int i = 0; i < 3; i++){
        NodePtr node = nodes.at(i);
        _tst_attrs(node, attrs);
    }

    // Valid # command cases
    // Undirected with single attribute
    graphType = GraphType::Undirected;
    attrs.resize(1);
    attrsScope.clear();
    col0 = AttributeRange::parse(0, names[0], "int[0,1000]");
    attrsScope.insert(col0->attrName(), col0);

    cmd = QString("#3;%1_value_%2").arg(names[0]).arg(values[0].toQString());
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);

    QCOMPARE(nodes.size(), 3);
    QCOMPARE(nodes.at(0)->attrs().value(names[0]), values[0]);
    node = nodes.at(0);
    _tst_attrs(node, attrs);

    // Undirected with multiple attributes
    col1 = AttributeRange::parse(1, names[1], "int[0,1000]");
    attrsScope.insert(col1->attrName(), col1);

    cmd = QString("#3;%1_value_%2;%3_value_%4").arg(names[0]).arg(values[0].toQString()).arg(names[1]).arg(values[1].toQString());
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);

    QCOMPARE(nodes.size(), 3);
    node = nodes.at(0);
    attrs.resize(2);
    for(int i = 0; i < 2; i++){
       attrs.replace(i, names[i], values[i]);
    }
    _tst_attrs(node, attrs);

    // Directed with single attribute
    graphType = GraphType::Directed;
    attrs.resize(1);
    attrsScope.clear();
    col0 = AttributeRange::parse(0, names[0], "int[0,1000]");
    attrsScope.insert(col0->attrName(), col0);

    cmd = QString("#3;%1_value_%2").arg(names[0]).arg(values[0].toQString());
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);

    QCOMPARE(nodes.size(), 3);
    QCOMPARE(nodes.at(0)->attrs().value(names[0]), values[0]);
    node = nodes.at(0);
    _tst_attrs(node, attrs);

    // Directed with multiple attributes
    col1 = AttributeRange::parse(1, names[1], "int[0,1000]");
    attrsScope.insert(col1->attrName(), col1);

    cmd = QString("#3;%1_value_%2;%3_value_%4").arg(names[0]).arg(values[0].toQString()).arg(names[1]).arg(values[1].toQString());
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);

    QCOMPARE(nodes.size(), 3);
    node = nodes.at(0);
    attrs.resize(2);
    for(int i = 0; i < 2; i++){
       attrs.replace(i, names[i], values[i]);
    }
    _tst_attrs(node, attrs);

    // Invalid commands
    // Undirected with empty attrsScope
    cmd = "this-is-invalid";
    attrsScope;
    graphType = GraphType::Undirected;
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    _tst_empty_nodes(nodes, 0);

    // Undirected with non-empty attrsScope
    col0 = AttributeRange::parse(0, names[0], "int[0,1000]");
    attrsScope.insert(col0->attrName(), col0);
    col1 = AttributeRange::parse(1, names[1], "int[0,1000]");
    attrsScope.insert(col1->attrName(), col1);
    col2 = AttributeRange::parse(2, names[2], "int[0,1000]");
    attrsScope.insert(col2->attrName(), col2);

    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    QCOMPARE(nodes.size(), 0);

    // Directed with empty attrsScope
    graphType = GraphType::Directed;
    attrsScope.clear();
    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    _tst_empty_nodes(nodes, 0);

    // Directed with non-empty attrsScope
    col0 = AttributeRange::parse(0, names[0], "int[0,1000]");
    attrsScope.insert(col0->attrName(), col0);
    col1 = AttributeRange::parse(1, names[1], "int[0,1000]");
    attrsScope.insert(col1->attrName(), col1);
    col2 = AttributeRange::parse(2, names[2], "int[0,1000]");
    attrsScope.insert(col2->attrName(), col2);

    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    QCOMPARE(nodes.size(), 0);

//     *      - negavite number of nodes
//    cmd = "#-3;myInt_value_123;myInt2_value_123";
//    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);

//     *      - zero nodes
//    cmd = "#0;myInt_value_123;myInt2_value_123";
//    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);

//     *      - cmd with an attribute that is not in attrsScope
//    cmd = "#3;myInt_value_123;myInt3_value_123";
//    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
//    // Fails, but only when the following tests are performed:
//    QCOMPARE(nodes.size(), 3);

//     *  - Invalid graph type: should fail
//    cmd = "#3;myInt_value_123;myInt2_value_123";
//    graphType = GraphType::Invalid;
//    nodes = Nodes::fromCmd(cmd, attrsScope, graphType, errorMsg);
    // Fails, but only when the following tests are performed:
//    QCOMPARE(nodes.size(), 3);

//     *  - Undirected graph: should return a set of UNodes
//     *  - Directed graph: should return a set of DNodes


     /** IMPORTANT! The command validation is powered by another class, which we will be testing later.
      *    So, the main purpose of this test is to check if the returned Nodes are reflecting the command,
      *    that's the number of nodes is right, the attrs are there etc.
      *    There's no need to extend it to the point of testing all possible combinations of commands
      *    NOR testing if all possible functions (eg., min, max, rand ...) work as expected -- it'll be done later for another class.
     */
}

void TestNodes::tst_fromFile_nodes_no_xy()
{
    QString errorMsg;
    GraphType graphType = GraphType::Undirected;

    // valid attrsScope for the existing file
    const QString filePath(":/data/data/nodes_no_xy.csv");
    AttributesScope attrsScope;
    AttributeRange* col0 = AttributeRange::parse(0, "int-zero-two", "int[0,2]");
    attrsScope.insert(col0->attrName(), col0);
    AttributeRange* col1 = AttributeRange::parse(1, "bool", "bool");
    attrsScope.insert(col1->attrName(), col1);
    AttributeRange* col2 = AttributeRange::parse(2, "any-string", "string");
    attrsScope.insert(col2->attrName(), col2);

    Nodes nodes = Nodes::fromFile(filePath, attrsScope, graphType, errorMsg);

    /*
     * Potential cases:
     *  - check if Nodes contains the same data as 'nodes_no_xy.csv'
     *  - what if attrsScope has more attributes than 'nodes_no_xy.csv'? should fail
     */
}

/****************** TO DO ****************************/
// valid attributes AND both xy coordinates
void TestNodes::tst_fromFile_nodes_with_xy() {
    QString errorMsg;
    GraphType graphType = GraphType::Undirected;

    // valid attrsScope for the existing file
    const QString filePath(":/data/data/nodes_with_xy.csv");
    AttributesScope attrsScope;

    // AttributeRange to be used for both sets of nodes
    AttributeRange* col0 = AttributeRange::parse(0, "bool", "bool");
    attrsScope.insert(col0->attrName(), col0);

    // Nodes with values read from file
    Nodes nodesFromFile = Nodes::fromFile(filePath, attrsScope, graphType, errorMsg);

    // Nodes to test against
    Nodes nodes = Nodes::fromCmd("*3;min", attrsScope, graphType, errorMsg);

    nodes.at(0)->setAttr(0, true);
    nodes.at(0)->setX(123);
    nodes.at(0)->setY(456);

    nodes.at(1)->setAttr(0, false);
    nodes.at(1)->setX(456);
    nodes.at(1)->setY(-789);

    nodes.at(2)->setAttr(0, true);
    nodes.at(2)->setX(-789);
    nodes.at(2)->setY(789);

    _compare_nodes(nodes,nodesFromFile);
}
// valid attributes AND x coordinates (only)
void TestNodes::tst_fromFile_nodes_with_x() {
//    QString errorMsg;
//    GraphType graphType = GraphType::Undirected;

//    // valid attrsScope for the existing file
//    const QString filePath(":/data/data/nodes_with_x.csv");
//    AttributesScope attrsScope;

//    // Attribute Range to be used for both sets of nodes
//    AttributeRange* col0 = AttributeRange::parse(0, "double-zero-one", "double[0,3]");
//    attrsScope.insert(col0->attrName(), col0);

//    // Nodes with values read from file
//    Nodes nodesFromFile = Nodes::fromFile(filePath, attrsScope, graphType, errorMsg);

//    // 'Nodes::fromFile gives a warning 'failed to read attributes from file' because
//    // 'Nodes::validateHeader' returns an empty QStringList
//    // This is because if can find a column with an x value, but no y value:
////    if ((xIdx != -1 && yIdx == -1) || (xIdx == -1 && yIdx != -1)) {
////        error = "missing 'x' or 'y' columns. It should have both or none.";
////        return QStringList();
////    }
//    // This is similar to the case for 'TestNodes::tst_fromFile_nodes_with_y'
//    // The error is not outputted, however.

//    // Nodes to test against ones from file
//    Nodes nodes = Nodes::fromCmd("*3;min", attrsScope, graphType, errorMsg);

//    nodes.at(0)->setAttr(0, 0.12345678);
//    nodes.at(0)->setX(123);

//    nodes.at(1)->setAttr(0, 1.49999999);
//    nodes.at(1)->setX(789);

//    nodes.at(2)->setAttr(0, 2);
//    nodes.at(2)->setX(456);

//    // equivalent to _compare_nodes without the y value test
//    QCOMPARE(nodes.size(), nodesFromFile.size());
//    for (int id = 0; id < static_cast<int>(nodes.size()); ++id) {
//        NodePtr nA = nodes.at(id);
//        NodePtr nB = nodesFromFile.at(id);
//        QCOMPARE(nA->id(), nB->id());
//        QCOMPARE(nA->attrs().names(), nB->attrs().names());
//        QCOMPARE(nA->attrs().values(), nB->attrs().values());
//        QCOMPARE(nA->x(), nB->x());
//    }
}
// valid attributes AND y coordinates (only)
void TestNodes::tst_fromFile_nodes_with_y() {
//    QString errorMsg;
//    GraphType graphType = GraphType::Undirected;

//    // valid attrsScope for the existing file
//    const QString filePath(":/data/data/nodes_with_y.csv");
//    AttributesScope attrsScope;

//    // Attribute Range to be used for both sets of nodes
//    AttributeRange* col0 = AttributeRange::parse(0, "double-zero-one", "double[0,2]");
//    attrsScope.insert(col0->attrName(), col0);

//    // Nodes with values read from file
//    Nodes nodesFromFile = Nodes::fromFile(filePath, attrsScope, graphType, errorMsg);

//    // Nodes to test against ones from file
//    Nodes nodes = Nodes::fromCmd("*3;min", attrsScope, graphType, errorMsg);

//    nodes.at(0)->setAttr(0, 1.49999999);
//    nodes.at(0)->setX(123);

//    nodes.at(1)->setAttr(0, 1.49999999);
//    nodes.at(1)->setX(789);

//    nodes.at(2)->setAttr(0, 2);
//    nodes.at(2)->setX(456);

//    // equivalent to _compare_nodes without the x value test
//    QCOMPARE(nodes.size(), nodesFromFile.size());
//    for (int id = 0; id < static_cast<int>(nodes.size()); ++id) {
//        NodePtr nA = nodes.at(id);
//        NodePtr nB = nodesFromFile.at(id);
//        QCOMPARE(nA->id(), nB->id());
//        QCOMPARE(nA->attrs().names(), nB->attrs().names());
//        QCOMPARE(nA->attrs().values(), nB->attrs().values());
//        QCOMPARE(nA->y(), nB->y());
//    }
}
// invalid attributes
void TestNodes::tst_fromFile_nodes_invalid_attrs() {
    QString errorMsg;
    GraphType graphType = GraphType::Undirected;

    // valid attrsScope for the existing file
    const QString filePath(":/data/data/nodes_with_invalid_attributes.csv");
    AttributesScope attrsScope;

    // Attribute Range to be used for both sets of nodes
    AttributeRange* col0 = AttributeRange::parse(0, "bool", "bool");
    attrsScope.insert(col0->attrName(), col0);

    // Nodes with values read from file
    Nodes nodesFromFile = Nodes::fromFile(filePath, attrsScope, graphType, errorMsg);

    QCOMPARE(nodesFromFile.size(), 0);
}
// invalid file
void TestNodes::tst_fromFile_nodes_invalid_file() {
    QString errorMsg;
    GraphType graphType = GraphType::Undirected;

    // valid attrsScope for the existing file
    const QString filePath(":/data/data/nodes_with_invalid_file.csv");
    AttributesScope attrsScope;

    // Attribute Range to be used for both sets of nodes
    AttributeRange* col0 = AttributeRange::parse(0, "bool", "bool");
    attrsScope.insert(col0->attrName(), col0);

    // Nodes with values read from file
    Nodes nodesFromFile = Nodes::fromFile(filePath, attrsScope, graphType, errorMsg);

    QCOMPARE(nodesFromFile.size(), 0);
}
/****************** TO DO ****************************/

void TestNodes::tst_saveToFile_no_attrs()
{
    QString errorMsg;
    const QString tempFilePath = QDir::temp().absoluteFilePath("nodes.csv");

    // saving a set of nodes without attributes
    AttributesScope attrsScope;
    Nodes nodes = Nodes::fromCmd("*5;min", attrsScope, GraphType::Undirected, errorMsg);
    QVERIFY(nodes.saveToFile(tempFilePath));

    // retrieve saved file
    Nodes nodesFromFile = Nodes::fromFile(tempFilePath, attrsScope, GraphType::Undirected, errorMsg);
    _compare_nodes(nodes, nodesFromFile);
}

void TestNodes::tst_saveToFile_with_attrs()
{
    QString errorMsg;
    const QString tempFilePath = QDir::temp().absoluteFilePath("nodes.csv");

    /** TO DO:
     * similar to 'tst_saveToFile_no_attrs()', but here we pass a non-empty attrsScope **/
}

void TestNodes::_compare_nodes(const Nodes& a, const Nodes& b) const
{
    QCOMPARE(a.size(), b.size());
    for (int id = 0; id < static_cast<int>(a.size()); ++id) {
        NodePtr nA = a.at(id);
        NodePtr nB = b.at(id);
        QCOMPARE(nA->id(), nB->id());
        QCOMPARE(nA->attrs().names(), nB->attrs().names());
        QCOMPARE(nA->attrs().values(), nB->attrs().values());
        QCOMPARE(nA->x(), nB->x());
        QCOMPARE(nA->y(), nB->y());
    }
}

} // evoplex
QTEST_MAIN(evoplex::TestNodes)
#include "tst_nodes.moc"
