
Vector2f EnemySpawner::GetSpawnPosition()
{
    Vector2f pos;


    do
    {
        pos = Vector2f(Math::random() * GameRoot::getInstance()->getViewportSize().width,
                       Math::random() * GameRoot::getInstance()->getViewportSize().height);

    } while (pos.distanceSquared(PlayerShip::getInstance()->getPosition()) < 250 * 250);

    return pos;
}


EnemySpawner::EnemySpawner()
    : m_inverseSpawnChance(90)
{

}


void EnemySpawner::update()
{
    if(!PlayerShip::getInstance()->getIsDead() && EntityManager::getInstance()->getCount() < 200)
    {
        if(Math::random() * m_inverseSpawnChance == 0)
        {
            EntityManager::getInstance()->add(Enemy::createSeeker(GetSpawnPosition()));
        }

        if(Math::random() * m_inverseSpawnChance == 0)
        {
            EntityManager::getInstance()->add(Enemy::createWanderer(GetSpawnPosition()));
        }
    }

    if(m_inverseSpawnChance > 30)
    {
        m_inverseSpawnChance -= 0.005f;
    }
}


void EnemySpawner::reset()
{
    m_inverseSpawnChance = 90;
}
