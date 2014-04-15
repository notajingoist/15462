#include "math/math.hpp"
#include <random>

#ifndef _462_MATH_RANDOM462_HPP_
#define _462_MATH_RANDOM462_HPP_
namespace _462{

    /**
     * Generate a uniform random real_t on the interval [0, 1)
     */
    inline real_t random_uniform()
    {
        return real_t(rand())/RAND_MAX;
    }

    /**
     * Generate a uniform random real_t from N(0, 1)
     */
    inline real_t random_gaussian()
    {
        static std::default_random_engine generator;
        static std::normal_distribution<real_t> dist =
            std::normal_distribution<real_t>();
        return dist(generator);
    }


}; // _462

#endif
