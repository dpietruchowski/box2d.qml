#include "gamesettings.h"

GameSettings::GameSettings(QObject *parent)
    : QObject(parent)
{
}

int GameSettings::highScore() const
{
    return m_settings.value("highScore", 0).toInt();
}

void GameSettings::setHighScore(int score)
{
    if (score == highScore())
        return;
    m_settings.setValue("highScore", score);
    emit highScoreChanged();
}
