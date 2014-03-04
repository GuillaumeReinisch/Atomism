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

//! \file PotentialEnergySurface.h Describes a surface of potential energy

#ifndef POTENTIALENERGYSURFACE_H
#define POTENTIALENERGYSURFACE_H

#include <GeneralizedCoordinates.h>

namespace atomism {
    
    /** \class PotentialEnergySurface
     *
     * \brief Describes a surface of potential energy
     */
    template<
    typename DerivedClass,
    typename Scalar      = double,
    typename Vector      = std::vector<Scalar>,
    typename Matrix      = std::vector< std::vector<Scalar> >,
    typename Positions   = std::array< std::vector<Scalar> ,3>
    >
    class PotentialEnergySurface {
        
    public:
        
        PotentialEnergySurface( boost::shared_ptr<Entity> entity ,
				boost::shared_ptr<ResourceManager<Scalar,Vector,Matrix>> _ResourceMngr
			       );
        
        Scalar evaluate(const GeneralizedCoordinates& q) const;
	
        Scalar evaluate(const GeneralizedCoordinates& q,
			const Positions& coors) const;
	
        void computeJacobian(const GeneralizedCoordinates& q,
			       Matrix& jacOfPES) const;
	
	void computeJacobian(const Matrix& jacOfDofs,
			       const Matrix& jacOfDisplX,
			       const Matrix& jacOfDisplY,
			       const Matrix& jacOfDisplZ,
			       Matrix& jacOfPES) const;
	
    private:
        
        PotentialEnergySurface();
        
        boost::shared_ptr<const Entity> _Entity;   

    protected:
      
	mutable boost::shared_ptr<ResourceManager<Scalar,Vector,Matrix>> _ResourceMngr; 
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename DerivedClass,
    typename Scalar      = double,
    typename Vector      = std::vector<Scalar>,
    typename Matrix      = std::vector< std::vector<Scalar> >,
    typename Positions   = std::array< std::vector<Scalar> ,3>
    >
    inline
    PotentialEnergySurface::PotentialEnergySurface( boost::shared_ptr<Entity> entity) {
      
        IMPACT_LOG();
        _Entity = entity;
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename DerivedClass,
    typename Scalar      = double,
    typename Vector      = std::vector<Scalar>,
    typename Matrix      = std::vector< std::vector<Scalar> >,
    typename Positions   = std::array< std::vector<Scalar> ,3>
    >
    inline
    void PotentialEnergySurface<DerivedClass,Scalar,Vector,Matrix,Positions>
    ::evaluate(const GeneralizedCoordinates& q,const Positions& coors) const {
        
        IMPACT_LOG();
        return r = static_cast<DerivedClass*>(this)->evaluate(q);
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename DerivedClass,
    typename Scalar      = double,
    typename Vector      = std::vector<Scalar>,
    typename Matrix      = std::vector< std::vector<Scalar> >,
    typename Positions   = std::array< std::vector<Scalar> ,3>
    >
    inline
    void PotentialEnergySurface<DerivedClass,Scalar,Vector,Matrix,Positions>
    ::evaluate(const GeneralizedCoordinates& q,const Positions& coors) const {
        
        IMPACT_LOG();
	auto& coors = _ResourceMngr->get<Positions>(_Entity->noOfElements());
	_Entity->computeCoordinates(q.getValues(),coors);
	
        return evaluate(q,coors);
    }
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename DerivedClass,
    typename Scalar      = double,
    typename Vector      = std::vector<Scalar>,
    typename Matrix      = std::vector< std::vector<Scalar> >,
    typename Positions   = std::array< std::vector<Scalar> ,3>
    >
    inline
    void PotentialEnergySurface<DerivedClass,Scalar,Vector,Matrix,Positions>
    ::computeJacobian(const GeneralizedCoordinates& q,Vector& jacOfPES) const {
      
        IMPACT_LOG();
	size_t n  = _Entity->noOfDofs();
	size_t n2 = _Entity->noOfElements();
	
        auto& JacX      = _ResourceMngr->get<Matrix>(n,n2);
	auto& JacY      = _ResourceMngr->get<Matrix>(n,n2);
	auto& JacZ      = _ResourceMngr->get<Matrix>(n,n2);
	auto& jacOfDofs = _ResourceMngr.get<Matrix>(n,n);
	
	_Entity->computeJacobian(q.getValues(), q.getdqs(),*JacX,*JacY,*JacZ);
	
	computeJacobian( jacOfDofs, *JacX, *JacY, *JacZ);
	
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename DerivedClass,
    typename Scalar      = double,
    typename Vector      = std::vector<Scalar>,
    typename Matrix      = std::vector< std::vector<Scalar> >,
    typename Positions   = std::array< std::vector<Scalar> ,3>
    >
    inline
    void PotentialEnergySurface<DerivedClass,Scalar,Vector,Matrix,Positions>
    ::computeJacobian(const Matrix& jacOfDofs,
		      const Matrix& jacOfDisplX,
		      const Matrix& jacOfDisplY,
	              const Matrix& jacOfDisplZ,
		      Matrix& jacOfPES) const {
      
        IMPACT_LOG();
	
	size_t n  = _Entity->noOfDofs();
		
	for(size_t i=0;i<n ;i++){
	    
	    jacOfPES[i] = evaluate( slice(jacOfDofs,i),
	  			    tie(slice(jacOfDisplX,i),
	                                slice(jacOfDisplY,i),
				        slice(jacOfDisplZ,i)));
	 }
	
    }
}
#endif // MSENTITY_H
