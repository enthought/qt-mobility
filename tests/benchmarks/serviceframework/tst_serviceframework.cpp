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

//TESTED_COMPONENT=src/serviceframework

#include <QtTest/QtTest>
#include "qservicemanager.h"
#include <QDebug>

QTM_USE_NAMESPACE

class tst_serviceframework : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void tst_createTime();
    void tst_addService_data();
    void tst_addService();

    void tst_findServices();

    void tst_loadService_data();
    void tst_loadService();

    void tst_invalidLoad();

private:
    QServiceManager *manager;

};

void tst_serviceframework::initTestCase()
{
    manager = new QServiceManager;
    QStringList exampleXmlFiles;
    //exampleXmlFiles <<"voipdialerservice.xml" << "landlinedialerservice.xml";
    exampleXmlFiles << "bm_sampleservice.xml";
    foreach (const QString &fileName, exampleXmlFiles) {
        QString path = QCoreApplication::applicationDirPath() + "/plugins" + "/xmldata/" + fileName;
        //qDebug() << "Addning: " << path;
        manager->addService(path);
    }
}

void tst_serviceframework::tst_createTime()
{
    QBENCHMARK {
        QServiceManager qsm;
    }    
}

void tst_serviceframework::tst_addService_data()
{    
    QTest::addColumn<QString>("service");

    QStringList exampleXmlFiles;
    //exampleXmlFiles <<"voipdialerservice.xml" << "landlinedialerservice.xml";
    exampleXmlFiles << "bm_sampleservice.xml";
    foreach (const QString &fileName, exampleXmlFiles) {        
        QString path = QCoreApplication::applicationDirPath() + "/plugins" + "/xmldata/" + fileName;
        //qDebug() << "Addning: " << path;
        QTest::newRow(fileName.toAscii()) << path;
    }
}

void tst_serviceframework::tst_addService()
{
    QServiceManager qsm;
    QFETCH(QString, service);

    QBENCHMARK {
        qsm.addService(service);
    }
}

Q_DECLARE_METATYPE ( QServiceInterfaceDescriptor )

void tst_serviceframework::tst_loadService_data()
{
    qRegisterMetaType<QServiceInterfaceDescriptor>("QServiceInterfaceDescriptor");

    QTest::addColumn<QString>("path");
    QTest::addColumn<QString>("service");
    QTest::addColumn<QServiceInterfaceDescriptor>("interface");    

    QStringList exampleXmlFiles;
    //exampleXmlFiles <<"voipdialerservice.xml" << "landlinedialerservice.xml";
    exampleXmlFiles << "bm_sampleservice.xml";
    foreach (const QString &fileName, exampleXmlFiles) {
        QServiceManager qsm;
        QString path = QCoreApplication::applicationDirPath() + "/plugins" + "/xmldata/" + fileName;
        //qDebug() << "Adding: " << path;
        qsm.addService(path);
        QStringList servicesList = qsm.findServices();
        foreach (QString service, servicesList) {
            //qDebug() << "Service: " << service;
            QList<QServiceInterfaceDescriptor> qlid = qsm.findInterfaces(service);
            while(!qlid.isEmpty()){
                QServiceInterfaceDescriptor id = qlid.takeFirst();
                QStringList name;
                QString verM, verm;
                verM.setNum(id.majorVersion(), 10);
                verm.setNum(id.minorVersion(), 10);
                name << "Service: " << service << " Interface: " << id.interfaceName() << " version: " << verM << "/" << verm;
                QTest::newRow(name.join("").toAscii()) << path << service << id;
            }
        }
    }
}

void tst_serviceframework::tst_loadService()
{
    QServiceManager qsm;
    QFETCH(QString, path);
    QFETCH(QString, service);
    QFETCH(QServiceInterfaceDescriptor, interface);    

    qsm.addService(path);

    QBENCHMARK {        
        QObject *o = qsm.loadInterface(interface);
        Q_UNUSED(o);
    }
}


void tst_serviceframework::tst_findServices()
{
    QServiceManager qsm;
    QStringList s;
    QBENCHMARK {
        s = qsm.findServices();
    }    
}

void tst_serviceframework::tst_invalidLoad()
{
    QServiceManager qsm;    

    QBENCHMARK {
        qsm.addService("invalid_filename_that_we_home_does_not_exist_aspasdm");
    }
}



QTEST_MAIN(tst_serviceframework)
#include "tst_serviceframework.moc"
