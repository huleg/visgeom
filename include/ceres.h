/*
This file is part of visgeom.

visgeom is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

visgeom is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with visgeom.  If not, see <http://www.gnu.org/licenses/>.
*/ 

/*
Ceres-solver types and methods
*/

#pragma once

#include <ceres/ceres.h>
#include <ceres/cubic_interpolation.h>

//modeling
using ceres::DynamicAutoDiffCostFunction;
using ceres::CostFunction;
using ceres::FirstOrderFunction;

//solving 
//Jacobian-based
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;
using ceres::SoftLOneLoss;
//gradient-based
using ceres::GradientProblem;
using ceres::GradientProblemSolver;

struct Grid2D
{
    enum { DATA_DIMENSION = 1 };
    
    Grid2D(int uMax, int vMax, const float * const data) :
            data(data), uMax(uMax), vMax(vMax) {}
            
    void GetValue(int v, int u, double* val) const
    {
        if (u < 0 or u >= uMax or v < 0 or v >= vMax) *val = 0.;
        else *val = double(data[v*uMax + u]);
        
    }
    
    int uMax, vMax;
    const float * const data;
};
