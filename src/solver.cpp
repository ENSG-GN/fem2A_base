#include "solver.h"
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <stdlib.h>


#include "../third_party/OpenNL_psm.h"

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

namespace FEM2A {

    bool solve(
        const SparseMatrix& A,
        const std::vector< double >& b,
        std::vector< double >& x )
    {
        assert(A.nb_rows() == b.size()) ;
        int n = b.size() ;
        x.resize( n ) ;

        NLContext nl_context = nlNewContext() ;
        nlSolverParameteri( NL_NB_VARIABLES, NLint( n    ) ) ;
        nlSolverParameteri( NL_MAX_ITERATIONS, NLint( 1e6 ) ) ;
        nlSolverParameterd( NL_THRESHOLD, NLdouble( 1e-12 ) ) ;
        nlEnable( NL_VERBOSE ) ;
        nlBegin( NL_SYSTEM ) ;
        nlBegin( NL_MATRIX ) ;
        for( int i = 0; i < n; i++ ) {
            const std::vector< int >& J = A.get_cols_at_line( i ) ;
            const std::vector< double >& V = A.get_vals_at_line( i ) ;
            assert( J.size() == V.size() ) ;
            nlBegin( NL_ROW ) ;
            for( unsigned int k = 0; k < J.size(); k++ ) {
                nlCoefficient( J[k], NLdouble( V[k] ) ) ;
            }
            nlRightHandSide( b[i] ) ;
            nlEnd( NL_ROW ) ;
        }
        nlEnd( NL_MATRIX ) ;
        nlEnd( NL_SYSTEM ) ;
        std::cout << "solving system with " << n << " unknowns .. " << std::endl ;

        if( !nlSolve() ) {
            std::cout << "Failure: OpenNL didn't manage to solve the system"
                << std::endl ;
            return false ;
        }

        for( int i = 0; i < n; i++ ) {
            x[i] = nlGetVariable( i ) ;
        }

        nlDeleteContext( nl_context ) ;

        std::cout << ".. system solved" << std::endl ;
        return true ;
    }

    bool test_opennl()
    {
        std::cout << "------------------------ \n" ;
        std::cout << "testing OpenNL solver .. \n" ;
        double matrix[9] = { 2., -1., -3., 3., 2., -2., -1., -4., 6. } ;

        double rhs[3] = { 1., -4., 22. } ;

        double sol[3] = { 2., -3., 2. } ;

        NLContext nl_context = nlNewContext() ;
        nlSolverParameteri( NL_NB_VARIABLES, NLint( 3 ) ) ;
        nlSolverParameteri( NL_MAX_ITERATIONS, NLint( 1e6 ) ) ;
        nlSolverParameterd( NL_THRESHOLD, NLdouble( 1e-8 ) ) ;
        nlEnable( NL_VERBOSE ) ;
        nlBegin( NL_SYSTEM ) ;
        nlBegin( NL_MATRIX ) ;
        for( unsigned int i = 0; i < 3; i++ ) {
            nlBegin( NL_ROW ) ;
            for( unsigned int j = 0; j < 3; j++ ) {
                nlCoefficient( j, NLdouble( matrix[3 * i + j] ) ) ;
            }
            nlRightHandSide( rhs[i] ) ;
            nlEnd( NL_ROW ) ;
        }
        nlEnd( NL_MATRIX ) ;
        nlEnd( NL_SYSTEM ) ;
        if( !nlSolve() ) {
            std::cout << " OpenNL didn't manage to solve the system" << std::endl ;
        }

        double epsilon = 1e-6 ;
        for( unsigned int i = 0; i < 3; i++ ) {
            if( std::fabs( sol[i] - nlGetVariable( i ) ) > epsilon ) {
                std::cout << ".. FAILED (wrong solution)" << std::endl ;
                return false ;
            }
        }
        nlDeleteContext( nl_context ) ;
        std::cout << ".. SUCCESS" << std::endl ;
        return true ;
    }
    
    /****************************************************************/
    /* Implementation of DenseMatrix */
    /****************************************************************/
    int DenseMatrix::height() const 
    {
        return height_;
    }
    int DenseMatrix::width() const {
        return width_;
    }
    void DenseMatrix::set_size( int height, int width )
    {
        height_ = height ;
        width_ = width ;
        data_.resize( height_ * width_, 0 ) ;
    }

    void DenseMatrix::set( int i, int j, double v )
    {
        ASSERT( i < height_, "access to element outside of the matrix" ) ;
        ASSERT( j < width_, "access to element outside of the matrix" ) ;
        data_[width_ * i + j] = v ;
    }

    void DenseMatrix::add( int i, int j, double v )
    {
        ASSERT( i < height_, "access to element outside of the matrix" ) ;
        ASSERT( j < width_, "access to element outside of the matrix" ) ;
        data_[width_ * i + j] += v ;
    }

    double DenseMatrix::get( int i, int j ) const
    {
        ASSERT( i < height_, "access to element outside of the matrix" ) ;
        ASSERT( j < width_, "access to element outside of the matrix" ) ;
        return data_[width_ * i + j] ;
    }

    DenseMatrix DenseMatrix::transpose() const {
        DenseMatrix T;
        T.set_size(width_, height_);
        for (int i = 0; i < width_; ++i) {
            for (int j = 0; j < height_; ++j) {
                T.set(i, j, get(j, i));
            }
        }
        return T;
    }

    double DenseMatrix::det_2x2() const
    {
        ASSERT( height_ == 2 && width_ == 2, "det_2x2 only for matrix 2x2" ) ;
        return data_[0] * data_[3] - data_[1] * data_[2] ;
    }

    DenseMatrix DenseMatrix::invert_2x2() const
    {
        ASSERT( height_ == 2 && width_ == 2, "invert_2x2 only for matrix 2x2" ) ;
        const double det = det_2x2() ;
        DenseMatrix I ;
        I.set_size( 2, 2 ) ;
        I.data_[0] =  data_[3] / det ;          I.data_[1] = -data_[1] / det ;
        I.data_[2] = -data_[2] / det ;          I.data_[3] =  data_[0] / det ;
        return I ;
    }

    vec2 DenseMatrix::mult_2x2_2( vec2 x ) const
    {
        ASSERT( height_ == 2 && width_ == 2, "mult_2x2_2 only for matrix 2x2" ) ;
        vec2 r ;
        r.x = data_[0] * x.x + data_[1] * x.y ;
        r.y = data_[2] * x.x + data_[3] * x.y ;
        return r ;
    }

    void DenseMatrix::print() const 
    {
        std::cout << std::setprecision(3);
        for(int i = 0; i < height_; ++i) {
            for(int j = 0; j < width_; ++j) {
                std::cout << std::right << std::setw(8) << data_[i*width_+j];
            }
            std::cout << std::endl;
        }
    }

    double dot( vec2 x, vec2 y )
    {
        return x.x * y.x + x.y * y.y ;
    }

    /****************************************************************/
    /* Implementation of SparseMatrix */
    /****************************************************************/

    SparseMatrix::SparseMatrix( int nb_dofs )
        : cols_at_line_( nb_dofs ), val_at_line_( nb_dofs )
    {

    }

    void SparseMatrix::add( int i, int j, double val )
    {
        bool found = false ;
        for( int k = 0; k < cols_at_line_[i].size(); k++ ) {
            if( cols_at_line_[i][k] == j ) {
                found = true ;
                val_at_line_[i][k] += val ;
            }
        }

        if( !found ) {
            cols_at_line_[i].push_back( j ) ;
            val_at_line_[i].push_back( val ) ;
        }
    }
    void SparseMatrix::multiply( int i, int j, double val )
    {
        bool found = false ;
        for( int k = 0; k < cols_at_line_[i].size(); k++ ) {
            if( cols_at_line_[i][k] == j ) {
                found = true ;
                val_at_line_[i][k] *= val ;
            }
        }

        if( !found ) {
            ASSERT(false,"Can't multiply a coefficient that does not exist") ;
        }
    }
    const std::vector< int > & SparseMatrix::get_cols_at_line( int i ) const
    {
        return cols_at_line_[i] ;
    }
    const std::vector< double > & SparseMatrix::get_vals_at_line( int i ) const
    {
        return val_at_line_[i] ;
    }

    int SparseMatrix::nb_rows() const
    {
        return cols_at_line_.size() ;
    }

    void SparseMatrix::print() const 
    {
        std::cout << std::setprecision(3);
        for(int i = 0; i < cols_at_line_.size(); ++i) {
            const std::vector<int>& J = cols_at_line_[i];
            const std::vector<double>& V = val_at_line_[i];
            std::cout << std::right << std::setw(3) << i << "|";
            for(int k = 0; k < J.size() ; ++k) {
                std::cout << std::right << std::setw(6) << "(" << J[k] << "," << V[k] << ") ";
            }
            std::cout << std::endl;
        }
    }

}
