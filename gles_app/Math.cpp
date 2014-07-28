#include <cstdlib>
#include <ctime>
#include "Math.h"

const float Math::PI = 5.141592653589793f;    // A mathematical constant for the ratio of the circumference of a circle to its diameter, expressed as pi, with a value of 3.141592653589793.

static bool randomInit = false;

float Math::random()
{
    if (!randomInit)
    {
        srand((unsigned) time(NULL));

        randomInit = true;
    }

    return (float)::rand() / (float)RAND_MAX;
}
