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
#include <GeneralizedCoordinates.h>

namespace atomism {
    
    /*! \class KineticOperator
     * \brief Describes the kinetic operator used to define the equations of motion in
     * the Lagrangian formalism.
     *
     */
    template<
    typename TheEntity,  
    typename Scalar           = double,
    typename Vector           = std::vector<Scalar>,
    typename Matrix 	      = std::vector<std::vector<Scalar>>         
    >
    class KineticOperator {
        
    public:
        
	KineticOperator(std::shared_ptr<const TheEntity> entity,
	                std::shared_ptr<ResourceManager<Scalar,Vector,Matrix> >  resource
	                );
        
        /*! \brief compute the kinetic matrix
         *
         * \param q  generalized coordinates
	 * \param KMatrix output: kinetic matrix 
         * \param env environment 
         */
        void computeKineticMatrix(const GeneralizedCoordinates<Scalar,Vector>& q,
				  Matrix& KMatrix ) const;
  
	/*! \brief compute the kinetic matrix
         *
         * \param q  generalized coordinates
	 * \param KMatrix output: kinetic matrix 
         * \param env environment 
         */
        double computeKineticEnergy(const GeneralizedCoordinates<Scalar,Vector>& q,
				    const GeneralizedCoordinates<Scalar,Vector>& qp ) const;
				    
    private:
        
        std::shared_ptr<const TheEntity > _Entity;
	
        //! This is used to create/obtain new elements within thread safety.
        mutable std::shared_ptr<ResourceManager<Scalar,Vector,Matrix> > _ResourceMngr;
	
        KineticOperator();
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<typename TheEntity,typename Scalar,typename Vector,typename Matrix>
    inline
    KineticOperator<TheEntity,Scalar,Vector,Matrix>::KineticOperator() { }
	
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<
    typename TheEntity,typename Scalar,typename Vector,typename Matrix>
    inline
    KineticOperator<TheEntity,Scalar,Vector,Matrix>
    ::KineticOperator(std::shared_ptr<const TheEntity > entity,
                      std::shared_ptr<ResourceManager<Scalar,Vector,Matrix> >  resource ) 
    :_Entity(entity),_ResourceMngr(resource) {
    }
	
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<typename TheEntity,typename Scalar,typename Vector,typename Matrix>
    inline
    double KineticOperator<TheEntity,Scalar,Vector,Matrix>
    ::computeKineticEnergy(const GeneralizedCoordinates<Scalar,Vector>& q,
			   const GeneralizedCoordinates<Scalar,Vector>& qp) const {
        
         ATOMISM_LOG();    
	 
	 size_t n     = _Entity->noOfDofs();
	 auto kmatrix = _ResourceMngr->requestMatrix(n,n);
	 computeKineticMatrix(q,*kmatrix);
	 	 	   
	 return evaluate(qp,*kmatrix,qp);	
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<typename TheEntity,typename Scalar,typename Vector,typename Matrix>
    inline
    void KineticOperator<TheEntity,Scalar,Vector,Matrix>
    ::computeKineticMatrix(const GeneralizedCoordinates<Scalar,Vector>& q,
			   Matrix& KMatrix )  const {
        
         ATOMISM_LOG();   
	 ATOMISM_VALUE_MISMATCH( [&](){return pow(n_elements(q.getValues()),2);},
	                         [&](){return n_elements(KMatrix);});
	 
	 size_t n  = _Entity->noOfDofs();
	 size_t n2 = _Entity->noOfElements();
	 
	 auto JacX = _ResourceMngr->requestMatrix(n,n2);
	 auto JacY = _ResourceMngr->requestMatrix(n,n2);
	 auto JacZ = _ResourceMngr->requestMatrix(n,n2);
	 
         _Entity->computeJacobian(q.getValues(),q.getdqs(),*JacX,*JacY,*JacZ);
	
	 init_constant(KMatrix,0.);
	 
	 KMatrix =  multiplyByTransposeAndWeigth( *JacX, _Entity->getMasses() )
	         +  multiplyByTransposeAndWeigth( *JacY, _Entity->getMasses() )
	         +  multiplyByTransposeAndWeigth( *JacZ, _Entity->getMasses() );
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
}
#endif // MSKINETICOPERATOR_H
