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
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    typename PositionVectorType = std::vector< std::array<ScalarType, 3> >
    >
    class Lagrangian
    {
        
    public:
        
	    Lagrangian(boost::shared_ptr<const KineticOperator> kin,
                   boost::shared_ptr<const PotentialEnergySurface> pot);
        
        ScalarType L(const GeneralizedCoordinates& q,
                     const GeneralizedCoordinates& qp,
                     Environment& Environment ) const;
        
        ScalarType T(const GeneralizedCoordinates& q,
                     const GeneralizedCoordinates& qp,
                     PositionVectorType& positions,
                     Environment& Environment ) const;
        
        ScalarType U(const GeneralizedCoordinates& q,
                     Environment& Environment) const;
        
    private:
        
        boost::shared_ptr<const KineticOperator> _KineticOperator;
        
        boost::shared_ptr<const PotentialEnergySurface> _PES;
        
        Lagrangian();
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    typename PositionVectorType = std::vector< std::array<ScalarType, 3> >
    >
    inline
    Lagrangian::Lagrangian() const {
        
        ATOMISM_LOGIN();
        ATOMISM_LOGOUT();
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
	template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    typename PositionVectorType = std::vector< std::array<ScalarType, 3> >
    >
    inline
    Lagrangian::Lagrangian(boost::shared_ptr<const KineticOperator> kin,
                           boost::shared_ptr<const PotentialEnergySurface> epot
                           ) const {
        
        ATOMISM_LOGIN();
        
        _KineticOperator = kin;
        _PES = epot;
        
        ATOMISM_LOGOUT();
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    typename PositionVectorType = std::vector< std::array<ScalarType, 3> >
    >
    inline
    ScalarType Lagrangian::L(const GeneralizedCoordinates& q,
                             const GeneralizedCoordinates& qp,
                             Environment& environment ) const {
        ATOMISM_LOGIN();
        double L = T( q, qp, environment ) - V( q, environment );
        ATOMISM_LOGOUT();
        return L;
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    typename PositionVectorType = std::vector< std::array<ScalarType, 3> >
    >
    inline
    ScalarType Lagrangian::T(const GeneralizedCoordinates& q,
                             const GeneralizedCoordinates& qp,
                             PositionVectorType& positions,
                             Environment& environment ) const {
        ATOMISM_LOGIN();
        _KineticOperator->computeKMatrix( q , environment );
        double T = evaluateOperator( qp, environment.KMatrix , qp );
        ATOMISM_LOGOUT();
        return T;
        
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    typename PositionVectorType = std::vector< std::array<ScalarType, 3> >
    >
    inline
    ScalarType Lagrangian::U(const GeneralizedCoordinates& q,
                             Environment& Environment) const {
        
        ATOMISM_LOGIN();
        double U = _PES->evaluate( q , environment );
        ATOMISM_LOGOUT();
        return U;
    }
    
    
}
#endif // MSLagrangian_H
