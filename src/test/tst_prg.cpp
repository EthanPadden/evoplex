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

#include <QtTest>
#include <prg.h>

using namespace evoplex;

class TestPRG: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {}
    void cleanupTestCase() {}
    void tst_prg();
    void tst_randD();
    void tst_randI();
    void tst_randS();
    void tst_randF();
};

void TestPRG::tst_prg()
{
    PRG* prg1 = new PRG(921);
    PRG* prg2 = new PRG(921);

    QCOMPARE(prg1->randD(), prg2->randD(0.0, 1.0));
    QCOMPARE(prg1->randD(), prg2->randD(1.0));
    QCOMPARE(prg1->randI(123), prg2->randI(0, 123));

    double d = prg1->randD();
    int i = prg1->randI(123);
    QCOMPARE(d, prg2->randD());
    QCOMPARE(i, prg2->randI(123));
    QVERIFY(d != prg2->randD());
    QVERIFY(i != prg2->randI(123));

    delete prg1;
    prg1 = new PRG(922);
    QVERIFY(d != prg1->randD());
    QVERIFY(i != prg1->randI(123));

    delete prg1;
    delete prg2;

    prg1 = new PRG(0);
    for (int i = 0; i < 1000; ++i) prg1->randD();
    prg2 = new PRG(0);
    for (int i = 0; i < 1000; ++i) prg2->randD();
    QCOMPARE(prg1->randD(), prg2->randD());
    QCOMPARE(prg1->randI(10), prg2->randI(10));

    delete prg1;
    delete prg2;
}

void TestPRG::tst_randD()
{
    PRG* prg = new PRG(0);
    double v, min, max;

    // [0, 1)
    min = 0.0;
    max = 1.0;
    v = prg->randD(min, max);
    QVERIFY(v >= min && v < max);
    v = prg->randD();
    QVERIFY(v >= min && v < max);

    // negative
    min = -100.0;
    max = -50.0;
    v = prg->randD(min, max);
    QVERIFY(v >= min && v < max);

    // same number
    min = 123.45;
    max = min;
    v = prg->randD(min, max);
    QVERIFY(v == min);

    delete prg;
}

void TestPRG::tst_randI()
{
    PRG* prg = new PRG(0);
    int v, min, max;

    // [0, 10)
    min = 0;
    max = 10;
    v = prg->randI(min, max);
    QVERIFY(v >= min && v <= max);
    v = prg->randI(max);
    QVERIFY(v >= min && v <= max);

    // negative
    min = -100;
    max = -50;
    v = prg->randI(min, max);
    QVERIFY(v >= min && v <= max);

    // same number
    min = 123;
    max = min;
    v = prg->randI(min, max);
    QVERIFY(v == min);

    delete prg;
}

void TestPRG::tst_randS()
{
    PRG* prg = new PRG(0);
    size_t v, min, max;

    // [0, 10)
    min = 0;
    max = 10;
    v = prg->randS(min, max);
    QVERIFY(v >= min && v <= max);

    // negative
    min = -100;
    max = -50;
    v = prg->randS(min, max);
    QVERIFY(v >= min && v <= max);

    // same number
    min = 123;
    max = min;
    v = prg->randS(min, max);
    QVERIFY(v == min);

    // large numbers
    min = UINT64_MAX - 10;
    max = UINT64_MAX;
    v = prg->randS(min, max);
    QVERIFY(v >= min && v <= max);

    delete prg;
}

void TestPRG::tst_randF()
{
    PRG* prg = new PRG(0);
    float v, min, max;

    // [0, 10)
    min = 0;
    max = 10;
    v = prg->randF(min, max);
    QVERIFY(v >= min && v <= max);
    v = prg->randF(max);
    QVERIFY(v >= min && v <= max);
    v = prg->randF();
    max = 1;
    QVERIFY(v >= min && v < max);

    // negative
    min = -100;
    max = -50;
    v = prg->randF(min, max);
    QVERIFY(v >= min && v <= max);

    // same number
    min = 123;
    max = min;
    v = prg->randF(min, max);
    QVERIFY(v == min);

    delete prg;
}
QTEST_MAIN(TestPRG)
#include "tst_prg.moc"
