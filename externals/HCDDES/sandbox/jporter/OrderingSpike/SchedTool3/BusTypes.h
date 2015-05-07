#ifndef BusTypes_h__
#define BusTypes_h__

#include "NWModel.h"

#include <string>
#include <iostream>

class TBus;
class TTBus;
class TTEBus;
class EthBus;
class AFDXBus;

typedef boost::shared_ptr< TBus > TBusPtr;
typedef std::set< TBusPtr > TBusSet;
typedef boost::shared_ptr< TTBus > TTBusPtr;
typedef std::set< TTBusPtr > TTBusSet;
typedef boost::shared_ptr< AFDXBus > AFDXBusPtr;
typedef std::set< AFDXBusPtr > AFDXBusSet;
typedef boost::shared_ptr< EthBus > EthBusPtr;
typedef std::set< EthBusPtr > EthBusSet;
typedef boost::shared_ptr< TTEBus > TTEBusPtr;
typedef std::set< TTEBusPtr > TTEBusSet;

class TBus : public Bus
{
public:
	TBus() { }
	TBus( const std::string & name, ProblemPtr parent, 
			double rate, double setuptime ) : 
		Bus( name, parent, rate ), _setup_time( setuptime ) { }
	virtual ~TBus() { }

	double getSetupTime() { return _setup_time; }

protected:

	virtual void PrintOtherParameters();

	virtual const std::string BusType() { return std::string("TBus"); }

	double _setup_time;

private:
};

class TTBus : public Bus
{
public:
	TTBus() { }
	TTBus( const std::string & name, ProblemPtr parent, 
			double rate, double setuptime, double slottime ) : 
		Bus( name, parent, rate ), _setup_time( setuptime ), 
				_slot_time( slottime ) { }
	virtual ~TTBus() { }

	virtual unsigned long long getNumSlots() { return (unsigned long long) _Procs.size(); }
	virtual double getSlotTime() { return _setup_time; }
	virtual double getSetupTime() { return _slot_time; }
	virtual double getMsgTransferTime( Bytes msgsize );

protected:

	virtual void PrintOtherParameters();
	virtual const std::string BusType() { return std::string("TTBus"); }

	double _setup_time;
	double _slot_time;
	
private:
};

class AFDXBus : public Bus
{
public:
	AFDXBus() { }
	AFDXBus( const std::string & name, ProblemPtr parent, 
			double rate, double chanrate, unsigned long long numchans ) : 
		Bus( name, parent, rate ), _chan_rate( chanrate ), _max_chans( numchans ) { }
	virtual ~AFDXBus() { }

	unsigned long long getMaxChannels() { return _max_chans; }
	double getChanRatebps() { return _chan_rate; }

	virtual double getMsgTransferTime( Bytes msgsize );

protected:

	virtual void PrintOtherParameters();
	virtual const std::string BusType() { return std::string("AFDXBus"); }
	
	double _chan_rate;
	unsigned long long _max_chans;
	
private:
};

class EthBus : public Bus
{
public:
	EthBus() { }
	EthBus( const std::string & name, ProblemPtr parent, 
			double datarate ) : 
		Bus( name, parent, datarate ) { }
	virtual ~EthBus() { }

	virtual void Print() { std::cout << "EthBus " << Name() << std::endl; }

protected:

	virtual void PrintOtherParameters() { }
	virtual const std::string BusType() { return std::string("EthBus"); }
	
private:
};

class TTEBus : public Bus
{
public:
	TTEBus() { }
	TTEBus( const std::string & name, ProblemPtr parent, 
			double rate, double setuptime, double slottime ) : 
		Bus( name, parent, rate ), _setup_time( setuptime ), _slot_time( slottime ) { }
	virtual ~TTEBus() { }

	double getSetupTime() { return _setup_time; }
	double getSlotTime() { return _slot_time; }
	virtual double getMsgTransferTime( Bytes msgsize );
	virtual unsigned long long getNumSlots() { return (unsigned long long) _Procs.size(); }

protected:

	virtual void PrintOtherParameters();
	virtual const std::string BusType() { return std::string("TTEBus"); }

	double _setup_time;
	double _slot_time;

private:
};


#endif // BusTypes_h__