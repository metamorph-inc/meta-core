// cpp(meta datanetwork format) source file TTSched.cpp generated from diagram TTSched
// generated on Mon Apr 09 17:49:36 2007

#include "TTSched.h"
#include "UmlExt.h"

#include "UdmStatic.h"

namespace TTSched {

	::Uml::Diagram umldiagram;

		::Uml::Class ConstraintProblem::meta;
		::Uml::Class FDVar::meta;
		::Uml::Class Constraint::meta;
		::Uml::Class EqualityConstraint::meta;
		::Uml::Class InequalityConstraint::meta;
		::Uml::Class SerialConstraint::meta;
		::Uml::Class Member::meta;
		::Uml::Class RangeConstraint::meta;
		::Uml::Class YPort::meta;
		::Uml::Class XPort::meta;
		::Uml::Class PairConstraint::meta;
		::Uml::Class Solution::meta;
		::Uml::Class Connectable::meta;
		::Uml::Class Assignment::meta;
		::Uml::Class InstanceRef::meta;
		::Uml::Class SearchConfig::meta;
		::Uml::Class Task::meta;
		::Uml::Class StartMsg::meta;
		::Uml::Class LatencySpec::meta;
		::Uml::Class TTSystem::meta;
		::Uml::Class Sender::meta;
		::Uml::Class EndMsg::meta;
		::Uml::Class Receiver::meta;
		::Uml::Class Dependency::meta;
		::Uml::Class Link::meta;
		::Uml::Class Message::meta;
		::Uml::Class TimeSync::meta;
		::Uml::Class Latency::meta;
		::Uml::Class MsgRef::meta;
		::Uml::Class TaskRef::meta;
		::Uml::Class Processor::meta;
		::Uml::Class Bus::meta;
		::Uml::Class MsgInstances::meta;
		::Uml::Class SenderInst::meta;
		::Uml::Class Dependencies::meta;
		::Uml::Class MsgInstance::meta;
		::Uml::Class TaskInstRef::meta;
		::Uml::Class InstanceModel::meta;
		::Uml::Class MsgInstRef::meta;
		::Uml::Class TaskInstances::meta;
		::Uml::Class TaskInstance::meta;
		::Uml::Class ReceiverInst::meta;
		::Uml::Class RootFolder::meta;
		::Uml::Class MgaObject::meta;
		::Uml::Attribute ConstraintProblem::meta_UpperBound;
		::Uml::Attribute ConstraintProblem::meta_LowerBound;
		::Uml::Attribute FDVar::meta_UpperBound;
		::Uml::Attribute FDVar::meta_LowerBound;
		::Uml::Attribute FDVar::meta_Size;
		::Uml::Attribute EqualityConstraint::meta_Offset;
		::Uml::Attribute InequalityConstraint::meta_Offset;
		::Uml::Attribute RangeConstraint::meta_UpperBound;
		::Uml::Attribute RangeConstraint::meta_LowerBound;
		::Uml::Attribute Assignment::meta_Value;
		::Uml::Attribute SearchConfig::meta_MaxRunTimeSec;
		::Uml::Attribute SearchConfig::meta_MaxFailures;
		::Uml::Attribute SearchConfig::meta_SearchEngine;
		::Uml::Attribute SearchConfig::meta_MaxSolutions;
		::Uml::Attribute Task::meta_WCExecTime;
		::Uml::Attribute Task::meta_Period;
		::Uml::Attribute Message::meta_MsgLength;
		::Uml::Attribute TimeSync::meta_MsgLength;
		::Uml::Attribute TimeSync::meta_Period;
		::Uml::Attribute Latency::meta_MaxLength;
		::Uml::Attribute Bus::meta_Resolution;
		::Uml::Attribute MsgInstance::meta_RefName;
		::Uml::Attribute MsgInstance::meta_StartTime;
		::Uml::Attribute MsgInstance::meta_InstID;
		::Uml::Attribute InstanceModel::meta_HyperPeriod;
		::Uml::Attribute TaskInstance::meta_RefName;
		::Uml::Attribute TaskInstance::meta_StartTime;
		::Uml::Attribute TaskInstance::meta_InstID;
		::Uml::Attribute RootFolder::meta_name;
		::Uml::Attribute MgaObject::meta_position;
		::Uml::Attribute MgaObject::meta_name;
		::Uml::AssociationRole FDVar::meta_dstMember, FDVar::meta_dstMember_rev;
		::Uml::AssociationRole Member::meta_dstMember_end_;
		::Uml::AssociationRole Member::meta_srcMember_end_;
		::Uml::AssociationRole Connectable::meta_srcMember, Connectable::meta_srcMember_rev;
		::Uml::AssociationRole InstanceRef::meta_ref;
		::Uml::AssociationRole Task::meta_referedbyTaskRef;
		::Uml::AssociationRole StartMsg::meta_srcStartMsg_end_;
		::Uml::AssociationRole StartMsg::meta_dstStartMsg_end_;
		::Uml::AssociationRole Sender::meta_dstSender_end_;
		::Uml::AssociationRole Sender::meta_srcSender_end_;
		::Uml::AssociationRole EndMsg::meta_dstEndMsg_end_;
		::Uml::AssociationRole EndMsg::meta_srcEndMsg_end_;
		::Uml::AssociationRole Receiver::meta_srcReceiver_end_;
		::Uml::AssociationRole Receiver::meta_dstReceiver_end_;
		::Uml::AssociationRole Dependency::meta_srcDependency_end_;
		::Uml::AssociationRole Dependency::meta_dstDependency_end_;
		::Uml::AssociationRole Link::meta_dstLink_end_;
		::Uml::AssociationRole Link::meta_srcLink_end_;
		::Uml::AssociationRole Message::meta_referedbyMsgRef;
		::Uml::AssociationRole Message::meta_dstReceiver, Message::meta_dstReceiver_rev;
		::Uml::AssociationRole Message::meta_srcSender, Message::meta_srcSender_rev;
		::Uml::AssociationRole Latency::meta_srcEndMsg, Latency::meta_srcEndMsg_rev;
		::Uml::AssociationRole Latency::meta_dstStartMsg, Latency::meta_dstStartMsg_rev;
		::Uml::AssociationRole MsgRef::meta_ref;
		::Uml::AssociationRole MsgRef::meta_dstEndMsg, MsgRef::meta_dstEndMsg_rev;
		::Uml::AssociationRole MsgRef::meta_srcStartMsg, MsgRef::meta_srcStartMsg_rev;
		::Uml::AssociationRole MsgRef::meta_dstDependency, MsgRef::meta_dstDependency_rev;
		::Uml::AssociationRole MsgRef::meta_srcDependency, MsgRef::meta_srcDependency_rev;
		::Uml::AssociationRole TaskRef::meta_ref;
		::Uml::AssociationRole TaskRef::meta_srcReceiver, TaskRef::meta_srcReceiver_rev;
		::Uml::AssociationRole TaskRef::meta_dstSender, TaskRef::meta_dstSender_rev;
		::Uml::AssociationRole Processor::meta_dstLink, Processor::meta_dstLink_rev;
		::Uml::AssociationRole Bus::meta_srcLink, Bus::meta_srcLink_rev;
		::Uml::AssociationRole SenderInst::meta_srcSenderInst_end_;
		::Uml::AssociationRole SenderInst::meta_dstSenderInst_end_;
		::Uml::AssociationRole MsgInstance::meta_referedbyMsgInstRef;
		::Uml::AssociationRole TaskInstRef::meta_ref;
		::Uml::AssociationRole TaskInstRef::meta_dstSenderInst, TaskInstRef::meta_dstSenderInst_rev;
		::Uml::AssociationRole TaskInstRef::meta_srcReceiverInst, TaskInstRef::meta_srcReceiverInst_rev;
		::Uml::AssociationRole InstanceModel::meta_referedbyInstanceRef;
		::Uml::AssociationRole MsgInstRef::meta_ref;
		::Uml::AssociationRole MsgInstRef::meta_srcSenderInst, MsgInstRef::meta_srcSenderInst_rev;
		::Uml::AssociationRole MsgInstRef::meta_dstReceiverInst, MsgInstRef::meta_dstReceiverInst_rev;
		::Uml::AssociationRole TaskInstance::meta_referedbyTaskInstRef;
		::Uml::AssociationRole ReceiverInst::meta_dstReceiverInst_end_;
		::Uml::AssociationRole ReceiverInst::meta_srcReceiverInst_end_;
		::Uml::CompositionParentRole ConstraintProblem::meta_RootFolder_parent;
		::Uml::CompositionParentRole FDVar::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole Constraint::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole Member::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole YPort::meta_PairConstraint_parent;
		::Uml::CompositionParentRole XPort::meta_PairConstraint_parent;
		::Uml::CompositionParentRole Solution::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole Assignment::meta_Solution_parent;
		::Uml::CompositionParentRole InstanceRef::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole SearchConfig::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole Task::meta_Processor_parent;
		::Uml::CompositionParentRole StartMsg::meta_LatencySpec_parent;
		::Uml::CompositionParentRole LatencySpec::meta_TTSystem_parent;
		::Uml::CompositionParentRole TTSystem::meta_RootFolder_parent;
		::Uml::CompositionParentRole Sender::meta_Bus_parent;
		::Uml::CompositionParentRole EndMsg::meta_LatencySpec_parent;
		::Uml::CompositionParentRole Receiver::meta_Bus_parent;
		::Uml::CompositionParentRole Dependency::meta_LatencySpec_parent;
		::Uml::CompositionParentRole Link::meta_TTSystem_parent;
		::Uml::CompositionParentRole Message::meta_Bus_parent;
		::Uml::CompositionParentRole TimeSync::meta_Bus_parent;
		::Uml::CompositionParentRole Latency::meta_LatencySpec_parent;
		::Uml::CompositionParentRole MsgRef::meta_LatencySpec_parent;
		::Uml::CompositionParentRole MsgRef::meta_MsgInstances_parent;
		::Uml::CompositionParentRole TaskRef::meta_TaskInstances_parent;
		::Uml::CompositionParentRole TaskRef::meta_Bus_parent;
		::Uml::CompositionParentRole Processor::meta_TTSystem_parent;
		::Uml::CompositionParentRole Processor::meta_InstanceModel_parent;
		::Uml::CompositionParentRole Bus::meta_TTSystem_parent;
		::Uml::CompositionParentRole Bus::meta_InstanceModel_parent;
		::Uml::CompositionParentRole MsgInstances::meta_Bus_parent;
		::Uml::CompositionParentRole SenderInst::meta_Dependencies_parent;
		::Uml::CompositionParentRole Dependencies::meta_Bus_parent;
		::Uml::CompositionParentRole MsgInstance::meta_MsgInstances_parent;
		::Uml::CompositionParentRole TaskInstRef::meta_Dependencies_parent;
		::Uml::CompositionParentRole InstanceModel::meta_RootFolder_parent;
		::Uml::CompositionParentRole MsgInstRef::meta_Dependencies_parent;
		::Uml::CompositionParentRole TaskInstances::meta_Processor_parent;
		::Uml::CompositionParentRole TaskInstance::meta_TaskInstances_parent;
		::Uml::CompositionParentRole ReceiverInst::meta_Dependencies_parent;
		::Uml::CompositionParentRole RootFolder::meta_RootFolder_parent;
		::Uml::CompositionChildRole ConstraintProblem::meta_InstanceRef_child;
		::Uml::CompositionChildRole ConstraintProblem::meta_Member_children;
		::Uml::CompositionChildRole ConstraintProblem::meta_SearchConfig_child;
		::Uml::CompositionChildRole ConstraintProblem::meta_Constraint_children;
		::Uml::CompositionChildRole ConstraintProblem::meta_FDVar_children;
		::Uml::CompositionChildRole ConstraintProblem::meta_Solution_children;
		::Uml::CompositionChildRole PairConstraint::meta_YPort_children;
		::Uml::CompositionChildRole PairConstraint::meta_XPort_children;
		::Uml::CompositionChildRole Solution::meta_Assignment_children;
		::Uml::CompositionChildRole LatencySpec::meta_Latency_children;
		::Uml::CompositionChildRole LatencySpec::meta_MsgRef_children;
		::Uml::CompositionChildRole LatencySpec::meta_StartMsg_children;
		::Uml::CompositionChildRole LatencySpec::meta_EndMsg_children;
		::Uml::CompositionChildRole LatencySpec::meta_Dependency_children;
		::Uml::CompositionChildRole TTSystem::meta_Bus_children;
		::Uml::CompositionChildRole TTSystem::meta_LatencySpec_children;
		::Uml::CompositionChildRole TTSystem::meta_Processor_children;
		::Uml::CompositionChildRole TTSystem::meta_Link_children;
		::Uml::CompositionChildRole Processor::meta_TaskInstances_child;
		::Uml::CompositionChildRole Processor::meta_Task_children;
		::Uml::CompositionChildRole Bus::meta_TaskRef_children;
		::Uml::CompositionChildRole Bus::meta_Receiver_children;
		::Uml::CompositionChildRole Bus::meta_Message_children;
		::Uml::CompositionChildRole Bus::meta_TimeSync_child;
		::Uml::CompositionChildRole Bus::meta_Sender_children;
		::Uml::CompositionChildRole Bus::meta_MsgInstances_child;
		::Uml::CompositionChildRole Bus::meta_Dependencies_children;
		::Uml::CompositionChildRole MsgInstances::meta_MsgRef_children;
		::Uml::CompositionChildRole MsgInstances::meta_MsgInstance_children;
		::Uml::CompositionChildRole Dependencies::meta_SenderInst_children;
		::Uml::CompositionChildRole Dependencies::meta_ReceiverInst_children;
		::Uml::CompositionChildRole Dependencies::meta_MsgInstRef_children;
		::Uml::CompositionChildRole Dependencies::meta_TaskInstRef_children;
		::Uml::CompositionChildRole InstanceModel::meta_Bus_children;
		::Uml::CompositionChildRole InstanceModel::meta_Processor_children;
		::Uml::CompositionChildRole TaskInstances::meta_TaskRef_children;
		::Uml::CompositionChildRole TaskInstances::meta_TaskInstance_children;
		::Uml::CompositionChildRole RootFolder::meta_TTSystem_children;
		::Uml::CompositionChildRole RootFolder::meta_ConstraintProblem_children;
		::Uml::CompositionChildRole RootFolder::meta_InstanceModel_children;
		::Uml::CompositionChildRole RootFolder::meta_RootFolder_children;
		void Creates()
		{
			ConstraintProblem::meta = ::Uml::Class::Create(umldiagram);
			FDVar::meta = ::Uml::Class::Create(umldiagram);
			Constraint::meta = ::Uml::Class::Create(umldiagram);
			EqualityConstraint::meta = ::Uml::Class::Create(umldiagram);
			InequalityConstraint::meta = ::Uml::Class::Create(umldiagram);
			SerialConstraint::meta = ::Uml::Class::Create(umldiagram);
			Member::meta = ::Uml::Class::Create(umldiagram);
			RangeConstraint::meta = ::Uml::Class::Create(umldiagram);
			YPort::meta = ::Uml::Class::Create(umldiagram);
			XPort::meta = ::Uml::Class::Create(umldiagram);
			PairConstraint::meta = ::Uml::Class::Create(umldiagram);
			Solution::meta = ::Uml::Class::Create(umldiagram);
			Connectable::meta = ::Uml::Class::Create(umldiagram);
			Assignment::meta = ::Uml::Class::Create(umldiagram);
			InstanceRef::meta = ::Uml::Class::Create(umldiagram);
			SearchConfig::meta = ::Uml::Class::Create(umldiagram);
			Task::meta = ::Uml::Class::Create(umldiagram);
			StartMsg::meta = ::Uml::Class::Create(umldiagram);
			LatencySpec::meta = ::Uml::Class::Create(umldiagram);
			TTSystem::meta = ::Uml::Class::Create(umldiagram);
			Sender::meta = ::Uml::Class::Create(umldiagram);
			EndMsg::meta = ::Uml::Class::Create(umldiagram);
			Receiver::meta = ::Uml::Class::Create(umldiagram);
			Dependency::meta = ::Uml::Class::Create(umldiagram);
			Link::meta = ::Uml::Class::Create(umldiagram);
			Message::meta = ::Uml::Class::Create(umldiagram);
			TimeSync::meta = ::Uml::Class::Create(umldiagram);
			Latency::meta = ::Uml::Class::Create(umldiagram);
			MsgRef::meta = ::Uml::Class::Create(umldiagram);
			TaskRef::meta = ::Uml::Class::Create(umldiagram);
			Processor::meta = ::Uml::Class::Create(umldiagram);
			Bus::meta = ::Uml::Class::Create(umldiagram);
			MsgInstances::meta = ::Uml::Class::Create(umldiagram);
			SenderInst::meta = ::Uml::Class::Create(umldiagram);
			Dependencies::meta = ::Uml::Class::Create(umldiagram);
			MsgInstance::meta = ::Uml::Class::Create(umldiagram);
			TaskInstRef::meta = ::Uml::Class::Create(umldiagram);
			InstanceModel::meta = ::Uml::Class::Create(umldiagram);
			MsgInstRef::meta = ::Uml::Class::Create(umldiagram);
			TaskInstances::meta = ::Uml::Class::Create(umldiagram);
			TaskInstance::meta = ::Uml::Class::Create(umldiagram);
			ReceiverInst::meta = ::Uml::Class::Create(umldiagram);
			RootFolder::meta = ::Uml::Class::Create(umldiagram);
			MgaObject::meta = ::Uml::Class::Create(umldiagram);
			ConstraintProblem::meta_UpperBound = ::Uml::Attribute::Create(ConstraintProblem::meta);
			ConstraintProblem::meta_LowerBound = ::Uml::Attribute::Create(ConstraintProblem::meta);
			FDVar::meta_UpperBound = ::Uml::Attribute::Create(FDVar::meta);
			FDVar::meta_LowerBound = ::Uml::Attribute::Create(FDVar::meta);
			FDVar::meta_Size = ::Uml::Attribute::Create(FDVar::meta);
			EqualityConstraint::meta_Offset = ::Uml::Attribute::Create(EqualityConstraint::meta);
			InequalityConstraint::meta_Offset = ::Uml::Attribute::Create(InequalityConstraint::meta);
			RangeConstraint::meta_UpperBound = ::Uml::Attribute::Create(RangeConstraint::meta);
			RangeConstraint::meta_LowerBound = ::Uml::Attribute::Create(RangeConstraint::meta);
			Assignment::meta_Value = ::Uml::Attribute::Create(Assignment::meta);
			SearchConfig::meta_MaxRunTimeSec = ::Uml::Attribute::Create(SearchConfig::meta);
			SearchConfig::meta_MaxFailures = ::Uml::Attribute::Create(SearchConfig::meta);
			SearchConfig::meta_SearchEngine = ::Uml::Attribute::Create(SearchConfig::meta);
			SearchConfig::meta_MaxSolutions = ::Uml::Attribute::Create(SearchConfig::meta);
			Task::meta_WCExecTime = ::Uml::Attribute::Create(Task::meta);
			Task::meta_Period = ::Uml::Attribute::Create(Task::meta);
			Message::meta_MsgLength = ::Uml::Attribute::Create(Message::meta);
			TimeSync::meta_MsgLength = ::Uml::Attribute::Create(TimeSync::meta);
			TimeSync::meta_Period = ::Uml::Attribute::Create(TimeSync::meta);
			Latency::meta_MaxLength = ::Uml::Attribute::Create(Latency::meta);
			Bus::meta_Resolution = ::Uml::Attribute::Create(Bus::meta);
			MsgInstance::meta_RefName = ::Uml::Attribute::Create(MsgInstance::meta);
			MsgInstance::meta_StartTime = ::Uml::Attribute::Create(MsgInstance::meta);
			MsgInstance::meta_InstID = ::Uml::Attribute::Create(MsgInstance::meta);
			InstanceModel::meta_HyperPeriod = ::Uml::Attribute::Create(InstanceModel::meta);
			TaskInstance::meta_RefName = ::Uml::Attribute::Create(TaskInstance::meta);
			TaskInstance::meta_StartTime = ::Uml::Attribute::Create(TaskInstance::meta);
			TaskInstance::meta_InstID = ::Uml::Attribute::Create(TaskInstance::meta);
			RootFolder::meta_name = ::Uml::Attribute::Create(RootFolder::meta);
			MgaObject::meta_position = ::Uml::Attribute::Create(MgaObject::meta);
			MgaObject::meta_name = ::Uml::Attribute::Create(MgaObject::meta);
		}
		void InitClassesAttributes()
		{
			ConstraintProblem::meta.name() = "ConstraintProblem";
			ConstraintProblem::meta.isAbstract() = false;
			ConstraintProblem::meta.stereotype() = "Model";
			FDVar::meta.name() = "FDVar";
			FDVar::meta.isAbstract() = false;
			FDVar::meta.stereotype() = "Atom";
			Constraint::meta.name() = "Constraint";
			Constraint::meta.isAbstract() = true;
			Constraint::meta.stereotype() = "Model";
			EqualityConstraint::meta.name() = "EqualityConstraint";
			EqualityConstraint::meta.isAbstract() = false;
			EqualityConstraint::meta.stereotype() = "Model";
			InequalityConstraint::meta.name() = "InequalityConstraint";
			InequalityConstraint::meta.isAbstract() = false;
			InequalityConstraint::meta.stereotype() = "Model";
			SerialConstraint::meta.name() = "SerialConstraint";
			SerialConstraint::meta.isAbstract() = false;
			SerialConstraint::meta.stereotype() = "Model";
			Member::meta.name() = "Member";
			Member::meta.isAbstract() = false;
			Member::meta.stereotype() = "Connection";
			RangeConstraint::meta.name() = "RangeConstraint";
			RangeConstraint::meta.isAbstract() = false;
			RangeConstraint::meta.stereotype() = "Model";
			YPort::meta.name() = "YPort";
			YPort::meta.isAbstract() = false;
			YPort::meta.stereotype() = "Atom";
			XPort::meta.name() = "XPort";
			XPort::meta.isAbstract() = false;
			XPort::meta.stereotype() = "Atom";
			PairConstraint::meta.name() = "PairConstraint";
			PairConstraint::meta.isAbstract() = true;
			PairConstraint::meta.stereotype() = "Model";
			Solution::meta.name() = "Solution";
			Solution::meta.isAbstract() = false;
			Solution::meta.stereotype() = "Model";
			Connectable::meta.name() = "Connectable";
			Connectable::meta.isAbstract() = true;
			Connectable::meta.stereotype() = "FCO";
			Assignment::meta.name() = "Assignment";
			Assignment::meta.isAbstract() = false;
			Assignment::meta.stereotype() = "Atom";
			InstanceRef::meta.name() = "InstanceRef";
			InstanceRef::meta.isAbstract() = false;
			InstanceRef::meta.stereotype() = "Reference";
			SearchConfig::meta.name() = "SearchConfig";
			SearchConfig::meta.isAbstract() = false;
			SearchConfig::meta.stereotype() = "Atom";
			Task::meta.name() = "Task";
			Task::meta.isAbstract() = false;
			Task::meta.stereotype() = "Atom";
			StartMsg::meta.name() = "StartMsg";
			StartMsg::meta.isAbstract() = false;
			StartMsg::meta.stereotype() = "Connection";
			LatencySpec::meta.name() = "LatencySpec";
			LatencySpec::meta.isAbstract() = false;
			LatencySpec::meta.stereotype() = "Model";
			TTSystem::meta.name() = "TTSystem";
			TTSystem::meta.isAbstract() = false;
			TTSystem::meta.stereotype() = "Model";
			Sender::meta.name() = "Sender";
			Sender::meta.isAbstract() = false;
			Sender::meta.stereotype() = "Connection";
			EndMsg::meta.name() = "EndMsg";
			EndMsg::meta.isAbstract() = false;
			EndMsg::meta.stereotype() = "Connection";
			Receiver::meta.name() = "Receiver";
			Receiver::meta.isAbstract() = false;
			Receiver::meta.stereotype() = "Connection";
			Dependency::meta.name() = "Dependency";
			Dependency::meta.isAbstract() = false;
			Dependency::meta.stereotype() = "Connection";
			Link::meta.name() = "Link";
			Link::meta.isAbstract() = false;
			Link::meta.stereotype() = "Connection";
			Message::meta.name() = "Message";
			Message::meta.isAbstract() = false;
			Message::meta.stereotype() = "Atom";
			TimeSync::meta.name() = "TimeSync";
			TimeSync::meta.isAbstract() = false;
			TimeSync::meta.stereotype() = "Atom";
			Latency::meta.name() = "Latency";
			Latency::meta.isAbstract() = false;
			Latency::meta.stereotype() = "Atom";
			MsgRef::meta.name() = "MsgRef";
			MsgRef::meta.isAbstract() = false;
			MsgRef::meta.stereotype() = "Reference";
			TaskRef::meta.name() = "TaskRef";
			TaskRef::meta.isAbstract() = false;
			TaskRef::meta.stereotype() = "Reference";
			Processor::meta.name() = "Processor";
			Processor::meta.isAbstract() = false;
			Processor::meta.stereotype() = "Model";
			Bus::meta.name() = "Bus";
			Bus::meta.isAbstract() = false;
			Bus::meta.stereotype() = "Model";
			MsgInstances::meta.name() = "MsgInstances";
			MsgInstances::meta.isAbstract() = false;
			MsgInstances::meta.stereotype() = "Model";
			SenderInst::meta.name() = "SenderInst";
			SenderInst::meta.isAbstract() = false;
			SenderInst::meta.stereotype() = "Connection";
			Dependencies::meta.name() = "Dependencies";
			Dependencies::meta.isAbstract() = false;
			Dependencies::meta.stereotype() = "Model";
			MsgInstance::meta.name() = "MsgInstance";
			MsgInstance::meta.isAbstract() = false;
			MsgInstance::meta.stereotype() = "Atom";
			TaskInstRef::meta.name() = "TaskInstRef";
			TaskInstRef::meta.isAbstract() = false;
			TaskInstRef::meta.stereotype() = "Reference";
			InstanceModel::meta.name() = "InstanceModel";
			InstanceModel::meta.isAbstract() = false;
			InstanceModel::meta.stereotype() = "Model";
			MsgInstRef::meta.name() = "MsgInstRef";
			MsgInstRef::meta.isAbstract() = false;
			MsgInstRef::meta.stereotype() = "Reference";
			TaskInstances::meta.name() = "TaskInstances";
			TaskInstances::meta.isAbstract() = false;
			TaskInstances::meta.stereotype() = "Model";
			TaskInstance::meta.name() = "TaskInstance";
			TaskInstance::meta.isAbstract() = false;
			TaskInstance::meta.stereotype() = "Atom";
			ReceiverInst::meta.name() = "ReceiverInst";
			ReceiverInst::meta.isAbstract() = false;
			ReceiverInst::meta.stereotype() = "Connection";
			RootFolder::meta.name() = "RootFolder";
			RootFolder::meta.isAbstract() = false;
			RootFolder::meta.stereotype() = "Folder";
			MgaObject::meta.name() = "MgaObject";
			MgaObject::meta.isAbstract() = true;
			
			ConstraintProblem::meta_UpperBound.name() = "UpperBound";
			ConstraintProblem::meta_UpperBound.type() = "Integer";
			ConstraintProblem::meta_UpperBound.min() = 1;
			ConstraintProblem::meta_UpperBound.max() = 1;
			ConstraintProblem::meta_UpperBound.nonpersistent() = false;
			ConstraintProblem::meta_UpperBound.registry() = false;
			ConstraintProblem::meta_UpperBound.ordered() = false;
			ConstraintProblem::meta_UpperBound.visibility() = "public";
			ConstraintProblem::meta_UpperBound.defvalue() = vector<string>();
			ConstraintProblem::meta_LowerBound.name() = "LowerBound";
			ConstraintProblem::meta_LowerBound.type() = "Integer";
			ConstraintProblem::meta_LowerBound.min() = 1;
			ConstraintProblem::meta_LowerBound.max() = 1;
			ConstraintProblem::meta_LowerBound.nonpersistent() = false;
			ConstraintProblem::meta_LowerBound.registry() = false;
			ConstraintProblem::meta_LowerBound.ordered() = false;
			ConstraintProblem::meta_LowerBound.visibility() = "public";
			ConstraintProblem::meta_LowerBound.defvalue() = vector<string>();
			FDVar::meta_UpperBound.name() = "UpperBound";
			FDVar::meta_UpperBound.type() = "Integer";
			FDVar::meta_UpperBound.min() = 1;
			FDVar::meta_UpperBound.max() = 1;
			FDVar::meta_UpperBound.nonpersistent() = false;
			FDVar::meta_UpperBound.registry() = false;
			FDVar::meta_UpperBound.ordered() = false;
			FDVar::meta_UpperBound.visibility() = "public";
			FDVar::meta_UpperBound.defvalue() = vector<string>();
			FDVar::meta_LowerBound.name() = "LowerBound";
			FDVar::meta_LowerBound.type() = "Integer";
			FDVar::meta_LowerBound.min() = 1;
			FDVar::meta_LowerBound.max() = 1;
			FDVar::meta_LowerBound.nonpersistent() = false;
			FDVar::meta_LowerBound.registry() = false;
			FDVar::meta_LowerBound.ordered() = false;
			FDVar::meta_LowerBound.visibility() = "public";
			FDVar::meta_LowerBound.defvalue() = vector<string>();
			FDVar::meta_Size.name() = "Size";
			FDVar::meta_Size.type() = "Integer";
			FDVar::meta_Size.min() = 1;
			FDVar::meta_Size.max() = 1;
			FDVar::meta_Size.nonpersistent() = false;
			FDVar::meta_Size.registry() = false;
			FDVar::meta_Size.ordered() = false;
			FDVar::meta_Size.visibility() = "public";
			FDVar::meta_Size.defvalue() = vector<string>();
			EqualityConstraint::meta_Offset.name() = "Offset";
			EqualityConstraint::meta_Offset.type() = "Integer";
			EqualityConstraint::meta_Offset.min() = 1;
			EqualityConstraint::meta_Offset.max() = 1;
			EqualityConstraint::meta_Offset.nonpersistent() = false;
			EqualityConstraint::meta_Offset.registry() = false;
			EqualityConstraint::meta_Offset.ordered() = false;
			EqualityConstraint::meta_Offset.visibility() = "public";
			EqualityConstraint::meta_Offset.defvalue() = vector<string>();
			InequalityConstraint::meta_Offset.name() = "Offset";
			InequalityConstraint::meta_Offset.type() = "Integer";
			InequalityConstraint::meta_Offset.min() = 1;
			InequalityConstraint::meta_Offset.max() = 1;
			InequalityConstraint::meta_Offset.nonpersistent() = false;
			InequalityConstraint::meta_Offset.registry() = false;
			InequalityConstraint::meta_Offset.ordered() = false;
			InequalityConstraint::meta_Offset.visibility() = "public";
			InequalityConstraint::meta_Offset.defvalue() = vector<string>();
			RangeConstraint::meta_UpperBound.name() = "UpperBound";
			RangeConstraint::meta_UpperBound.type() = "Integer";
			RangeConstraint::meta_UpperBound.min() = 1;
			RangeConstraint::meta_UpperBound.max() = 1;
			RangeConstraint::meta_UpperBound.nonpersistent() = false;
			RangeConstraint::meta_UpperBound.registry() = false;
			RangeConstraint::meta_UpperBound.ordered() = false;
			RangeConstraint::meta_UpperBound.visibility() = "public";
			RangeConstraint::meta_UpperBound.defvalue() = vector<string>();
			RangeConstraint::meta_LowerBound.name() = "LowerBound";
			RangeConstraint::meta_LowerBound.type() = "Integer";
			RangeConstraint::meta_LowerBound.min() = 1;
			RangeConstraint::meta_LowerBound.max() = 1;
			RangeConstraint::meta_LowerBound.nonpersistent() = false;
			RangeConstraint::meta_LowerBound.registry() = false;
			RangeConstraint::meta_LowerBound.ordered() = false;
			RangeConstraint::meta_LowerBound.visibility() = "public";
			RangeConstraint::meta_LowerBound.defvalue() = vector<string>();
			Assignment::meta_Value.name() = "Value";
			Assignment::meta_Value.type() = "Integer";
			Assignment::meta_Value.min() = 1;
			Assignment::meta_Value.max() = 1;
			Assignment::meta_Value.nonpersistent() = false;
			Assignment::meta_Value.registry() = false;
			Assignment::meta_Value.ordered() = false;
			Assignment::meta_Value.visibility() = "public";
			Assignment::meta_Value.defvalue() = vector<string>();
			vector<string> SearchConfig_MaxRunTimeSec_dva;
			SearchConfig_MaxRunTimeSec_dva.push_back("10000000");
			SearchConfig::meta_MaxRunTimeSec.name() = "MaxRunTimeSec";
			SearchConfig::meta_MaxRunTimeSec.type() = "Integer";
			SearchConfig::meta_MaxRunTimeSec.min() = 1;
			SearchConfig::meta_MaxRunTimeSec.max() = 1;
			SearchConfig::meta_MaxRunTimeSec.nonpersistent() = false;
			SearchConfig::meta_MaxRunTimeSec.registry() = false;
			SearchConfig::meta_MaxRunTimeSec.ordered() = false;
			SearchConfig::meta_MaxRunTimeSec.visibility() = "public";
			SearchConfig::meta_MaxRunTimeSec.defvalue() = SearchConfig_MaxRunTimeSec_dva;
			vector<string> SearchConfig_MaxFailures_dva;
			SearchConfig_MaxFailures_dva.push_back("1000000");
			SearchConfig::meta_MaxFailures.name() = "MaxFailures";
			SearchConfig::meta_MaxFailures.type() = "Integer";
			SearchConfig::meta_MaxFailures.min() = 1;
			SearchConfig::meta_MaxFailures.max() = 1;
			SearchConfig::meta_MaxFailures.nonpersistent() = false;
			SearchConfig::meta_MaxFailures.registry() = false;
			SearchConfig::meta_MaxFailures.ordered() = false;
			SearchConfig::meta_MaxFailures.visibility() = "public";
			SearchConfig::meta_MaxFailures.defvalue() = SearchConfig_MaxFailures_dva;
			vector<string> SearchConfig_SearchEngine_dva;
			SearchConfig_SearchEngine_dva.push_back("DepthFirst");
			SearchConfig::meta_SearchEngine.name() = "SearchEngine";
			SearchConfig::meta_SearchEngine.type() = "String";
			SearchConfig::meta_SearchEngine.min() = 1;
			SearchConfig::meta_SearchEngine.max() = 1;
			SearchConfig::meta_SearchEngine.nonpersistent() = false;
			SearchConfig::meta_SearchEngine.registry() = false;
			SearchConfig::meta_SearchEngine.ordered() = false;
			SearchConfig::meta_SearchEngine.visibility() = "public";
			SearchConfig::meta_SearchEngine.defvalue() = SearchConfig_SearchEngine_dva;
			vector<string> SearchConfig_MaxSolutions_dva;
			SearchConfig_MaxSolutions_dva.push_back("10");
			SearchConfig::meta_MaxSolutions.name() = "MaxSolutions";
			SearchConfig::meta_MaxSolutions.type() = "Integer";
			SearchConfig::meta_MaxSolutions.min() = 1;
			SearchConfig::meta_MaxSolutions.max() = 1;
			SearchConfig::meta_MaxSolutions.nonpersistent() = false;
			SearchConfig::meta_MaxSolutions.registry() = false;
			SearchConfig::meta_MaxSolutions.ordered() = false;
			SearchConfig::meta_MaxSolutions.visibility() = "public";
			SearchConfig::meta_MaxSolutions.defvalue() = SearchConfig_MaxSolutions_dva;
			vector<string> Task_WCExecTime_dva;
			Task_WCExecTime_dva.push_back("1");
			Task::meta_WCExecTime.name() = "WCExecTime";
			Task::meta_WCExecTime.type() = "Integer";
			Task::meta_WCExecTime.min() = 1;
			Task::meta_WCExecTime.max() = 1;
			Task::meta_WCExecTime.nonpersistent() = false;
			Task::meta_WCExecTime.registry() = false;
			Task::meta_WCExecTime.ordered() = false;
			Task::meta_WCExecTime.visibility() = "public";
			Task::meta_WCExecTime.defvalue() = Task_WCExecTime_dva;
			vector<string> Task_Period_dva;
			Task_Period_dva.push_back("1");
			Task::meta_Period.name() = "Period";
			Task::meta_Period.type() = "Integer";
			Task::meta_Period.min() = 1;
			Task::meta_Period.max() = 1;
			Task::meta_Period.nonpersistent() = false;
			Task::meta_Period.registry() = false;
			Task::meta_Period.ordered() = false;
			Task::meta_Period.visibility() = "public";
			Task::meta_Period.defvalue() = Task_Period_dva;
			Message::meta_MsgLength.name() = "MsgLength";
			Message::meta_MsgLength.type() = "Integer";
			Message::meta_MsgLength.min() = 1;
			Message::meta_MsgLength.max() = 1;
			Message::meta_MsgLength.nonpersistent() = false;
			Message::meta_MsgLength.registry() = false;
			Message::meta_MsgLength.ordered() = false;
			Message::meta_MsgLength.visibility() = "public";
			Message::meta_MsgLength.defvalue() = vector<string>();
			TimeSync::meta_MsgLength.name() = "MsgLength";
			TimeSync::meta_MsgLength.type() = "Integer";
			TimeSync::meta_MsgLength.min() = 1;
			TimeSync::meta_MsgLength.max() = 1;
			TimeSync::meta_MsgLength.nonpersistent() = false;
			TimeSync::meta_MsgLength.registry() = false;
			TimeSync::meta_MsgLength.ordered() = false;
			TimeSync::meta_MsgLength.visibility() = "public";
			TimeSync::meta_MsgLength.defvalue() = vector<string>();
			vector<string> TimeSync_Period_dva;
			TimeSync_Period_dva.push_back("1");
			TimeSync::meta_Period.name() = "Period";
			TimeSync::meta_Period.type() = "Integer";
			TimeSync::meta_Period.min() = 1;
			TimeSync::meta_Period.max() = 1;
			TimeSync::meta_Period.nonpersistent() = false;
			TimeSync::meta_Period.registry() = false;
			TimeSync::meta_Period.ordered() = false;
			TimeSync::meta_Period.visibility() = "public";
			TimeSync::meta_Period.defvalue() = TimeSync_Period_dva;
			Latency::meta_MaxLength.name() = "MaxLength";
			Latency::meta_MaxLength.type() = "Integer";
			Latency::meta_MaxLength.min() = 1;
			Latency::meta_MaxLength.max() = 1;
			Latency::meta_MaxLength.nonpersistent() = false;
			Latency::meta_MaxLength.registry() = false;
			Latency::meta_MaxLength.ordered() = false;
			Latency::meta_MaxLength.visibility() = "public";
			Latency::meta_MaxLength.defvalue() = vector<string>();
			Bus::meta_Resolution.name() = "Resolution";
			Bus::meta_Resolution.type() = "Integer";
			Bus::meta_Resolution.min() = 1;
			Bus::meta_Resolution.max() = 1;
			Bus::meta_Resolution.nonpersistent() = false;
			Bus::meta_Resolution.registry() = false;
			Bus::meta_Resolution.ordered() = false;
			Bus::meta_Resolution.visibility() = "public";
			Bus::meta_Resolution.defvalue() = vector<string>();
			MsgInstance::meta_RefName.name() = "RefName";
			MsgInstance::meta_RefName.type() = "String";
			MsgInstance::meta_RefName.min() = 1;
			MsgInstance::meta_RefName.max() = 1;
			MsgInstance::meta_RefName.nonpersistent() = false;
			MsgInstance::meta_RefName.registry() = false;
			MsgInstance::meta_RefName.ordered() = false;
			MsgInstance::meta_RefName.visibility() = "public";
			MsgInstance::meta_RefName.defvalue() = vector<string>();
			MsgInstance::meta_StartTime.name() = "StartTime";
			MsgInstance::meta_StartTime.type() = "Integer";
			MsgInstance::meta_StartTime.min() = 1;
			MsgInstance::meta_StartTime.max() = 1;
			MsgInstance::meta_StartTime.nonpersistent() = false;
			MsgInstance::meta_StartTime.registry() = false;
			MsgInstance::meta_StartTime.ordered() = false;
			MsgInstance::meta_StartTime.visibility() = "public";
			MsgInstance::meta_StartTime.defvalue() = vector<string>();
			MsgInstance::meta_InstID.name() = "InstID";
			MsgInstance::meta_InstID.type() = "Integer";
			MsgInstance::meta_InstID.min() = 1;
			MsgInstance::meta_InstID.max() = 1;
			MsgInstance::meta_InstID.nonpersistent() = false;
			MsgInstance::meta_InstID.registry() = false;
			MsgInstance::meta_InstID.ordered() = false;
			MsgInstance::meta_InstID.visibility() = "public";
			MsgInstance::meta_InstID.defvalue() = vector<string>();
			InstanceModel::meta_HyperPeriod.name() = "HyperPeriod";
			InstanceModel::meta_HyperPeriod.type() = "Integer";
			InstanceModel::meta_HyperPeriod.min() = 1;
			InstanceModel::meta_HyperPeriod.max() = 1;
			InstanceModel::meta_HyperPeriod.nonpersistent() = false;
			InstanceModel::meta_HyperPeriod.registry() = false;
			InstanceModel::meta_HyperPeriod.ordered() = false;
			InstanceModel::meta_HyperPeriod.visibility() = "public";
			InstanceModel::meta_HyperPeriod.defvalue() = vector<string>();
			TaskInstance::meta_RefName.name() = "RefName";
			TaskInstance::meta_RefName.type() = "String";
			TaskInstance::meta_RefName.min() = 1;
			TaskInstance::meta_RefName.max() = 1;
			TaskInstance::meta_RefName.nonpersistent() = false;
			TaskInstance::meta_RefName.registry() = false;
			TaskInstance::meta_RefName.ordered() = false;
			TaskInstance::meta_RefName.visibility() = "public";
			TaskInstance::meta_RefName.defvalue() = vector<string>();
			TaskInstance::meta_StartTime.name() = "StartTime";
			TaskInstance::meta_StartTime.type() = "Integer";
			TaskInstance::meta_StartTime.min() = 1;
			TaskInstance::meta_StartTime.max() = 1;
			TaskInstance::meta_StartTime.nonpersistent() = false;
			TaskInstance::meta_StartTime.registry() = false;
			TaskInstance::meta_StartTime.ordered() = false;
			TaskInstance::meta_StartTime.visibility() = "public";
			TaskInstance::meta_StartTime.defvalue() = vector<string>();
			TaskInstance::meta_InstID.name() = "InstID";
			TaskInstance::meta_InstID.type() = "Integer";
			TaskInstance::meta_InstID.min() = 1;
			TaskInstance::meta_InstID.max() = 1;
			TaskInstance::meta_InstID.nonpersistent() = false;
			TaskInstance::meta_InstID.registry() = false;
			TaskInstance::meta_InstID.ordered() = false;
			TaskInstance::meta_InstID.visibility() = "public";
			TaskInstance::meta_InstID.defvalue() = vector<string>();
			RootFolder::meta_name.name() = "name";
			RootFolder::meta_name.type() = "String";
			RootFolder::meta_name.min() = 0;
			RootFolder::meta_name.max() = 1;
			RootFolder::meta_name.nonpersistent() = false;
			RootFolder::meta_name.registry() = false;
			RootFolder::meta_name.ordered() = false;
			RootFolder::meta_name.visibility() = "public";
			RootFolder::meta_name.defvalue() = vector<string>();
			MgaObject::meta_position.name() = "position";
			MgaObject::meta_position.type() = "String";
			MgaObject::meta_position.min() = 0;
			MgaObject::meta_position.max() = 1;
			MgaObject::meta_position.nonpersistent() = false;
			MgaObject::meta_position.registry() = false;
			MgaObject::meta_position.ordered() = false;
			MgaObject::meta_position.visibility() = "public";
			MgaObject::meta_position.defvalue() = vector<string>();
			MgaObject::meta_name.name() = "name";
			MgaObject::meta_name.type() = "String";
			MgaObject::meta_name.min() = 0;
			MgaObject::meta_name.max() = 1;
			MgaObject::meta_name.nonpersistent() = false;
			MgaObject::meta_name.registry() = false;
			MgaObject::meta_name.ordered() = false;
			MgaObject::meta_name.visibility() = "public";
			MgaObject::meta_name.defvalue() = vector<string>();
			
		}
		void CreatesNamespaces()
		{
		}
		void InitNamespaces()
		{
		}
		void InitAssociation0()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Member";
			ass.assocClass() = ::TTSched::Member::meta;
			::TTSched::Connectable::meta_srcMember = ::Uml::AssociationRole::Create(ass);
			::TTSched::Connectable::meta_srcMember.name() = "srcMember";
			::TTSched::Connectable::meta_srcMember.min() = 0;
			::TTSched::Connectable::meta_srcMember.max() = -1;
			::TTSched::Connectable::meta_srcMember.isNavigable() = true;
			::TTSched::Connectable::meta_srcMember.isPrimary() = false;
			::TTSched::Connectable::meta_srcMember.target() = ::TTSched::FDVar::meta;
			::TTSched::Member::meta_srcMember_end_ = ::TTSched::FDVar::meta_dstMember_rev = ::TTSched::Connectable::meta_srcMember;
			::TTSched::FDVar::meta_dstMember = ::Uml::AssociationRole::Create(ass);
			::TTSched::FDVar::meta_dstMember.name() = "dstMember";
			::TTSched::FDVar::meta_dstMember.min() = 0;
			::TTSched::FDVar::meta_dstMember.max() = -1;
			::TTSched::FDVar::meta_dstMember.isNavigable() = true;
			::TTSched::FDVar::meta_dstMember.isPrimary() = false;
			::TTSched::FDVar::meta_dstMember.target() = ::TTSched::Connectable::meta;
			::TTSched::Member::meta_dstMember_end_ = ::TTSched::Connectable::meta_srcMember_rev = ::TTSched::FDVar::meta_dstMember;
		};
		void InitAssociation1()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			::TTSched::InstanceModel::meta_referedbyInstanceRef = ::Uml::AssociationRole::Create(ass);
			::TTSched::InstanceModel::meta_referedbyInstanceRef.name() = "referedbyInstanceRef";
			::TTSched::InstanceModel::meta_referedbyInstanceRef.min() = 0;
			::TTSched::InstanceModel::meta_referedbyInstanceRef.max() = -1;
			::TTSched::InstanceModel::meta_referedbyInstanceRef.isNavigable() = true;
			::TTSched::InstanceModel::meta_referedbyInstanceRef.isPrimary() = false;
			::TTSched::InstanceModel::meta_referedbyInstanceRef.target() = ::TTSched::InstanceRef::meta;
			::TTSched::InstanceRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::TTSched::InstanceRef::meta_ref.name() = "ref";
			::TTSched::InstanceRef::meta_ref.min() = 0;
			::TTSched::InstanceRef::meta_ref.max() = 1;
			::TTSched::InstanceRef::meta_ref.isNavigable() = true;
			::TTSched::InstanceRef::meta_ref.isPrimary() = false;
			::TTSched::InstanceRef::meta_ref.target() = ::TTSched::InstanceModel::meta;
		};
		void InitAssociation2()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			::TTSched::Task::meta_referedbyTaskRef = ::Uml::AssociationRole::Create(ass);
			::TTSched::Task::meta_referedbyTaskRef.name() = "referedbyTaskRef";
			::TTSched::Task::meta_referedbyTaskRef.min() = 0;
			::TTSched::Task::meta_referedbyTaskRef.max() = -1;
			::TTSched::Task::meta_referedbyTaskRef.isNavigable() = true;
			::TTSched::Task::meta_referedbyTaskRef.isPrimary() = false;
			::TTSched::Task::meta_referedbyTaskRef.target() = ::TTSched::TaskRef::meta;
			::TTSched::TaskRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::TTSched::TaskRef::meta_ref.name() = "ref";
			::TTSched::TaskRef::meta_ref.min() = 0;
			::TTSched::TaskRef::meta_ref.max() = 1;
			::TTSched::TaskRef::meta_ref.isNavigable() = true;
			::TTSched::TaskRef::meta_ref.isPrimary() = false;
			::TTSched::TaskRef::meta_ref.target() = ::TTSched::Task::meta;
		};
		void InitAssociation3()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			::TTSched::Message::meta_referedbyMsgRef = ::Uml::AssociationRole::Create(ass);
			::TTSched::Message::meta_referedbyMsgRef.name() = "referedbyMsgRef";
			::TTSched::Message::meta_referedbyMsgRef.min() = 0;
			::TTSched::Message::meta_referedbyMsgRef.max() = -1;
			::TTSched::Message::meta_referedbyMsgRef.isNavigable() = true;
			::TTSched::Message::meta_referedbyMsgRef.isPrimary() = false;
			::TTSched::Message::meta_referedbyMsgRef.target() = ::TTSched::MsgRef::meta;
			::TTSched::MsgRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::TTSched::MsgRef::meta_ref.name() = "ref";
			::TTSched::MsgRef::meta_ref.min() = 0;
			::TTSched::MsgRef::meta_ref.max() = 1;
			::TTSched::MsgRef::meta_ref.isNavigable() = true;
			::TTSched::MsgRef::meta_ref.isPrimary() = false;
			::TTSched::MsgRef::meta_ref.target() = ::TTSched::Message::meta;
		};
		void InitAssociation4()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Receiver";
			ass.assocClass() = ::TTSched::Receiver::meta;
			::TTSched::Message::meta_dstReceiver = ::Uml::AssociationRole::Create(ass);
			::TTSched::Message::meta_dstReceiver.name() = "dstReceiver";
			::TTSched::Message::meta_dstReceiver.min() = 0;
			::TTSched::Message::meta_dstReceiver.max() = -1;
			::TTSched::Message::meta_dstReceiver.isNavigable() = true;
			::TTSched::Message::meta_dstReceiver.isPrimary() = false;
			::TTSched::Message::meta_dstReceiver.target() = ::TTSched::TaskRef::meta;
			::TTSched::Receiver::meta_dstReceiver_end_ = ::TTSched::TaskRef::meta_srcReceiver_rev = ::TTSched::Message::meta_dstReceiver;
			::TTSched::TaskRef::meta_srcReceiver = ::Uml::AssociationRole::Create(ass);
			::TTSched::TaskRef::meta_srcReceiver.name() = "srcReceiver";
			::TTSched::TaskRef::meta_srcReceiver.min() = 0;
			::TTSched::TaskRef::meta_srcReceiver.max() = -1;
			::TTSched::TaskRef::meta_srcReceiver.isNavigable() = true;
			::TTSched::TaskRef::meta_srcReceiver.isPrimary() = false;
			::TTSched::TaskRef::meta_srcReceiver.target() = ::TTSched::Message::meta;
			::TTSched::Receiver::meta_srcReceiver_end_ = ::TTSched::Message::meta_dstReceiver_rev = ::TTSched::TaskRef::meta_srcReceiver;
		};
		void InitAssociation5()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Sender";
			ass.assocClass() = ::TTSched::Sender::meta;
			::TTSched::Message::meta_srcSender = ::Uml::AssociationRole::Create(ass);
			::TTSched::Message::meta_srcSender.name() = "srcSender";
			::TTSched::Message::meta_srcSender.min() = 0;
			::TTSched::Message::meta_srcSender.max() = 1;
			::TTSched::Message::meta_srcSender.isNavigable() = true;
			::TTSched::Message::meta_srcSender.isPrimary() = false;
			::TTSched::Message::meta_srcSender.target() = ::TTSched::TaskRef::meta;
			::TTSched::Sender::meta_srcSender_end_ = ::TTSched::TaskRef::meta_dstSender_rev = ::TTSched::Message::meta_srcSender;
			::TTSched::TaskRef::meta_dstSender = ::Uml::AssociationRole::Create(ass);
			::TTSched::TaskRef::meta_dstSender.name() = "dstSender";
			::TTSched::TaskRef::meta_dstSender.min() = 0;
			::TTSched::TaskRef::meta_dstSender.max() = -1;
			::TTSched::TaskRef::meta_dstSender.isNavigable() = true;
			::TTSched::TaskRef::meta_dstSender.isPrimary() = false;
			::TTSched::TaskRef::meta_dstSender.target() = ::TTSched::Message::meta;
			::TTSched::Sender::meta_dstSender_end_ = ::TTSched::Message::meta_srcSender_rev = ::TTSched::TaskRef::meta_dstSender;
		};
		void InitAssociation6()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "EndMsg";
			ass.assocClass() = ::TTSched::EndMsg::meta;
			::TTSched::Latency::meta_srcEndMsg = ::Uml::AssociationRole::Create(ass);
			::TTSched::Latency::meta_srcEndMsg.name() = "srcEndMsg";
			::TTSched::Latency::meta_srcEndMsg.min() = 0;
			::TTSched::Latency::meta_srcEndMsg.max() = -1;
			::TTSched::Latency::meta_srcEndMsg.isNavigable() = true;
			::TTSched::Latency::meta_srcEndMsg.isPrimary() = false;
			::TTSched::Latency::meta_srcEndMsg.target() = ::TTSched::MsgRef::meta;
			::TTSched::EndMsg::meta_srcEndMsg_end_ = ::TTSched::MsgRef::meta_dstEndMsg_rev = ::TTSched::Latency::meta_srcEndMsg;
			::TTSched::MsgRef::meta_dstEndMsg = ::Uml::AssociationRole::Create(ass);
			::TTSched::MsgRef::meta_dstEndMsg.name() = "dstEndMsg";
			::TTSched::MsgRef::meta_dstEndMsg.min() = 0;
			::TTSched::MsgRef::meta_dstEndMsg.max() = -1;
			::TTSched::MsgRef::meta_dstEndMsg.isNavigable() = true;
			::TTSched::MsgRef::meta_dstEndMsg.isPrimary() = false;
			::TTSched::MsgRef::meta_dstEndMsg.target() = ::TTSched::Latency::meta;
			::TTSched::EndMsg::meta_dstEndMsg_end_ = ::TTSched::Latency::meta_srcEndMsg_rev = ::TTSched::MsgRef::meta_dstEndMsg;
		};
		void InitAssociation7()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "StartMsg";
			ass.assocClass() = ::TTSched::StartMsg::meta;
			::TTSched::Latency::meta_dstStartMsg = ::Uml::AssociationRole::Create(ass);
			::TTSched::Latency::meta_dstStartMsg.name() = "dstStartMsg";
			::TTSched::Latency::meta_dstStartMsg.min() = 0;
			::TTSched::Latency::meta_dstStartMsg.max() = -1;
			::TTSched::Latency::meta_dstStartMsg.isNavigable() = true;
			::TTSched::Latency::meta_dstStartMsg.isPrimary() = false;
			::TTSched::Latency::meta_dstStartMsg.target() = ::TTSched::MsgRef::meta;
			::TTSched::StartMsg::meta_dstStartMsg_end_ = ::TTSched::MsgRef::meta_srcStartMsg_rev = ::TTSched::Latency::meta_dstStartMsg;
			::TTSched::MsgRef::meta_srcStartMsg = ::Uml::AssociationRole::Create(ass);
			::TTSched::MsgRef::meta_srcStartMsg.name() = "srcStartMsg";
			::TTSched::MsgRef::meta_srcStartMsg.min() = 0;
			::TTSched::MsgRef::meta_srcStartMsg.max() = -1;
			::TTSched::MsgRef::meta_srcStartMsg.isNavigable() = true;
			::TTSched::MsgRef::meta_srcStartMsg.isPrimary() = false;
			::TTSched::MsgRef::meta_srcStartMsg.target() = ::TTSched::Latency::meta;
			::TTSched::StartMsg::meta_srcStartMsg_end_ = ::TTSched::Latency::meta_dstStartMsg_rev = ::TTSched::MsgRef::meta_srcStartMsg;
		};
		void InitAssociation8()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Dependency";
			ass.assocClass() = ::TTSched::Dependency::meta;
			::TTSched::MsgRef::meta_dstDependency = ::Uml::AssociationRole::Create(ass);
			::TTSched::MsgRef::meta_dstDependency.name() = "dstDependency";
			::TTSched::MsgRef::meta_dstDependency.min() = 0;
			::TTSched::MsgRef::meta_dstDependency.max() = -1;
			::TTSched::MsgRef::meta_dstDependency.isNavigable() = true;
			::TTSched::MsgRef::meta_dstDependency.isPrimary() = false;
			::TTSched::MsgRef::meta_dstDependency.target() = ::TTSched::MsgRef::meta;
			::TTSched::Dependency::meta_dstDependency_end_ = ::TTSched::MsgRef::meta_srcDependency_rev = ::TTSched::MsgRef::meta_dstDependency;
			::TTSched::MsgRef::meta_srcDependency = ::Uml::AssociationRole::Create(ass);
			::TTSched::MsgRef::meta_srcDependency.name() = "srcDependency";
			::TTSched::MsgRef::meta_srcDependency.min() = 0;
			::TTSched::MsgRef::meta_srcDependency.max() = -1;
			::TTSched::MsgRef::meta_srcDependency.isNavigable() = true;
			::TTSched::MsgRef::meta_srcDependency.isPrimary() = false;
			::TTSched::MsgRef::meta_srcDependency.target() = ::TTSched::MsgRef::meta;
			::TTSched::Dependency::meta_srcDependency_end_ = ::TTSched::MsgRef::meta_dstDependency_rev = ::TTSched::MsgRef::meta_srcDependency;
		};
		void InitAssociation9()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Link";
			ass.assocClass() = ::TTSched::Link::meta;
			::TTSched::Bus::meta_srcLink = ::Uml::AssociationRole::Create(ass);
			::TTSched::Bus::meta_srcLink.name() = "srcLink";
			::TTSched::Bus::meta_srcLink.min() = 0;
			::TTSched::Bus::meta_srcLink.max() = -1;
			::TTSched::Bus::meta_srcLink.isNavigable() = true;
			::TTSched::Bus::meta_srcLink.isPrimary() = false;
			::TTSched::Bus::meta_srcLink.target() = ::TTSched::Processor::meta;
			::TTSched::Link::meta_srcLink_end_ = ::TTSched::Processor::meta_dstLink_rev = ::TTSched::Bus::meta_srcLink;
			::TTSched::Processor::meta_dstLink = ::Uml::AssociationRole::Create(ass);
			::TTSched::Processor::meta_dstLink.name() = "dstLink";
			::TTSched::Processor::meta_dstLink.min() = 0;
			::TTSched::Processor::meta_dstLink.max() = -1;
			::TTSched::Processor::meta_dstLink.isNavigable() = true;
			::TTSched::Processor::meta_dstLink.isPrimary() = false;
			::TTSched::Processor::meta_dstLink.target() = ::TTSched::Bus::meta;
			::TTSched::Link::meta_dstLink_end_ = ::TTSched::Bus::meta_srcLink_rev = ::TTSched::Processor::meta_dstLink;
		};
		void InitAssociation10()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			::TTSched::MsgInstance::meta_referedbyMsgInstRef = ::Uml::AssociationRole::Create(ass);
			::TTSched::MsgInstance::meta_referedbyMsgInstRef.name() = "referedbyMsgInstRef";
			::TTSched::MsgInstance::meta_referedbyMsgInstRef.min() = 0;
			::TTSched::MsgInstance::meta_referedbyMsgInstRef.max() = -1;
			::TTSched::MsgInstance::meta_referedbyMsgInstRef.isNavigable() = true;
			::TTSched::MsgInstance::meta_referedbyMsgInstRef.isPrimary() = false;
			::TTSched::MsgInstance::meta_referedbyMsgInstRef.target() = ::TTSched::MsgInstRef::meta;
			::TTSched::MsgInstRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::TTSched::MsgInstRef::meta_ref.name() = "ref";
			::TTSched::MsgInstRef::meta_ref.min() = 0;
			::TTSched::MsgInstRef::meta_ref.max() = 1;
			::TTSched::MsgInstRef::meta_ref.isNavigable() = true;
			::TTSched::MsgInstRef::meta_ref.isPrimary() = false;
			::TTSched::MsgInstRef::meta_ref.target() = ::TTSched::MsgInstance::meta;
		};
		void InitAssociation11()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			::TTSched::TaskInstRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::TTSched::TaskInstRef::meta_ref.name() = "ref";
			::TTSched::TaskInstRef::meta_ref.min() = 0;
			::TTSched::TaskInstRef::meta_ref.max() = 1;
			::TTSched::TaskInstRef::meta_ref.isNavigable() = true;
			::TTSched::TaskInstRef::meta_ref.isPrimary() = false;
			::TTSched::TaskInstRef::meta_ref.target() = ::TTSched::TaskInstance::meta;
			::TTSched::TaskInstance::meta_referedbyTaskInstRef = ::Uml::AssociationRole::Create(ass);
			::TTSched::TaskInstance::meta_referedbyTaskInstRef.name() = "referedbyTaskInstRef";
			::TTSched::TaskInstance::meta_referedbyTaskInstRef.min() = 0;
			::TTSched::TaskInstance::meta_referedbyTaskInstRef.max() = -1;
			::TTSched::TaskInstance::meta_referedbyTaskInstRef.isNavigable() = true;
			::TTSched::TaskInstance::meta_referedbyTaskInstRef.isPrimary() = false;
			::TTSched::TaskInstance::meta_referedbyTaskInstRef.target() = ::TTSched::TaskInstRef::meta;
		};
		void InitAssociation12()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "SenderInst";
			ass.assocClass() = ::TTSched::SenderInst::meta;
			::TTSched::TaskInstRef::meta_dstSenderInst = ::Uml::AssociationRole::Create(ass);
			::TTSched::TaskInstRef::meta_dstSenderInst.name() = "dstSenderInst";
			::TTSched::TaskInstRef::meta_dstSenderInst.min() = 0;
			::TTSched::TaskInstRef::meta_dstSenderInst.max() = -1;
			::TTSched::TaskInstRef::meta_dstSenderInst.isNavigable() = true;
			::TTSched::TaskInstRef::meta_dstSenderInst.isPrimary() = false;
			::TTSched::TaskInstRef::meta_dstSenderInst.target() = ::TTSched::MsgInstRef::meta;
			::TTSched::SenderInst::meta_dstSenderInst_end_ = ::TTSched::MsgInstRef::meta_srcSenderInst_rev = ::TTSched::TaskInstRef::meta_dstSenderInst;
			::TTSched::MsgInstRef::meta_srcSenderInst = ::Uml::AssociationRole::Create(ass);
			::TTSched::MsgInstRef::meta_srcSenderInst.name() = "srcSenderInst";
			::TTSched::MsgInstRef::meta_srcSenderInst.min() = 0;
			::TTSched::MsgInstRef::meta_srcSenderInst.max() = -1;
			::TTSched::MsgInstRef::meta_srcSenderInst.isNavigable() = true;
			::TTSched::MsgInstRef::meta_srcSenderInst.isPrimary() = false;
			::TTSched::MsgInstRef::meta_srcSenderInst.target() = ::TTSched::TaskInstRef::meta;
			::TTSched::SenderInst::meta_srcSenderInst_end_ = ::TTSched::TaskInstRef::meta_dstSenderInst_rev = ::TTSched::MsgInstRef::meta_srcSenderInst;
		};
		void InitAssociation13()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "ReceiverInst";
			ass.assocClass() = ::TTSched::ReceiverInst::meta;
			::TTSched::TaskInstRef::meta_srcReceiverInst = ::Uml::AssociationRole::Create(ass);
			::TTSched::TaskInstRef::meta_srcReceiverInst.name() = "srcReceiverInst";
			::TTSched::TaskInstRef::meta_srcReceiverInst.min() = 0;
			::TTSched::TaskInstRef::meta_srcReceiverInst.max() = -1;
			::TTSched::TaskInstRef::meta_srcReceiverInst.isNavigable() = true;
			::TTSched::TaskInstRef::meta_srcReceiverInst.isPrimary() = false;
			::TTSched::TaskInstRef::meta_srcReceiverInst.target() = ::TTSched::MsgInstRef::meta;
			::TTSched::ReceiverInst::meta_srcReceiverInst_end_ = ::TTSched::MsgInstRef::meta_dstReceiverInst_rev = ::TTSched::TaskInstRef::meta_srcReceiverInst;
			::TTSched::MsgInstRef::meta_dstReceiverInst = ::Uml::AssociationRole::Create(ass);
			::TTSched::MsgInstRef::meta_dstReceiverInst.name() = "dstReceiverInst";
			::TTSched::MsgInstRef::meta_dstReceiverInst.min() = 0;
			::TTSched::MsgInstRef::meta_dstReceiverInst.max() = -1;
			::TTSched::MsgInstRef::meta_dstReceiverInst.isNavigable() = true;
			::TTSched::MsgInstRef::meta_dstReceiverInst.isPrimary() = false;
			::TTSched::MsgInstRef::meta_dstReceiverInst.target() = ::TTSched::TaskInstRef::meta;
			::TTSched::ReceiverInst::meta_dstReceiverInst_end_ = ::TTSched::TaskInstRef::meta_srcReceiverInst_rev = ::TTSched::MsgInstRef::meta_dstReceiverInst;
		};
		void InitComposition0()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::InstanceRef::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::InstanceRef::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::TTSched::InstanceRef::meta_ConstraintProblem_parent.isNavigable() = true;
			::TTSched::InstanceRef::meta_ConstraintProblem_parent.target() = ::TTSched::ConstraintProblem::meta;
			::TTSched::ConstraintProblem::meta_InstanceRef_child = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::ConstraintProblem::meta_InstanceRef_child.name() = "InstanceRef";
			::TTSched::ConstraintProblem::meta_InstanceRef_child.min() = 0;
			::TTSched::ConstraintProblem::meta_InstanceRef_child.max() = 1;
			::TTSched::ConstraintProblem::meta_InstanceRef_child.isNavigable() = true;
			::TTSched::ConstraintProblem::meta_InstanceRef_child.target() = ::TTSched::InstanceRef::meta;
		}
		void InitComposition1()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Member::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Member::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::TTSched::Member::meta_ConstraintProblem_parent.isNavigable() = true;
			::TTSched::Member::meta_ConstraintProblem_parent.target() = ::TTSched::ConstraintProblem::meta;
			::TTSched::ConstraintProblem::meta_Member_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::ConstraintProblem::meta_Member_children.name() = "Member";
			::TTSched::ConstraintProblem::meta_Member_children.min() = 0;
			::TTSched::ConstraintProblem::meta_Member_children.max() = -1;
			::TTSched::ConstraintProblem::meta_Member_children.isNavigable() = true;
			::TTSched::ConstraintProblem::meta_Member_children.target() = ::TTSched::Member::meta;
		}
		void InitComposition2()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::SearchConfig::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::SearchConfig::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::TTSched::SearchConfig::meta_ConstraintProblem_parent.isNavigable() = true;
			::TTSched::SearchConfig::meta_ConstraintProblem_parent.target() = ::TTSched::ConstraintProblem::meta;
			::TTSched::ConstraintProblem::meta_SearchConfig_child = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::ConstraintProblem::meta_SearchConfig_child.name() = "SearchConfig";
			::TTSched::ConstraintProblem::meta_SearchConfig_child.min() = 0;
			::TTSched::ConstraintProblem::meta_SearchConfig_child.max() = 1;
			::TTSched::ConstraintProblem::meta_SearchConfig_child.isNavigable() = true;
			::TTSched::ConstraintProblem::meta_SearchConfig_child.target() = ::TTSched::SearchConfig::meta;
		}
		void InitComposition3()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Constraint::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Constraint::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::TTSched::Constraint::meta_ConstraintProblem_parent.isNavigable() = true;
			::TTSched::Constraint::meta_ConstraintProblem_parent.target() = ::TTSched::ConstraintProblem::meta;
			::TTSched::ConstraintProblem::meta_Constraint_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::ConstraintProblem::meta_Constraint_children.name() = "Constraint";
			::TTSched::ConstraintProblem::meta_Constraint_children.min() = 0;
			::TTSched::ConstraintProblem::meta_Constraint_children.max() = -1;
			::TTSched::ConstraintProblem::meta_Constraint_children.isNavigable() = true;
			::TTSched::ConstraintProblem::meta_Constraint_children.target() = ::TTSched::Constraint::meta;
		}
		void InitComposition4()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::FDVar::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::FDVar::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::TTSched::FDVar::meta_ConstraintProblem_parent.isNavigable() = true;
			::TTSched::FDVar::meta_ConstraintProblem_parent.target() = ::TTSched::ConstraintProblem::meta;
			::TTSched::ConstraintProblem::meta_FDVar_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::ConstraintProblem::meta_FDVar_children.name() = "FDVar";
			::TTSched::ConstraintProblem::meta_FDVar_children.min() = 0;
			::TTSched::ConstraintProblem::meta_FDVar_children.max() = -1;
			::TTSched::ConstraintProblem::meta_FDVar_children.isNavigable() = true;
			::TTSched::ConstraintProblem::meta_FDVar_children.target() = ::TTSched::FDVar::meta;
		}
		void InitComposition5()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Solution::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Solution::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::TTSched::Solution::meta_ConstraintProblem_parent.isNavigable() = true;
			::TTSched::Solution::meta_ConstraintProblem_parent.target() = ::TTSched::ConstraintProblem::meta;
			::TTSched::ConstraintProblem::meta_Solution_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::ConstraintProblem::meta_Solution_children.name() = "Solution";
			::TTSched::ConstraintProblem::meta_Solution_children.min() = 0;
			::TTSched::ConstraintProblem::meta_Solution_children.max() = -1;
			::TTSched::ConstraintProblem::meta_Solution_children.isNavigable() = true;
			::TTSched::ConstraintProblem::meta_Solution_children.target() = ::TTSched::Solution::meta;
		}
		void InitComposition6()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::YPort::meta_PairConstraint_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::YPort::meta_PairConstraint_parent.name() = "PairConstraint_parent";
			::TTSched::YPort::meta_PairConstraint_parent.isNavigable() = true;
			::TTSched::YPort::meta_PairConstraint_parent.target() = ::TTSched::PairConstraint::meta;
			::TTSched::PairConstraint::meta_YPort_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::PairConstraint::meta_YPort_children.name() = "YPort";
			::TTSched::PairConstraint::meta_YPort_children.min() = 0;
			::TTSched::PairConstraint::meta_YPort_children.max() = -1;
			::TTSched::PairConstraint::meta_YPort_children.isNavigable() = true;
			::TTSched::PairConstraint::meta_YPort_children.target() = ::TTSched::YPort::meta;
		}
		void InitComposition7()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::XPort::meta_PairConstraint_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::XPort::meta_PairConstraint_parent.name() = "PairConstraint_parent";
			::TTSched::XPort::meta_PairConstraint_parent.isNavigable() = true;
			::TTSched::XPort::meta_PairConstraint_parent.target() = ::TTSched::PairConstraint::meta;
			::TTSched::PairConstraint::meta_XPort_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::PairConstraint::meta_XPort_children.name() = "XPort";
			::TTSched::PairConstraint::meta_XPort_children.min() = 0;
			::TTSched::PairConstraint::meta_XPort_children.max() = -1;
			::TTSched::PairConstraint::meta_XPort_children.isNavigable() = true;
			::TTSched::PairConstraint::meta_XPort_children.target() = ::TTSched::XPort::meta;
		}
		void InitComposition8()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Assignment::meta_Solution_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Assignment::meta_Solution_parent.name() = "Solution_parent";
			::TTSched::Assignment::meta_Solution_parent.isNavigable() = true;
			::TTSched::Assignment::meta_Solution_parent.target() = ::TTSched::Solution::meta;
			::TTSched::Solution::meta_Assignment_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Solution::meta_Assignment_children.name() = "Assignment";
			::TTSched::Solution::meta_Assignment_children.min() = 0;
			::TTSched::Solution::meta_Assignment_children.max() = -1;
			::TTSched::Solution::meta_Assignment_children.isNavigable() = true;
			::TTSched::Solution::meta_Assignment_children.target() = ::TTSched::Assignment::meta;
		}
		void InitComposition9()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Latency::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Latency::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::TTSched::Latency::meta_LatencySpec_parent.isNavigable() = true;
			::TTSched::Latency::meta_LatencySpec_parent.target() = ::TTSched::LatencySpec::meta;
			::TTSched::LatencySpec::meta_Latency_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::LatencySpec::meta_Latency_children.name() = "Latency";
			::TTSched::LatencySpec::meta_Latency_children.min() = 0;
			::TTSched::LatencySpec::meta_Latency_children.max() = -1;
			::TTSched::LatencySpec::meta_Latency_children.isNavigable() = true;
			::TTSched::LatencySpec::meta_Latency_children.target() = ::TTSched::Latency::meta;
		}
		void InitComposition10()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::MsgRef::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::MsgRef::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::TTSched::MsgRef::meta_LatencySpec_parent.isNavigable() = true;
			::TTSched::MsgRef::meta_LatencySpec_parent.target() = ::TTSched::LatencySpec::meta;
			::TTSched::LatencySpec::meta_MsgRef_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::LatencySpec::meta_MsgRef_children.name() = "MsgRef";
			::TTSched::LatencySpec::meta_MsgRef_children.min() = 0;
			::TTSched::LatencySpec::meta_MsgRef_children.max() = -1;
			::TTSched::LatencySpec::meta_MsgRef_children.isNavigable() = true;
			::TTSched::LatencySpec::meta_MsgRef_children.target() = ::TTSched::MsgRef::meta;
		}
		void InitComposition11()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::StartMsg::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::StartMsg::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::TTSched::StartMsg::meta_LatencySpec_parent.isNavigable() = true;
			::TTSched::StartMsg::meta_LatencySpec_parent.target() = ::TTSched::LatencySpec::meta;
			::TTSched::LatencySpec::meta_StartMsg_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::LatencySpec::meta_StartMsg_children.name() = "StartMsg";
			::TTSched::LatencySpec::meta_StartMsg_children.min() = 0;
			::TTSched::LatencySpec::meta_StartMsg_children.max() = -1;
			::TTSched::LatencySpec::meta_StartMsg_children.isNavigable() = true;
			::TTSched::LatencySpec::meta_StartMsg_children.target() = ::TTSched::StartMsg::meta;
		}
		void InitComposition12()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::EndMsg::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::EndMsg::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::TTSched::EndMsg::meta_LatencySpec_parent.isNavigable() = true;
			::TTSched::EndMsg::meta_LatencySpec_parent.target() = ::TTSched::LatencySpec::meta;
			::TTSched::LatencySpec::meta_EndMsg_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::LatencySpec::meta_EndMsg_children.name() = "EndMsg";
			::TTSched::LatencySpec::meta_EndMsg_children.min() = 0;
			::TTSched::LatencySpec::meta_EndMsg_children.max() = -1;
			::TTSched::LatencySpec::meta_EndMsg_children.isNavigable() = true;
			::TTSched::LatencySpec::meta_EndMsg_children.target() = ::TTSched::EndMsg::meta;
		}
		void InitComposition13()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Dependency::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Dependency::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::TTSched::Dependency::meta_LatencySpec_parent.isNavigable() = true;
			::TTSched::Dependency::meta_LatencySpec_parent.target() = ::TTSched::LatencySpec::meta;
			::TTSched::LatencySpec::meta_Dependency_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::LatencySpec::meta_Dependency_children.name() = "Dependency";
			::TTSched::LatencySpec::meta_Dependency_children.min() = 0;
			::TTSched::LatencySpec::meta_Dependency_children.max() = -1;
			::TTSched::LatencySpec::meta_Dependency_children.isNavigable() = true;
			::TTSched::LatencySpec::meta_Dependency_children.target() = ::TTSched::Dependency::meta;
		}
		void InitComposition14()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Bus::meta_TTSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Bus::meta_TTSystem_parent.name() = "TTSystem_parent";
			::TTSched::Bus::meta_TTSystem_parent.isNavigable() = true;
			::TTSched::Bus::meta_TTSystem_parent.target() = ::TTSched::TTSystem::meta;
			::TTSched::TTSystem::meta_Bus_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::TTSystem::meta_Bus_children.name() = "Bus";
			::TTSched::TTSystem::meta_Bus_children.min() = 0;
			::TTSched::TTSystem::meta_Bus_children.max() = -1;
			::TTSched::TTSystem::meta_Bus_children.isNavigable() = true;
			::TTSched::TTSystem::meta_Bus_children.target() = ::TTSched::Bus::meta;
		}
		void InitComposition15()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::LatencySpec::meta_TTSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::LatencySpec::meta_TTSystem_parent.name() = "TTSystem_parent";
			::TTSched::LatencySpec::meta_TTSystem_parent.isNavigable() = true;
			::TTSched::LatencySpec::meta_TTSystem_parent.target() = ::TTSched::TTSystem::meta;
			::TTSched::TTSystem::meta_LatencySpec_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::TTSystem::meta_LatencySpec_children.name() = "LatencySpec";
			::TTSched::TTSystem::meta_LatencySpec_children.min() = 0;
			::TTSched::TTSystem::meta_LatencySpec_children.max() = -1;
			::TTSched::TTSystem::meta_LatencySpec_children.isNavigable() = true;
			::TTSched::TTSystem::meta_LatencySpec_children.target() = ::TTSched::LatencySpec::meta;
		}
		void InitComposition16()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Processor::meta_TTSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Processor::meta_TTSystem_parent.name() = "TTSystem_parent";
			::TTSched::Processor::meta_TTSystem_parent.isNavigable() = true;
			::TTSched::Processor::meta_TTSystem_parent.target() = ::TTSched::TTSystem::meta;
			::TTSched::TTSystem::meta_Processor_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::TTSystem::meta_Processor_children.name() = "Processor";
			::TTSched::TTSystem::meta_Processor_children.min() = 0;
			::TTSched::TTSystem::meta_Processor_children.max() = -1;
			::TTSched::TTSystem::meta_Processor_children.isNavigable() = true;
			::TTSched::TTSystem::meta_Processor_children.target() = ::TTSched::Processor::meta;
		}
		void InitComposition17()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Link::meta_TTSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Link::meta_TTSystem_parent.name() = "TTSystem_parent";
			::TTSched::Link::meta_TTSystem_parent.isNavigable() = true;
			::TTSched::Link::meta_TTSystem_parent.target() = ::TTSched::TTSystem::meta;
			::TTSched::TTSystem::meta_Link_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::TTSystem::meta_Link_children.name() = "Link";
			::TTSched::TTSystem::meta_Link_children.min() = 0;
			::TTSched::TTSystem::meta_Link_children.max() = -1;
			::TTSched::TTSystem::meta_Link_children.isNavigable() = true;
			::TTSched::TTSystem::meta_Link_children.target() = ::TTSched::Link::meta;
		}
		void InitComposition18()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::TaskInstances::meta_Processor_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::TaskInstances::meta_Processor_parent.name() = "Processor_parent";
			::TTSched::TaskInstances::meta_Processor_parent.isNavigable() = true;
			::TTSched::TaskInstances::meta_Processor_parent.target() = ::TTSched::Processor::meta;
			::TTSched::Processor::meta_TaskInstances_child = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Processor::meta_TaskInstances_child.name() = "TaskInstances";
			::TTSched::Processor::meta_TaskInstances_child.min() = 0;
			::TTSched::Processor::meta_TaskInstances_child.max() = 1;
			::TTSched::Processor::meta_TaskInstances_child.isNavigable() = true;
			::TTSched::Processor::meta_TaskInstances_child.target() = ::TTSched::TaskInstances::meta;
		}
		void InitComposition19()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Task::meta_Processor_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Task::meta_Processor_parent.name() = "Processor_parent";
			::TTSched::Task::meta_Processor_parent.isNavigable() = true;
			::TTSched::Task::meta_Processor_parent.target() = ::TTSched::Processor::meta;
			::TTSched::Processor::meta_Task_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Processor::meta_Task_children.name() = "Task";
			::TTSched::Processor::meta_Task_children.min() = 0;
			::TTSched::Processor::meta_Task_children.max() = -1;
			::TTSched::Processor::meta_Task_children.isNavigable() = true;
			::TTSched::Processor::meta_Task_children.target() = ::TTSched::Task::meta;
		}
		void InitComposition20()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::TaskRef::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::TaskRef::meta_Bus_parent.name() = "Bus_parent";
			::TTSched::TaskRef::meta_Bus_parent.isNavigable() = true;
			::TTSched::TaskRef::meta_Bus_parent.target() = ::TTSched::Bus::meta;
			::TTSched::Bus::meta_TaskRef_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Bus::meta_TaskRef_children.name() = "TaskRef";
			::TTSched::Bus::meta_TaskRef_children.min() = 0;
			::TTSched::Bus::meta_TaskRef_children.max() = -1;
			::TTSched::Bus::meta_TaskRef_children.isNavigable() = true;
			::TTSched::Bus::meta_TaskRef_children.target() = ::TTSched::TaskRef::meta;
		}
		void InitComposition21()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Receiver::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Receiver::meta_Bus_parent.name() = "Bus_parent";
			::TTSched::Receiver::meta_Bus_parent.isNavigable() = true;
			::TTSched::Receiver::meta_Bus_parent.target() = ::TTSched::Bus::meta;
			::TTSched::Bus::meta_Receiver_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Bus::meta_Receiver_children.name() = "Receiver";
			::TTSched::Bus::meta_Receiver_children.min() = 0;
			::TTSched::Bus::meta_Receiver_children.max() = -1;
			::TTSched::Bus::meta_Receiver_children.isNavigable() = true;
			::TTSched::Bus::meta_Receiver_children.target() = ::TTSched::Receiver::meta;
		}
		void InitComposition22()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Message::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Message::meta_Bus_parent.name() = "Bus_parent";
			::TTSched::Message::meta_Bus_parent.isNavigable() = true;
			::TTSched::Message::meta_Bus_parent.target() = ::TTSched::Bus::meta;
			::TTSched::Bus::meta_Message_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Bus::meta_Message_children.name() = "Message";
			::TTSched::Bus::meta_Message_children.min() = 0;
			::TTSched::Bus::meta_Message_children.max() = -1;
			::TTSched::Bus::meta_Message_children.isNavigable() = true;
			::TTSched::Bus::meta_Message_children.target() = ::TTSched::Message::meta;
		}
		void InitComposition23()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::TimeSync::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::TimeSync::meta_Bus_parent.name() = "Bus_parent";
			::TTSched::TimeSync::meta_Bus_parent.isNavigable() = true;
			::TTSched::TimeSync::meta_Bus_parent.target() = ::TTSched::Bus::meta;
			::TTSched::Bus::meta_TimeSync_child = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Bus::meta_TimeSync_child.name() = "TimeSync";
			::TTSched::Bus::meta_TimeSync_child.min() = 0;
			::TTSched::Bus::meta_TimeSync_child.max() = 1;
			::TTSched::Bus::meta_TimeSync_child.isNavigable() = true;
			::TTSched::Bus::meta_TimeSync_child.target() = ::TTSched::TimeSync::meta;
		}
		void InitComposition24()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Sender::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Sender::meta_Bus_parent.name() = "Bus_parent";
			::TTSched::Sender::meta_Bus_parent.isNavigable() = true;
			::TTSched::Sender::meta_Bus_parent.target() = ::TTSched::Bus::meta;
			::TTSched::Bus::meta_Sender_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Bus::meta_Sender_children.name() = "Sender";
			::TTSched::Bus::meta_Sender_children.min() = 0;
			::TTSched::Bus::meta_Sender_children.max() = -1;
			::TTSched::Bus::meta_Sender_children.isNavigable() = true;
			::TTSched::Bus::meta_Sender_children.target() = ::TTSched::Sender::meta;
		}
		void InitComposition25()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::MsgInstances::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::MsgInstances::meta_Bus_parent.name() = "Bus_parent";
			::TTSched::MsgInstances::meta_Bus_parent.isNavigable() = true;
			::TTSched::MsgInstances::meta_Bus_parent.target() = ::TTSched::Bus::meta;
			::TTSched::Bus::meta_MsgInstances_child = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Bus::meta_MsgInstances_child.name() = "MsgInstances";
			::TTSched::Bus::meta_MsgInstances_child.min() = 0;
			::TTSched::Bus::meta_MsgInstances_child.max() = 1;
			::TTSched::Bus::meta_MsgInstances_child.isNavigable() = true;
			::TTSched::Bus::meta_MsgInstances_child.target() = ::TTSched::MsgInstances::meta;
		}
		void InitComposition26()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Dependencies::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Dependencies::meta_Bus_parent.name() = "Bus_parent";
			::TTSched::Dependencies::meta_Bus_parent.isNavigable() = true;
			::TTSched::Dependencies::meta_Bus_parent.target() = ::TTSched::Bus::meta;
			::TTSched::Bus::meta_Dependencies_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Bus::meta_Dependencies_children.name() = "Dependencies";
			::TTSched::Bus::meta_Dependencies_children.min() = 0;
			::TTSched::Bus::meta_Dependencies_children.max() = -1;
			::TTSched::Bus::meta_Dependencies_children.isNavigable() = true;
			::TTSched::Bus::meta_Dependencies_children.target() = ::TTSched::Dependencies::meta;
		}
		void InitComposition27()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::MsgRef::meta_MsgInstances_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::MsgRef::meta_MsgInstances_parent.name() = "MsgInstances_parent";
			::TTSched::MsgRef::meta_MsgInstances_parent.isNavigable() = true;
			::TTSched::MsgRef::meta_MsgInstances_parent.target() = ::TTSched::MsgInstances::meta;
			::TTSched::MsgInstances::meta_MsgRef_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::MsgInstances::meta_MsgRef_children.name() = "MsgRef";
			::TTSched::MsgInstances::meta_MsgRef_children.min() = 0;
			::TTSched::MsgInstances::meta_MsgRef_children.max() = -1;
			::TTSched::MsgInstances::meta_MsgRef_children.isNavigable() = true;
			::TTSched::MsgInstances::meta_MsgRef_children.target() = ::TTSched::MsgRef::meta;
		}
		void InitComposition28()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::MsgInstance::meta_MsgInstances_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::MsgInstance::meta_MsgInstances_parent.name() = "MsgInstances_parent";
			::TTSched::MsgInstance::meta_MsgInstances_parent.isNavigable() = true;
			::TTSched::MsgInstance::meta_MsgInstances_parent.target() = ::TTSched::MsgInstances::meta;
			::TTSched::MsgInstances::meta_MsgInstance_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::MsgInstances::meta_MsgInstance_children.name() = "MsgInstance";
			::TTSched::MsgInstances::meta_MsgInstance_children.min() = 0;
			::TTSched::MsgInstances::meta_MsgInstance_children.max() = -1;
			::TTSched::MsgInstances::meta_MsgInstance_children.isNavigable() = true;
			::TTSched::MsgInstances::meta_MsgInstance_children.target() = ::TTSched::MsgInstance::meta;
		}
		void InitComposition29()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::SenderInst::meta_Dependencies_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::SenderInst::meta_Dependencies_parent.name() = "Dependencies_parent";
			::TTSched::SenderInst::meta_Dependencies_parent.isNavigable() = true;
			::TTSched::SenderInst::meta_Dependencies_parent.target() = ::TTSched::Dependencies::meta;
			::TTSched::Dependencies::meta_SenderInst_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Dependencies::meta_SenderInst_children.name() = "SenderInst";
			::TTSched::Dependencies::meta_SenderInst_children.min() = 0;
			::TTSched::Dependencies::meta_SenderInst_children.max() = -1;
			::TTSched::Dependencies::meta_SenderInst_children.isNavigable() = true;
			::TTSched::Dependencies::meta_SenderInst_children.target() = ::TTSched::SenderInst::meta;
		}
		void InitComposition30()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::ReceiverInst::meta_Dependencies_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::ReceiverInst::meta_Dependencies_parent.name() = "Dependencies_parent";
			::TTSched::ReceiverInst::meta_Dependencies_parent.isNavigable() = true;
			::TTSched::ReceiverInst::meta_Dependencies_parent.target() = ::TTSched::Dependencies::meta;
			::TTSched::Dependencies::meta_ReceiverInst_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Dependencies::meta_ReceiverInst_children.name() = "ReceiverInst";
			::TTSched::Dependencies::meta_ReceiverInst_children.min() = 0;
			::TTSched::Dependencies::meta_ReceiverInst_children.max() = -1;
			::TTSched::Dependencies::meta_ReceiverInst_children.isNavigable() = true;
			::TTSched::Dependencies::meta_ReceiverInst_children.target() = ::TTSched::ReceiverInst::meta;
		}
		void InitComposition31()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::MsgInstRef::meta_Dependencies_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::MsgInstRef::meta_Dependencies_parent.name() = "Dependencies_parent";
			::TTSched::MsgInstRef::meta_Dependencies_parent.isNavigable() = true;
			::TTSched::MsgInstRef::meta_Dependencies_parent.target() = ::TTSched::Dependencies::meta;
			::TTSched::Dependencies::meta_MsgInstRef_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Dependencies::meta_MsgInstRef_children.name() = "MsgInstRef";
			::TTSched::Dependencies::meta_MsgInstRef_children.min() = 0;
			::TTSched::Dependencies::meta_MsgInstRef_children.max() = -1;
			::TTSched::Dependencies::meta_MsgInstRef_children.isNavigable() = true;
			::TTSched::Dependencies::meta_MsgInstRef_children.target() = ::TTSched::MsgInstRef::meta;
		}
		void InitComposition32()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::TaskInstRef::meta_Dependencies_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::TaskInstRef::meta_Dependencies_parent.name() = "Dependencies_parent";
			::TTSched::TaskInstRef::meta_Dependencies_parent.isNavigable() = true;
			::TTSched::TaskInstRef::meta_Dependencies_parent.target() = ::TTSched::Dependencies::meta;
			::TTSched::Dependencies::meta_TaskInstRef_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::Dependencies::meta_TaskInstRef_children.name() = "TaskInstRef";
			::TTSched::Dependencies::meta_TaskInstRef_children.min() = 0;
			::TTSched::Dependencies::meta_TaskInstRef_children.max() = -1;
			::TTSched::Dependencies::meta_TaskInstRef_children.isNavigable() = true;
			::TTSched::Dependencies::meta_TaskInstRef_children.target() = ::TTSched::TaskInstRef::meta;
		}
		void InitComposition33()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Bus::meta_InstanceModel_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Bus::meta_InstanceModel_parent.name() = "InstanceModel_parent";
			::TTSched::Bus::meta_InstanceModel_parent.isNavigable() = true;
			::TTSched::Bus::meta_InstanceModel_parent.target() = ::TTSched::InstanceModel::meta;
			::TTSched::InstanceModel::meta_Bus_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::InstanceModel::meta_Bus_children.name() = "Bus";
			::TTSched::InstanceModel::meta_Bus_children.min() = 0;
			::TTSched::InstanceModel::meta_Bus_children.max() = -1;
			::TTSched::InstanceModel::meta_Bus_children.isNavigable() = true;
			::TTSched::InstanceModel::meta_Bus_children.target() = ::TTSched::Bus::meta;
		}
		void InitComposition34()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::Processor::meta_InstanceModel_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::Processor::meta_InstanceModel_parent.name() = "InstanceModel_parent";
			::TTSched::Processor::meta_InstanceModel_parent.isNavigable() = true;
			::TTSched::Processor::meta_InstanceModel_parent.target() = ::TTSched::InstanceModel::meta;
			::TTSched::InstanceModel::meta_Processor_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::InstanceModel::meta_Processor_children.name() = "Processor";
			::TTSched::InstanceModel::meta_Processor_children.min() = 0;
			::TTSched::InstanceModel::meta_Processor_children.max() = -1;
			::TTSched::InstanceModel::meta_Processor_children.isNavigable() = true;
			::TTSched::InstanceModel::meta_Processor_children.target() = ::TTSched::Processor::meta;
		}
		void InitComposition35()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::TaskRef::meta_TaskInstances_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::TaskRef::meta_TaskInstances_parent.name() = "TaskInstances_parent";
			::TTSched::TaskRef::meta_TaskInstances_parent.isNavigable() = true;
			::TTSched::TaskRef::meta_TaskInstances_parent.target() = ::TTSched::TaskInstances::meta;
			::TTSched::TaskInstances::meta_TaskRef_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::TaskInstances::meta_TaskRef_children.name() = "TaskRef";
			::TTSched::TaskInstances::meta_TaskRef_children.min() = 0;
			::TTSched::TaskInstances::meta_TaskRef_children.max() = -1;
			::TTSched::TaskInstances::meta_TaskRef_children.isNavigable() = true;
			::TTSched::TaskInstances::meta_TaskRef_children.target() = ::TTSched::TaskRef::meta;
		}
		void InitComposition36()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::TaskInstance::meta_TaskInstances_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::TaskInstance::meta_TaskInstances_parent.name() = "TaskInstances_parent";
			::TTSched::TaskInstance::meta_TaskInstances_parent.isNavigable() = true;
			::TTSched::TaskInstance::meta_TaskInstances_parent.target() = ::TTSched::TaskInstances::meta;
			::TTSched::TaskInstances::meta_TaskInstance_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::TaskInstances::meta_TaskInstance_children.name() = "TaskInstance";
			::TTSched::TaskInstances::meta_TaskInstance_children.min() = 0;
			::TTSched::TaskInstances::meta_TaskInstance_children.max() = -1;
			::TTSched::TaskInstances::meta_TaskInstance_children.isNavigable() = true;
			::TTSched::TaskInstances::meta_TaskInstance_children.target() = ::TTSched::TaskInstance::meta;
		}
		void InitComposition37()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::TTSystem::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::TTSystem::meta_RootFolder_parent.name() = "RootFolder_parent";
			::TTSched::TTSystem::meta_RootFolder_parent.isNavigable() = true;
			::TTSched::TTSystem::meta_RootFolder_parent.target() = ::TTSched::RootFolder::meta;
			::TTSched::RootFolder::meta_TTSystem_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::RootFolder::meta_TTSystem_children.name() = "TTSystem";
			::TTSched::RootFolder::meta_TTSystem_children.min() = 0;
			::TTSched::RootFolder::meta_TTSystem_children.max() = -1;
			::TTSched::RootFolder::meta_TTSystem_children.isNavigable() = true;
			::TTSched::RootFolder::meta_TTSystem_children.target() = ::TTSched::TTSystem::meta;
		}
		void InitComposition38()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::ConstraintProblem::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::ConstraintProblem::meta_RootFolder_parent.name() = "RootFolder_parent";
			::TTSched::ConstraintProblem::meta_RootFolder_parent.isNavigable() = true;
			::TTSched::ConstraintProblem::meta_RootFolder_parent.target() = ::TTSched::RootFolder::meta;
			::TTSched::RootFolder::meta_ConstraintProblem_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::RootFolder::meta_ConstraintProblem_children.name() = "ConstraintProblem";
			::TTSched::RootFolder::meta_ConstraintProblem_children.min() = 0;
			::TTSched::RootFolder::meta_ConstraintProblem_children.max() = -1;
			::TTSched::RootFolder::meta_ConstraintProblem_children.isNavigable() = true;
			::TTSched::RootFolder::meta_ConstraintProblem_children.target() = ::TTSched::ConstraintProblem::meta;
		}
		void InitComposition39()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::InstanceModel::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::InstanceModel::meta_RootFolder_parent.name() = "RootFolder_parent";
			::TTSched::InstanceModel::meta_RootFolder_parent.isNavigable() = true;
			::TTSched::InstanceModel::meta_RootFolder_parent.target() = ::TTSched::RootFolder::meta;
			::TTSched::RootFolder::meta_InstanceModel_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::RootFolder::meta_InstanceModel_children.name() = "InstanceModel";
			::TTSched::RootFolder::meta_InstanceModel_children.min() = 0;
			::TTSched::RootFolder::meta_InstanceModel_children.max() = -1;
			::TTSched::RootFolder::meta_InstanceModel_children.isNavigable() = true;
			::TTSched::RootFolder::meta_InstanceModel_children.target() = ::TTSched::InstanceModel::meta;
		}
		void InitComposition40()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::TTSched::RootFolder::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::TTSched::RootFolder::meta_RootFolder_parent.name() = "RootFolder_parent";
			::TTSched::RootFolder::meta_RootFolder_parent.isNavigable() = true;
			::TTSched::RootFolder::meta_RootFolder_parent.target() = ::TTSched::RootFolder::meta;
			::TTSched::RootFolder::meta_RootFolder_children = ::Uml::CompositionChildRole::Create(comp);
			::TTSched::RootFolder::meta_RootFolder_children.name() = "RootFolder";
			::TTSched::RootFolder::meta_RootFolder_children.min() = 0;
			::TTSched::RootFolder::meta_RootFolder_children.max() = -1;
			::TTSched::RootFolder::meta_RootFolder_children.isNavigable() = true;
			::TTSched::RootFolder::meta_RootFolder_children.target() = ::TTSched::RootFolder::meta;
		}
		void InitCrossNSCompositions()
		{
		}
		void InitInheritence()
		{
			Constraint::meta.subTypes() +=  ::TTSched::SerialConstraint::meta;
			Constraint::meta.subTypes() +=  ::TTSched::RangeConstraint::meta;
			Constraint::meta.subTypes() +=  ::TTSched::PairConstraint::meta;
			PairConstraint::meta.subTypes() +=  ::TTSched::EqualityConstraint::meta;
			PairConstraint::meta.subTypes() +=  ::TTSched::InequalityConstraint::meta;
			Connectable::meta.subTypes() +=  ::TTSched::SerialConstraint::meta;
			Connectable::meta.subTypes() +=  ::TTSched::RangeConstraint::meta;
			Connectable::meta.subTypes() +=  ::TTSched::YPort::meta;
			Connectable::meta.subTypes() +=  ::TTSched::XPort::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::ConstraintProblem::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::FDVar::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Constraint::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Member::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Solution::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Connectable::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Assignment::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::InstanceRef::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::SearchConfig::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Task::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::StartMsg::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::LatencySpec::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::TTSystem::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Sender::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::EndMsg::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Receiver::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Dependency::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Link::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Message::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::TimeSync::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Latency::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::MsgRef::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::TaskRef::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Processor::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Bus::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::MsgInstances::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::SenderInst::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::Dependencies::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::MsgInstance::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::TaskInstRef::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::InstanceModel::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::MsgInstRef::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::TaskInstances::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::TaskInstance::meta;
			MgaObject::meta.subTypes() +=  ::TTSched::ReceiverInst::meta;
		}
		void InitCrossNSInheritence()
		{
		}
	 void InitializeDgr()
	{
		Creates();
		InitClassesAttributes();
		CreatesNamespaces();
		InitNamespaces();
			InitAssociation0();
			InitAssociation1();
			InitAssociation2();
			InitAssociation3();
			InitAssociation4();
			InitAssociation5();
			InitAssociation6();
			InitAssociation7();
			InitAssociation8();
			InitAssociation9();
			InitAssociation10();
			InitAssociation11();
			InitAssociation12();
			InitAssociation13();
		InitComposition0();
		InitComposition1();
		InitComposition2();
		InitComposition3();
		InitComposition4();
		InitComposition5();
		InitComposition6();
		InitComposition7();
		InitComposition8();
		InitComposition9();
		InitComposition10();
		InitComposition11();
		InitComposition12();
		InitComposition13();
		InitComposition14();
		InitComposition15();
		InitComposition16();
		InitComposition17();
		InitComposition18();
		InitComposition19();
		InitComposition20();
		InitComposition21();
		InitComposition22();
		InitComposition23();
		InitComposition24();
		InitComposition25();
		InitComposition26();
		InitComposition27();
		InitComposition28();
		InitComposition29();
		InitComposition30();
		InitComposition31();
		InitComposition32();
		InitComposition33();
		InitComposition34();
		InitComposition35();
		InitComposition36();
		InitComposition37();
		InitComposition38();
		InitComposition39();
		InitComposition40();
		InitInheritence();
		
	}
		 void InitializeDgr(const ::Uml::Diagram & dgr)
		{
			::Uml::SetClass(ConstraintProblem::meta, dgr, "ConstraintProblem");
			::Uml::SetAttribute(ConstraintProblem::meta_UpperBound,ConstraintProblem::meta, "UpperBound");
			::Uml::SetAttribute(ConstraintProblem::meta_LowerBound,ConstraintProblem::meta, "LowerBound");
			::Uml::SetClass(FDVar::meta, dgr, "FDVar");
			::Uml::SetAttribute(FDVar::meta_UpperBound,FDVar::meta, "UpperBound");
			::Uml::SetAttribute(FDVar::meta_LowerBound,FDVar::meta, "LowerBound");
			::Uml::SetAttribute(FDVar::meta_Size,FDVar::meta, "Size");
			::Uml::SetClass(Constraint::meta, dgr, "Constraint");
			::Uml::SetClass(EqualityConstraint::meta, dgr, "EqualityConstraint");
			::Uml::SetAttribute(EqualityConstraint::meta_Offset,EqualityConstraint::meta, "Offset");
			::Uml::SetClass(InequalityConstraint::meta, dgr, "InequalityConstraint");
			::Uml::SetAttribute(InequalityConstraint::meta_Offset,InequalityConstraint::meta, "Offset");
			::Uml::SetClass(SerialConstraint::meta, dgr, "SerialConstraint");
			::Uml::SetClass(Member::meta, dgr, "Member");
			::Uml::SetClass(RangeConstraint::meta, dgr, "RangeConstraint");
			::Uml::SetAttribute(RangeConstraint::meta_UpperBound,RangeConstraint::meta, "UpperBound");
			::Uml::SetAttribute(RangeConstraint::meta_LowerBound,RangeConstraint::meta, "LowerBound");
			::Uml::SetClass(YPort::meta, dgr, "YPort");
			::Uml::SetClass(XPort::meta, dgr, "XPort");
			::Uml::SetClass(PairConstraint::meta, dgr, "PairConstraint");
			::Uml::SetClass(Solution::meta, dgr, "Solution");
			::Uml::SetClass(Connectable::meta, dgr, "Connectable");
			::Uml::SetClass(Assignment::meta, dgr, "Assignment");
			::Uml::SetAttribute(Assignment::meta_Value,Assignment::meta, "Value");
			::Uml::SetClass(InstanceRef::meta, dgr, "InstanceRef");
			::Uml::SetClass(SearchConfig::meta, dgr, "SearchConfig");
			::Uml::SetAttribute(SearchConfig::meta_MaxRunTimeSec,SearchConfig::meta, "MaxRunTimeSec");
			::Uml::SetAttribute(SearchConfig::meta_MaxFailures,SearchConfig::meta, "MaxFailures");
			::Uml::SetAttribute(SearchConfig::meta_SearchEngine,SearchConfig::meta, "SearchEngine");
			::Uml::SetAttribute(SearchConfig::meta_MaxSolutions,SearchConfig::meta, "MaxSolutions");
			::Uml::SetClass(Task::meta, dgr, "Task");
			::Uml::SetAttribute(Task::meta_WCExecTime,Task::meta, "WCExecTime");
			::Uml::SetAttribute(Task::meta_Period,Task::meta, "Period");
			::Uml::SetClass(StartMsg::meta, dgr, "StartMsg");
			::Uml::SetClass(LatencySpec::meta, dgr, "LatencySpec");
			::Uml::SetClass(TTSystem::meta, dgr, "TTSystem");
			::Uml::SetClass(Sender::meta, dgr, "Sender");
			::Uml::SetClass(EndMsg::meta, dgr, "EndMsg");
			::Uml::SetClass(Receiver::meta, dgr, "Receiver");
			::Uml::SetClass(Dependency::meta, dgr, "Dependency");
			::Uml::SetClass(Link::meta, dgr, "Link");
			::Uml::SetClass(Message::meta, dgr, "Message");
			::Uml::SetAttribute(Message::meta_MsgLength,Message::meta, "MsgLength");
			::Uml::SetClass(TimeSync::meta, dgr, "TimeSync");
			::Uml::SetAttribute(TimeSync::meta_MsgLength,TimeSync::meta, "MsgLength");
			::Uml::SetAttribute(TimeSync::meta_Period,TimeSync::meta, "Period");
			::Uml::SetClass(Latency::meta, dgr, "Latency");
			::Uml::SetAttribute(Latency::meta_MaxLength,Latency::meta, "MaxLength");
			::Uml::SetClass(MsgRef::meta, dgr, "MsgRef");
			::Uml::SetClass(TaskRef::meta, dgr, "TaskRef");
			::Uml::SetClass(Processor::meta, dgr, "Processor");
			::Uml::SetClass(Bus::meta, dgr, "Bus");
			::Uml::SetAttribute(Bus::meta_Resolution,Bus::meta, "Resolution");
			::Uml::SetClass(MsgInstances::meta, dgr, "MsgInstances");
			::Uml::SetClass(SenderInst::meta, dgr, "SenderInst");
			::Uml::SetClass(Dependencies::meta, dgr, "Dependencies");
			::Uml::SetClass(MsgInstance::meta, dgr, "MsgInstance");
			::Uml::SetAttribute(MsgInstance::meta_RefName,MsgInstance::meta, "RefName");
			::Uml::SetAttribute(MsgInstance::meta_StartTime,MsgInstance::meta, "StartTime");
			::Uml::SetAttribute(MsgInstance::meta_InstID,MsgInstance::meta, "InstID");
			::Uml::SetClass(TaskInstRef::meta, dgr, "TaskInstRef");
			::Uml::SetClass(InstanceModel::meta, dgr, "InstanceModel");
			::Uml::SetAttribute(InstanceModel::meta_HyperPeriod,InstanceModel::meta, "HyperPeriod");
			::Uml::SetClass(MsgInstRef::meta, dgr, "MsgInstRef");
			::Uml::SetClass(TaskInstances::meta, dgr, "TaskInstances");
			::Uml::SetClass(TaskInstance::meta, dgr, "TaskInstance");
			::Uml::SetAttribute(TaskInstance::meta_RefName,TaskInstance::meta, "RefName");
			::Uml::SetAttribute(TaskInstance::meta_StartTime,TaskInstance::meta, "StartTime");
			::Uml::SetAttribute(TaskInstance::meta_InstID,TaskInstance::meta, "InstID");
			::Uml::SetClass(ReceiverInst::meta, dgr, "ReceiverInst");
			::Uml::SetClass(RootFolder::meta, dgr, "RootFolder");
			::Uml::SetAttribute(RootFolder::meta_name,RootFolder::meta, "name");
			::Uml::SetClass(MgaObject::meta, dgr, "MgaObject");
			::Uml::SetAttribute(MgaObject::meta_position,MgaObject::meta, "position");
			::Uml::SetAttribute(MgaObject::meta_name,MgaObject::meta, "name");
			//composition child roles 
			::Uml::SetParentRole(ConstraintProblem::meta_RootFolder_parent,ConstraintProblem::meta,::TTSched::RootFolder::meta, "", "");
			::Uml::SetParentRole(FDVar::meta_ConstraintProblem_parent,FDVar::meta,::TTSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(Constraint::meta_ConstraintProblem_parent,Constraint::meta,::TTSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(Member::meta_ConstraintProblem_parent,Member::meta,::TTSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(YPort::meta_PairConstraint_parent,YPort::meta,::TTSched::PairConstraint::meta, "", "");
			::Uml::SetParentRole(XPort::meta_PairConstraint_parent,XPort::meta,::TTSched::PairConstraint::meta, "", "");
			::Uml::SetParentRole(Solution::meta_ConstraintProblem_parent,Solution::meta,::TTSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(Assignment::meta_Solution_parent,Assignment::meta,::TTSched::Solution::meta, "", "");
			::Uml::SetParentRole(InstanceRef::meta_ConstraintProblem_parent,InstanceRef::meta,::TTSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(SearchConfig::meta_ConstraintProblem_parent,SearchConfig::meta,::TTSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(Task::meta_Processor_parent,Task::meta,::TTSched::Processor::meta, "", "");
			::Uml::SetParentRole(StartMsg::meta_LatencySpec_parent,StartMsg::meta,::TTSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(LatencySpec::meta_TTSystem_parent,LatencySpec::meta,::TTSched::TTSystem::meta, "", "");
			::Uml::SetParentRole(TTSystem::meta_RootFolder_parent,TTSystem::meta,::TTSched::RootFolder::meta, "", "");
			::Uml::SetParentRole(Sender::meta_Bus_parent,Sender::meta,::TTSched::Bus::meta, "", "");
			::Uml::SetParentRole(EndMsg::meta_LatencySpec_parent,EndMsg::meta,::TTSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(Receiver::meta_Bus_parent,Receiver::meta,::TTSched::Bus::meta, "", "");
			::Uml::SetParentRole(Dependency::meta_LatencySpec_parent,Dependency::meta,::TTSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(Link::meta_TTSystem_parent,Link::meta,::TTSched::TTSystem::meta, "", "");
			::Uml::SetParentRole(Message::meta_Bus_parent,Message::meta,::TTSched::Bus::meta, "", "");
			::Uml::SetParentRole(TimeSync::meta_Bus_parent,TimeSync::meta,::TTSched::Bus::meta, "", "");
			::Uml::SetParentRole(Latency::meta_LatencySpec_parent,Latency::meta,::TTSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(MsgRef::meta_LatencySpec_parent,MsgRef::meta,::TTSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(MsgRef::meta_MsgInstances_parent,MsgRef::meta,::TTSched::MsgInstances::meta, "", "");
			::Uml::SetParentRole(TaskRef::meta_TaskInstances_parent,TaskRef::meta,::TTSched::TaskInstances::meta, "", "");
			::Uml::SetParentRole(TaskRef::meta_Bus_parent,TaskRef::meta,::TTSched::Bus::meta, "", "");
			::Uml::SetParentRole(Processor::meta_TTSystem_parent,Processor::meta,::TTSched::TTSystem::meta, "", "");
			::Uml::SetParentRole(Processor::meta_InstanceModel_parent,Processor::meta,::TTSched::InstanceModel::meta, "", "");
			::Uml::SetParentRole(Bus::meta_TTSystem_parent,Bus::meta,::TTSched::TTSystem::meta, "", "");
			::Uml::SetParentRole(Bus::meta_InstanceModel_parent,Bus::meta,::TTSched::InstanceModel::meta, "", "");
			::Uml::SetParentRole(MsgInstances::meta_Bus_parent,MsgInstances::meta,::TTSched::Bus::meta, "", "");
			::Uml::SetParentRole(SenderInst::meta_Dependencies_parent,SenderInst::meta,::TTSched::Dependencies::meta, "", "");
			::Uml::SetParentRole(Dependencies::meta_Bus_parent,Dependencies::meta,::TTSched::Bus::meta, "", "");
			::Uml::SetParentRole(MsgInstance::meta_MsgInstances_parent,MsgInstance::meta,::TTSched::MsgInstances::meta, "", "");
			::Uml::SetParentRole(TaskInstRef::meta_Dependencies_parent,TaskInstRef::meta,::TTSched::Dependencies::meta, "", "");
			::Uml::SetParentRole(InstanceModel::meta_RootFolder_parent,InstanceModel::meta,::TTSched::RootFolder::meta, "", "");
			::Uml::SetParentRole(MsgInstRef::meta_Dependencies_parent,MsgInstRef::meta,::TTSched::Dependencies::meta, "", "");
			::Uml::SetParentRole(TaskInstances::meta_Processor_parent,TaskInstances::meta,::TTSched::Processor::meta, "", "");
			::Uml::SetParentRole(TaskInstance::meta_TaskInstances_parent,TaskInstance::meta,::TTSched::TaskInstances::meta, "", "");
			::Uml::SetParentRole(ReceiverInst::meta_Dependencies_parent,ReceiverInst::meta,::TTSched::Dependencies::meta, "", "");
			::Uml::SetParentRole(RootFolder::meta_RootFolder_parent,RootFolder::meta,::TTSched::RootFolder::meta, "", "");
			// composition parentroles 
			::Uml::SetChildRole(ConstraintProblem::meta_InstanceRef_child,ConstraintProblem::meta,::TTSched::InstanceRef::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_Member_children,ConstraintProblem::meta,::TTSched::Member::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_SearchConfig_child,ConstraintProblem::meta,::TTSched::SearchConfig::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_Constraint_children,ConstraintProblem::meta,::TTSched::Constraint::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_FDVar_children,ConstraintProblem::meta,::TTSched::FDVar::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_Solution_children,ConstraintProblem::meta,::TTSched::Solution::meta, "", "");
			::Uml::SetChildRole(PairConstraint::meta_YPort_children,PairConstraint::meta,::TTSched::YPort::meta, "", "");
			::Uml::SetChildRole(PairConstraint::meta_XPort_children,PairConstraint::meta,::TTSched::XPort::meta, "", "");
			::Uml::SetChildRole(Solution::meta_Assignment_children,Solution::meta,::TTSched::Assignment::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_Latency_children,LatencySpec::meta,::TTSched::Latency::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_MsgRef_children,LatencySpec::meta,::TTSched::MsgRef::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_StartMsg_children,LatencySpec::meta,::TTSched::StartMsg::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_EndMsg_children,LatencySpec::meta,::TTSched::EndMsg::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_Dependency_children,LatencySpec::meta,::TTSched::Dependency::meta, "", "");
			::Uml::SetChildRole(TTSystem::meta_Bus_children,TTSystem::meta,::TTSched::Bus::meta, "", "");
			::Uml::SetChildRole(TTSystem::meta_LatencySpec_children,TTSystem::meta,::TTSched::LatencySpec::meta, "", "");
			::Uml::SetChildRole(TTSystem::meta_Processor_children,TTSystem::meta,::TTSched::Processor::meta, "", "");
			::Uml::SetChildRole(TTSystem::meta_Link_children,TTSystem::meta,::TTSched::Link::meta, "", "");
			::Uml::SetChildRole(Processor::meta_TaskInstances_child,Processor::meta,::TTSched::TaskInstances::meta, "", "");
			::Uml::SetChildRole(Processor::meta_Task_children,Processor::meta,::TTSched::Task::meta, "", "");
			::Uml::SetChildRole(Bus::meta_TaskRef_children,Bus::meta,::TTSched::TaskRef::meta, "", "");
			::Uml::SetChildRole(Bus::meta_Receiver_children,Bus::meta,::TTSched::Receiver::meta, "", "");
			::Uml::SetChildRole(Bus::meta_Message_children,Bus::meta,::TTSched::Message::meta, "", "");
			::Uml::SetChildRole(Bus::meta_TimeSync_child,Bus::meta,::TTSched::TimeSync::meta, "", "");
			::Uml::SetChildRole(Bus::meta_Sender_children,Bus::meta,::TTSched::Sender::meta, "", "");
			::Uml::SetChildRole(Bus::meta_MsgInstances_child,Bus::meta,::TTSched::MsgInstances::meta, "", "");
			::Uml::SetChildRole(Bus::meta_Dependencies_children,Bus::meta,::TTSched::Dependencies::meta, "", "");
			::Uml::SetChildRole(MsgInstances::meta_MsgRef_children,MsgInstances::meta,::TTSched::MsgRef::meta, "", "");
			::Uml::SetChildRole(MsgInstances::meta_MsgInstance_children,MsgInstances::meta,::TTSched::MsgInstance::meta, "", "");
			::Uml::SetChildRole(Dependencies::meta_SenderInst_children,Dependencies::meta,::TTSched::SenderInst::meta, "", "");
			::Uml::SetChildRole(Dependencies::meta_ReceiverInst_children,Dependencies::meta,::TTSched::ReceiverInst::meta, "", "");
			::Uml::SetChildRole(Dependencies::meta_MsgInstRef_children,Dependencies::meta,::TTSched::MsgInstRef::meta, "", "");
			::Uml::SetChildRole(Dependencies::meta_TaskInstRef_children,Dependencies::meta,::TTSched::TaskInstRef::meta, "", "");
			::Uml::SetChildRole(InstanceModel::meta_Bus_children,InstanceModel::meta,::TTSched::Bus::meta, "", "");
			::Uml::SetChildRole(InstanceModel::meta_Processor_children,InstanceModel::meta,::TTSched::Processor::meta, "", "");
			::Uml::SetChildRole(TaskInstances::meta_TaskRef_children,TaskInstances::meta,::TTSched::TaskRef::meta, "", "");
			::Uml::SetChildRole(TaskInstances::meta_TaskInstance_children,TaskInstances::meta,::TTSched::TaskInstance::meta, "", "");
			::Uml::SetChildRole(RootFolder::meta_TTSystem_children,RootFolder::meta,::TTSched::TTSystem::meta, "", "");
			::Uml::SetChildRole(RootFolder::meta_ConstraintProblem_children,RootFolder::meta,::TTSched::ConstraintProblem::meta, "", "");
			::Uml::SetChildRole(RootFolder::meta_InstanceModel_children,RootFolder::meta,::TTSched::InstanceModel::meta, "", "");
			::Uml::SetChildRole(RootFolder::meta_RootFolder_children,RootFolder::meta,::TTSched::RootFolder::meta, "", "");
			// Association roles 
			::Uml::SetAssocRole(FDVar::meta_dstMember,FDVar::meta,::TTSched::Connectable::meta, "srcMember");
			::TTSched::Connectable::meta_srcMember_rev = FDVar::meta_dstMember;
			::Uml::SetAssocRole(Connectable::meta_srcMember,Connectable::meta,::TTSched::FDVar::meta, "dstMember");
			::TTSched::FDVar::meta_dstMember_rev = Connectable::meta_srcMember;
			::Uml::SetAssocRole(InstanceRef::meta_ref,InstanceRef::meta,::TTSched::InstanceModel::meta, "referedbyInstanceRef");
			::Uml::SetAssocRole(Task::meta_referedbyTaskRef,Task::meta,::TTSched::TaskRef::meta, "ref");
			::Uml::SetAssocRole(Message::meta_referedbyMsgRef,Message::meta,::TTSched::MsgRef::meta, "ref");
			::Uml::SetAssocRole(Message::meta_dstReceiver,Message::meta,::TTSched::TaskRef::meta, "srcReceiver");
			::TTSched::TaskRef::meta_srcReceiver_rev = Message::meta_dstReceiver;
			::Uml::SetAssocRole(Message::meta_srcSender,Message::meta,::TTSched::TaskRef::meta, "dstSender");
			::TTSched::TaskRef::meta_dstSender_rev = Message::meta_srcSender;
			::Uml::SetAssocRole(Latency::meta_srcEndMsg,Latency::meta,::TTSched::MsgRef::meta, "dstEndMsg");
			::TTSched::MsgRef::meta_dstEndMsg_rev = Latency::meta_srcEndMsg;
			::Uml::SetAssocRole(Latency::meta_dstStartMsg,Latency::meta,::TTSched::MsgRef::meta, "srcStartMsg");
			::TTSched::MsgRef::meta_srcStartMsg_rev = Latency::meta_dstStartMsg;
			::Uml::SetAssocRole(MsgRef::meta_ref,MsgRef::meta,::TTSched::Message::meta, "referedbyMsgRef");
			::Uml::SetAssocRole(MsgRef::meta_dstEndMsg,MsgRef::meta,::TTSched::Latency::meta, "srcEndMsg");
			::TTSched::Latency::meta_srcEndMsg_rev = MsgRef::meta_dstEndMsg;
			::Uml::SetAssocRole(MsgRef::meta_srcStartMsg,MsgRef::meta,::TTSched::Latency::meta, "dstStartMsg");
			::TTSched::Latency::meta_dstStartMsg_rev = MsgRef::meta_srcStartMsg;
			::Uml::SetAssocRole(MsgRef::meta_dstDependency,MsgRef::meta,::TTSched::MsgRef::meta, "srcDependency");
			::TTSched::MsgRef::meta_srcDependency_rev = MsgRef::meta_dstDependency;
			::Uml::SetAssocRole(MsgRef::meta_srcDependency,MsgRef::meta,::TTSched::MsgRef::meta, "dstDependency");
			::TTSched::MsgRef::meta_dstDependency_rev = MsgRef::meta_srcDependency;
			::Uml::SetAssocRole(TaskRef::meta_ref,TaskRef::meta,::TTSched::Task::meta, "referedbyTaskRef");
			::Uml::SetAssocRole(TaskRef::meta_srcReceiver,TaskRef::meta,::TTSched::Message::meta, "dstReceiver");
			::TTSched::Message::meta_dstReceiver_rev = TaskRef::meta_srcReceiver;
			::Uml::SetAssocRole(TaskRef::meta_dstSender,TaskRef::meta,::TTSched::Message::meta, "srcSender");
			::TTSched::Message::meta_srcSender_rev = TaskRef::meta_dstSender;
			::Uml::SetAssocRole(Processor::meta_dstLink,Processor::meta,::TTSched::Bus::meta, "srcLink");
			::TTSched::Bus::meta_srcLink_rev = Processor::meta_dstLink;
			::Uml::SetAssocRole(Bus::meta_srcLink,Bus::meta,::TTSched::Processor::meta, "dstLink");
			::TTSched::Processor::meta_dstLink_rev = Bus::meta_srcLink;
			::Uml::SetAssocRole(MsgInstance::meta_referedbyMsgInstRef,MsgInstance::meta,::TTSched::MsgInstRef::meta, "ref");
			::Uml::SetAssocRole(TaskInstRef::meta_ref,TaskInstRef::meta,::TTSched::TaskInstance::meta, "referedbyTaskInstRef");
			::Uml::SetAssocRole(TaskInstRef::meta_dstSenderInst,TaskInstRef::meta,::TTSched::MsgInstRef::meta, "srcSenderInst");
			::TTSched::MsgInstRef::meta_srcSenderInst_rev = TaskInstRef::meta_dstSenderInst;
			::Uml::SetAssocRole(TaskInstRef::meta_srcReceiverInst,TaskInstRef::meta,::TTSched::MsgInstRef::meta, "dstReceiverInst");
			::TTSched::MsgInstRef::meta_dstReceiverInst_rev = TaskInstRef::meta_srcReceiverInst;
			::Uml::SetAssocRole(InstanceModel::meta_referedbyInstanceRef,InstanceModel::meta,::TTSched::InstanceRef::meta, "ref");
			::Uml::SetAssocRole(MsgInstRef::meta_ref,MsgInstRef::meta,::TTSched::MsgInstance::meta, "referedbyMsgInstRef");
			::Uml::SetAssocRole(MsgInstRef::meta_srcSenderInst,MsgInstRef::meta,::TTSched::TaskInstRef::meta, "dstSenderInst");
			::TTSched::TaskInstRef::meta_dstSenderInst_rev = MsgInstRef::meta_srcSenderInst;
			::Uml::SetAssocRole(MsgInstRef::meta_dstReceiverInst,MsgInstRef::meta,::TTSched::TaskInstRef::meta, "srcReceiverInst");
			::TTSched::TaskInstRef::meta_srcReceiverInst_rev = MsgInstRef::meta_dstReceiverInst;
			::Uml::SetAssocRole(TaskInstance::meta_referedbyTaskInstRef,TaskInstance::meta,::TTSched::TaskInstRef::meta, "ref");
			Member::meta_dstMember_end_ = ::TTSched::FDVar::meta_dstMember;
			Member::meta_srcMember_end_ = ::TTSched::Connectable::meta_srcMember;
			StartMsg::meta_srcStartMsg_end_ = ::TTSched::MsgRef::meta_srcStartMsg;
			StartMsg::meta_dstStartMsg_end_ = ::TTSched::Latency::meta_dstStartMsg;
			Sender::meta_dstSender_end_ = ::TTSched::TaskRef::meta_dstSender;
			Sender::meta_srcSender_end_ = ::TTSched::Message::meta_srcSender;
			EndMsg::meta_dstEndMsg_end_ = ::TTSched::MsgRef::meta_dstEndMsg;
			EndMsg::meta_srcEndMsg_end_ = ::TTSched::Latency::meta_srcEndMsg;
			Receiver::meta_srcReceiver_end_ = ::TTSched::TaskRef::meta_srcReceiver;
			Receiver::meta_dstReceiver_end_ = ::TTSched::Message::meta_dstReceiver;
			Dependency::meta_srcDependency_end_ = ::TTSched::MsgRef::meta_srcDependency;
			Dependency::meta_dstDependency_end_ = ::TTSched::MsgRef::meta_dstDependency;
			Link::meta_dstLink_end_ = ::TTSched::Processor::meta_dstLink;
			Link::meta_srcLink_end_ = ::TTSched::Bus::meta_srcLink;
			SenderInst::meta_srcSenderInst_end_ = ::TTSched::MsgInstRef::meta_srcSenderInst;
			SenderInst::meta_dstSenderInst_end_ = ::TTSched::TaskInstRef::meta_dstSenderInst;
			ReceiverInst::meta_dstReceiverInst_end_ = ::TTSched::MsgInstRef::meta_dstReceiverInst;
			ReceiverInst::meta_srcReceiverInst_end_ = ::TTSched::TaskInstRef::meta_srcReceiverInst;

			// namespaces

			
		}
	 void Initialize()
	{
		static bool first = true;
		if(!first) return;
		first = false;
		::Uml::Initialize();

		ASSERT( umldiagram == Udm::null );
		UdmStatic::StaticDataNetwork * meta_dn = new UdmStatic::StaticDataNetwork(::Uml::diagram);
		meta_dn->CreateNew("TTSched.mem", "", ::Uml::Diagram::meta, Udm::CHANGES_LOST_DEFAULT);
		umldiagram = ::Uml::Diagram::Cast(meta_dn->GetRootObject());
		umldiagram.name() ="TTSched";
		umldiagram.version() ="1.00";
		InitializeDgr();

		InitCrossNSInheritence();
		InitCrossNSCompositions();

	};

	 void Initialize(const ::Uml::Diagram &dgr)
	{
		umldiagram = dgr;
		InitializeDgr(dgr);

	};

	 Udm::UdmDiagram diagram = { &umldiagram, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			Udm::MetaDepository::StoreDiagram("TTSched", diagram);
		};
		~_regClass()
		{
			Udm::MetaDepository::RemoveDiagram("TTSched");
		};
	} __regUnUsed;

}
// END TTSched.cpp
