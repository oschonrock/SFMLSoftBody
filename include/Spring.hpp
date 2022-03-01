#pragma once

#include "Point.hpp"
#include "Vector2.hpp"
#include "damper.hpp"

class Spring {
  public:
    Point& p1;
    Point& p2;
    double length;
    double springConstant;
    double dampFactor;
    // too much noise in this differential veclocity vector will make the system unstable with high
    // dampFactors. So we use a `damper` (exponential damping) to smooth out the noise
    damper<Vec2> dampedExtensionVelocity;

    static constexpr unsigned exponentialDampingTimeConstant = 8;
    
    Spring(Point& p1_, Point& p2_, double length_, double springConstant_, double dampFactor_)
        : p1(p1_), p2(p2_), length(length_), springConstant(springConstant_), dampFactor(dampFactor_),
          dampedExtensionVelocity(exponentialDampingTimeConstant) {}

    void updatePointForces() {
        Vec2   diff     = p1.pos - p2.pos; // broken out alot "yes this is faster! really like 3x"
        double diffMag  = diff.mag();
        Vec2   diffNorm = diff / diffMag;
        double ext      = diffMag - length;
        double springf  = -springConstant * ext; // -ke spring force and also if a diagonal increase
                                                 // spring constant for stability // test

        // damping force: note the use of a `damper` to smooth out noise in differential veclocity
        double dampf =
            diffNorm.dot(dampedExtensionVelocity(p2.vel - p1.vel)) * dampFactor;
        Vec2 force = (springf + dampf) * diffNorm;
        p1.f += force; // equal and opposite reaction
        p2.f -= force;
    }
};
