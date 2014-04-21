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
#include <boost/concept_check.hpp>

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
    typename Positions    = std::tuple<Vector&,Vector&,Vector&>
    >
    class ResourceManager {
    public:  
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
				  
	Resource( T& ptr, size_t& count ) 
	: _Object(&ptr) { _Counts.push_back(&count);
			  count++;
	};
	
	
	Resource( T& ptr, std::vector<size_t*> counts ) 
	: _Object(&ptr) { _Counts = counts;
	                   for( auto count: _Counts) (*count)++; 
			 };
	
        T*    _Object;
	
	std::vector<size_t*> _Counts;
	
    public:
      
        T*  get() {return _Object;}
        
	T&  operator*(){ return *_Object;}
	
	~Resource() {
	    
	    for( auto count: _Counts) (*count)--;
	}
		     
	Resource<T>& operator=(const Resource<T>& resource) {
	             
	    _Object  = resource._Object;
	    _Counts  = resource._Counts;
	    for( auto count: _Counts) (*count)++;
	    return *this;
	}
		
	Resource(const Resource<T>& resource) { operator=(resource); }
			  
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
        typedef std::tuple<Vector,std::size_t,std::size_t>                VectorResource;
	typedef std::tuple<Matrix,std::size_t,std::size_t,std::size_t>    MatrixResource;
	typedef Positions             		  		  	  PositionsResource;
	
    public:
        
        ResourceManager();
        	
	Resource<Vector>    requestVector(std::size_t n);
	Resource<Matrix>    requestMatrix(std::size_t n,std::size_t n2);
	Resource<Positions> requestPositions(std::size_t n);
		
	void clear();  
	
    //private:
      
        std::vector<VectorResource>      _Vectors;	
        std::vector<MatrixResource>      _Matrices;	 
	std::vector<PositionsResource>   _Positions;	
	
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename Scalar,typename Vector,typename Matrix,typename Positions>
    inline
    ResourceManager<Scalar,Vector,Matrix,Positions>
    ::ResourceManager() { ATOMISM_LOG(); _Vectors.reserve(100);}
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename Scalar,typename Vector,typename Matrix,typename Positions>
    inline
    ResourceManager<Scalar,Vector,Matrix,Positions>::Resource<Vector> 
    ResourceManager<Scalar,Vector,Matrix,Positions>::requestVector(size_t n) {
      
        ATOMISM_LOG();
	
	for(  auto& it : _Vectors )
	    if( ( get<1>(it) == 0) && ( std::get<2>(it) == n)){
	      
	        LOGGER_WRITE(Logger::DEBUG,stringstream()<<"Vector available, return reference (size="
		                            <<n_elements(get<0>(it))<<") "<<&(std::get<0>(it))<<endl);
	        Resource<Vector> resource(std::get<0>(it),std::get<1>(it));
	        return resource;
	    }
	    
	LOGGER_WRITE(Logger::DEBUG,"Vector not available, create a new one.");
	
	_Vectors.push_back(VectorResource(Vector(),0,n));
	
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
	size_t i=0;
	for(  auto it : _Matrices ) {
	  
	    if( ( get<1>(it) == 0) && ( get<2>(it) == n1) && ( get<3>(it) == n2)){
	      
	        Resource<Matrix> resource(get<0>(it),get<1>(it));
	        return resource;
	    }
	}
	LOGGER_WRITE(Logger::DEBUG,"Matrix not available, create a new one.");
	
	_Matrices.push_back(MatrixResource(Matrix(),1,n1,n2));
	
	allocate(get<0>(_Matrices.back()),n1,n2);
	
	Resource<Matrix> resource(get<0>(_Matrices.back()),
				  get<1>(_Matrices.back()));
	
	return resource;
    }   
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename Scalar,typename Vector,typename Matrix,typename Positions>
    inline
    ResourceManager<Scalar,Vector,Matrix,Positions>::Resource<Positions> 
    ResourceManager<Scalar,Vector,Matrix,Positions>::requestPositions(size_t n) {
      
        ATOMISM_LOG();
	std::vector<Vector*> vectors;
	std::vector<size_t*> counts;
	
	for(  auto& it : _Vectors )
	    if( ( get<1>(it) == 0) && ( std::get<2>(it) == n)){
	      
	        LOGGER_WRITE(Logger::DEBUG,"One vector component available, use it");
	        vectors.push_back(&std::get<0>(it));
		counts.push_back(&(get<1>(it)));
		if(vectors.size()==3) break;
	    }
	    
        LOGGER_WRITE(Logger::DEBUG,stringstream()<<"Create the "
		     <<3 - vectors.size()<<" component(s)");
	
        for(size_t i=vectors.size();i<3;i++){
	  
	   _Vectors.push_back(VectorResource(Vector(),0,n));
	    allocate(get<0>(_Vectors.back()),n);
	    vectors.push_back(&std::get<0>(_Vectors.back()));
	    counts.push_back(&std::get<1>(_Vectors.back()));
	}
	
	Positions pos = std::tie(*(vectors[0]),*(vectors[1]),*(vectors[2]));
		
	_Positions.push_back(pos);
	Resource<Positions> resource(_Positions.back(),counts);
	cout<<*this<<endl;
	return resource;
	//return resource;
    } 
    
    
    template<typename Scalar,typename Vector,typename Matrix,typename Positions>  
    ostream& operator<<(ostream& out,
			const ResourceManager<Scalar,Vector,Matrix,Positions>& resource) {
      
      out<<"resource abstract"<<endl;
      for(size_t i=0;i<resource._Vectors.size();i++){
	
	  out<<i<<"\t"<<get<1>(resource._Vectors[i])<<"\t"
	  <<std::get<2>(resource._Vectors[i])<<"\t"
	  <<&(std::get<0>(resource._Vectors[i]))<<endl;
      }
      return out;
    };
    
    
}
#endif // MSENTITY_H
