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
#include <ResourceManager.h>

namespace atomism {
    
    /** \class PotentialEnergySurface
     *
     * \brief Describes a surface of potential energy
     */
    template<
    typename TheEntity,
    typename DerivedClass,
    typename Scalar      = double,
    typename Vector      = std::vector<Scalar>,
    typename Matrix      = std::vector< std::vector<Scalar> >,
    typename Positions   = std::tuple<Vector&,Vector&,Vector&>
    >
    class PotentialEnergySurface {
        
    public:
        
        PotentialEnergySurface( std::shared_ptr<const TheEntity> entity ,
				std::shared_ptr<ResourceManager<Scalar,Vector,Matrix>> _ResourceMngr
			       );
        
	std::shared_ptr<const TheEntity> getEntity() const {return _Entity;}
	
        Scalar evaluate(const GeneralizedCoordinates<Scalar,Vector>& q) const;
	
        Scalar evaluate(const GeneralizedCoordinates<Scalar,Vector>& q,
			const Positions& coors) const;
	/*
        void computeJacobian(const GeneralizedCoordinates<Scalar,Vector>& q,
			       Matrix& jacOfPES) const;
	
	void computeJacobian(const Matrix& jacOfDofs,
			       const Matrix& jacOfDisplX,
			       const Matrix& jacOfDisplY,
			       const Matrix& jacOfDisplZ,
			       Matrix& jacOfPES) const;
	*/
    private:
        
        PotentialEnergySurface();
        
        std::shared_ptr<const TheEntity> _Entity;   

    protected:
      
	mutable std::shared_ptr<ResourceManager<Scalar,Vector,Matrix>> _ResourceMngr; 
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename TheEntity, typename DerivedClass, typename Scalar, typename Vector ,
    typename Matrix , typename Positions
    >
    inline
    PotentialEnergySurface<TheEntity,DerivedClass,Scalar,Vector,Matrix,Positions>
    ::PotentialEnergySurface( std::shared_ptr<const TheEntity> entity ,
			      std::shared_ptr<ResourceManager<Scalar,Vector,Matrix>> resource)
    : _Entity(entity),_ResourceMngr(resource){
        ATOMISM_LOG();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename TheEntity, typename DerivedClass, typename Scalar, typename Vector ,
    typename Matrix , typename Positions
    >
    inline
    Scalar PotentialEnergySurface<TheEntity,DerivedClass,Scalar,Vector,Matrix,Positions>
    ::evaluate(const GeneralizedCoordinates<Scalar,Vector>& q,const Positions& coors) const {
        
        ATOMISM_LOG();
	//const DerivedClass* a = static_cast<DerivedClass*>(this);
        return  static_cast<const DerivedClass*>(this)->evaluate(q,coors);
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    template<
    typename TheEntity, typename DerivedClass, typename Scalar, typename Vector ,
    typename Matrix , typename Positions
    >
    inline
    Scalar PotentialEnergySurface<TheEntity,DerivedClass,Scalar,Vector,Matrix,Positions>
    ::evaluate(const GeneralizedCoordinates<Scalar,Vector>& q) const {
        
        ATOMISM_LOG();
	
	auto coors = _ResourceMngr->requestPositions(_Entity->noOfElements());
	
	ATOMISM_VALUE_MISMATCH( [&](){return n_elements(q.getValues());} ,
				[&](){return n_elements(*coors);});
	
	_Entity->computeCoordinates(q.getValues(),*coors);
        return evaluate(q,*coors);
    }
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    /*
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
	
    }*/
}
#endif // MSENTITY_H
