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


#ifndef GeneralizedCoordinates_H
#define GeneralizedCoordinates_H

#include <Entity.h>

namespace atomism {
    
    /*! \class GeneralizedCoordinates
     * \brief Container fo generalized coordinates
     */
	template<
    typename ScalarType=double,
    typename StateType=std::vector<ScalarType>
    >
    class GeneralizedCoordinates
    {
        
    public:
        
	    GeneralizedCoordinates(const StateType& Values,
                               const StateType& mins, const StateType& maxs,
                               const StateType dqs,   const StateType& Dqs);
        
        GeneralizedCoordinates(const GeneralizedCoordinates& coors);
        
        GeneralizedCoordinates(GeneralizedCoordinates&& coors);
        
    private:
        
        StateType _Values;
        StateType _dqs;
        StateType _Dqs;
        StateType _Mins;
        StateType _Maxs;
        
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template< typename ScalarType=double,
    typename VectorType=std::vector<ScalarType>
    >
    inline
    void GeneralizedCoordinates::addVariable(ScalarType values,ScalarType mins, ScalarType maxs,
                                             ScalarType dqs, ScalarType Dqs) {
        
        ATOMISM_LOGIN();
        
        ATOMISM_EXCEPT_IF( [&](){ return ((Value.size()<mins.size()) or
                                          (Value.size()<maxs.size()) or
                                          (Value.size()<dqs.size())  or
                                          (Value.size()<Dqs.size())); },
                          "size of arguments not consistent" );
        _Values = values;
        _Dqs =    Dqs;
        _dqs =    dqs;
        _Min =    mins;
        _Maxs =   maxs;
        
        ATOMISM_LOGOUT();
        return KMatrix;
    };
    
}
#endif // MSGeneralizedCoordinates_H
