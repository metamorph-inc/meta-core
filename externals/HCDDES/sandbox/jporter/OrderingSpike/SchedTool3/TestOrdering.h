#ifndef TestOrdering_h__
#define TestOrdering_h__

#include "ExtractSubgraph.h"

#include <gecode/int.hh>
#include <gecode/search.hh>

class TestOrdering : public Gecode::Space {
protected:
	Gecode::IntVarArray l;
public:
	TestOrdering(const DepGraph & G, const DepVertex & firstvert, const DepVertex & lastvert);

	TestOrdering( bool share, TestOrdering & s) : Gecode::Space( share, s ) {
		l.update(*this, share, s.l);
	}

	virtual Gecode::Space* copy( bool share) {
		return new TestOrdering(share,*this);
	}

	void print() const {
		std::cout << l << std::endl;
	}
};

/*  Gecode example */
//try {
//
//	//TestOrdering * m = new TestOrdering(G,0,5);
//	TestOrdering * m = new TestOrdering(G,0,3);
//	Gecode::DFS<TestOrdering> e(m);
//	delete m;
//
//	while (TestOrdering * s = e.next()) {
//		s->print(); delete s;
//	}
//} catch (Gecode::Exception e) {
//	std::cerr << "Gecode exception: " << e.what() << std::endl;
//	return 1;
//}

#endif // TestOrdering_h__