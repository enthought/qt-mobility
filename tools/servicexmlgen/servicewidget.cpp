/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "servicewidget.h"
#include "interfacestabwidget.h"
#include "mandatorylineedit.h"
#include "errorcollector.h"

#include <servicemetadata_p.h>

#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>

#define SERVICE_IPC_PREFIX "_q_ipc_addr:"

//QTM_USE_NAMESPACE

ServiceWidget::ServiceWidget(QWidget *parent)
    : QWidget(parent),
      m_ifacesTabs(new InterfacesTabWidget),
      m_title(new QLabel),
      m_name(new MandatoryLineEdit(tr("(Name required)"))),
      m_path(new MandatoryLineEdit(tr("(Path required)"))),
      m_desc(new QLineEdit),
      m_type(new QComboBox),
      m_text(QLatin1String("filepath"))
{
    connect(m_name, SIGNAL(textChanged(QString)), SLOT(setTitle(QString)));

    connect(m_name, SIGNAL(textEdited(QString)), SIGNAL(dataChanged()));
    connect(m_path, SIGNAL(textEdited(QString)), SIGNAL(dataChanged()));
    connect(m_desc, SIGNAL(textEdited(QString)), SIGNAL(dataChanged()));
    connect(m_ifacesTabs, SIGNAL(dataChanged()), SIGNAL(dataChanged()));
    connect(m_type, SIGNAL(currentIndexChanged(int)), SLOT(serviceTypeChanged(int)));

    QFont f = m_title->font();
    f.setBold(true);
    m_title->setFont(f);
    setTitle(QString());

    QPushButton *buttonAdd = new QPushButton(tr("Add interface"));
    buttonAdd->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(buttonAdd, SIGNAL(clicked()), m_ifacesTabs, SLOT(addInterface()));

    QStringList serviceTypes;
    serviceTypes << tr("* Plugin path") << tr("* IPC address");
    m_type->addItems(serviceTypes);

    QGroupBox *serviceAttrsGroup = new QGroupBox(tr("Main attributes"));
    QFormLayout *form = new QFormLayout;
    form->addRow(tr("* Name"), m_name);
    form->addRow(m_type, m_path);
    form->addRow(tr("Description"), m_desc);
    serviceAttrsGroup->setLayout(form);

    QGroupBox *interfaceTabsGroup = new QGroupBox(tr("Interfaces (must have at least one)"));
    QVBoxLayout *groupLayout = new QVBoxLayout;
    groupLayout->addWidget(m_ifacesTabs);
    groupLayout->addWidget(buttonAdd);
    interfaceTabsGroup->setLayout(groupLayout);

    QVBoxLayout *main = new QVBoxLayout;
    main->addWidget(m_title, 0, Qt::AlignHCenter);
    main->addWidget(serviceAttrsGroup);
    main->addWidget(interfaceTabsGroup);
    main->addWidget(new QLabel(tr("* Denotes mandatory attributes.")), 0, Qt::AlignHCenter);
    setLayout(main);
}

void ServiceWidget::load(const QTM_PREPEND_NAMESPACE(ServiceMetaDataResults) &data)
{
    m_name->setText(data.name);
    m_path->setText(data.location);
    m_desc->setText(data.description);
    m_ifacesTabs->load(data.interfaces);

    m_name->setFocus();
}

void ServiceWidget::validate(ErrorCollector *errors)
{
    m_name->validate(errors);
    m_path->validate(errors);
    m_ifacesTabs->validate(errors);

    if (m_path->text().startsWith(SERVICE_IPC_PREFIX))
        errors->addError("Invalid service plugin path, avoid private prefixes.");
}

void ServiceWidget::writeXml(QXmlStreamWriter *writer) const
{
    writer->writeTextElement(QLatin1String("name"), !m_name->hasText() ? QString() : m_name->text());
    writer->writeTextElement(m_text, !m_path->hasText() ? QString() : m_path->text());
    writer->writeTextElement(QLatin1String("description"), m_desc->text());
    m_ifacesTabs->writeXml(writer);
}

void ServiceWidget::setTitle(const QString &text)
{
    if (text.isEmpty())
        m_title->setText(tr("[New Service]"));
    else
        m_title->setText(text);
}

void ServiceWidget::serviceTypeChanged(int type)
{
    if (type == 0)
        m_text = QLatin1String("filepath");
    else
        m_text = QLatin1String("ipcaddress");

    emit dataChanged();
}
