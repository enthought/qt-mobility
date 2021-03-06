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
#ifndef QMAILADDRESS_H
#define QMAILADDRESS_H

#include "qmailglobal.h"
#include "qmailipc.h"
#include <QList>
#include <QSharedDataPointer>
#include <QString>
#include <QStringList>

class QMailAddressPrivate;

class QTOPIAMAIL_EXPORT QMailAddress
{
public:
    QMailAddress();
    explicit QMailAddress(const QString& addressText);
    QMailAddress(const QString& name, const QString& emailAddress);
    QMailAddress(const QMailAddress& other);
    ~QMailAddress();

    bool isNull() const;

    QString name() const;
    QString address() const;

    bool isGroup() const;
    QList<QMailAddress> groupMembers() const;

    bool isPhoneNumber() const;
    bool isEmailAddress() const;

    QString minimalPhoneNumber() const;

    QString toString(bool forceDelimited = false) const;

    bool operator==(const QMailAddress& other) const;
    bool operator!=(const QMailAddress& other) const;

    const QMailAddress& operator=(const QMailAddress& other);

    template <typename Stream> void serialize(Stream &stream) const;
    template <typename Stream> void deserialize(Stream &stream);

    static QStringList toStringList(const QList<QMailAddress>& list, bool forceDelimited = false);
    static QList<QMailAddress> fromStringList(const QString& list);
    static QList<QMailAddress> fromStringList(const QStringList& list);

    static QString removeComments(const QString& input);
    static QString removeWhitespace(const QString& input);

    static QString phoneNumberPattern();
    static QString emailAddressPattern();

private:
    QSharedDataPointer<QMailAddressPrivate> d;
};

Q_DECLARE_USER_METATYPE(QMailAddress)

typedef QList<QMailAddress> QMailAddressList;

Q_DECLARE_METATYPE(QMailAddressList);
Q_DECLARE_USER_METATYPE_TYPEDEF(QMailAddressList, QMailAddressList);

#endif
