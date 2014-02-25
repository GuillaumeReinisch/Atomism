/*
 <one line to give the library's name and an idea of what it does.>
 Copyright (C) 2013  Guillaume <email>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


namespace atomism {
    
    /** \class Environment
     *
     * \brief Manage the variables in memory required by a Lagrangian system
     */
	template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    typename PositionVectorType = std::vector< std::array<ScalarType, 3> >,
    typename MatrixType         = std::vector<ScalarType>,
    typename Vector3dType       = std::array<ScalarType, 3>,
    typename Matrix3dType       = std::array< std::array<ScalarType, 3> , 3>,
    int NELEMENTS,
    int NDOFS
    >
    class Environment {
        
        friend class Entity;
        friend class KineticOperator;
        friend class Lagrangian;
        
    public:
        
        Environment();
        
    private:
        
        MatrixType _KMatrix;
        
        PositionVectorType _JacobianOfDisplacments;
        
        StateType _JacobianOfDofs;
        
        PositionVectorType _Positions;
        
        PositionVectorType _Displacments;
        
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
	template<
    int NELEMENTS,
    int NDOFS,
    typename ScalarType         = double,
    typename StateType          = std::array<ScalarType, NDOFS>,
    typename MatrixType         = std::array< StateType, NDOFS>,
    typename Vector3dType       = std::array<ScalarType, 3>,
    typename Matrix3dType       = std::array<Vector3dType, 3>,
    typename PositionVectorType = std::array< Vector3dType, NELEMENTS  >,
    typename JacobianType1      = std::array<ScalarType,  NELEMENTS*NDOFS>,
    typename JacobianType2      = std::array<Vector3dType, NELEMENTS*NDOFS>,
    >
    inline
    void Environment::Environment() {
        
        ATOMISM_LOGIN();
        
        _KMatrix        = createZeroMatrix(NDOFS,NDOFS);
        _Positions      = createZeroPositionVectorType(NELEMENTS);
        _Displacments   = createZeroPositionVectorType(NELEMENTS);
        _JacobianOfDofs = createZeroStateType(NELEMENTS*NDOFS);
        _JacobianOfDisplacments = createZeroPositionVectorType(NELEMENTS*NDOFS);
        
        ATOMISM_LOGOUT();
    }
}
#endif // MSENTITY_H
