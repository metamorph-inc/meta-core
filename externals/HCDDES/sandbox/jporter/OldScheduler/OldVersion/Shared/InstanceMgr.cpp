
#include "InstanceMgr.h"

#include "UdmUtil.h"

InstanceMgr::InstanceMgr( Udm::DataNetwork* p_backend, TTSystem & sys ) :
	_cstrMgr( p_backend, sys )
{
    RootFolder rootObj = RootFolder::Cast(p_backend->GetRootObject());
    _sys = sys;
	_instModel = InstanceModel::Create( rootObj );	
	_instModel.name() = string(sys.name()) + "Instances";

    CopyProcHierarchy( p_backend );
    CopyBusHierarchy( p_backend );

    ReviewTasks();
    CreateTaskInstances();
    CreateMsgInstances();

    CreateConstraints();
}

InstanceMgr::~InstanceMgr()
{

}

void InstanceMgr::CopyProcHierarchy( Udm::DataNetwork* p_backend )
{
    /// Create Processors as model references by copy 
	set<Processor> procs = _sys.Processor_children();
    set<Processor>::iterator pProc;
    for ( pProc = procs.begin(); pProc != procs.end(); pProc++ )
    {
        // Make a copy of the processor object and everything below it
        Processor tp = Processor::Create( _instModel );
        UdmUtil::copy_assoc_map cam;
        UdmUtil::CopyObjectHierarchy( (*pProc).__impl(), tp.__impl(), p_backend, cam );

        // Create a task instances holder to keep everything straight
        TaskInstances ti = TaskInstances::Create(tp);
        ti.name() = string(tp.name()) + "Instances";
    }
}

void InstanceMgr::CopyBusHierarchy(Udm::DataNetwork * &p_backend)
{
    /// Create Buses by copy
    set< Bus > buses = _sys.Bus_kind_children();
    set< Bus >::iterator pBus;
    for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
    {
        Bus tb = Bus::Create(_instModel);
        UdmUtil::copy_assoc_map cam;
        UdmUtil::CopyObjectHierarchy( (*pBus).__impl(), tb.__impl(), p_backend, cam );

        // Create a msg instances holder to keep everything straight
        MsgInstances mi = MsgInstances::Create(tb);
        mi.name() = string(tb.name()) + "Instances";

        /// Within buses, create a dependency model
        Dependencies dp = Dependencies::Create( tb );
        dp.name() = string(tb.name()) + "Dependencies";
    }
}

void InstanceMgr::ReviewTasks()
{
    /// Adapt the instance generation code from the original version
    ///    Start with the LCM determination of the hyperperiod
    ///    Gather all of the task references from the processors
    set<Processor> newprocs = _instModel.Processor_kind_children();
	set<Processor>::iterator pProc;
    int global_period = 1;
    for ( pProc = newprocs.begin(); pProc != newprocs.end(); pProc++ )
    {
        set< Task > tasks = pProc->Task_kind_children();
        set< TaskInstances > taskinst = (pProc)->TaskInstances_kind_children();

        set< TaskInstances >::iterator pti = taskinst.begin();  // Just get it!
        set< Task >::iterator pTsk;
        for ( pTsk = tasks.begin(); pTsk != tasks.end(); pTsk++ )
        {
            _idxMgr.MapTasktoTaskInstances( *pTsk, *pti );
			global_period = MiscUtil::LCM( global_period, (int) pTsk->Period() ); 
        }
    } // loop over procs

    /// Set the hyperperiod in the instance model
    _instModel.HyperPeriod() = global_period;
}


void InstanceMgr::CreateTaskInstances()
{
    /// Create the actual instances from the specs and the hyperperiod length
    // iterate over the Tasks, use the associated task inst list
    // from the taskmap, and create the new instances in the inst list
    
    int task_instance_counter = 0; // Keep instance indices
	int global_period = (int) _instModel.HyperPeriod();
    std::map< Task , TaskInstances >::iterator pTskPr;
    std::vector< string > tasks;
	_idxMgr.GetAllTaskNames( tasks );
    std::vector< string >::iterator pTsk;
    //for ( pTskPr = taskmap.begin(); pTskPr != taskmap.end(); pTskPr++ )
    for ( pTsk = tasks.begin(); pTsk != tasks.end(); pTsk++ )
    {
        Task t = _idxMgr.GetTaskbyName( *pTsk );
        TaskInstances tis = _idxMgr.GetTaskInstancesObjectforTask( *pTsk );

        string oldname;
        // Go over the right # of repetitions and create instances
        for ( int ridx = 0; ridx < (global_period / t.Period()); ridx++ )
        {
            TaskInstance ti = TaskInstance::Create( tis );
            ostringstream namestr;
            namestr << t.name();
            ti.RefName() = t.name(); // namestr.str();
            //namestr << "_" << task_instance_counter;
            ti.name() = string(t.name()) + "_" + MiscUtil::itos(task_instance_counter); // namestr.str();
            ti.InstID() = task_instance_counter++;
            ti.StartTime() = 0;

            // Save the info to make constraint generation easier
            if ( ridx > 0 )
            {
                // Save order and period
                //pdlist[ oldname ] = make_pair( namestr.str(), (int) t.Period() );
                _idxMgr.StoreExactInstanceDistance( oldname, string(ti.name()), (int) t.Period() );
            }
            _idxMgr.StoreInstanceforTask( t, ti );
            
            oldname = ti.name();

            // Also for cumulatives
            Processor p = Processor::Cast( t.parent() );
            _idxMgr.StoreInstanceforProc( p, ti );
        }
    }
}


void InstanceMgr::CreateMsgInstances()
{
    /// Create message instances
    int msg_instance_counter = 0;
    set< Bus > newbuses = _instModel.Bus_kind_children();
	set< Bus >::iterator pBus;

    for ( pBus = newbuses.begin(); pBus != newbuses.end(); pBus++ )
    {
        // We need to have MsgInstances and Dependencies available here, to keep the msg instances
        // and their relationships
        set< MsgInstances > smis = pBus->MsgInstances_kind_children();
        MsgInstances mis( *smis.begin() );
        set< Dependencies > sdeps = pBus->Dependencies_kind_children();
        Dependencies deps( *sdeps.begin() );

        set< Message > msgs = pBus->Message_kind_children();
        set< Message >::iterator pMsg;
        for ( pMsg = msgs.begin(); pMsg != msgs.end(); pMsg++ )
        {
			std::cout << "For message " << pMsg->name() << " ";
            // Get sender/receiver task objects -- only one sender for this msg, 
            // but possibly multiple receivers
            Sender sndr = pMsg->srcSender();

            set< Receiver > rcvrs = pMsg->dstReceiver();
            set< Receiver >::iterator pRcvr;

            TaskRef sndTaskRef = sndr.srcSender_end();
            Task sndTask = sndTaskRef.ref();

            // Before 4/18/2007:
            // Collect up the receiver periods -- if any period is less than the sending period,
            // then always send a message (msg period = sending period)
            // If all periods are less than the sending period, use the minimum period as
            // the message period (msg period = min rcvr period)

            // Get the instance lists associated with each sender/receiver pair
            // This is complicated -- if the receivers all have the same rate, then we're good
            // to go.  Otherwise, we have to do something interesting:
            //
            // Here's one way to do it:
            // Create an empty vector of the same size as the sending instance list (time vector)
            // For each reciever:
            //    Figure out the relationship between receiver rate and sender rate
            //    Create the proper # of instances of the message, putting them in the right places in
            //       the "time" vector
            // Then we can use this vector to create all of the constraints

            // If we're selecting receiver instances going to a task with a shorter period, we may
            // want to select which task in the sender period to send to.  If one sender instance
            // sends to the first task in the receiver period, then all of them do, or else we
            // introduce jitter.  The trick is modeling this in the constraints.  How can we get
            // the sender/receiver relationship to try all of the possible offsets without ending
            // up with weak propagation?
            // FOR NOW: Hook them all up to the first following instance, and hope it works out :-)

            // This leads me to believe that the "send and forget" model of Schild/Wurst is 
            // actually the most flexible, if not the most efficient.  Here's a possible variant:
            // Figure out the maximum rate of the recievers -- determine a decimation value for
            // the sender only if the maximum rate of receivers is less than the sending rate.
            // Otherwise, sending all of the time is the best you can do.  Then we determine the
            // actual relationship between the instances using the latency constraints.

            vector< TaskInstance > SndInst;
            vector< TaskInstance >::iterator pSndInst;
            
            if ( _idxMgr.GetInstancesforTask( sndTask.name(), SndInst ) )
            {
				std::cout << "Sender " << sndTask.name() << " with " << SndInst.size() << " instances rcvd by:" << std::endl;
                // Sender-indexed vector of receiver instances
                vector< set< TaskInstance > > timeVec( SndInst.size() ); 

                // Go through receiver list for this message
                for ( pRcvr = rcvrs.begin(); pRcvr != rcvrs.end(); pRcvr++ )
                {
                    TaskRef rcvrTaskRef = pRcvr->dstReceiver_end();
                    Task rcvrTask = rcvrTaskRef.ref();
  
					std::cout << rcvrTask.name() << " ";
                    // Retrieve the task instance list for this message
                    vector < TaskInstance > RcvInst;
                    vector < TaskInstance >::iterator pRcvInst;

                    if ( _idxMgr.GetInstancesforTask( rcvrTask.name(), RcvInst ) )
                    {
                        // Get the step relationship
                        int sendPd = (int) sndTask.Period();
                        int rcvrPd = (int) rcvrTask.Period();

                        int sendstep, rcvrstep;
                        if ( sendPd >= rcvrPd )
                        {
                            sendstep = 1;
                            rcvrstep = sendPd / rcvrPd; 
                        } else
                        {
                            sendstep = rcvrPd / sendPd;
                            rcvrstep = 1;
                        }

                        // Map the instances onto the time vector, scaled appropriately
                        int sendidx = 0, rcvridx = 0;
                        for ( pRcvInst = RcvInst.begin(); pRcvInst != RcvInst.end(); pRcvInst++ )
                        {
							std::cout << " inst " << pRcvInst->name() << " mapped to idx " << sendidx << std::endl;
                            // We're decimating the receivers
                            if ( rcvridx % rcvrstep == 0 )
                            {
                                timeVec[ sendidx % SndInst.size() ].insert( *pRcvInst );
                                rcvrstep++;
                            }

                            sendidx += sendstep;
                        }
                    }
                }

                // Iterate over the time vector to build the relationships
                vector< set< TaskInstance > >::iterator pTime;
                for ( pSndInst = SndInst.begin(), pTime = timeVec.begin(); 
                    pTime != timeVec.end(); pTime++, pSndInst++ )
                {
                    if ( pTime->size() > 0 )
                    {
                        MsgInstance minst = MsgInstance::Create( mis );

                        //ostringstream namestr;
                        //namestr << pMsg->name();
                        minst.RefName() = pMsg->name();

                        //namestr << "_" << msg_instance_counter;
                        minst.name() = string(pMsg->name()) + "_" + MiscUtil::itos(msg_instance_counter);
                        minst.InstID() = msg_instance_counter++;
                        minst.StartTime() = 0;
                        _idxMgr.StoreInstanceforMsg( *pMsg, minst );
                        _idxMgr.StoreInstanceforBus( *pBus, minst );

                        TaskInstRef tinstref = TaskInstRef::Create( deps );
                        SenderInst sinst = SenderInst::Create( deps );
                        MsgInstRef minstref = MsgInstRef::Create( deps );

                        tinstref.name() = pSndInst->name();
                        tinstref.ref() = *pSndInst; // Point at the sending task instance
                        minstref.name() = minst.name();
                        minstref.ref() = minst; // Point at the msg instance
                        sinst.srcSenderInst_end() = tinstref; // Hook up the src end
                        sinst.dstSenderInst_end() = minstref; // Hook up the dst end

                        // Iterate over the receivers in the set
                        set< TaskInstance >::iterator pRcvrInst;
                        for ( pRcvrInst = pTime->begin(); pRcvrInst != pTime->end(); pRcvrInst++ )
                        {
                            TaskInstRef tinstref2 = TaskInstRef::Create( deps );
                            ReceiverInst rinst = ReceiverInst::Create( deps );

                            tinstref2.name() = pRcvrInst->name();
                            tinstref2.ref() = *pRcvrInst; // Point at the receiving task instance
                            rinst.srcReceiverInst_end() = minstref;  // Hook up the src end
                            rinst.dstReceiverInst_end() = tinstref2; // Hook up the dst end
                        }
                    }
                }
            } 
        }
    }
}

void InstanceMgr::CreateConstraints()
{

	_cstrMgr.SetInstanceModelRef( _instModel );
	int global_period = (int) _instModel.HyperPeriod();

    // Then walk through the different structures,
    //   Do processors first -- one variable per task instance
	set< Processor > newprocs = _instModel.Processor_children();
	set< Processor >::iterator pProc;
    for ( pProc = newprocs.begin(); pProc != newprocs.end(); pProc++ )
    {
        set< Task > tasks = pProc->Task_kind_children();
        set< Task >::iterator pTsk;

        for ( pTsk = tasks.begin(); pTsk != tasks.end(); pTsk++ )
        {
            std::vector< string > taskInstances;
            _idxMgr.GetTaskInstanceNames( pTsk->name(), taskInstances );
            vector< string >::iterator pTskName;
            for( pTskName = taskInstances.begin(); pTskName != taskInstances.end(); pTskName++ )
            {
				_cstrMgr.AddTaskInstance( *pTskName, (int) pTsk->WCExecTime(), global_period ); 
            }
        }

        // Create constraints for pairs of task instances to reflect period relationships
		vector< string > procInstances;
		//_idxMgr.GetAllTaskInstanceNames( allInstances );
		_idxMgr.GetProcInstanceNames( pProc->name(), procInstances );
        vector< string >::iterator pInstance;
        //for ( pInstance = allInstances.begin(); pInstance != allInstances.end(); pInstance++ )
		for ( pInstance = procInstances.begin(); pInstance != procInstances.end(); pInstance++ )
        {   
            int dist;
			std::string nextInst;
            if ( _idxMgr.GetNextInstanceAndDist( *pInstance, nextInst, dist ) )
            {
                _cstrMgr.AddEqualityConstraint( *pInstance, nextInst, dist );
            }
        }

        // Now for the cumulatives (serial constraints)
        //vector< string > procInstances;
		// _idxMgr.GetProcInstanceNames( pProc->name(), procInstances );
        if ( procInstances.size() >= 2 )  // If # tasks < 2, don't even bother
        {
            _cstrMgr.AddSerialConstraint( "processor " + string(pProc->name()), procInstances );
        }
    }

	set< Bus > newbuses = _instModel.Bus_children();
	set< Bus >::iterator pBus;

    //   Do buses second -- one variable per msg instance, serialize bus, 
    //   order message instances, and then build latencies
    map< string, FDVar > msg_inst_varmap;
    for ( pBus = newbuses.begin(); pBus != newbuses.end(); pBus++ )
    {
        set< Message > msgs = pBus->Message_kind_children();
        set< Message >::iterator pMsg;

        // Create one variable for each message instance
        for ( pMsg = msgs.begin(); pMsg != msgs.end(); pMsg++ )
        {
            vector<string> msgInstances;
            _idxMgr.GetMsgInstanceNames( pMsg->name(), msgInstances );
            vector< string >::iterator pMsgName;
            for ( pMsgName = msgInstances.begin();
                  pMsgName != msgInstances.end();
                  pMsgName++ )
            {
                _cstrMgr.AddMsgInstance( *pMsgName, (int) pMsg->MsgLength(), global_period );
            }
        }

        //set< MsgInstances > smis = pBus->MsgInstances_kind_children();
        //MsgInstances mis( *smis.begin() );
        //set< Dependencies > sdeps = pBus->Dependencies_kind_children();
        //Dependencies deps( *sdeps.begin() );

        ////set< MsgInstance > mi = mis.MsgInstance_kind_children();
        ////set< MsgInstance >::iterator pMI;

        //// We need several different constraints -- 
        //// we need the sender pair constraint (sender inst1 -> msg -> sender inst2): 
        ////   we can get that from the dependencies and the earlier map of task instance pairs
        //set< SenderInst > csndrs = deps.SenderInst_kind_children();
        //set< SenderInst >::iterator pCSndr;
        //for ( pCSndr = csndrs.begin(); pCSndr != csndrs.end(); pCSndr++ )
        //{
        //	TaskInstRef sndref = pCSndr->srcSenderInst_end(); // Get the actual sender instance
        //	TaskInstance sndr = sndref.ref();

        //	// Don't create the constraints if this is the last task in the period
        //	if ( pdlist.find( sndr.name() ) != pdlist.end() )
        //	{
        //		MsgInstRef msgref = pCSndr->dstSenderInst_end(); // Get the message instance
        //		MsgInstance msginst = msgref.ref();

        //		InequalityConstraint pre_ieq = InequalityConstraint::Create( cstr );
        //		InequalityConstraint post_ieq = InequalityConstraint::Create( cstr );
        //		ostringstream pre_ieqcname, post_ieqcname;

        //		// We do NOT assume that messages are strictly periodic within the hyperperiod, rather they can 
        //		// be shuffled around to meet the constraints (referring to the paper)
        //		
        //		// First, the sending task has to end before the message begins
        //		XPort pre_x = XPort::Create( pre_ieq );
        //		YPort pre_y = YPort::Create( pre_ieq );
        //		Member pre_xmem = Member::Create( cstr );
        //		Member pre_ymem = Member::Create( cstr );

        //		
        //		pre_xmem.srcMember_end() = tsk_inst_varmap[ sndr.name() ];
        //		pre_xmem.dstMember_end() = pre_x;
        //		pre_ymem.srcMember_end() = msg_inst_varmap[ msginst.name() ];
        //		pre_ymem.dstMember_end() = pre_y;
        //		Task sndrtask = inst_tsk_map[ sndr.name() ];
        //		pre_ieq.Offset() = -( sndrtask.WCExecTime() );
        //		pre_ieqcname << sndr.name() << " <= " << msginst.name() << " + " << pre_ieq.Offset();
        //		pre_ieq.name() = pre_ieqcname.str();

        //		// Second, the message has to end before the next sender begins
        //		XPort post_x = XPort::Create( post_ieq );
        //		YPort post_y = YPort::Create( post_ieq );
        //		Member post_xmem = Member::Create( cstr );
        //		Member post_ymem = Member::Create( cstr );

        //		post_xmem.srcMember_end() = msg_inst_varmap[ msginst.name() ];
        //		post_xmem.dstMember_end() = post_y;
        //		post_ymem.srcMember_end() = tsk_inst_varmap[ pdlist[ sndr.name() ].first ]; // Use the next successive task
        //		post_ymem.dstMember_end() = post_x;
        //		Message instmsg = inst_msg_map[ msginst.name() ];
        //		post_ieq.Offset() = -( instmsg.MsgLength() );
        //		post_ieqcname << msginst.name() << " <= " << pdlist[ sndr.name() ].first << " + " << post_ieq.Offset();
        //		post_ieq.name() = post_ieqcname.str();

        //	}
        //}

        //// Order the messages with their receivers
        //set< ReceiverInst > crcvrs = deps.ReceiverInst_children();
        //set< ReceiverInst >::iterator pCRcvr;
        //for ( pCRcvr = crcvrs.begin(); pCRcvr != crcvrs.end(); pCRcvr++ )
        //{
        //	TaskInstRef rcvref = pCRcvr->dstReceiverInst_end(); // Get the actual sender instance
        //	TaskInstance rcvr = rcvref.ref();

        //	// Don't create the constraints if this is the last task in the period
        //	if ( pdlist.find( rcvr.name() ) != pdlist.end() )
        //	{
        //		MsgInstRef msgref = pCRcvr->srcReceiverInst_end(); // Get the message instance
        //		MsgInstance msginst = msgref.ref();

        //		// Model the dependency on the receiver
        //		InequalityConstraint rcv_ieq = InequalityConstraint::Create( cstr );
        //		ostringstream rcv_ieqcname;

        //		XPort rcv_x = XPort::Create( rcv_ieq );
        //		YPort rcv_y = YPort::Create( rcv_ieq );
        //		Member rcv_xmem = Member::Create( cstr );
        //		Member rcv_ymem = Member::Create( cstr );

        //		rcv_xmem.srcMember_end() = msg_inst_varmap[ msginst.name() ];
        //		rcv_xmem.dstMember_end() = rcv_x;
        //		rcv_ymem.srcMember_end() = tsk_inst_varmap[ rcvr.name() ]; 
        //		rcv_ymem.dstMember_end() = rcv_y;
        //		Message instmsg = inst_msg_map[ msginst.name() ];
        //		rcv_ieq.Offset() = -( instmsg.MsgLength() );

        //		rcv_ieqcname << msginst.name() << " <= " << rcvr.name() << " + " << rcv_ieq.Offset();
        //		rcv_ieq.name() = rcv_ieqcname.str();
        //	}
        //}
        
        // What about the latencies?
        // we need the receiver latency constraints
        //   this will have to come from the dependencies that were created -- where will we get latency values?

        // we need the bus cumulative constraint 
        //   just like the processor cumulative constraint
        std::vector< string > busInstances;
        _idxMgr.GetBusInstanceNames( pBus->name(), busInstances );
        if ( busInstances.size() >= 2 ) // If # msg inst < 2, don't do it
        {
            _cstrMgr.AddSerialConstraint( "bus: " + string(pBus->name()), busInstances );
        }

        // Now we need cumulatives for each sender/receiver pair (set), and ordering for the messages
        multimap< string, Task > msg_associates;  // Msg name -> all senders and receivers for that msg
        set< Sender > sndrs = pBus->Sender_children();
        set< Sender >::iterator pSndr;
        for ( pSndr = sndrs.begin(); pSndr != sndrs.end(); pSndr++ )
        {
            // Look up instances for both ends
            TaskRef tr = pSndr->srcSender_end();
            Message m = pSndr->dstSender_end();
            Task t = tr.ref();
            msg_associates.insert( make_pair( m.name(), t ) );

            // Now let's make constraints
            /*SerialConstraint sc = SerialConstraint::Create( cstr );
            //ostringstream scname;
            //scname << t.name() << " sends msg " << m.name();
            //sc.name() = scname.str();
            sc.name() = t.name() + " sends msg " + m.name(); */

            // Hook up senders to serial constraint
            
            vector< string > taskInstances;
            _idxMgr.GetTaskInstanceNames( t.name(), taskInstances );
            /*vector< string >::iterator pTI;
            for ( pTI = taskInstances.begin(); pTI != taskInstances.end(); pTI++ )
            {
                Member mem = Member::Create( cstr );
                mem.srcMember_end() = tsk_inst_varmap[ *pTI ];
                mem.dstMember_end() = sc;
            }*/

            // Hook up messages to serial constraint
            vector< string > msgInstances;
            _idxMgr.GetMsgInstanceNames( m.name(), msgInstances );
            /*vector< string >::iterator pMI;
            for ( pMI = msgInstances.begin(); pMI != msgInstances.end(); pMI++ )
            {
                Member mem = Member::Create( cstr );
                mem.srcMember_end() = msg_inst_varmap[ *pMI ];
                mem.dstMember_end() = sc;
            }*/

            // Append one list to the other
            taskInstances.insert( taskInstances.end(), msgInstances.begin(), msgInstances.end() );
            _cstrMgr.AddSerialConstraint( string(t.name()) + " sending msg " + string(m.name()), taskInstances );
        }

        // Now we need cumulatives for each sender/receiver pair (set), and ordering for the messages
        set< Receiver > rcvrs = pBus->Receiver_children();
        set< Receiver >::iterator pRcvr;
        for ( pRcvr = rcvrs.begin(); pRcvr != rcvrs.end(); pRcvr++ )
        {
            // Look up instances for both ends
            Message m = pRcvr->srcReceiver_end();
            TaskRef tr = pRcvr->dstReceiver_end();
            Task t = tr.ref();
            msg_associates.insert( make_pair( m.name(), t ) );

            // Now let's make constraints
            /* SerialConstraint sc = SerialConstraint::Create( cstr );
            ostringstream scname;
            scname << t.name() << " receives msg " << m.name();
            sc.name() = scname.str(); */

            // Hook up Receivers to serial constraint
            vector< string > taskInstances;
            _idxMgr.GetTaskInstanceNames( t.name(), taskInstances );
            /* vector< string >::iterator pTI;
            for ( pTI = taskInstances.begin(); pTI != taskInstances.end(); pTI++ )
            {
                Member mem = Member::Create( cstr );
                mem.srcMember_end() = tsk_inst_varmap[ *pTI ];
                mem.dstMember_end() = sc;
            } */

            // Hook up messages to serial constraint
            vector< string > msgInstances;
            _idxMgr.GetMsgInstanceNames( m.name(), msgInstances );
            /* vector< string >::iterator pMI;
            for ( pMI = msgInstances.begin(); pMI != msgInstances.end(); pMI++ )
            {
                Member mem = Member::Create( cstr );
                mem.srcMember_end() = msg_inst_varmap[ *pMI ];
                mem.dstMember_end() = sc;
            } */

            // Append one list to the other
            taskInstances.insert( taskInstances.end(), msgInstances.begin(), msgInstances.end() );
            _cstrMgr.AddSerialConstraint( string(t.name()) + " receiving msg " + string(m.name()), taskInstances );
        }

        // Get all Tasks connected to each message, find the maximum duration within those tasks
        // Create an order for the instances, with minimum distance of period - max task duration
        // Here period is defined by hyperperiod / # msg instances (round up for safety)
        for ( pMsg = msgs.begin(); pMsg != msgs.end(); pMsg++ )
        {
            multimap< string, Task >::iterator pTask;
            pair< multimap< string, Task >::iterator, multimap< string, Task >::iterator > range;
            range = msg_associates.equal_range( pMsg->name() );

            int max_duration = 0;
            for ( pTask = range.first; pTask != range.second; pTask++ )
            {
                Task t = pTask->second;
                max_duration = ( t.WCExecTime() > max_duration ) ? t.WCExecTime() : max_duration;
            }
            
			std::vector< string > msgInstances;
            _idxMgr.GetMsgInstanceNames( pMsg->name(), msgInstances );

            int msg_period = (int) (((double) global_period / (double) msgInstances.size() ) + 1.0);
            int min_spacing = msg_period - max_duration; // Relax the constraint a little bit, but not too much
            min_spacing = ( min_spacing < 0 ) ? 0 : min_spacing;  // Can't have any of this!

            
            vector< string >::iterator pMI = msgInstances.begin();
            vector< string >::iterator pPrev = pMI;
            for ( pMI++ ; pMI != msgInstances.end(); pMI++ ) // Start with the second instance!
            {
                _cstrMgr.AddInequalityConstraint( *pPrev, *pMI, -min_spacing );
                pPrev = pMI; // Keep the previous pointer
            }
            // Handle the wrap-around at the end of the schedule
            _cstrMgr.AddInequalityConstraint( *(msgInstances.end()-1), *(msgInstances.begin()), global_period-min_spacing );

        }        
    }
}

bool InstanceMgr::GetSolutions( const std::string & sysname )
{
    std::string stats;
    _cstrMgr.SolveConstraints(stats, 5000);
    std::vector< std::string > slns;
    _cstrMgr.GetSolutionNames( slns );

	if ( slns.size() == 0 )
	{
		return false;
	}

    std::vector< std::string >::iterator pSln;
	int count = 0;
    for ( pSln = slns.begin(); pSln != slns.end(); pSln++ )
    {
		if ( ++count == 4998 )
		{
			_cstrMgr.DrawSchedule( sysname, *pSln );
			_cstrMgr.WriteSchedule( sysname, *pSln );
		}
    }

	return true;
}




