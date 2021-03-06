/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#include "cntfilterdefault.h"
#include "cnttransformcontact.h"

CntFilterDefault::CntFilterDefault(CContactDatabase& contactDatabase,CntSymbianSrvConnection &cntServer,CntDbInfo& dbInfo) 
                                        :m_contactdatabase(contactDatabase),
                                         m_srvConnection(cntServer),
                                         m_dbInfo(dbInfo)
{
   
}

CntFilterDefault::~CntFilterDefault()
{
    
}

QList<QContactLocalId> CntFilterDefault::contacts(
        const QContactFilter &filter,
        const QList<QContactSortOrder> &sortOrders,
        bool &filterSupportedflag,
        QContactManager::Error* error)
{
    Q_UNUSED(sortOrders);
    Q_UNUSED(filterSupportedflag);
    //Check if any invalid filter is passed 
    if(!filterSupported(filter))
            {
            *error = QContactManager::NotSupportedError;
            return QList<QContactLocalId>();
            }
    QList<QContactLocalId> idList;
    
    //Create the query
    QString sqlQuery;
    createSelectQuery( filter,sqlQuery,error);
    QString sortQuery = m_dbInfo.getSortQuery(sortOrders, sqlQuery, error);
    
    //fetch the contacts
    if(*error == QContactManager::NoError)
    {
        idList =  m_srvConnection.searchContacts(sortQuery, error);
    }
    
    return idList;
    
}


bool CntFilterDefault::filterSupported(const QContactFilter& filter) 
{
    bool result = false;
       if(QContactFilter::DefaultFilter == filter.type())
           {
           result = true;
           }
    
       return result;
}


void CntFilterDefault::createSelectQuery(const QContactFilter& filter,
                              QString& sqlQuery,
                              QContactManager::Error* error)

{
    //Check if any invalid filter is passed 
    if(!filterSupported(filter))
            {
            *error =  QContactManager::NotSupportedError;
            }
   //For default filter, just return the below query
    sqlQuery = "SELECT DISTINCT contact_id FROM  contact WHERE (type_flags>>24)<=1 OR (type_flags>>24)=3";

    
}
