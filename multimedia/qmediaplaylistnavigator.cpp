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

#include <qmediaplaylistnavigator.h>
#include <qmediaplaylistprovider.h>
#include <qmediaplaylist.h>
#include <qmediaobject_p.h>

#include <QtCore/qdebug.h>

class QMediaPlaylistNullProvider : public QMediaPlaylistProvider
{
public:
    QMediaPlaylistNullProvider() :QMediaPlaylistProvider() {}
    virtual ~QMediaPlaylistNullProvider() {}
    virtual int size() const {return 0;}
    virtual QMediaContent media(int) const { return QMediaContent(); }
};

Q_GLOBAL_STATIC(QMediaPlaylistNullProvider, _q_nullMediaPlaylist)

class QMediaPlaylistNavigatorPrivate
{
    Q_DECLARE_NON_CONST_PUBLIC(QMediaPlaylistNavigator)
public:
    QMediaPlaylistNavigatorPrivate()
        :playlist(0),
        currentPos(-1),
        lastValidPos(-1),
        playbackMode(QMediaPlaylist::Linear),
        randomPositionsOffset(-1)
    {
    }

    QMediaPlaylistProvider *playlist;
    int currentPos;
    int lastValidPos; //to be used with CurrentItemOnce playback mode
    QMediaPlaylist::PlaybackMode playbackMode;
    QMediaContent currentItem;

    mutable QList<int> randomModePositions;
    mutable int randomPositionsOffset;

    int nextItemPos(int steps = 1) const;
    int previousItemPos(int steps = 1) const;

    void _q_itemsInserted(int start, int end);
    void _q_itemsRemoved(int start, int end);
    void _q_itemsChanged(int start, int end);

    QMediaPlaylistNavigator *q_ptr;
};


int QMediaPlaylistNavigatorPrivate::nextItemPos(int steps) const
{
    if (playlist->size() == 0)
        return -1;

    if (steps == 0)
        return currentPos;

    switch (playbackMode) {
        case QMediaPlaylist::CurrentItemOnce:
            return /*currentPos == -1 ? lastValidPos :*/ -1;
        case QMediaPlaylist::CurrentItemInLoop:
            return currentPos;
        case QMediaPlaylist::Linear:
            {
                int nextPos = currentPos+steps;
                return nextPos < playlist->size() ? nextPos : -1;
            }
        case QMediaPlaylist::Loop:
            return (currentPos+steps) % playlist->size();
        case QMediaPlaylist::Random:
            {
                //TODO: limit the history size

                if (randomPositionsOffset == -1) {
                    randomModePositions.clear();
                    randomModePositions.append(currentPos);
                    randomPositionsOffset = 0;
                }

                while (randomModePositions.size() < randomPositionsOffset+steps+1)
                    randomModePositions.append(-1);
                int res = randomModePositions[randomPositionsOffset+steps];
                if (res<0 || res >= playlist->size()) {
                    res = qrand() % playlist->size();
                    randomModePositions[randomPositionsOffset+steps] = res;
                }

                return res;
            }
    }

    return -1;
}

int QMediaPlaylistNavigatorPrivate::previousItemPos(int steps) const
{
    if (playlist->size() == 0)
        return -1;

    if (steps == 0)
        return currentPos;

    switch (playbackMode) {
        case QMediaPlaylist::CurrentItemOnce:
            return /*currentPos == -1 ? lastValidPos :*/ -1;
        case QMediaPlaylist::CurrentItemInLoop:
            return currentPos;
        case QMediaPlaylist::Linear:
            {
                int prevPos = currentPos == -1 ? playlist->size() - steps : currentPos - steps;
                return prevPos>=0 ? prevPos : -1;
            }
        case QMediaPlaylist::Loop:
            {
                int prevPos = currentPos - steps;
                while (prevPos<0)
                    prevPos += playlist->size();
                return prevPos;
            }
        case QMediaPlaylist::Random:
            {
                //TODO: limit the history size

                if (randomPositionsOffset == -1) {
                    randomModePositions.clear();
                    randomModePositions.append(currentPos);
                    randomPositionsOffset = 0;
                }

                while (randomPositionsOffset-steps < 0) {
                    randomModePositions.prepend(-1);
                    randomPositionsOffset++;
                }

                int res = randomModePositions[randomPositionsOffset-steps];
                if (res<0 || res >= playlist->size()) {
                    res = qrand() % playlist->size();
                    randomModePositions[randomPositionsOffset-steps] = res;
                }

                return res;
            }
    }

    return -1;
}



/*!
  Create a new \a playlist navigator object.
  */
QMediaPlaylistNavigator::QMediaPlaylistNavigator(QMediaPlaylistProvider *playlist, QObject *parent)
    : QObject(parent)
    , d_ptr(new QMediaPlaylistNavigatorPrivate)
{
    d_ptr->q_ptr = this;

    setPlaylist(playlist ? playlist : _q_nullMediaPlaylist());
}

/*!
  Destroys the playlist.
  */
QMediaPlaylistNavigator::~QMediaPlaylistNavigator()
{
    delete d_ptr;
}


/*!
  */
QMediaPlaylist::PlaybackMode QMediaPlaylistNavigator::playbackMode() const
{
    return d_func()->playbackMode;
}

/*!
  */
void QMediaPlaylistNavigator::setPlaybackMode(QMediaPlaylist::PlaybackMode mode)
{
    Q_D(QMediaPlaylistNavigator);
    if (d->playbackMode == mode)
        return;

    if (mode == QMediaPlaylist::Random) {
        d->randomPositionsOffset = 0;
        d->randomModePositions.append(d->currentPos);
    } else if (d->playbackMode == QMediaPlaylist::Random) {
        d->randomPositionsOffset = -1;
        d->randomModePositions.clear();
    }

    d->playbackMode = mode;

    emit playbackModeChanged(mode);
    emit surroundingItemsChanged();
}

QMediaPlaylistProvider *QMediaPlaylistNavigator::playlist() const
{
    return d_func()->playlist;
}

void QMediaPlaylistNavigator::setPlaylist(QMediaPlaylistProvider *playlist)
{
    Q_D(QMediaPlaylistNavigator);

    if (d->playlist == playlist)
        return;

    if (d->playlist) {
        d->playlist->disconnect(this);
    }

    if (playlist) {
        d->playlist = playlist;
    } else {
        //assign to shared readonly null playlist
        d->playlist = _q_nullMediaPlaylist();
    }

    connect(d->playlist, SIGNAL(itemsInserted(int,int)), SLOT(_q_itemsInserted(int,int)));
    connect(d->playlist, SIGNAL(itemsRemoved(int,int)), SLOT(_q_itemsRemoved(int,int)));
    connect(d->playlist, SIGNAL(itemsChanged(int,int)), SLOT(_q_itemsChanged(int,int)));

    d->randomPositionsOffset = -1;
    d->randomModePositions.clear();

    if (d->currentPos != -1) {
        d->currentPos = -1;
        emit currentPositionChanged(-1);
    }

    if (!d->currentItem.isNull()) {
        d->currentItem = QMediaContent();
        emit activated(d->currentItem); //stop playback
    }
}

/*!
  Returns the current playlist item.
  It is the same as itemAt(currentPosition());

  \sa currentPosition()
  */
QMediaContent QMediaPlaylistNavigator::currentItem() const
{
    return itemAt(d_func()->currentPos);
}

/*!
  */
QMediaContent QMediaPlaylistNavigator::nextItem(int steps) const
{
    return itemAt(nextPosition(steps));
}

/*!
  */

QMediaContent QMediaPlaylistNavigator::previousItem(int steps) const
{
    return itemAt(previousPosition(steps));
}

/*!
  Returns the media source at playlist position \a pos or
  invalid media source object if \a pos is out the playlist positions range.
  */
QMediaContent QMediaPlaylistNavigator::itemAt(int pos) const
{
    return d_func()->playlist->media(pos);
}

/*!
  Returns the current items position in the playlist
  or -1 if no items should be currently played.

  \sa nextPosition(int), previousPosition(int), seek(int)
  */
int QMediaPlaylistNavigator::currentPosition() const
{
    return d_func()->currentPos;
}

/*!
  Returns the next position in the playlist to be played or -1 if playback should be stopped.
  It's usually but not necessary currentPosition()+1 depending on playbackMode().

  For example, for CurrentItemInLoop playback mode it's always the same as currentPosition().

  If \a steps parameter is passed, the item position expected after \a step next() actions returned.

  \sa currentPosition(), previousPosition(int), PlaybackMode
  */
int QMediaPlaylistNavigator::nextPosition(int steps) const
{
    return d_func()->nextItemPos(steps);
}

/*!
  Returns the previously position in playlist.
  It's usually but not necessary currentPosition()-1 depending on playbackMode().

  If \a steps parameter is passed, the item position expected after \a step previous() actions returned.

  \sa nextPosition
  */
int QMediaPlaylistNavigator::previousPosition(int steps) const
{
    return d_func()->previousItemPos(steps);
}

/*!
  Advance to the next item in the playlist.

  \sa previous(), seek(int), playbackMode()
  */
void QMediaPlaylistNavigator::next()
{
    Q_D(QMediaPlaylistNavigator);

    int nextPos = d->nextItemPos();

    if ( playbackMode() == QMediaPlaylist::Random )
            d->randomPositionsOffset++;

    jump(nextPos);
}

/*!
  Advance to the previously item in the playlist,
  depending on playback mode.

  \sa next(), seek(int), playbackMode()
  */
void QMediaPlaylistNavigator::previous()
{
    Q_D(QMediaPlaylistNavigator);

    int prevPos = d->previousItemPos();
    if ( playbackMode() == QMediaPlaylist::Random )
        d->randomPositionsOffset--;

    jump(prevPos);
}

/*!
  Jump to the item at position \a pos.
  The item is also activated.
  */
void QMediaPlaylistNavigator::jump(int pos)
{
    Q_D(QMediaPlaylistNavigator);

    if (pos<-1 || pos>=d->playlist->size()) {
        qWarning() << "QMediaPlaylistNavigator: Jump outside playlist range";
        pos = -1;
    }

    if (pos != -1)
        d->lastValidPos = pos;

    if (playbackMode() == QMediaPlaylist::Random) {
        if (d->randomModePositions[d->randomPositionsOffset] != pos) {
            d->randomModePositions.clear();
            d->randomModePositions.append(pos);
            d->randomPositionsOffset = 0;
        }
    }

    if (pos != -1)
        d->currentItem = d->playlist->media(pos);
    else
        d->currentItem = QMediaContent();

    if (pos != d->currentPos) {
        d->currentPos = pos;
        emit currentPositionChanged(d->currentPos);
        emit surroundingItemsChanged();
    }

    emit activated(d->currentItem);
}

/*!
  \internal
  */
void QMediaPlaylistNavigatorPrivate::_q_itemsInserted(int start, int end)
{
    Q_Q(QMediaPlaylistNavigator);

    if (currentPos >= start) {
        currentPos = end-start+1;
        q->jump(currentPos);
    }

    //TODO: check if they really changed
    emit q->surroundingItemsChanged();
}

/*!
  \internal
  */
void QMediaPlaylistNavigatorPrivate::_q_itemsRemoved(int start, int end)
{
    Q_Q(QMediaPlaylistNavigator);

    if (currentPos > end) {
        currentPos = currentPos - end-start+1;
        q->jump(currentPos);
    } else if (currentPos >= start) {
        //current item was removed
        currentPos = qMin(start, playlist->size()-1);
        q->jump(currentPos);
    }

    //TODO: check if they really changed
    emit q->surroundingItemsChanged();
}

/*!
  \internal
  */
void QMediaPlaylistNavigatorPrivate::_q_itemsChanged(int start, int end)
{
    Q_Q(QMediaPlaylistNavigator);

    if (currentPos >= start && currentPos<=end) {
        QMediaContent src = playlist->media(currentPos);
        if (src != currentItem) {
            currentItem = src;
            emit q->activated(src);
        }
    }

    //TODO: check if they really changed
    emit q->surroundingItemsChanged();
}

/*!
    \fn void QMediaPlaylistNavigator::activated(const QMediaContent &content)

    Signal the playback of \a source should be started.
    it's usually related to change of the current item
    in playlist.
*/

/*!
  \fn void QMediaPlaylistNavigator::currentPositionChanged(int)
  */


#include "moc_qmediaplaylistnavigator.cpp"