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

#include <QVBoxLayout>
#include <QSpacerItem>

#include "queuepage.h"
#include "ui_queuepage.h"
#include "core/experiment.h"
#include "core/project.h"
#include "core/mainapp.h"

namespace evoplex {

QueuePage::QueuePage(MainGUI* mainGUI)
    : QScrollArea(mainGUI)
    , m_ui(new Ui_QueuePage)
{
    m_ui->setupUi(this);

    int col = 0;
    m_headerIdx.insert(TableWidget::H_BUTTON, col++);
    m_headerIdx.insert(TableWidget::H_PROJID, col++);
    m_headerIdx.insert(TableWidget::H_EXPID, col++);
    m_headerIdx.insert(TableWidget::H_STOPAT, col++);
    m_headerIdx.insert(TableWidget::H_TRIALS, col++);
    const QList<TableWidget::Header> header = m_headerIdx.keys();
    m_ui->tableRunning->insertColumns(header);
    m_ui->tableQueue->insertColumns(header);
    m_ui->tableIdle->insertColumns(header);

    m_ui->running->hide();
    m_ui->queue->hide();
    m_ui->idle->hide();
/* FIXME
    ExperimentsMgr* expMgr = mainGUI->mainApp()->expMgr();
    connect(expMgr, SIGNAL(statusChanged(Experiment*)), SLOT(slotStatusChanged(Experiment*)));

    connect(m_ui->bClearQueue, SIGNAL(clicked(bool)), expMgr, SLOT(clearQueue()));
    connect(m_ui->bClearIdle, SIGNAL(clicked(bool)), expMgr, SLOT(clearIdle()));

    connect(m_ui->tableIdle, &QTableWidget::cellClicked, [this]() {
            m_ui->tableQueue->clearSelection(); m_ui->tableRunning->clearSelection();});
    connect(m_ui->tableQueue, &QTableWidget::cellClicked, [this]() {
            m_ui->tableIdle->clearSelection(); m_ui->tableRunning->clearSelection();});
    connect(m_ui->tableRunning, &QTableWidget::cellClicked, [this]() {
            m_ui->tableIdle->clearSelection(); m_ui->tableQueue->clearSelection();});
*/
}

void QueuePage::slotStatusChanged(Experiment* exp)
{
    Q_UNUSED(exp);
    /* FIXME
    const rowKey key = std::make_pair(exp->project()->id(), exp->id());
    Row prev = m_rows.value(key, Row());
    Row next;

    const Experiment::Status s = exp->expStatus();
    if (s == Experiment::RUNNING) {
        next.table = m_ui->tableRunning;
        next.section = m_ui->running;
    } else if (s == Experiment::QUEUED) {
        next.table = m_ui->tableQueue;
        next.section = m_ui->queue;
    } else if (s == Experiment::INVALID || exp->autoDeleteTrials() || exp->trials().empty()) {
        removeRow(prev);
        m_rows.remove(key);
        return;
    } else {
        next.table = m_ui->tableIdle;
        next.section = m_ui->idle;
    }

    next.section->show();
    if (prev.table == next.table) {
        return;
    } else if (prev.table) {
        next.item = prev.item;
        moveRow(prev.table, prev.item->row(), next.table, exp);
        prev.section->setVisible(prev.table->rowCount() > 0);
    } else {
        next.item = insertRow(next.table, exp);
    }

    m_rows.insert(key, next);
    emit (isEmpty(false));*/
}

QTableWidgetItem* QueuePage::insertRow(TableWidget* table, Experiment* exp)
{
    const int row = table->insertRow(exp);

    auto add = [this, table, row](TableWidget::Header header, int label) {
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(label));
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, m_headerIdx.value(header), item);
        return item;
    };

    add(TableWidget::H_STOPAT, exp->stopAt());
    add(TableWidget::H_TRIALS, exp->numTrials());
    add(TableWidget::H_PROJID, exp->project()->id());
    return add(TableWidget::H_EXPID, exp->id());
}

void QueuePage::moveRow(TableWidget* prevTable, int preRow, TableWidget* nextTable, Experiment* exp)
{
    const int nextRow = nextTable->insertRow(exp);
    const int cols = prevTable->columnCount();
    for (int col = 0; col < cols; ++col) {
        QTableWidgetItem* item = prevTable->takeItem(preRow, col);
        nextTable->setItem(nextRow, col, item);
    }
    prevTable->removeRow(preRow);
}

void QueuePage::removeRow(const Row& r)
{
    if (!r.table || !r.item) {
        return;
    }

    r.table->removeRow(r.item->row());
    r.section->setVisible(r.table->rowCount() > 0);
    emit (isEmpty(!m_ui->tableIdle->rowCount()
                  && !m_ui->tableRunning->rowCount()
                  && !m_ui->tableQueue->rowCount()));
}
}
