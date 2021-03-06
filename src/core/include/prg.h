/**
 *  This file is part of Evoplex.
 *
 *  Evoplex is a multi-agent system for networks.
 *  Copyright (C) 2016 - Marcos Cardinot <marcos@cardinot.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PRG_H
#define PRG_H

#include <random>

namespace evoplex
{
class PRG
{
public:
    explicit PRG(unsigned int seed);

    // Generate a random double [0, 1)
    inline double randD();
    // Generate a random double [0, max)
    inline double randD(double max);
    // Generate a random double [min, max)
    double randD(double min, double max);

    // Generate a random float [0, 1)
    inline float randF();
    // Generate a random float [0, max)
    inline float randF(float max);
    // Generate a random float [min, max)
    float randF(float min, float max);

    // Generate a random integer [0, max]
    inline int randI(int max);
    // Generate a random integer [min, max]
    int randI(int min, int max);

    // Generate a random integer [0, max]
    inline size_t randI(size_t max);
    // Generate a random integer [min, max]
    size_t randS(size_t min, size_t max);

private:
    std::mt19937 m_mteng; //  Mersenne Twister engine
    std::uniform_real_distribution<double> m_doubleZeroOne;
    std::uniform_real_distribution<float> m_floatZeroOne;
};

/************************************************************************
   PRG: Inline member functions
 ************************************************************************/

inline double PRG::randD()
{ return m_doubleZeroOne(m_mteng); }

inline double PRG::randD(double max)
{ return randD(0.0, max); }

inline float PRG::randF()
{ return m_floatZeroOne(m_mteng); }

inline float PRG::randF(float max)
{ return randF(0.f, max); }

inline int PRG::randI(int max)
{ return randI(0, max); }

inline size_t PRG::randI(size_t max)
{ return randS(0, max); }

} // evoplex
#endif // PRG_H
