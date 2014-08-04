#pragma once

#include <string>
#include "Singleton.h"

class PlayerStatus
    : public Singleton<PlayerStatus>
{
protected:
    static const float          k_multiplierExpiryTime;
    static const int            k_maxMultiplier;
    static const std::string    k_highScoreFilename;

    float   m_multiplierTimeLeft;
    int     m_lives;
    int     m_score;
    int     m_highScore;
    int     m_multiplier;
    int     m_scoreForExtraLife;
    unsigned int    m_lastTime;

protected:
    int     LoadHighScore();
    void    SaveHighScore(int score);

protected:
    PlayerStatus();

public:
    void    reset();
    void    update();
    void    addPoints(int basePoints);
    void    increaseMultiplier();
    void    resetMultiplier();
    void    removeLife();

    int     getLives() const;
    int     getScore() const;
    int     getHighScore() const;
    int     getMultiplier() const;
    int     getIsGameOver() const;

    friend class Singleton<PlayerStatus>;
};
