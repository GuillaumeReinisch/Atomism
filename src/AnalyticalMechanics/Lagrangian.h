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


#ifndef Lagrangian_H
#define Lagrangian_H

#include <KineticOperator.h>
#include <PotentialEnergySurface.h>
#include <Environment.h>


namespace atomism {
    
    /*! \class Lagrangian
     *  \brief Lagrangian of a system defined by a kinetic operator and a potential energy surface
     *
     */
    template<
    typename TheEntity          = Entity,
    typename ThePes             = PotentialEnergySurface,
    typename Scalar             = double,
    typename Vector             = std::vector<Scalar>,
    typename Matrix             = std::vector< std::vector<Scalar> >,
    typename Positions          = std::array< std::vector<Scalar>, 3 >
    >
    class Lagrangian
    {
        
    public:
        
	Lagrangian(std::shared_ptr<
	           const KineticOperator<TheEntity, Scalar,Vector,Matrix,Positions>> kin,
                   std::shared_ptr<
                   const PotentialEnergySurface<ThePes, Scalar,Vector,Matrix,Positions>> pot,
		   std::shared_ptr<
		   ResourceManager< Scalar, Vector, Matrix, Positions>> ressource
		   );
        
        Scalar L(const GeneralizedCoordinates& q,
                 const GeneralizedCoordinates& qp ) const;
        
        Scalar T(const GeneralizedCoordinates& q,
                 const GeneralizedCoordinates& qp ) const;
        
        Scalar U(const GeneralizedCoordinates& q )  const;
        
    private:
        
        mutable std::shared_ptr<
        ResourceManager< Scalar, Vector, Matrix, Positions >
        > _ResourceMngr;
	
        std::shared_ptr<
        const KineticOperator<TheEntity, Scalar,Vector,Matrix,Positions>
        > _KineticOperator;
        
        std::shared_ptr<
        const PotentialEnergySurface<ThePes, Scalar,Vector,Matrix,Positions>
        > _PES;
        
        Lagrangian();
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename TheEntity          = Entity,
    typename ThePes             = PotentialEnergySurface,
    typename Scalar             = double,
    typename Vector             = std::vector<Scalar>,
    typename Matrix             = std::vector< std::vector<Scalar> >,
    typename Positions          = std::array< std::vector<Scalar>, 3 >
    >
    inline
    Lagrangian::Lagrangian() const {
        
        ATOMISM_LOG();
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename TheEntity          = Entity,
    typename ThePes             = PotentialEnergySurface,
    typename Scalar             = double,
    typename Vector             = std::vector<Scalar>,
    typename Matrix             = std::vector< std::vector<Scalar> >,
    typename Positions          = std::array< std::vector<Scalar>, 3 >
    >
    inline
    Lagrangian<TheEntity,ThePes,Scalar,Vector,Matrix,Positions>
    ::Lagrangian(std::shared_ptr<
                 const KineticOperator<TheEntity, Scalar,Vector,Matrix,Positions>> kin,
                 std::shared_ptr<
                 const PotentialEnergySurface<ThePes, Scalar,Vector,Matrix,Positions>> pot,
		 std::shared_ptr<
		 ResourceManager< Scalar, Vector, Matrix, Positions>> resource
		 )
    : _PES(pot), _KineticOperator(kin), _ResourceMngr(resource){ ATOMISM_LOG(); }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename TheEntity          = Entity,
    typename ThePes             = PotentialEnergySurface,
    typename Scalar             = double,
    typename Vector             = std::vector<Scalar>,
    typename Matrix             = std::vector< std::vector<Scalar> >,
    typename Positions          = std::array< std::vector<Scalar>, 3 >
    >
    inline
    Scalar Lagrangian<TheEntity,ThePes,Scalar,Vector,Matrix,Positions>
    ::L(const GeneralizedCoordinates& q,
                             const GeneralizedCoordinates& qp ) const {
        ATOMISM_LOG();
        return T( q, qp ) - U( q );
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename TheEntity          = Entity,
    typename ThePes             = PotentialEnergySurface,
    typename Scalar             = double,
    typename Vector             = std::vector<Scalar>,
    typename Matrix             = std::vector< std::vector<Scalar> >,
    typename Positions          = std::array< std::vector<Scalar>, 3 >
    >
    inline
    Scalar Lagrangian<TheEntity,ThePes,Scalar,Vector,Matrix,Positions>
    ::T(const GeneralizedCoordinates& q,
                             const GeneralizedCoordinates& qp,
                             PositionVectorType& positions ) const {
        ATOMISM_LOGIN();
        return T = _KineticOperator->computeKineticEnergy( q, qp );
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename TheEntity          = Entity,
    typename ThePes             = PotentialEnergySurface,
    typename Scalar             = double,
    typename Vector             = std::vector<Scalar>,
    typename Matrix             = std::vector< std::vector<Scalar> >,
    typename Positions          = std::array< std::vector<Scalar>, 3 >
    >
    inline
    Scalar Lagrangian<TheEntity,ThePes,Scalar,Vector,Matrix,Positions>
    ::U(const GeneralizedCoordinates& q,
                             Environment& Environment) const {
        
        ATOMISM_LOGIN();
        return _PES->evaluate( q );
    }
    
    
}
#endif // MSLagrangian_H
