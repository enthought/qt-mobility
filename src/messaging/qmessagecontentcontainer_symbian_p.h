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
#ifndef QMESSAGECONTENTCONTAINER_P_H
#define QMESSAGECONTENTCONTAINER_P_H

#include "qmessagecontentcontainer.h"
#include "qmessage.h"

#include <QSharedData>
#include <QList>
#include <QMultiMap>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <eikenv.h>
#include <apgcli.h>		// for RApaLsSession
#include <utf.h>       	// CnvUtfConverter
#include <apmrec.h>		// TDataRecognitionResult
#ifdef FREESTYLEMAILUSED
#include "emailapidefs.h" // TMessageContentId
#endif

QTM_BEGIN_NAMESPACE
class QMessageContentContainerPrivate
{
    Q_DECLARE_PUBLIC(QMessageContentContainer)

public:
    

    QMessageContentContainer *q_ptr;
    QMessage *_message;

    bool _available;
    int _size;
#ifdef FREESTYLEMAILUSED
    mutable bool _contentRetrieved;
    bool _freestyleAttachment;
    EmailInterface::TMessageContentId _fsContentId;
#endif
    QByteArray _type;
    QByteArray _subType;
    QByteArray _charset;
    QByteArray _name;
    QByteArray _content;
    QString _textContent;
    QByteArray _filename;
    QMessageId _messageId;
    QMessageContentContainerId _id;
    QList<QMessageContentContainer> _attachments;
    QMultiMap<QByteArray, QString>  _header;
    long int _containingMessageId;
    unsigned int _attachmentId;
    
    QMessageContentContainerPrivate(QMessageContentContainer *contentContainer)
            :
            q_ptr(contentContainer), _message(0), _available(false), _size(0),
#ifdef FREESTYLEMAILUSED
            _contentRetrieved(true),
            _freestyleAttachment(false),
            _fsContentId(),
#endif
            _containingMessageId(0), _attachmentId(0)
    {
    }

    QMessageContentContainerPrivate(const QMessageContentContainerPrivate& other)
    	: 
    	q_ptr(other.q_ptr), _message(other._message), _available(other._available),
        _size(other._size),
#ifdef FREESTYLEMAILUSED
        _contentRetrieved(other._contentRetrieved),
        _freestyleAttachment(other._freestyleAttachment),
        _fsContentId(other._fsContentId),
#endif
        _type(other._type), _subType(other._subType),
    	_charset(other._charset), _name(other._name), _content(other._content),
    	_textContent(other._textContent), _filename(other._filename), _messageId(other._messageId),
    	_id(other._id),  _attachments(other._attachments), _header(other._header),
        _containingMessageId(other._containingMessageId), _attachmentId(other._attachmentId)
    {
    }

    QMessageContentContainerPrivate& operator=(const QMessageContentContainerPrivate &other);
    
    bool isMessage() const;

    void setDerivedMessage(QMessage *derived);

    void clearContents();
    
    QMessageContentContainer* attachment(const QMessageContentContainerId &id);
    
    const QMessageContentContainer* attachment(const QMessageContentContainerId &id) const;
    
    void setContentType(const QByteArray &type, const QByteArray &subType, const QByteArray &charset);

    void setContent(const QString &content, const QByteArray &type, const QByteArray &subType, const QByteArray &charset);

    void setContent(const QByteArray &content, const QByteArray &type, const QByteArray &subType, const QByteArray &charset);

    void setHeaderField(const QByteArray &name, const QByteArray &value);

    bool createAttachment(const QString& attachmentPath);

    QMessageContentContainerId appendContent(QMessageContentContainer& container);

    QMessageContentContainerId prependContent(QMessageContentContainer& container);

    static QMessageContentContainerId bodyContentId();
    
    static QByteArray attachmentFilename(const QMessageContentContainer& container);
    
#ifdef FREESTYLEMAILUSED
    static QMessageContentContainer from(long int messageId, unsigned int attachmentId, QByteArray &name,
                                         QByteArray &mimeType, QByteArray &mimeSubType, int size,
                                         EmailInterface::TMessageContentId fsContentId = EmailInterface::TMessageContentId());
#else
    static QMessageContentContainer from(long int messageId, unsigned int attachmentId, QByteArray &name,
                                         QByteArray &mimeType, QByteArray &mimeSubType, int size);
#endif
    
    static QMessageContentContainerPrivate* implementation(const QMessageContentContainer &container);
};

QTM_END_NAMESPACE

#endif
