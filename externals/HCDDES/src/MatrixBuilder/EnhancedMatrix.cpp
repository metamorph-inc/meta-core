#include "EnhancedMatrix.hpp"


std::ostream &EnhancedMatrix::print( std::ostream &os ) const {
  for( EnhancedMatrix::const_iterator1 mtxci1 = begin1() ; mtxci1 != end1() ; (void)++mtxci1 ) {
    for( EnhancedMatrix::const_iterator2 mtxci2 = mtxci1.begin() ; mtxci2 != mtxci1.end() ; (void)++mtxci2 ) {
      os << *mtxci2 << " ";
    }
    os << std::endl;
  }
return os;
}

std::ostream &operator<<( std::ostream &os, const EnhancedMatrix &enhancedMatrix ) {
  return enhancedMatrix.print( os );
}

EnhancedMatrix EnhancedMatrix::rowJoin( const EnhancedMatrix &rhs ) const {

//std::cerr << "rowJoin:" << std::endl;
//std::cerr << "this:" << std::endl;
//print( std::cerr );
//std::cerr << std::endl;
//std::cerr << "rhs:" << std::endl;
//rhs.print( std::cerr );
//std::cerr << std::endl;

  if ( empty() ) return rhs;
  if ( rhs.empty() ) return *this;

  assert( size1() == rhs.size1() );

  EnhancedMatrix rwj( *this );
  rwj.resize( rwj.size1(), rwj.size2() + rhs.size2() );

  EnhancedMatrix::iterator2 rwjIt2 = rwj.begin2();
  rwjIt2 += size2();

  EnhancedMatrix::const_iterator2 rhsIt2 = rhs.begin2();

  while( rwjIt2 != rwj.end2() ) {
    EnhancedMatrix::iterator1 rwjIt1 = rwjIt2.begin();
    EnhancedMatrix::const_iterator1 rhsIt1 = rhsIt2.begin();

    while( rhsIt1 != rhsIt2.end() ) {
      *rwjIt1 = *rhsIt1;
      (void)++rwjIt1; (void)++rhsIt1;
    }

    (void)++rwjIt2; (void)++rhsIt2;
  }

//std::cerr << "rwj:" << std::endl;
//rwj.print( std::cerr );
//std::cerr << std::endl << std::endl;
return rwj;
}


EnhancedMatrix EnhancedMatrix::colJoin( const EnhancedMatrix &rhs ) const {

//std::cerr << "colJoin:" << std::endl;
//std::cerr << "this:" << std::endl;
//print( std::cerr );
//std::cerr << std::endl;
//std::cerr << "rhs:" << std::endl;
//rhs.print( std::cerr );
//std::cerr << std::endl;

  if ( empty() ) return rhs;
  if ( rhs.empty() ) return *this;

  assert( size2() == rhs.size2() );

  EnhancedMatrix clj( *this );
  clj.resize( clj.size1() + rhs.size1(), clj.size2() );

  EnhancedMatrix::iterator1 cljIt1 = clj.begin1();
  cljIt1 += size1();

  EnhancedMatrix::const_iterator1 rhsIt1 = rhs.begin1();

  while( cljIt1 != clj.end1() ) {
    EnhancedMatrix::iterator2 cljIt2 = cljIt1.begin();
    EnhancedMatrix::const_iterator2 rhsIt2 = rhsIt1.begin();

    while( rhsIt2 != rhsIt1.end() ) {
      *cljIt2 = *rhsIt2;
      (void)++cljIt2; (void)++rhsIt2;
    }

    (void)++cljIt1; (void)++rhsIt1;
  }

//std::cerr << "clj:" << std::endl;
//clj.print( std::cerr );
//std::cerr << std::endl << std::endl;
return clj;
}

EnhancedMatrix EnhancedMatrix::transpose( void ) const {
	EnhancedMatrix ret( size2(), size1() );
	const_iterator1 this_cir1 = begin1();
	iterator2 ret_itc2 = ret.begin2();

	while( this_cir1 != end1() ) {
		const_iterator2 this_cic2 = this_cir1.begin();
		iterator1 ret_itr1 = ret_itc2.begin();

		while( this_cic2 != this_cir1.end() ) {
			*ret_itr1 = *this_cic2;
			(void)++this_cic2; (void)++ret_itr1;
		}
		(void)++this_cir1; (void)++ret_itc2;
	}

	return ret;
}