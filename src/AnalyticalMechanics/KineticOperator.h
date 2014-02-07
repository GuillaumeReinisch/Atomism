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

#include <Atomism/Entity.h>

namespace atomism {
        
        /*! \class KineticOperator
         * \brief Describes the kinetic operator used to define the equations of motion in
         * the Lagrangian formalism.
         *
         * The kinetic operator describes how the masses in the system move with respect
         * to the generalized coordinates. 
        * ~~~~~~~~~~~~~
        */
	template<typename ScalarType=double,
	         typename VectorType=std::vector<ScalarType>,
		 DerivedClass 
		 >
        class KineticOperator 
        {
            
        public:
            
	    KineticOperator() { }
                        
	    KineticOperator(boost::shared_ptr<GeneralizedCoordinates> coords,
			    boost::shared_ptr<Entity> entity);
            
	    
            /*! \brief add a dynamic degree of freedom
             *
             * Define the dynamic DoF 'i' of the 'entity' as dynamic and defined by the scalar function 'fct'
	     * 
             * \param index index of the Dof targeted in 'entity'
             * \param fct scalar function defining the Dof value w/ generalized coordinates
             */
            boost::shared_ptr<msTreeMapper> addDynamicDof( size_t index ,
                                                           boost::shared_ptr<ScalarFunction> fct);
            
            /*! \brief add a dynamic degree of freedom represented by a linear scalar function
             *
             * Define the dynamic DoF 'i' of the 'entity' as a linear scalar function
             * of a variable of the coorindates.
             * \param entity entity containing the target Dof
             * \param indexDof index of the Dof targeted in 'entity'
             * \param indexVariable index of the variable in the generalized coordinates'
             */
            boost::shared_ptr<msTreeMapper> addDynamicDofLinear( size_t indexDof ,
                                                                 size_t indexVariable );
            
            //! set the value of the dynamic Dof in the entites w/ current value of the generalized coordinates
            void setDynamicDofs();
            
            double kineticFunction();
	    
            KineticMatrix&   computeKMat();
	                
        private:
            
                        
            boost::shared_ptr<GeneralizedCoordinates>   generalizedCoordinates;
            boost::shared_ptr<Entity>                   entity;
            
	    // linearDynamicDofs[i]=index of var or -1
	    VectorType linearDynamicDofs; 
	    
	    //map<int,ScalarFunction> customDynamicDofs;
        };
  
	
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
	
    template< typename ScalarType=double,
              typename VectorType=std::vector<ScalarType>
             >
    inline  
    KineticMatrix KineticOperator::computeKMat() { 
	      
        ATOMISM_LOGIN();
	
        size_t n = generalizedCoordinates->size();
	
	VectorType dofsValue0;
	entity.getDofValue(dofsValue0);
		
	VectorType dofsValue_i, dofsValue_j,
	           atomDispl_iX,atomDispl_iY,atomDispl_iZ,
		   atomDispl_jX,atomDispl_jY,atomDispl_jZ;
	
        for(size_t i=0;i<n;i++){
	
	   for(size_t j=0;j<n;j++){
	
	       // loop over Dof
	       incrementDofValues(linearDynamicDofs,dofsValue0,i,dofsValue_i,generalizedCoordinates[i].dq,
				  linearDynamicDofs,dofsValue0,j,dofsValue_j,generalizedCoordinates[j].dq);
	       
	       // loop over Atoms
	       entity->getAtomicDisplacments(dofsValue0, dofsValue_i,atomDispl_iX,atomDispl_iY,atomDispl_iZ);
	       entity->getAtomicDisplacments(dofsValue0, dofsValue_j,atomDispl_jX,atomDispl_jY,atomDispl_jZ);
	       
	       setKMatElement(KMatrix,i,j,
			      atomDispl_iX,atomDispl_iY,atomDispl_iZ,
			      atomDispl_jX,atomDispl_jY,atomDispl_jZ,
			      entity->MassElements);
	   }
	};
	    
        ATOMISM_LOGOUT();  //Anything about logging can be removed by configuration
	
        return KMatrix;       
   };
    
}
#endif // MSKINETICOPERATOR_H
