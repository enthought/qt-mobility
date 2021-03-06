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

#include "qgallerytrackermetadataedit_p.h"

#include <QtDBus/qdbuspendingcall.h>

QTM_BEGIN_NAMESPACE

QGalleryTrackerMetaDataEdit::QGalleryTrackerMetaDataEdit(
        const QGalleryDBusInterfacePointer &metaDataInterface,
        const QString &uri,
        const QString &service,
        QObject *parent)
    : QObject(parent)
    , m_watcher(0)
    , m_index(-1)
    , m_metaDataInterface(metaDataInterface)
    , m_uri(uri)
    , m_service(service)
{
}

QGalleryTrackerMetaDataEdit::~QGalleryTrackerMetaDataEdit()
{
}

void QGalleryTrackerMetaDataEdit::commit()
{
    if (m_values.isEmpty()) {
        emit finished(this);
    } else {
        m_watcher = new QDBusPendingCallWatcher(m_metaDataInterface->asyncCall(
                QLatin1String("Set"),
                m_service,
                m_uri,
                QStringList(m_values.keys()),
                QStringList(m_values.values())), this);

        if (m_watcher->isFinished()) {
            watcherFinished(m_watcher);
        } else {
            connect(m_watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                    this, SLOT(watcherFinished(QDBusPendingCallWatcher*)));
        }
    }
}

void QGalleryTrackerMetaDataEdit::itemsInserted(int index, int count)
{
    if (index < m_index)
        m_index += count;
}

void QGalleryTrackerMetaDataEdit::itemsRemoved(int index, int count)
{
    if (index + count < m_index)
        m_index -= count;
    else if (index < m_index)
        m_index = -1;
}

void QGalleryTrackerMetaDataEdit::watcherFinished(QDBusPendingCallWatcher *watcher)
{
    Q_ASSERT(watcher == m_watcher);

    m_watcher->deleteLater();
    m_watcher = 0;

    if (watcher->isError()) {
        qWarning("DBUS error %s", qPrintable(watcher->error().message()));

        m_values.clear();
    }

    emit finished(this);
}

#include "moc_qgallerytrackermetadataedit_p.cpp"

QTM_END_NAMESPACE
