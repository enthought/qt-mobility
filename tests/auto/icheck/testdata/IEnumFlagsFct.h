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

class IEnumFlags 
{
public:
    Q_ENUMS(Enum1 Enum2 Enum3)
    enum Enum1 {
        E10 = 0,
        E11,
        E12,
        E13
    };

    enum Enum2 {
        E20 = 0,
        E21,
        E22,
        E23
    };

    enum Enum3 {
        E30 = 0,
        E31,
        E32,
        E33
    };

    //using wrong flag name
    Q_FLAGS(EnumF1 EnumF2 EnumF3)
    enum EnumF1 {
        EF10 = 0,
        EF11,
        EF12,
        EF13
    };

    enum EnumF2 {
        EF20 = 0,
        EF21,
        EF22,
        EF23
    };

    enum EnumF3 {
        EF30 = 0,
        EF31,
        EF32,
        EF33
    };

    enum EnumDF1 {
        DF10 = 0,
        DF11,
        DF12,
        DF13
    };

    enum EnumDF2 {
        DF20 = 0,
        DF21,
        DF22,
        DF23
    };
    Q_DECLARE_FLAGS(FlagsDF1, EnumDF1)
    Q_DECLARE_FLAGS(FlagsDF2, EnumDF2)
    Q_FLAGS(FlagsDF1 FlagsDF2)

    //this enum is implemented in QFrame
    Q_ENUMS(Shape)
    enum Shape{
        NoFrame = 0,
        Box = 0x0001,
        Panel = 0x0002,
        StyledPanel = 0x0006,
        HLine = 0x0004,
        VLine = 0x0005,
        WinPanel = 0x0003
    };
};
