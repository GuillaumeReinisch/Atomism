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
#ifndef ATOMISM_METAPROGRAMMING_H
#define ATOMISM_METAPROGRAMMING_H


#include <metaprogramming_decl.h>


namespace Atomism
{
  
  template < typename T, int N , int M = 1 >
  inline
  T zero() { return T(N*M,0); 
  };
  
  template <typename T>
  inline
  void set_zero(T& output) { output = 0; }

  template <typename T>
  inline
  T zero_clone(const T& /* example */) { return 0; }

  template <typename T, typename T2>
  inline
  void zero_clone(T& output, const T2& /* example */) { output = 0; }

  template <typename T>
  inline
  void init_clone(T& output, const T& example) { output = example; }

  template <typename Vector, typename Scalar>
  inline
  void init_constant(Vector& output, const Scalar& example) {
    
    for ( size_t i=0; i != output.size(); ++i)
        init_clone(output[i], example);  
  }

  template <typename T, typename Scalar>
  inline
  T constant_clone(const T& /* example */, const Scalar& value) { return value; }

  
  template <typename Scalar,typename Matrix>
  inline 
  const Scalar& getElement(Matrix example, size_t i, size_t j, size_t size ){ return example[i*size+j];}
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
  template<typename Scalar, typename Vector>
  inline
  Scalar sum(const Vector& x) {
    
    ATOMISM_LOGIN();
    
    if(! x.size()) return ATOMISM_RETURN(Scalar(0));
    
    Scalar s = zero_clone(x[0]);
    for(size_t i=1; i<x.size(); i++) s+=x[i];

    return ATOMISM_RETURN(s);
  }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
  template<typename VectorType, typename MatrixType>
  inline
  VectorType multiply(const MatrixType& mat, const VectorType& vec) {
    
     ATOMISM_LOGIN();
     ATOMISM_EXCEPT_IF([&](){return double(mat.size()) / vec.size() != 
			     mat.size() / vec.size();});
     
     VectorType result = zero_clone(vec);
        
     for(size_t i=0;i<vec.size();i++)
         for(size_t j=0;j<mat.size() / vec.size() ;j++)
             result[i] += getElement(mat,i,j,vec.size())*vec[i];
        
     ATOMISM_RETURN( result );
  }
  
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
  template<typename Scalar, typename Vector >
  inline
  Scalar innerProduct(const Vector& x, const Vector& y){        
  
      ATOMISM_LOGIN();
      ATOMISM_EXCEPT_IF([&](){return x.size()!=y.size();});
      
      if( x.size()==0 ) ATOMISM_RETURN( 0);
      
      ScalarType s = init_clone(x[0]*y[0]);
      for(size_t i=1;i<x.size();i++) s+=x[i]*y[i];
      
      ATOMISM_RETURN(s);
    }
  
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

  template< typename Positions, typename Vector3d>
  inline
  void translate( Positions& coors, const Vector3d& trans ) {
      
      ATOMISM_LOGIN();
      for(size_t i=0;i<coors.size();i++)
	 for(size_t j=0; j<3;j++) 
	     coors[i][j] += trans[j];
	 
      ATOMISM_LOGOUT();
  };
  
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

  template<typename Positions, typename Matrix3d>
  inline
  void rotate(Positions& coors, const Matrix3d& rot ) {
      
      ATOMISM_LOGIN();
      for(size_t i=0;i<coors.size();i++)
	  coors[i] = multiply( rot , coors[i]);
      
      ATOMISM_LOGOUT();
  };
  
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

  template<typename Vector3d, typename Positions >
  inline
  Vector3d totalOutterProduct( const Positions& x, const Positions& y ) {
    
       ATOMISM_LOGIN();
       ATOMISM_EXCEPT_IF([&](){return x.size()!=y.size();});
      
       if (x.size==0) ATOMISM_RETURN( zero<Vector3d,3>() );
       
       auto sum = zero<Vector3d,3>();
        
       for( size_t i = 0; i <x.size(); i++ ) {
            
            sum[0] += x[i][1]*y[i][2] - x[i][2]*y[i][1];
            sum[1] += x[i][0]*y[i][2] - x[i][2]*y[i][0];
            sum[2] += x[i][0]*y[i][1] - x[i][1]*y[i][0];
        }
        
        ATOMISM_RETURN(sum);
  };

    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

  template<typename Vector, typename Vector3d, typename Positions>
  inline
  Vector3d totalLinearMomentum(const Positions& x, const Positions& y,
                               const Vector& masses) {
    
       ATOMISM_LOGIN();;
       ATOMISM_EXCEPT_IF( [&](){ return x.size()!=y.size();});
       ATOMISM_EXCEPT_IF( [&](){ return masses.size()!=x.size();}); 
       ATOMISM_EXCEPT_IF( [&](){ return masses.size()==0;});        
       
       auto linearMomentum = zero_clone(x[0]);
        
       for( size_t i = 0;i <x.size(); i++ )
           for(size_t j=0; j<3;j++)
               linearMomentum[j] += masses[i] * ( y[i][j] - x[i][j] ) ;
        
       ATOMISM_RETURN(linearMomentum);
  }
  
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

  template< typename Matrix3D,typename Vector, typename Positions>
  inline
  void linearMomentum( Positions& out, const Positions& x,
		      const Vector& masses, const Matrix3D& rot
 		      ) {
    
       ATOMISM_LOGIN();
       ATOMISM_EXCEPT_IF([&](){return masses.size()!=x.size();}); 
       ATOMISM_EXCEPT_IF([&](){return masses.size()==0;});
       ATOMISM_EXCEPT_IF([&](){return masses.size()!=out.size();});
        
       for( size_t i = 0;i <x.size(); i++ )
           for(size_t j=0; j<3;j++)
               
	       out[i][j] = masses[i] * (getElement(rot,j,0,3) * x[i][0] +
                                        getElement(rot,j,1,3) * x[i][1] +
                                        getElement(rot,j,2,3) * x[i][2] - x[i][j] );
        
       ATOMISM_LOGOUT();
  }
  
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

  template< typename Matrix3D,typename Vector,typename Vector3d,typename Positions>
  inline
  Vector3d totalAngularMomentum( const Matrix3D& rot, const Positions& x, const Vector& masses) {
    
       ATOMISM_LOGIN();
       ATOMISM_EXCEPT_IF([&](){return masses.size()!=x.size();});
       
       auto out = zero_clone(x);
       linearMomentum( out, x, masses, rot);
       Vector3d J = totalOutterProduct(x,out);
        
       ATOMISM_RETURN(J);
  }
  
  
} // end namespace Antioch

#endif //ATOMISM_METAPROGRAMMING_H
