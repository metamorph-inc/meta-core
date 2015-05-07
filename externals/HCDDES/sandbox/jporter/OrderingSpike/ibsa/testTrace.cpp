#include <string>

#include "traceViz.h"

using namespace std;

int main()
{
	TraceViz tr( string( "test" ), string( "test.svg" ), string( "sched_viz.tpl" ) );

	TVSuperblock * tvsb = tr.AddSuperblock( string("n1"), 3.0 );
        TVTrace * trace = tvsb->AddTrace( "task1" );
	TVSchedulable * sched = new TVSchedulable( Task, "task1", true );
	sched->AddInstance( trace, 0.5, 0.1 );
	sched->AddInstance( trace, 1.5, 0.1 );
	sched->AddInstance( trace, 2.5, 0.1 );

	TVTrace * mtrace = tvsb->AddTrace( "msg1" );
	TVSchedulable * msched = new TVSchedulable( Message, "msg1", false );
	msched->AddInstance( mtrace, 0.2, 0.15 );
	msched->AddInstance( mtrace, 1.2, 0.15 );
	msched->AddInstance( mtrace, 2.2, 0.18 );
	
	TVSuperblock * tvsb2 = tr.AddSuperblock( string("b1"), 3.0 );
        TVTrace * trace3 = tvsb2->AddTrace( "msg2" );
	TVSchedulable * sched3 = new TVSchedulable( Message, "msg2", true );
	sched->AddInstance( trace3, 0.51, 0.1 );
	sched->AddInstance( trace3, 1.51, 0.1 );
	sched->AddInstance( trace3, 2.51, 0.1 );

	

	tr.Render();

	return 0;
}
