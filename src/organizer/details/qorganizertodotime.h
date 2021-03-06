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

#ifndef QORGANIZERTODOTIME_H
#define QORGANIZERTODOTIME_H

#include <QString>

#include "qtorganizerglobal.h"
#include "qorganizeritemdetail.h"
#include "qorganizeritem.h"

QTM_BEGIN_NAMESPACE

/* Leaf class */
class Q_ORGANIZER_EXPORT QOrganizerTodoTime : public QOrganizerItemDetail
{
public:
#ifdef Q_QDOC
    static const QLatin1Constant DefinitionName;
    static const QLatin1Constant FieldStartDateTime;
    static const QLatin1Constant FieldDueDateTime;
    static const QLatin1Constant FieldAllDay;
#else
    Q_DECLARE_CUSTOM_ORGANIZER_DETAIL(QOrganizerTodoTime, "TodoTime")
    Q_DECLARE_LATIN1_CONSTANT(FieldStartDateTime, "StartDateTime");
    Q_DECLARE_LATIN1_CONSTANT(FieldDueDateTime, "DueDateTime");
    Q_DECLARE_LATIN1_CONSTANT(FieldAllDay, "AllDay");
#endif

    void setStartDateTime(const QDateTime& startDateTime) {setValue(FieldStartDateTime, startDateTime);}
    QDateTime startDateTime() const {return value<QDateTime>(FieldStartDateTime);}
    void setDueDateTime(const QDateTime& dueDateTime) {setValue(FieldDueDateTime, dueDateTime);}
    QDateTime dueDateTime() const {return value<QDateTime>(FieldDueDateTime);}
    void setAllDay(bool isAllDay) {setValue(FieldAllDay, isAllDay);}
    bool isAllDay() const {return value<bool>(FieldAllDay);}
};

QTM_END_NAMESPACE

#endif

