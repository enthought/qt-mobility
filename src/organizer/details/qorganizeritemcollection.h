/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QORGANIZERITEMCOLLECTION_H
#define QORGANIZERITEMCOLLECTION_H

#include <QString>

#include "qtorganizerglobal.h"
#include "qorganizeritemdetail.h"
#include "qorganizeritem.h"

QTM_BEGIN_NAMESPACE

/* Leaf class */
class Q_ORGANIZER_EXPORT QOrganizerItemCollection : public QOrganizerItemDetail
{
public:
#ifdef Q_QDOC
    const char* DefinitionName;
    const char* FieldCollectionIds;
#else
    Q_DECLARE_CUSTOM_ORGANIZER_DETAIL(QOrganizerItemCollection, "Collection")
    Q_DECLARE_LATIN1_CONSTANT(FieldCollectionId, "CollectionId");
#endif

    // XXX TODO: add this to the schema, register mapping from id to variant.
    // In default schema, this is a read-only detail; no setCollectionId() fn.

    QOrganizerItemId collectionId() const
    {
        return QOrganizerItemId();
        // XXX TODO
    }
};

QTM_END_NAMESPACE

#endif
