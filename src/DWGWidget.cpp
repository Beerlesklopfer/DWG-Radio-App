#include <DWGWidget.h>
#include <QDebug>
#include <QCloseEvent>

DWGWidget::DWGWidget(QWidget *parent) :
    QWidget(parent)
  , m_settings(QApplication::organizationName(), QApplication::applicationName() )
{
    m_playPauseAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"));
    m_stopAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"));
    m_visibleAction= new QAction();

    m_closeAction = new QAction(style()->standardIcon(QStyle::SP_MessageBoxCritical), tr("Exit"));

    this->setWindowIcon(QIcon(":/icons/icon.png"));

    if(QSystemTrayIcon::isSystemTrayAvailable()){
        QMenu *trayMenu = new QMenu();
        trayMenu->addAction(m_playPauseAction);
        trayMenu->addAction(m_stopAction);
        trayMenu->addSeparator();
        trayMenu->addAction(m_visibleAction);
        trayMenu->addSeparator();
        trayMenu->addAction(m_closeAction);

        m_trayIcon = new QSystemTrayIcon(this->windowIcon());
        m_trayIcon->setContextMenu(trayMenu);
        m_trayIcon->show();
    } else {
        qWarning() << tr("Your Os does not support a system tray icon.");
    }


    m_mediaPlayer = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    m_mediaPlayer->setMedia(QUrl("http://server23644.streamplus.de"));

    m_playPauseButton = new QActionButton(tr("Play"));
    m_playPauseButton->setAction(m_playPauseAction);

    m_stopButton = new QActionButton(tr("Stop"));
    m_stopButton->setAction( m_stopAction );
    m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setRange(0, 100);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(m_playPauseButton);
    controlLayout->addWidget(m_stopButton);


    QBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(controlLayout);
    layout->addWidget(new QLabel(tr("Volume")));
    layout->addWidget(m_volumeSlider);

    this->setLayout(layout);

    connect(m_playPauseAction, &QAction::triggered,
            this, &DWGWidget::playOrPause);
    connect(m_stopAction, &QAction::triggered,
            this, &DWGWidget::playOrPause);
    connect(m_closeAction, &QAction::triggered,
            this, &DWGWidget::close);

    connect(m_visibleAction, &QAction::triggered,
            this, &DWGWidget::showOrHide);

    connect(m_volumeSlider, &QAbstractSlider::valueChanged,
            m_mediaPlayer, &QMediaPlayer::setVolume);

    connect(m_mediaPlayer, &QMediaPlayer::currentMediaChanged,
              this,  &DWGWidget::setMediaTitle);

    switch( m_settings.value("state", QMediaPlayer::PlayingState).toInt() ){
    case QMediaPlayer::StoppedState:
        m_mediaPlayer->stop();
        m_playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        m_playPauseButton->setText(tr("Play"));
        m_playPauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        m_playPauseAction->setText(tr("Play"));
        break;
    case QMediaPlayer::PlayingState:
        m_mediaPlayer->play();
        m_playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        m_playPauseButton->setText(tr("Pause"));
        m_playPauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        m_playPauseAction->setText(tr("Pause"));
        break;
    case QMediaPlayer::PausedState:
        m_mediaPlayer->play();
        m_playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }

    this->setVisible( m_settings.value("visibility", true ).toBool() );
    m_volumeSlider->setValue( m_settings.value("volume", 50 ).toInt() );
    this->restoreGeometry( m_settings.value("geometry").toByteArray() );


    m_visibleAction->setChecked(m_settings.value("visibility", true ).toBool());
    this->setFixedSize(this->width(), this->height());
    this->setWindowFlags(this->windowFlags() &(~Qt::WindowMaximizeButtonHint));
    this->setVisible(m_settings.value("visibility", true ).toBool());
}

DWGWidget::~DWGWidget()
{
    delete m_mediaPlayer;
    delete m_volumeSlider;
    delete m_trayIcon;
}

bool DWGWidget::event(QEvent *ev)
{
    switch (ev->type()){
    case QEvent::WindowStateChange:
        if (this->isMinimized()) {
            this->hide();
            ev->ignore();
        } else {
            ev->accept();
        }
        break;
    case QEvent::Show:
        m_visibleAction->setChecked(true);
        m_visibleAction->setText(tr("Hide"));
        break;
    case QEvent::Hide:
        m_visibleAction->setChecked(false);
        m_visibleAction->setText(tr("Show"));
        break;
    case QEvent::Close:
        m_settings.setValue("state", m_mediaPlayer->state() );
        m_settings.setValue("volume", m_mediaPlayer->volume() );
        m_settings.setValue("geometry", this->saveGeometry() );
        m_settings.setValue("visibility", this->isVisible() );
        break;
    default:
        ev->accept();
    }

    return QWidget::event(ev);
}

void DWGWidget::playOrPause()
{
    switch( m_mediaPlayer->state() ){
    case QMediaPlayer::StoppedState:
        m_mediaPlayer->play();
        m_playPauseButton->setText(tr("Pause"));
        m_playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        m_playPauseAction->setText(tr("Pause"));
        m_playPauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    case QMediaPlayer::PlayingState:
    case QMediaPlayer::PausedState:
        m_mediaPlayer->stop();
        m_playPauseButton->setText(tr("Play"));
        m_playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        m_playPauseAction->setText(tr("Play"));
        m_playPauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void DWGWidget::showOrHide()
{
    this->setVisible(!this->isVisible());
}

void DWGWidget::setMediaTitle(const QMediaContent &media)
{
    qDebug() << media.resources()[0].url().toString();

}
