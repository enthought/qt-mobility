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

#ifndef GCONFLAYER_H
#define GCONFLAYER_H

#include "qvaluespace_p.h"
#include "qvaluespacepublisher.h"

#include <QSet>
#include "gconfitem_p.h"
#include <QMutex>

QTM_BEGIN_NAMESPACE

class GConfLayer : public QAbstractValueSpaceLayer
{
    Q_OBJECT

public:
    GConfLayer();
    virtual ~GConfLayer();

protected:
    /*virtual*/ QString name();

    /*virtual*/ bool startup(Type type);

    /*virtual*/ QUuid id();
    /*virtual*/ unsigned int order();

    /*virtual*/ Handle item(Handle parent, const QString &subPath);
    /*virtual*/ void removeHandle(Handle handle);
    /*virtual*/ void setProperty(Handle handle, Properties properties);

    /*virtual*/ bool value(Handle handle, QVariant *data);
    /*virtual*/ bool value(Handle handle, const QString &subPath, QVariant *data);
    /*virtual*/ QSet<QString> children(Handle handle);

    /*virtual*/ QValueSpace::LayerOptions layerOptions() const;

    /* QValueSpaceSubscriber functions */
    /*virtual*/ bool supportsInterestNotification() const;
    /*virtual*/ bool notifyInterest(Handle handle, bool interested);

    /* QValueSpacePublisher functions */
    /*virtual*/ bool setValue(QValueSpacePublisher *creator, Handle handle,
                              const QString &subPath, const QVariant &value);
    /*virtual*/ bool removeValue(QValueSpacePublisher *creator, Handle handle,
                                 const QString &subPath);
    /*virtual*/ bool removeSubTree(QValueSpacePublisher *creator, Handle handle);
    /*virtual*/ void addWatch(QValueSpacePublisher *creator, Handle handle);
    /*virtual*/ void removeWatches(QValueSpacePublisher *creator, Handle parent);
    /*virtual*/ void sync();

public:
    static GConfLayer *instance();

private slots:
    void notifyChanged(const QString &key, const QVariant &value);

private:
    struct GConfHandle {
        GConfHandle(const QString &p)
        :   path(p), refCount(1)
        {
        }

        QString path;
        unsigned int refCount;
    };

    QHash<QString, GConfHandle *> m_handles;

    GConfHandle *gConfHandle(Handle handle)
    {
        if (handle == InvalidHandle)
            return 0;

        GConfHandle *h = reinterpret_cast<GConfHandle *>(handle);
        if (m_handles.values().contains(h))
            return h;

        return 0;
    }

    //private methods not locking a mutex
    bool getValue(Handle handle, const QString &subPath, QVariant *data);
    Handle getItem(Handle parent, const QString &subPath);
    void doRemoveHandle(Handle handle);

private:    //data
    QSet<GConfHandle *> m_monitoringHandles;
    QMap<QString, GConfItem *> m_monitoringItems;
    QMutex m_mutex;
};

QTM_END_NAMESPACE

#endif //GCONFLAYER_H
