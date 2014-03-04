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


#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>

#include <Logger.h>
#include <Exceptions.h>

#include <vector_utils.h>

namespace atomism {
    
 
    /** \class ResourceManager
     *
     * \brief Manage the resources, ensure thread safety and promote resources recycling.
     * 
     * To obtain a new resources, use the requestVector, requestMatrix or
     * requestPositions methods. If a resource of equal size already exist and is not used,
     * a reference to it is returned; otherwise a new resource is allocated and the reference
     * returned.
     * 
     * The object pointed to by a resource is locked while the resource stays in the scope (no other
     * thread can use it). When the resource gets out of scope, the object pointed to is unlocked 
     * and another thread can use it.
     */
    template<
    typename Scalar       = double,
    typename Vector       = std::vector<Scalar>,
    typename Matrix       = std::vector<std::vector<Scalar>>,
    typename Positions    = std::array< std::vector<Scalar> , 3>
    >
    class ResourceManager {
        
     /** \class Resource
     *
     * \brief Wrapper to ensure correct liberation of resources 
     * 
     * When the resource is going out of scope, the _Object pointed is to is 
     * freed in the ressource manager.
     */ 
    template< typename T > class Resource {
        
        friend ResourceManager;
	
	Resource() : _Object(0){};
	
	Resource(T& ptr,bool& locked) 
	: _Object(&ptr),_Locked(&locked) {*_Locked=1; };
	
        T*    _Object;
	bool* _Locked;
	
    public:
      
        T*  get() {return _Object;}
        
	T&  operator*(){ return *_Object;}
	
	~Resource() { _Locked=0;}
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
        typedef std::tuple<Vector,bool,size_t>           VectorResource;
	typedef std::tuple<Vector,bool,size_t,size_t>    MatrixResource;
	typedef std::tuple<Positions,bool,size_t>        PositionsResource;
	
    public:
        
        ResourceManager();
        	
	Resource<Vector>    requestVector(size_t n);
	Resource<Matrix>    requestMatrix(size_t n,size_t n2);
	Resource<Positions> requestPositions(size_t n);
		
	void clear();  
	
    private:
      
        std::vector<VectorResource>      _Vectors;	
        std::vector<MatrixResource>      _Matrices;	 
	std::vector<PositionsResource>   _Positions;	
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename Scalar,typename Vector,typename Matrix,typename Positions>
    inline
    ResourceManager<Scalar,Vector,Matrix,Positions>
    ::ResourceManager() { ATOMISM_LOG(); }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename Scalar,typename Vector,typename Matrix,typename Positions>
    inline
    ResourceManager<Scalar,Vector,Matrix,Positions>::Resource<Vector> 
    ResourceManager<Scalar,Vector,Matrix,Positions>::requestVector(size_t n) {
      
        ATOMISM_LOG();
	
	for(  auto it : _Vectors )
	    if( ( get<1>(it) == 0) && ( get<2>(it) == n)){
	      
	        LOGGER_WRITE(Logger::DEBUG,"Vector available, return reference.");
	        Resource<Vector> resource(get<0>(it),get<1>(it));
	        return resource;
	    }
	LOGGER_WRITE(Logger::DEBUG,"Vector not available, create a new one.");
	
	_Vectors.push_back(VectorResource(Vector(),1,n));
	
	allocate(get<0>(_Vectors.back()),n);
	
	Resource<Vector> resource(get<0>(_Vectors.back()),
				  get<1>(_Vectors.back()));
	
	return resource;
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename Scalar,typename Vector,typename Matrix,typename Positions>
    inline
    ResourceManager<Scalar,Vector,Matrix,Positions>::Resource<Matrix> 
    ResourceManager<Scalar,Vector,Matrix,Positions>::requestMatrix(size_t n1,size_t n2) {
      
        ATOMISM_LOG();
	
	for(  auto it : _Vectors )
	    if( ( get<1>(*it) == 0) && ( get<2>(*it) == n1) && ( get<3>(*it) == n2))
	        return get<0>(*it);
	
	Matrix v;
	allocate(v,n1,n2);
	MatrixResource allocated = MatrixResource(v,1,n1,n2);
	
	_Matrices.push_back(allocated);
	return get<0>(_Vectors.back());
    }   
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename Scalar,typename Vector,typename Matrix,typename Positions>
    inline
    ResourceManager<Scalar,Vector,Matrix,Positions>::Resource<Positions> 
    ResourceManager<Scalar,Vector,Matrix,Positions>::requestPositions(size_t n) {
      
        ATOMISM_LOG();
	
	for(  auto it : _Positions )
	    if( ( get<1>(it) == 0) && ( get<2>(it) == n)){
	        Resource<Positions> resource(get<0>(it),get<1>(it));
	        return resource;
	    }
	
	LOGGER_WRITE(Logger::DEBUG,"Positions not available, create a new one.");
	
	_Positions.push_back(PositionsResource(Positions(),1,n));
	
	allocate(get<0>(_Positions.back()),n);
	
	Resource<Positions> resource(get<0>(_Positions.back()),
				     get<1>(_Positions.back()));
	
	return resource;
    } 
}
#endif // MSENTITY_H
