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
    typename StateType          = std::vector<ScalarType>
    >
    class PairForceField : public PotentialEnergySurface<ScalarType,StateType,PairForceField> {
        
        
    public:
        
        ScalarType evaluate(const StateType& q, const Environment& env) const;
        
    private:
        
        PairForceField();
        
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
    PairForceField::PairForceField( boost::shared_ptr<Entity> entity)
        , PotentialEnergySurface(entity) {
    
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    VEX_FUNCTION(computeInteractionEnergy,
                 // size of the vector, index of the current element, x, y, z positions of the elements, and cutoff separation
                 double(size_t, size_t, double*, double*, double*),
                 VEX_STRINGIZE_SOURCE(
                                      double sum = 0;
                                      double X = prm3[prm2];
                                      double Y = prm4[prm2];
                                      double Z = prm5[prm2];
                                      for(size_t i = 0; i < prm1; ++i)
                                      
                                      if (i != prm2) {
                                          
                                          r=sqrt( (prm3[i]-X)^2 + (prm4[i]-Y)^2 + (prm5[i]-Z)^2 );
                                          
                                          if(r<prm6)
                                              sum += 4*1.08e-21*( (0.32e-9/r)^12 - (0.32e-9/r)^6 );
                                      }
                                      return sum;
                                      )
                 );
    
    
    template<
    typename ScalarType         = double,
    typename StateType          = std::vector<ScalarType>
    >
    inline
    void PairForceField::evaluate(const StateType& q, const Environment& env) const {
        
        IMPACT_LOGIN();
        
        const PositionVectorType& positions = env.getPositions();
        
        energy = computeInteractionEnergy( positions.size(),
                                           vex::element_index(),
                                           vex::raw_pointer(positions(0)),
                                           vex::raw_pointer(positions(1)),
                                           vex::raw_pointer(positions(2)),
                                           1e-9
                                          );
        double e = sum(energy);
        IMPACT_LOGOUT();
        return e;
    }
    
}
#endif // MSENTITY_H
