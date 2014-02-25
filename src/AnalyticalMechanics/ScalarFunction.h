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

//! \file Entity.h definition of the class Entity, msVectorOfDofs, and msVectorOfPtrDof and msKineticWork

#ifndef SCALARFUNCTION_H
#define SCALARFUNCTION_H


namespace atomism {
    
    /** \class Balls
     *
     * \brief An assembly of elements that move on the surface of a sphere.
     */
	template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    class DerivedClass
    >
    class PotentialEnergySurface {
        
        
    public:
        
        PotentialEnergySurface( boost::shared_ptr<Entity> entity);
        
        ScalarType evaluate(const StateType& q) const;
        
    private:
        
        PotentialEnergySurface();
        
        boost::shared_ptr<Entity> _Entity;        
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
	template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    class DerivedClass
    >
    inline
    PotentialEnergySurface::PotentialEnergySurface( boost::shared_ptr<Entity> entity) {
        
        _Entity = entity;
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>,
    class DerivedClass
    >
    inline
    void ScalarFunction::evaluate(const StateType& q) const {
        
        IMPACT_LOGIN();
        ScalarType r = static_cast<DerivedClass*>(this)->evaluate();
        IMPACT_LOGOUT();
        return r;
    }
    
}
#endif // MSENTITY_H