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
        
	KineticOperator(boost::shared_ptr<const Entity<TheEntity,Scalar,Vector,Matrix> > entity,
	                boost::shared_ptr<ResourceManager<Scalar,Vector,Matrix> >  resource
	                );
        
        /*! \brief compute the kinetic matrix
         *
         * \param q  generalized coordinates
	 * \param KMatrix output: kinetic matrix 
         * \param env environment 
         */
        double computeKineticMatrix(const GeneralizedCoordinates& q,
				    Matrix& KMatrix ) const;
  
	/*! \brief compute the kinetic matrix
         *
         * \param q  generalized coordinates
	 * \param KMatrix output: kinetic matrix 
         * \param env environment 
         */
        double computeKineticEnergy(const GeneralizedCoordinates& q,
				    const GeneralizedCoordinates& qp ) const;
				    
    private:
        
        boost::shared_ptr<const Entity<TheEntity,Scalar,Vector,Matrix> > _Entity;
	
        //! This is used to create/obtain new elements within thread safety.
        mutable std::shared_ptr<ResourceManager<Scalar,Vector,Matrix> > _ResourceMngr;
	
        KineticOperator();
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<
    typename TheEntity,
    typename Scalar           = double,
    typename Vector           = std::vector<Scalar>,
    typename Matrix 	      = std::vector<std::vector<Scalar>>
    >
    inline
    KineticOperator<Scalar,Vector,Matrix>::KineticOperator() { }
	
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<
    typename TheEntity,
    typename Scalar           = double,
    typename Vector           = std::vector<Scalar>,
    typename Matrix 	      = std::vector<std::vector<Scalar>>
    >
    inline
    KineticOperator<Scalar,Vector,Matrix>
    ::KineticOperator(boost::shared_ptr<const Entity<TheEntity,Scalar,Vector,Matrix> > entity,
                    boost::shared_ptr<ResourceManager<Scalar,Vector,Matrix> >  resource ) 
    :_Entity(entity),_ResourceMngr(resource) {
    }
	
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<
    typename TheEntity, 
    typename Scalar           = double,
    typename Vector           = std::vector<Scalar>,
    typename Matrix 	      = std::vector<std::vector<Scalar>>
    >
    inline
    double KineticOperator<Scalar,Vector,Matrix>
    ::computeKineticEnergy(const Vector& q,
			   const Vector& qp) const {
        
         ATOMISM_LOG();    
	 
	 size_t n     = _Entity->noOfDofs();
	 auto kmatrix = _ResourceMngr->get<Matrix>(n,n);
	 computeKineticMatrix(q,*kmatrix);
	 	 	   
	 return evaluate(qp,*kmatrix,qp);	
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<
    typename Scalar           = double,
    typename Vector           = std::vector<Scalar>,
    typename Matrix 	      = std::vector<std::vector<Scalar>>
    >
    inline
    void KineticOperator<Scalar,Vector,Matrix>::
    computeKineticMatrix(const Vector& q,
			 const Vector& dq,
			 Matrix& KMatrix )  const {
        
         ATOMISM_LOG();   
	 ATOMISM_EXCEPT_IF( [&](){return q.size()!=dq.size();} );
	 
	 size_t n  = _Entity->noOfDofs();
	 size_t n2 = _Entity->noOfElements();
	 
	 auto JacX = _ResourceMngr->get<Matrix>(n,n2);
	 auto JacY = _ResourceMngr->get<Matrix>(n,n2);
	 auto JacZ = _ResourceMngr->get<Matrix>(n,n2);
	 
         _Entity->computeJacobian(q, dq,*JacX,*JacY,*JacZ);
	
	 set_zero(KMatrix);
	 
	 KMatrix +=  multiplyByTransposeAndWeigth( *JacX, _Entity->getMasses() )
	           + multiplyByTransposeAndWeigth( *JacY, _Entity->getMasses() )
	           + multiplyByTransposeAndWeigth( *JacZ, _Entity->getMasses() );
		   
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
}
#endif // MSKINETICOPERATOR_H
