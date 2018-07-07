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

#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>

#include "core/project.h"

class Ui_SaveDialog;

namespace evoplex {

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(QWidget* parent);
    ~SaveDialog();

    bool save(ProjectPtr project);
    bool saveAs(ProjectPtr project);

private slots:
    void browseDir();

private:
    Ui_SaveDialog* m_ui;
    ProjectPtr m_currProject;
};
}
#endif // SAVEDIALOG_H
