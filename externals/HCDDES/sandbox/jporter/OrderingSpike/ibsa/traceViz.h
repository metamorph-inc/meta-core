#ifndef __TRACEVIZ_H__
#define __TRACEVIZ_H__

/*** Included Header Files ***/
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctemplate/template.h>


/*** Class Predefines ***/
class TVSchedulable;
class TVTrace;
class TVSuperblock;
class TraceViz;


/*** Locally Defined Values ***/
enum TraceType {
	Task,
	Message
};


/***************************************************************************************/


typedef struct TVInstance {
	TVTrace			*trace;
	TVSchedulable	*schedulable;
	unsigned int	period;
	double			start;
	double			duration;
	TVTrace			*preDep, *postDep;
} TVInstance;


class TVSchedulable {
private:
	static unsigned int						_colorID;
	TraceType								_type;
	std::string								_name;
	std::string								_color;
	std::list< TVInstance* >				_instances;

	TVSchedulable();
public:
	// Constructors and Destructors
	TVSchedulable( const TraceType &type, const std::string &name, 
				   const bool inKey, const std::string &color="" );
	~TVSchedulable();

	// General Access Methods
	TVInstance* AddInstance( TVTrace* trace, const unsigned int &period, const double &start, const double &duration );
	TVInstance* AddInstance( TVTrace* trace, const double &start, const double &duration );
	void AddDependenceArrow( TVInstance* instance, TVTrace* preDependency, TVTrace* postDependency );
	const std::string Color(void)			{ return this->_color; }
};


/***************************************************************************************/


class TVTrace {
private:
	unsigned int							_order;
	std::string								_name;
	unsigned int							_currentPeriod;
	unsigned int							_maxPeriod;
	unsigned int							_vertPosition;
	std::vector< std::map< double, TVInstance* > >	_instances;

	TVTrace();
	TVTrace( const unsigned int &order, const std::string &name );
	unsigned int AddInstance( const unsigned int &period, TVInstance *instance );
	unsigned int AddInstance( TVInstance *instance );
	unsigned int Render( ctemplate::TemplateDictionary *dictionary, unsigned int &x,
						 unsigned int &y, const unsigned int &width, const double &hyperperiod );
	friend class TVSuperblock;
	friend class TVSchedulable;
public:
	// General Access Methods
};


/***************************************************************************************/


class TVSuperblock {
private:
	unsigned int							_order;
	std::string								_name;
	double									_hyperperiodSec;
	std::list< TVTrace* >					_traces;

	TVSuperblock();
	TVSuperblock( const unsigned int &order, const std::string &name, const double &hyperperiod );
	unsigned int Render( ctemplate::TemplateDictionary *dictionary, unsigned int &x,
						 unsigned int &y, const double &ratio );
	friend class TraceViz;
public:
	// Constructors and Destructors
	~TVSuperblock();

	// General Access Methods
	TVTrace* AddTrace( const std::string &name );
};


/***************************************************************************************/


class TraceViz {
private:
	std::string							_name;
	std::string							_filename;
	std::string							_templateFile;
	std::list< TVSuperblock* >			_superblocks;
	double								_maxHyperperiod;
	std::map< std::string,std::string >	_key;

	TraceViz();
	void AddKey( const std::string &name, const std::string &color );
	unsigned int RenderKey( ctemplate::TemplateDictionary* dictionary, 
							unsigned int &x, unsigned int &y );
	friend class TVSchedulable;
public:
	// Constructors and Destructors
	TraceViz( const std::string &traceName, const std::string &filename, const std::string &templateFile );
	~TraceViz();

	// General Access Methods
	TVSuperblock* AddSuperblock( const std::string &name, const double &hyperperiodSec );
	bool Render(void) ;
};



/***************************************************************************************/


#endif //__TRACEVIZ_H__

