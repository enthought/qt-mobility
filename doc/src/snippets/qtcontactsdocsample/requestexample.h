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


#ifndef REQUESTEXAMPLE_H
#define REQUESTEXAMPLE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QObject>
#include "qcontactfetchrequest.h"

class RequestExample : public QObject
{
    Q_OBJECT

public:
    RequestExample()
            : QObject(),
            m_previousLastIndex(-1),
            m_manager(0),
            m_fetchRequest(new QContactFetchRequest)
    {
    }

    ~RequestExample()
    {
        delete m_fetchRequest;
    }

    void setManager(QContactManager* manager)
    {
        m_manager = manager;
        m_fetchRequest->setManager(m_manager);
    }

private slots:
    void performRequest();
    void printContacts(QContactFetchRequest* request, bool appendOnly);

private:
    int m_previousLastIndex;
    QContactManager *m_manager;
    QContactFetchRequest *m_fetchRequest;
};

#endif