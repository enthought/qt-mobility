/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import Qt 4.7
import QtMultimediaKit 1.1

Rectangle {
    id : cameraUI
    color: "black"
    state: "PhotoCapture"

    states: [
        State {
            name: "PhotoCapture"
            StateChangeScript {
                script: {
                    camera.visible = true
                    camera.focus = true
                    stillControls.visible = true
                    photoPreview.visible = false
                }
            }
        },
        State {
            name: "PhotoPreview"
            StateChangeScript {
                script: {
                    camera.visible = false                    
                    stillControls.visible = false
                    photoPreview.visible = true
                    photoPreview.focus = true
                }
            }
        }
    ]

    PhotoPreview {
        id : photoPreview
        anchors.fill : parent
        onClosed: cameraUI.state = "PhotoCapture"
        focus: visible

        Keys.onPressed : {
            //return to capture mode if the shutter button is touched
            if (event.key == Qt.Key_CameraFocus && !event.isAutoRepeat) {
                cameraUI.state = "PhotoCapture"
                event.accepted = true;
            }
        }
    }

    Camera {
        id: camera
        x: 0
        y: 0
        width: parent.width - stillControls.buttonsPanelWidth
        height: parent.height
        focus: visible //to receive focus and capture key events
        //captureResolution : "640x480"

        flashMode: stillControls.flashMode
        whiteBalanceMode: stillControls.whiteBalance
        exposureCompensation: stillControls.exposureCompensation

        onImageCaptured : {
            photoPreview.source = preview
            stillControls.previewAvailable = true
            cameraUI.state = "PhotoPreview"
        }
    }

    CaptureControls {
        id: stillControls
        anchors.fill: parent
        camera: camera
        onPreviewSelected: cameraUI.state = "PhotoPreview"
    }

}
