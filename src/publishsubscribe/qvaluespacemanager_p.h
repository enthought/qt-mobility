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

#ifndef QVALUESPACEMANAGER_P_H
#define QVALUESPACEMANAGER_P_H

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

#include "qvaluespace_p.h"

QT_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QValueSpaceManager
{
public:
    QValueSpaceManager();

    void initServer();
    void init();
    bool isServer() const;

    void install(QAbstractValueSpaceLayer * layer);
    void install(QValueSpace::LayerCreateFunc func);
    QList<QAbstractValueSpaceLayer *> const & getLayers();

    static QValueSpaceManager *instance();

private:
    void commonInit(QAbstractValueSpaceLayer::Type);
    bool initLayer(QAbstractValueSpaceLayer* layer);

    enum { Uninit, Server, Client } type;
    QList<QAbstractValueSpaceLayer *> layers;
    QList<QValueSpace::LayerCreateFunc> funcs;
};

QT_END_NAMESPACE

#endif // QVALUESPACEMANAGER_P_H