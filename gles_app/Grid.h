#pragma

#include <vector>
#include "Vector.h"
#include "Rect.h"
#include "Singleton.h"

class PointMass
{
protected:
    Vector3f    m_acceleration;
    float       m_damping;

public:
    Vector3f    m_position;
    Vector3f    m_velocity;
    float       m_inverseMass;

public:
    PointMass();
    PointMass(const Vector3f &position, float invMass);

    void applyForce(const Vector3f &force);
    void increaseDamping(float factor);
    void update();
};


class Spring
{
public:
    PointMass  *m_end1;
    PointMass  *m_end2;
    float       m_targetLength;
    float       m_stiffness;
    float       m_damping;

public:
    Spring(
        PointMass  *end1,
        PointMass  *end2,
        float       stiffness,
        float       damping
    );

    void update();
};


class Grid
	: public Singleton<Grid>
{
public:
    std::vector<Spring> m_springs;
    PointMass          *m_points;
    Vector2f            m_screenSize;
    int                 m_cols;
    int                 m_rows;

protected:
    void SetPointMass(
        PointMass *array,
        int x,
        int y,
        const PointMass &val
        );
    PointMass *GetPointMass(
        PointMass *array,
        int x,
        int y
        );

public:
    Grid();
    void gridInit(
        const Rectf    &rect,
        const Vector2f &spacing
        );

    void applyDirectedForce(const Vector2f &force, const Vector2f &position, float radius);
    void applyDirectedForce(const Vector3f &force, const Vector3f &position, float radius);
    void applyImplosiveForce(float force, const Vector2f &position, float radius);
    void applyImplosiveForce(float force, const Vector3f &position, float radius);
    void applyExplosiveForce(float force, const Vector2f &position, float radius);
    void applyExplosiveForce(float force, const Vector3f &position, float radius);

    void update();
    void draw();

    Vector2f toVec2(const Vector3f &v);

	friend class Singleton<Grid>;
};
