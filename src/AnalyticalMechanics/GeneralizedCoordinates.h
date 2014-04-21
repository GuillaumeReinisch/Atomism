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
#include <mutex>

namespace atomism {
    
    /*! \class GeneralizedCoordinates
     * \brief Container fo generalized coordinates
     */
    template<
    typename Scalar=double,
    typename Vector=std::vector<Scalar>
    >
    class GeneralizedCoordinates
    {
        
    public:
        
	GeneralizedCoordinates(const size_t n,     const Scalar& Values,
                               const Scalar& mins, const Scalar& maxs,
                               const Scalar dqs,   const Scalar& Dqs,
			       std::shared_ptr<ResourceManager<Scalar,Vector>> resourcemngr
			      );
 /*
	GeneralizedCoordinates(const Vector& Values, const Vector& mins, 
			       const Vector& maxs,   const Vector& dqs,  
			       const Vector& Dqs,
			       std::shared_ptr<ResourceManager<Scalar,Vector>> resourcemngr
			      );*/
	
	const Vector& getValues() const  {return _Values;};
	
	const Vector& getdqs()    const  {return _dqs;   };
	
	const Vector& getDqs()    const  {return _Dqs;   };
	
	const Vector& getMins()   const  {return _Mins;  };
	
	const Vector& getMaxs()   const  {return _Maxs;  };	
	
	void setValues(Vector& values)   { std::lock_guard<std::mutex> guard(_Mutex);
					   init_clone(_Values,values);
	                                  }
    private:
        
        Vector _Values;
        Vector _dqs;
        Vector _Dqs;
        Vector _Mins;
        Vector _Maxs;
        
	std::mutex _Mutex;
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<typename Scalar,typename Vector>
    inline
    GeneralizedCoordinates<Scalar,Vector>::GeneralizedCoordinates(
                               const size_t n, const Scalar& values,
                               const Scalar& mins, const Scalar& maxs,
                               const Scalar dqs,   const Scalar& Dqs,
			       std::shared_ptr<ResourceManager<Scalar,Vector>> resourcemngr
 								) {
        
        ATOMISM_LOG();
	
	auto v=resourcemngr->requestVector(n);
	_Values=constant_clone(*v,values);
	_Dqs   =constant_clone(*v,Dqs);
	_dqs   =constant_clone(*v,dqs);
	_Mins  =constant_clone(*v,mins);
	_Maxs  =constant_clone(*v,maxs);
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	/*
    template<typename Scalar,typename Vector>
    inline
    GeneralizedCoordinates<Scalar,Vector>::GeneralizedCoordinates(const Vector& values,
                               const Vector& mins, const Vector& maxs,
                               const Vector& dqs,   const Vector& Dqs) {
        
        ATOMISM_LOG();
	ATOMISM_EXCEPT_IF( [&](){return ((values.size()!=mins.size()) ||
	                                 (values.size()!=maxs.size()) ||
					 (values.size()!=dqs.size())  ||
					 (values.size()!=Dqs.size()) );} );
	
	clone(_Values,values);
	clone(_Dqs,   Dqs   );
	clone(_dqs,   dqs   );
	clone(_Mins,  mins  );
	clone(_Maxs,  maxs  );
    }; */
}
#endif // MSGeneralizedCoordinates_H
