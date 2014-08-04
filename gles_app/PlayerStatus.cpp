#include "PlayerShip.h"
#include "PlayerStatus.h" 
#include "Timer.h"

const float PlayerStatus::k_multiplierExpiryTime = 0.8f;
const int PlayerStatus::k_maxMultiplier = 20;
const std::string PlayerStatus::k_highScoreFilename("highScore.txt");

int PlayerStatus::LoadHighScore()
{
    int score = 0;


//    std::string fstring;

    // TODO:
    // don't care for now.
    return score;
}


void PlayerStatus::SaveHighScore(int score)
{
    // TODO:
}


PlayerStatus::PlayerStatus()
{
    m_score = 0;
    m_highScore = LoadHighScore();
    reset();

    m_lastTime = Timer::getTimeMS();
}


void PlayerStatus::reset()
{
    if(m_score > m_highScore)
    {
        m_highScore = m_score;
        SaveHighScore(m_highScore);
    }

    m_score = 0;
    m_multiplier = 1;
    m_lives = 4;
    m_scoreForExtraLife = 2000;
    m_multiplierTimeLeft = 0;
}


void PlayerStatus::update()
{
    if(m_multiplier > 1)
    {
        m_multiplierTimeLeft -= float(Timer::getTimeMS() - m_lastTime) / 1000.0f;

        if(m_multiplierTimeLeft <= 0)
        {
            m_multiplierTimeLeft = k_multiplierExpiryTime;
            resetMultiplier();
        }
    }

    m_lastTime = Timer::getTimeMS();
}


void PlayerStatus::addPoints(int basePoints)
{
    if(!PlayerShip::getInstance()->getIsDead())
    {
        m_score += basePoints * m_multiplier;
        while(m_score >= m_scoreForExtraLife)
        {
            m_scoreForExtraLife += 2000;

            m_lives++;
        }
    }
}


void PlayerStatus::increaseMultiplier()
{
    if(!PlayerShip::getInstance()->getIsDead())
    {
        m_multiplierTimeLeft = k_multiplierExpiryTime;

        if(m_multiplier < k_maxMultiplier)
        {
            m_multiplier++;
        }
    }
}


void PlayerStatus::resetMultiplier()
{
    m_multiplier = 1;
}


void PlayerStatus::removeLife()
{
    m_lives--;
}


int PlayerStatus::getLives() const
{
    return m_lives;
}


int PlayerStatus::getScore() const
{
    return m_score;
}


int PlayerStatus::getHighScore() const
{
    return m_highScore;
}


int PlayerStatus::getMultiplier() const
{
    return m_multiplier;
}


int PlayerStatus::getIsGameOver() const
{
    return m_lives == 0;
}
