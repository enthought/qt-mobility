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

#include "cntsimstore.h"
#include "cntsimstoreprivate.h"
#include "cntsymbiansimtransformerror.h"

CntSimStore::CntSimStore(CntSymbianSimEngine* engine, QString storeName, QContactManager::Error* error)
    :QObject((QObject *)engine),
     d_ptr(0)
{
    *error = QContactManager::NoError;
    
    // We need to register these meta types for signals because connect() with 
    // Qt::QueuedConnection it is required.
    qRegisterMetaType<QContact>("QContact");
    qRegisterMetaType<QList<QContact> >("QList<QContact>");
    qRegisterMetaType<QContactManager::Error>("QContactManager::Error");
    qRegisterMetaType<QList<int> >("QList<int>");
    
    TRAPD(err, d_ptr = CntSimStorePrivate::NewL(*engine, *this, storeName));
    CntSymbianSimTransformError::transformError(err, error);
}

CntSimStore::~CntSimStore()
{
    delete d_ptr;
}

SimStoreInfo CntSimStore::storeInfo()
{
    return d_ptr->storeInfo();
}

bool CntSimStore::read(int index, int numSlots, QContactManager::Error* error)
{
    return d_ptr->read(index, numSlots, error);
}

bool CntSimStore::write(const QContact &contact, QContactManager::Error* error)
{
    return d_ptr->write(contact, error);
}

bool CntSimStore::remove(int index, QContactManager::Error* error)
{
    return d_ptr->remove(index, error);
}

bool CntSimStore::getReservedSlots(QContactManager::Error* error)
{
    return d_ptr->getReservedSlots(error);
}

void CntSimStore::cancel()
{
    d_ptr->Cancel();
}

bool CntSimStore::isBusy()
{
    return d_ptr->IsActive();
}

TInt CntSimStore::lastAsyncError()
{
    return d_ptr->lastAsyncError();
}
