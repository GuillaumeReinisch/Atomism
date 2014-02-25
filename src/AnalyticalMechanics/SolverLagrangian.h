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


#ifndef MSSOLVERLAGRANGIAN_H
#define MSSOLVERLAGRANGIAN_H

#include<msLagrangian.h>
#include<math.h>


namespace atomism
{
    
    /** \class msSolverLagrangian
     *  \brief Compute the dynamic of a Lagrangian motion
     *
     * The system of ODE of the Lagrangian (see msLagragian) is solved using the 4 order
     * Runge Kunta (based on Peter Selinger code and updated by Norman Hardy).
     * Based on ideas and code by Peter Lynch, Met Eireann, Glasnevin Hill, Dublin. \n
     * The Lagrangian equations of motion are
     * \f[
     \[ \frac{d}{dt}\frac{\partial L}{\partial{qp}_i} =
     \frac{\partial L}{\partial q_i} \]
     \f]
     * or equivalently
     * \f[
     \[ \sum_j\frac{\partial^2 L}{\partial {qp}_i\partial{qp}_j}
     \qpp_j+\sum_j\frac{\partial^2 L}{\partial {qp}_i\partial
     q_j} {qp}_j=\frac{\partial L}{\partial q_i}.
     \]
     \f]
     * Here we assume that \f$ L \f$ has no explicit time dependency, although it would be
     * easy to add that feature. There is one equation for each unconstrained coordinate
     * \f$ i \f$. The above is a system of linear equations  \f$ A.qpp+B.qp=v \f$, where
     \f[
     \[  A_{ij}=\frac{\partial^2 L}{\partial {qp}_i\partial {qp}_j},\
     B_{ij}=\frac{\partial^2 L}{\partial {qp}_i\partial q_j},\
     v_i=\frac{\partial L}{\partial q_i}.
     \]
     \f]
     * Here, \f$ i \f$ ranges over unconstrained coordinates, and \f$ j \f$ over all
     * coordinates. At a given instant, \f$ q_j \f$ and \f$ qp_j \f$ are known, and
     * so is \f$ qpp_j \f$ for any constrained coordinate \f$ j \f$. The unknowns
     * are \f$ qpp_i \f$, where \f$ i \f$ is unconstrained. \n
     * Issues can arise when dynamic of a system described by a Z-Matrice in which
     * a bending and dihedral angles of a same atom are defined as coordinates. Indeed,
     * when the system is close to a bending angle=0 or 180 degrees the determinant
     * of the K-Matrice become null. The ODEs are hard to solve close to these points.
     */
    class msSolverLagrangian : public msTreeMapper
    {
    protected:
        
        msSolverLagrangian() : msTreeMapper() { constructVar("msSolverLagrangian","SolverLagrangian","lagrangian solver");
            Ndof=0; t_current=0; t_tot=100; SiUnits.setSI();
        }
        
    public:
        
        void dynamic( msLagrangian& lagrangien , double E );
        
        boost::shared_ptr<msTreeMapper> set_ttot(double t){ t_tot=t; return mySharedPtr();};
        
        std::ostream& print(std::ostream& out) const;
        
        boost::shared_ptr<msParamsManager> getParameters() const{ return Parameters.getSharedPtr(); }
        
    private:
        
        //@{
        
        double dEk;
        double t_tot;
        
        double Ek,Ep,Et,L;
        //@}
        msLagrangian* Lagrangien;
        
        //! @name Variables used by the classe for the calculations NOTE: everything is in SI units
        //@{
        double t_current;
        vector_type q;
        vector_type qp;
        
        vector_type Epsilon;
        vector_type Epsilonp;
        
        vector_type K_0;
        vector_type K_1;
        vector_type K_2;
        vector_type K_3;
        vector_type Kp_0;
        vector_type Kp_1;
        vector_type Kp_2;
        vector_type Kp_3;
        
        vector_type rhs;
        matrix_type A;
        //@}
        double diffq(int i, vector_type& q_, vector_type& qp_);
        double diffpp(int i, int j, vector_type& q_, vector_type& qp_);
        double diffqp(int i, int j, vector_type& q_, vector_type& qp_);
        
        bool calc_qpp(vector_type& q_,vector_type& qp_, vector_type& qpp_);
        void solve(matrix_type& A, vector_type& v, vector_type& w);
        void solve2(matrix_type& A, vector_type& v, vector_type& w);
        int Ndof;
        
    };
    
}


bool msSolverLagrangian::step(double dt) {
    
    vector_type qtmp(  Ndof );
    vector_type qptmp( Ndof );
    vector_type qpp(   Ndof );
    
    if( !calc_qpp(q, qp, qpp) )  return 0;
    
    for( int i=0; i<Ndof; i++) { K_0[i]  = dt * qp[i];
        Kp_0[i] = dt * qpp[i];
    }
    
    // calculate K1
    for( int i=0; i<Ndof; i++) { qtmp[i]  = q[i]  + 0.5 * K_0[i];
        qptmp[i] = qp[i] + 0.5 * Kp_0[i];
    }
    if( !calc_qpp(qtmp, qptmp, qpp) ) return 0;
    
    for(int i=0; i<Ndof; i++) { K_1[i]  = dt * qptmp[i];
        Kp_1[i] = dt * qpp[i];
    }
    // calculate K2
    for( int i=0; i<Ndof; i++){ qtmp[i]  = q[i]  + 0.5 * K_1[i];
        qptmp[i] = qp[i] + 0.5 * Kp_1[i];
    }
    if( !calc_qpp(qtmp, qptmp, qpp) ) return 0;
    
    for (int i=0; i<Ndof; i++) { K_2[i]  = dt * qptmp[i];
        Kp_2[i] = dt * qpp[i];
    }
    // calculate K3
    for (int i=0; i<Ndof; i++) { qtmp[i]  = q[i] + K_2[i];
        qptmp[i] = qp[i] + Kp_2[i];
    }
    if( !calc_qpp(qtmp, qptmp, qpp)) return 0;
    
    for( int i=0; i<Ndof; i++) { K_3[i]  = dt * qptmp[i];
        Kp_3[i] = dt * qpp[i];
    }
    // Advance step
    for( int i=0; i<Ndof; i++) {
        
        q[i]  += (  K_0[i] + 2*K_1[i]  + 2*K_2[i]  + K_3[i] ) / 6 ;
        qp[i] += ( Kp_0[i]+ 2*Kp_1[i] + 2*Kp_2[i] + Kp_3[i] ) / 6 ;
    }
    
    //q2Coordinfo();
    t_current += dt;
    return 1;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

double msSolverLagrangian::diffq(int i, vector_type& q_, vector_type& qp_)
{
    double L0, L1;
    
    q_[i] +=  Epsilon[i];
    L1     =  Lagrangien->L(q_, qp_);
    q_[i] -=  2*Epsilon[i];
    L0     =  Lagrangien->L(q_, qp_);
    q_[i] +=  Epsilon[i];
    
    return (L1-L0)/(2*Epsilon[i]);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

double msSolverLagrangian::diffpp(int i, int j, vector_type& q_, vector_type& qp_)
{
    double L00, L01, L10, L11;
    
    double epsilonp_i=Epsilonp[i]; double epsilonp_j=Epsilonp[j];
    
    Lagrangien->GeneralizedCoordinates->setUnfreezedValues(SiUnits,q_);
    Lagrangien->KineticOperator->computeKMat( SiUnits );
    
    qp_[i] += epsilonp_i;
    qp_[j] += epsilonp_j;
    L11     = Lagrangien->L(q_, qp_, 0); qp_[j] -= 2*epsilonp_j;
    L10     = Lagrangien->L(q_, qp_, 0); qp_[i] -= 2*epsilonp_i;
    L00     = Lagrangien->L(q_, qp_, 0); qp_[j] += 2*epsilonp_j;
    L01     = Lagrangien->L(q_, qp_, 0); qp_[j] -= epsilonp_j;
    qp_[i] += epsilonp_i;
    
    return ((L11-L01)/(2*epsilonp_i)-(L10-L00)/(2*epsilonp_i))/(2*epsilonp_j);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

double msSolverLagrangian::diffqp(int i, int j, vector_type& q_, vector_type& qp_) {
    
    double L00, L01, L10, L11;
    
    q_[i]  += Epsilon[i];
    qp_[j] += Epsilonp[j];
    Lagrangien->GeneralizedCoordinates->setUnfreezedValues(SiUnits,q_);
    Lagrangien->KineticOperator->computeKMat( SiUnits );
    L11    = Lagrangien->L(q_, qp_,0);
    
    qp_[j] -= 2*Epsilonp[j];
    L10    = Lagrangien->L(q_, qp_,0);
    
    q_[i]  -= 2*Epsilon[i];
    Lagrangien->GeneralizedCoordinates->setUnfreezedValues(SiUnits,q_);
    Lagrangien->KineticOperator->computeKMat( SiUnits );
    L00    = Lagrangien->L(q_, qp_,0);
    
    qp_[j] += 2*Epsilonp[j];
    L01    = Lagrangien->L(q_, qp_,0);
    
    qp_[j] -= Epsilonp[j];
    q_[i]  += Epsilon[i];
    
    return ((L11-L01)/(2*Epsilon[i])-(L10-L00)/(2*Epsilon[i]))/(2*Epsilonp[j]);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool msSolverLagrangian::calc_qpp(vector_type& q_, vector_type& qp_, vector_type& qpp_)
{
    // Calculate system of equations
    for (int i=0; i<Ndof; i++) {
        
        rhs[i] = diffq(i,q_,qp_);
        for (int j=0; j<Ndof; j++) {
            
            A(i,j) = diffpp(j,i,q_,qp_);
            
            if(A(i,j)==0){ printError(AijError); return 0;  }
            // calculate B
            rhs[i] -= diffqp(j,i,q_,qp_) * qp_[j];
        }
    }
    // Solve A * q'' = rhs
    solve(A,rhs,qpp_);
    return 1;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void msSolverLagrangian::solve(matrix_type& A, vector_type& v, vector_type& w ) {
    
    if (Ndof==2) { solve2(A, v, w); return;
    }
    
    int* r = new int[Ndof];  // indexing rows and columns for quick reordering
    int* c = new int[Ndof];
    int  n = 0;              // n is the total number of unconstrained rows
    
    for (int i=0; i<Ndof; i++) { c[n]=i; r[n]=i; n++;
    }
    
    for (int j=0; j<n-1; j++) {  // make j'th column zero from j+1'st row
        
        for (int i=j+1; i<n; i++) {  // make Aij zero
            
            if (fabs(A( r[j] , c[j] ) )<fabs(A(r[i],c[j]) )) {
                int tmp;
                tmp=r[i]; r[i]=r[j]; r[j]=tmp;
            }
            
            if (A(r[i],c[j]) != 0.0) {
                double m = A(r[i] , c[j]) / A( r[j] , c[j]);
                
                for (int k=j; k<n; k++)  A(r[i] , c[k]) -= m*A( r[j] , c[k]);
                
                v[r[i]] -= m*v[r[j]];
            }
        }
    }
    // calculate result vector
    
    for (int j=n-1; j>=0; j--) {
        
        double b=v[r[j]];
        for (int k=j+1; k<n; k++)  b-=w[c[k]]*A( r[j] , c[k]);
        
        w[c[j]] = b / A( r[j] , c[j]);
    }
    
    delete[] r;delete[] c;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void msSolverLagrangian::solve2(matrix_type& A, vector_type& v, vector_type& w) {
    
    double det=A.determinant();
    w[0]=( A(1,1)*v[0]-A(0,1)*v[1])/det;
    w[1]=(-A(1,0)*v[0]+A(0,0)*v[1])/det;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


#endif // MSSOLVERLAGRANGIAN_H
