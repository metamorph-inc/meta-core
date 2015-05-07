#include "BusTypes.h"

#include <string>
#include <iostream>

using namespace std;


void TBus::PrintOtherParameters()
{
	cout << " setup time " << getSetupTime();
}


void TTBus::PrintOtherParameters()
{
	cout << " setup time " << getSetupTime() << " slot time " << getSlotTime();
}

double TTBus::getMsgTransferTime( Bytes msgsize )
{
	// TDMA rounds and slots
	double msg_length = ( (double) msgsize ) / ( getDataRatebps() * 8.0 );
	double round_length = ( (double) getNumSlots() ) * getSlotTime();
	double rounds_to_send = msg_length / getSlotTime();
	double trans_length = rounds_to_send * round_length;
	return getSetupTime() + trans_length;
}


void AFDXBus::PrintOtherParameters()
{
	cout << " channel rate " << getChanRatebps() << " max channels " << getMaxChannels();
}

double AFDXBus::getMsgTransferTime( Bytes msgsize )
{
	double msg_length = ( (double) msgsize ) / ( getChanRatebps() * 8.0 );
	return getSetupTime() + msg_length;
}


void TTEBus::PrintOtherParameters()
{
	cout << " setup time " << getSetupTime() << " slot time " << getSlotTime();
}

double TTEBus::getMsgTransferTime( Bytes msgsize )
{
	// TDMA rounds and slots (for now assume traffic is TDMA until we extend the spec)
	double msg_length = ( (double) msgsize ) / ( getDataRatebps() * 8.0 );
	double round_length = ( (double) getNumSlots() ) * getSlotTime();
	double rounds_to_send = msg_length / getSlotTime();
	double trans_length = rounds_to_send * round_length;
	return getSetupTime() + trans_length;
}

