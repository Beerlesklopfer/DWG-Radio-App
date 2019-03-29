#include <QtWidgets>
#include <QMediaPlayer>
#include <QSettings>

#include "qactionbutton.h"

class DWGWidget : public QWidget
{
public:
    DWGWidget(QWidget *parent=nullptr);
    ~DWGWidget();

protected:
    bool event(QEvent *ev);

private slots:
    void playOrPause();
    void showOrHide();
    void setMediaTitle(const QMediaContent &media);

private:
    QSettings m_settings;
    QSystemTrayIcon *m_trayIcon;
    QAction *m_playPauseAction;
    QAction *m_stopAction;
    QAction *m_visibleAction;
    QAction *m_closeAction;
    QMediaPlayer* m_mediaPlayer;
    QActionButton *m_playPauseButton;
    QActionButton *m_stopButton;
    QSlider *m_volumeSlider;

};
