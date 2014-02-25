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


#ifndef KINETICOPERATOR_H
#define KINETICOPERATOR_H

#include <Entity.h>

namespace atomism {
    
    /*! \class KineticOperator
     * \brief Describes the kinetic operator used to define the equations of motion in
     * the Lagrangian formalism.
     *
     */
	template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>
    >
    class KineticOperator {
        
    public:
        
	    KineticOperator(boost::shared_ptr<const Entity> entity);
        
        /* \brief compute the kinetic matrix
         *
         * At the end of the function, the Environment parameter is
         * updated for the kinetic matrix, the positions and the jacobians.
         
         * \param q generalized coordinates
         * \param env environment 
         */
        double computeKineticMatrix(const _GeneralizedCoordinates& q,
                                    Environment& env);
        
    private:
        
        boost::shared_ptr<const Entity>                   _Entity;
        
        KineticOperator();
	    //map<int,ScalarFunction> customDynamicDofs;
    };
    
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template< typename ScalarType=double,
    typename VectorType=std::vector<ScalarType>
    >
    inline
    KineticMatrix KineticOperator::setDynamicDofs() {
        
        ATOMISM_LOGIN();
        _Entity->setDofValues(_GeneralizedCoordinates->getValues());
        ATOMISM_LOGOUT();
        return KMatrix;
    };

    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
	template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>
    >
    inline
    KineticMatrix KineticOperator::computeKineticMatrix(
                                               const _GeneralizedCoordinates& q,
                                               Environment& env) const {
        
        ATOMISM_LOGIN();        
        
        _Entity->computeJacobian(q.getValues(),
                                 q.getDerivationSize(),
                                 env,
                                 );
        
        setMatrixElements( env.KMatrix, [&env] (size_t i, size_t j)  {
            
                              return entity->getMasses() *
                                     innerProduct( env._JacOfDispl[i],
                                                   env._JacOfDispl[j]);
                          });
        
        double KinEnergy = KMatrix * qp;
        
        ATOMISM_LOGOUT();
        return KinEnergy;
    };
    
}
#endif // MSKINETICOPERATOR_H
