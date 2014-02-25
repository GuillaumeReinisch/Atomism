
#include "std_definitions.h"

namespace atomism {

    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename ScalarType>
    ScalarType sum<ScalarType,vex::vector<ScalarType> >(const vex::vector<ScalarType>& x) {
        
        return vex::reduce<SUM>(x);
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename ScalarType>
    vex::vector<ScalarType> multiply<ScalarType,vex::vector<ScalarType>, vex::vectorScalarType >(
                                                                              const vex::vector<ScalarType>& mat,
                                                                              const vex::vector<ScalarType>& vec) {
        ATOMISM_LOGGIN();
        vex::vector<ScalarType> result = vex::reduce<SUM>( vex::extents[N][M],
                                  mat * reshape( vec, extents[n][m], extents[1] ),
                                  1
                                  );
        ATOMISM_LOGOUT();
        return result;
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline
    ScalarType innerProduct<ScalarType,vex::vector<ScalarType> >(const vex::vector<ScalarType>& x,
                                                                 const vex::vector<ScalarType>& y) {
        ATOMISM_LOGGIN();
        return vex::reduce<SUM>(x*y);
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<typename ScalarType,typename Vector3dType>
    inline
    void translate<ScalarType,vex::multivector<ScalarType,3>,Vector3dType>(vex::multivector<ScalarType,3>& coors,
                                                                           const Vector3dType& trans ) {
        
        ATOMISM_LOGGIN();
        coors = std::tie(coors(0) + trans[0],
                         coors(1) + trans[1],
                         coors(2) + trans[2]);
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template< typename ScalarType,typename Matrix3DType>
    inline
    void rotate<ScalarType,vex::multivector<ScalarType,3>,Vector3dType>(vex::multivector<ScalarType,3>& coors,
                                                                        const Matrix3DType& rot ) {
        
        ATOMISM_LOGGIN();
        coors = std::tie(coors(0) * rot[0][0] + coors(1) * rot[0][1] + coors(2) * rot[0][2] ,
                         coors(0) * rot[1][0] + coors(1) * rot[1][1] + coors(2) * rot[1][2] ,
                         coors(0) * rot[2][0] + coors(1) * rot[2][1] + coors(2) * rot[2][2]
                         );
        
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template< typename ScalarType,typename Vector3dType >
    inline
    Vector3dType sumOutterProduct<ScalarType,vex::multivector<ScalarType,3>,Vector3dType>
    (const vex::multivector<ScalarType,3>& coors0,
     const vex::multivector<ScalarType,3>& coors1 ) {
        
        ATOMISM_LOGGIN();
        
        VectorPositionType tmp = std::tie(coors0(1)*coors1(2) - coors0(2)*coors1(1),
                                          coors0(0)*coors1(2) - coors0(2)*coors1(0),
                                          coors0(0)*coors1(1) - coors0(1)*coors1(0));
        
        ATOMISM_LOGOUT();
        return sum(tmp);
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template< typename ScalarType,typename Vector3dType >
    inline
    Vector3dType computeLinearMomentum<ScalarType,vex::multivector<ScalarType,3>,Vector3dType>
    (const VectorPositionType& coors0,
     const VectorPositionType& coors1,
     const VectorType& masses){
        
        ATOMISM_LOGGIN();
        
        linearMomentum = std::tie(masses*( coors1(0) - coors0(0) ),
                                  masses*( coors1(1) - coors0(1) ),
                                  masses*( coors1(2) - coors0(2) ));
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template< typename ScalarType,typename Vector3dType >
    inline
    Vector3dType computeLinearMomentum<ScalarType,vex::multivector<ScalarType,3>,Vector3dType>
    (const Matrix3D& rot,
     const VectorPositionType& coors,
     const VectorType& masses,
     VectorPositionType& linearMomentum){
        
        ATOMISM_LOGGIN();
        ATOMISM_EXCEPT_IF([&]() { return coors.size()!=masses.size(); },
                          "unconsistent size between coordinates and masses");
        
        linearMomentum = std::tie(masses*( rot[0][0] * coors0(0) + rot[0][1]* coors0(1) + rot[0][2]* coors0(2) - coors0(0)),
                                  masses*( rot[1][0] * coors0(0) + rot[1][1]* coors0(1) + rot[1][2]* coors0(2) - coors0(1)),
                                  masses*( rot[2][0] * coors0(0) + rot[2][1]* coors0(1) + rot[2][2]* coors0(2) - coors0(2)) );
        ATOMISM_LOGOUT();
    }
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template< typename ScalarType,typename Vector3dType,typename Matrix3dType >
    inline
    Vector3DType computeAngularMomentum<ScalarType,vex::multivector<ScalarType,3>,Vector3dType,Matrix3dType>(
    const Matrix3DType& rot,
    const VectorPositionType& coors,
    const VectorType& masses){
        
        ATOMISM_LOGGIN();
        
        vex::multivector<ScalarType,3> linearMomentum;
        
        computeLinearMomentum( rot, coors, masses, linearMomentum);
        Vector3DType J = sumOutterProducts(coors,linearMomentum);
        
        ATOMISM_LOGOUT();
        return J;
    }
    
}