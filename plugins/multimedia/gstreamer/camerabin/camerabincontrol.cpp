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

#include "camerabincontrol.h"
#include "camerabincontainer.h"
#include "camerabinaudioencoder.h"
#include "camerabinvideoencoder.h"
#include "camerabinimageencoder.h"

#include <QtCore/qdebug.h>
#include <QtCore/qfile.h>

#include <linux/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

CameraBinControl::CameraBinControl(CameraBinSession *session)
    :QCameraControl(session),
    m_session(session),
    m_state(QCamera::UnloadedState),
    m_status(QCamera::UnloadedStatus)
{
    connect(m_session, SIGNAL(stateChanged(QCamera::State)),
            this, SLOT(updateStatus()));

    connect(m_session->audioEncodeControl(), SIGNAL(settingsChanged()),
            SLOT(reloadLater()));
    connect(m_session->videoEncodeControl(), SIGNAL(settingsChanged()),
            SLOT(reloadLater()));
    connect(m_session->mediaContainerControl(), SIGNAL(settingsChanged()),
            SLOT(reloadLater()));
    connect(m_session->imageEncodeControl(), SIGNAL(settingsChanged()),
            SLOT(reloadLater()));
}

CameraBinControl::~CameraBinControl()
{
}

QCamera::CaptureMode CameraBinControl::captureMode() const
{
    return m_session->captureMode();
}

void CameraBinControl::setCaptureMode(QCamera::CaptureMode mode)
{
    if (m_session->captureMode() != mode) {
        m_session->setCaptureMode(mode);
        reloadLater();
    }
}

void CameraBinControl::setState(QCamera::State state)
{
    qDebug() << Q_FUNC_INFO << state;
    if (m_state != state) {
        m_state = state;
        m_session->setState(state);
        emit stateChanged(m_state);
    }
}

QCamera::State CameraBinControl::state() const
{
    return m_state;
}

void CameraBinControl::updateStatus()
{
    QCamera::State sessionState = m_session->state();
    QCamera::Status oldStatus = m_status;

    switch (m_state) {
    case QCamera::UnloadedState:
        m_status = QCamera::UnloadedStatus;
        break;
    case QCamera::LoadedState:
        if (sessionState != QCamera::UnloadedState)
            m_status = QCamera::LoadedStatus;
        else
            m_status = QCamera::LoadingStatus;
        break;
    case QCamera::ActiveState:
        switch (sessionState) {
        case QCamera::UnloadedState:
            m_status = QCamera::LoadingStatus;
            break;
        case QCamera::LoadedState:
            m_status = QCamera::StartingStatus;
            break;
        case QCamera::ActiveState:
            m_status = QCamera::ActiveStatus;
            break;
        }
    }

    if (m_status != oldStatus) {
#ifdef CAMEABIN_DEBUG
        qDebug() << "Camera status changed" << m_status;
#endif
        emit statusChanged(m_status);
    }

}

void CameraBinControl::reloadLater()
{
#ifdef CAMEABIN_DEBUG
    qDebug() << "reload pipeline requested";
#endif
    if (!m_reloadPending && m_state == QCamera::ActiveState) {
        m_reloadPending = true;
        QMetaObject::invokeMethod(this, "reloadPipeline", Qt::QueuedConnection);
    }
}

void CameraBinControl::reloadPipeline()
{
#ifdef CAMEABIN_DEBUG
    qDebug() << "reload pipeline";
#endif
    if (m_reloadPending) {
        m_reloadPending = false;
        if (m_state == QCamera::ActiveState) {
            m_session->setState(QCamera::LoadedState);
            m_session->setState(QCamera::ActiveState);
        }
    }
}