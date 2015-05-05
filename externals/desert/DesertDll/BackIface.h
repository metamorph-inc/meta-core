
#ifndef BACKIFACE_H
#define BACKIFACE_H



namespace BackIfaceFunctions
{

	/*
		This is the interface between the DLL and the DesertTool
		in iverse direction
		DesertFinit() calls give back a void * to a DBConfigurations list
	*/

	struct DBConfiguration
	{
		long id;										//configuration id
		
		CList<long, long> assignments;					//valid assignments
														//the ID's of assignments
														//which are valid(present) in this configuration

		CMap<long, long, long, long> alt_assignments;   //alternative selections
	};
	typedef CList<DBConfiguration*, DBConfiguration*> DBConfigurations;

}

#endif BACKIFACE_H



