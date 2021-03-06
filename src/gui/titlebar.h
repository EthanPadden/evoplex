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

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QDockWidget>
#include <QWidget>

#include "core/experiment.h"

class Ui_TitleBar;

namespace evoplex {

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(Experiment* exp, QDockWidget* parent);
    ~TitleBar();

signals:
    void trialSelected(int);
    void openSettingsDlg();

private slots:
    void slotRestarted();

protected:
    virtual void paintEvent(QPaintEvent* pe);

private:
    Ui_TitleBar* m_ui;
    Experiment* m_exp;
};
}

#endif // TITLEBAR_H
