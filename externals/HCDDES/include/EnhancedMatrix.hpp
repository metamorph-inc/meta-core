#ifndef _ISIS_ENHANCEDMATRIX
#define _ISIS_ENHANCEDMATRIX

#include <iostream>
#include <string>
#include <boost/numeric/ublas/matrix.hpp>

class EnhancedMatrix : public boost::numeric::ublas::matrix< double > {

	typedef boost::numeric::ublas::matrix< double > Matrix;

public:

	std::ostream &print( std::ostream &os ) const;

	bool empty( void ) const {
		return size1() == 0 || size2() == 0;
	}

	EnhancedMatrix() { }

	EnhancedMatrix( double value ) : Matrix(1,1) {
		insert_element( 0, 0, value );
	}

	EnhancedMatrix( int x, int y ) : Matrix( x, y ) { }

	EnhancedMatrix rowJoin( const EnhancedMatrix &rhs ) const;
	EnhancedMatrix colJoin( const EnhancedMatrix &rhs ) const;

	EnhancedMatrix transpose( void ) const;
};

std::ostream &operator<<( std::ostream &os, const EnhancedMatrix &enhancedMatrix );

#endif
