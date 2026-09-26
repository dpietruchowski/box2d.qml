#pragma once

#include <QObject>
#include <QSettings>

// Persistent game data (high score), registered for QML as the
// Pinball.GameSettings singleton.
class GameSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int highScore READ highScore WRITE setHighScore NOTIFY highScoreChanged)

public:
    explicit GameSettings(QObject *parent = nullptr);

    int highScore() const;
    void setHighScore(int score);

signals:
    void highScoreChanged();

private:
    QSettings m_settings;
};
