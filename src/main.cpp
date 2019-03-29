#include <QApplication>
#include "DWGWidget.h"

int main (int argc, char** argv)
{

    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("DWG-RADIO");
    QCoreApplication::setOrganizationName("DWG Radio");
    QGuiApplication::setApplicationDisplayName(QCoreApplication::applicationName());
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    DWGWidget player;
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&player);


    player.show();

    return app.exec();
}
