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

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <functional>

#include "savedialog.h"
#include "ui_savedialog.h"

namespace evoplex {

SaveDialog::SaveDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::SaveDialog)
    , m_currProject(nullptr)
{
    m_ui->setupUi(this);

    connect(m_ui->browse, SIGNAL(clicked(bool)), SLOT(browseDir()));
    connect(m_ui->btn, SIGNAL(rejected()), SLOT(hide()));
    connect(m_ui->btn, &QDialogButtonBox::accepted, [this]() {
        m_currProject->setFilePath(QString("%1/%2.csv")
                .arg(m_ui->dest->text()).arg(m_ui->pname->text()));
        save(m_currProject);
    });
}

SaveDialog::~SaveDialog()
{
    delete m_ui;
}

bool SaveDialog::save(ProjectPtr project)
{
    if (!project) {
        return false;
    } else if (project->filepath().isEmpty()) {
        return saveAs(project);
    }

    hide();
    QProgressDialog progress("Saving project...", QString(), 0, 100, this);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.show();

    QString errMsg;
    std::function<void(int)> f = [&progress](int p){ progress.setValue(p); };
    project->saveProject(errMsg, f);
    m_currProject = project;
    if (!errMsg.isEmpty()) {
        QMessageBox::warning(this, "Saving project...", errMsg);
        return false;
    }
    return true;
}

bool SaveDialog::saveAs(ProjectPtr project)
{
    if (!project) {
        return false;
    }
    m_ui->pname->setText(project->name());
    m_ui->dest->setText(project->filepath().isEmpty() ? QDir::homePath() : QFileInfo(project->filepath()).absolutePath());
    m_currProject = project;
    return exec() == QDialog::Accepted;
}

void SaveDialog::browseDir()
{
    m_ui->dest->setText(QFileDialog::getExistingDirectory(this, tr("Project Directory"),
            m_ui->dest->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

}
