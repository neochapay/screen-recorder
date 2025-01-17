/*******************************************************************************
**
** Copyright (C) 2023 Yura Beznos at You-ra.info
**
** This file is part of the Screen recorder application project.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write an email to the license@you-ra.info
*******************************************************************************/

#include <QtQuick>
#include <auroraapp.h>

#include "interfacer.h"
#include "recordingsmodel.h"
#include "settings.h"

int main(int argc, char* argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("info.you_ra"));
    application->setApplicationName(QStringLiteral("screen_recorder"));

    QScopedPointer<QQuickView> view(Aurora::Application::createView());

    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", "/usr/libexec/info.you_ra.screen_recorder/");
    process.setProcessEnvironment(env);
    if (!process.startDetached("/usr/libexec/info.you_ra.screen_recorder/lipstick2vnc")) {
        qWarning() << "CANT START libstick2vnc";
    }

    // Add Qt translations
    QTranslator* translator = new QTranslator();
    translator->load(QLocale::system(), "screen_recorder", "_", Aurora::Application::pathTo("translations").toLocalFile(), ".qm");
    application->installTranslator(translator);

    auto context = view->rootContext();
    qmlRegisterType<Interfacer>("info.you_ra.ScreenRecorder", 1, 0, "Interfacer");
    qmlRegisterType<Settings>("info.you_ra.ScreenRecorder", 1, 0, "Settings");

    Interfacer interfacer;
    context->setContextProperty("srec", &interfacer);

    RecordingsModel sourceModel;
    sourceModel.setInterfacer(&interfacer);

    QSortFilterProxyModel recordingsModel;
    recordingsModel.setSourceModel(&sourceModel);
    recordingsModel.setSortRole(RecordingsModel::Modified);
    recordingsModel.setDynamicSortFilter(true);
    recordingsModel.sort(0, Qt::DescendingOrder);
    context->setContextProperty("recordingsModel", &recordingsModel);

    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/screen_recorder.qml")));
    view->show();

    return application->exec();
}
