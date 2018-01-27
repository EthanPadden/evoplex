/**
 * Copyright (C) 2017 - Marcos Cardinot
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */

#ifndef QUEUEPAGE_H
#define QUEUEPAGE_H

#include <QScrollArea>

#include "maingui.h"
#include "tablewidget.h"

class Ui_QueuePage;

namespace evoplex {

class QueuePage : public QScrollArea
{
    Q_OBJECT

public:
    explicit QueuePage(MainGUI* mainGUI);

signals:
    void isEmpty(bool empty);

private slots:
    void slotRemoveRow(Experiment* exp);
    void slotStatusChanged(Experiment* exp);

private:
    typedef std::pair<int, int> rowKey; // <projId, expId>

    struct Row {
        QTableWidgetItem* item = nullptr; // hold an item just to get access to the current row number
        TableWidget* table = nullptr;
        QWidget* section = nullptr;
    };

    Ui_QueuePage* m_ui;

    QHash<rowKey, Row> m_rows; // map 'projId.expId' to the Row
    QMap<TableWidget::Header, int> m_headerIdx; // map Header to column index

    QTableWidgetItem* insertRow(TableWidget* table, Experiment* exp);
    void moveRow(TableWidget* prevTable, int preRow, TableWidget* nextTable, Experiment* exp);
};
}
#endif // QUEUEPAGE_H
