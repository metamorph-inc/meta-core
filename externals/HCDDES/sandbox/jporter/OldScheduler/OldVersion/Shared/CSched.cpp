// cpp(meta datanetwork format) source file CSched.cpp generated from diagram CSched
// generated on Tue May 08 09:55:15 2007

#include "CSched.h"
#include "UmlExt.h"

#include "UdmStatic.h"

namespace CSched {

	::Uml::Diagram umldiagram;

		::Uml::Class Assignment::meta;
		::Uml::Class InstanceRef::meta;
		::Uml::Class Solution::meta;
		::Uml::Class SearchConfig::meta;
		::Uml::Class Connectable::meta;
		::Uml::Class PairConstraint::meta;
		::Uml::Class XPort::meta;
		::Uml::Class YPort::meta;
		::Uml::Class RangeConstraint::meta;
		::Uml::Class Member::meta;
		::Uml::Class SerialConstraint::meta;
		::Uml::Class Constraint::meta;
		::Uml::Class ConstraintProblem::meta;
		::Uml::Class FDVar::meta;
		::Uml::Class InequalityConstraint::meta;
		::Uml::Class EqualityConstraint::meta;
		::Uml::Class RootFolder::meta;
		::Uml::Class MgaObject::meta;
		::Uml::Class ReceiverInst::meta;
		::Uml::Class MsgInstance::meta;
		::Uml::Class SenderInst::meta;
		::Uml::Class MsgInstRef::meta;
		::Uml::Class InstanceModel::meta;
		::Uml::Class TaskInstRef::meta;
		::Uml::Class Dependencies::meta;
		::Uml::Class TaskInstances::meta;
		::Uml::Class MsgInstances::meta;
		::Uml::Class TaskInstance::meta;
		::Uml::Class Latency::meta;
		::Uml::Class LatencySpec::meta;
		::Uml::Class Processor::meta;
		::Uml::Class MsgRef::meta;
		::Uml::Class StartMsg::meta;
		::Uml::Class TaskRef::meta;
		::Uml::Class Dependency::meta;
		::Uml::Class TimeSync::meta;
		::Uml::Class EndMsg::meta;
		::Uml::Class Receiver::meta;
		::Uml::Class TTSystem::meta;
		::Uml::Class Message::meta;
		::Uml::Class Bus::meta;
		::Uml::Class Sender::meta;
		::Uml::Class Link::meta;
		::Uml::Class Task::meta;
		::Uml::Attribute Assignment::meta_Value;
		::Uml::Attribute SearchConfig::meta_MaxRunTimeSec;
		::Uml::Attribute SearchConfig::meta_MaxFailures;
		::Uml::Attribute SearchConfig::meta_SearchEngine;
		::Uml::Attribute SearchConfig::meta_MaxSolutions;
		::Uml::Attribute RangeConstraint::meta_UpperBound;
		::Uml::Attribute RangeConstraint::meta_LowerBound;
		::Uml::Attribute ConstraintProblem::meta_UpperBound;
		::Uml::Attribute ConstraintProblem::meta_LowerBound;
		::Uml::Attribute FDVar::meta_UpperBound;
		::Uml::Attribute FDVar::meta_LowerBound;
		::Uml::Attribute FDVar::meta_Size;
		::Uml::Attribute InequalityConstraint::meta_Offset;
		::Uml::Attribute EqualityConstraint::meta_Offset;
		::Uml::Attribute RootFolder::meta_name;
		::Uml::Attribute MgaObject::meta_position;
		::Uml::Attribute MgaObject::meta_name;
		::Uml::Attribute MsgInstance::meta_RefName;
		::Uml::Attribute MsgInstance::meta_StartTime;
		::Uml::Attribute MsgInstance::meta_InstID;
		::Uml::Attribute InstanceModel::meta_HyperPeriod;
		::Uml::Attribute TaskInstance::meta_RefName;
		::Uml::Attribute TaskInstance::meta_StartTime;
		::Uml::Attribute TaskInstance::meta_InstID;
		::Uml::Attribute Latency::meta_MaxLength;
		::Uml::Attribute TimeSync::meta_MsgLength;
		::Uml::Attribute TimeSync::meta_Period;
		::Uml::Attribute Message::meta_MsgLength;
		::Uml::Attribute Bus::meta_Resolution;
		::Uml::Attribute Task::meta_WCExecTime;
		::Uml::Attribute Task::meta_Period;
		::Uml::AssociationRole InstanceRef::meta_ref;
		::Uml::AssociationRole Connectable::meta_srcMember, Connectable::meta_srcMember_rev;
		::Uml::AssociationRole Member::meta_dstMember_end_;
		::Uml::AssociationRole Member::meta_srcMember_end_;
		::Uml::AssociationRole FDVar::meta_dstMember, FDVar::meta_dstMember_rev;
		::Uml::AssociationRole ReceiverInst::meta_srcReceiverInst_end_;
		::Uml::AssociationRole ReceiverInst::meta_dstReceiverInst_end_;
		::Uml::AssociationRole MsgInstance::meta_referedbyMsgInstRef;
		::Uml::AssociationRole SenderInst::meta_dstSenderInst_end_;
		::Uml::AssociationRole SenderInst::meta_srcSenderInst_end_;
		::Uml::AssociationRole MsgInstRef::meta_ref;
		::Uml::AssociationRole MsgInstRef::meta_dstReceiverInst, MsgInstRef::meta_dstReceiverInst_rev;
		::Uml::AssociationRole MsgInstRef::meta_srcSenderInst, MsgInstRef::meta_srcSenderInst_rev;
		::Uml::AssociationRole InstanceModel::meta_referedbyInstanceRef;
		::Uml::AssociationRole TaskInstRef::meta_srcReceiverInst, TaskInstRef::meta_srcReceiverInst_rev;
		::Uml::AssociationRole TaskInstRef::meta_dstSenderInst, TaskInstRef::meta_dstSenderInst_rev;
		::Uml::AssociationRole TaskInstRef::meta_ref;
		::Uml::AssociationRole TaskInstance::meta_referedbyTaskInstRef;
		::Uml::AssociationRole Latency::meta_srcEndMsg, Latency::meta_srcEndMsg_rev;
		::Uml::AssociationRole Latency::meta_dstStartMsg, Latency::meta_dstStartMsg_rev;
		::Uml::AssociationRole Processor::meta_dstLink, Processor::meta_dstLink_rev;
		::Uml::AssociationRole MsgRef::meta_dstEndMsg, MsgRef::meta_dstEndMsg_rev;
		::Uml::AssociationRole MsgRef::meta_srcStartMsg, MsgRef::meta_srcStartMsg_rev;
		::Uml::AssociationRole MsgRef::meta_dstDependency, MsgRef::meta_dstDependency_rev;
		::Uml::AssociationRole MsgRef::meta_srcDependency, MsgRef::meta_srcDependency_rev;
		::Uml::AssociationRole MsgRef::meta_ref;
		::Uml::AssociationRole StartMsg::meta_srcStartMsg_end_;
		::Uml::AssociationRole StartMsg::meta_dstStartMsg_end_;
		::Uml::AssociationRole TaskRef::meta_ref;
		::Uml::AssociationRole TaskRef::meta_dstSender, TaskRef::meta_dstSender_rev;
		::Uml::AssociationRole TaskRef::meta_srcReceiver, TaskRef::meta_srcReceiver_rev;
		::Uml::AssociationRole Dependency::meta_srcDependency_end_;
		::Uml::AssociationRole Dependency::meta_dstDependency_end_;
		::Uml::AssociationRole EndMsg::meta_dstEndMsg_end_;
		::Uml::AssociationRole EndMsg::meta_srcEndMsg_end_;
		::Uml::AssociationRole Receiver::meta_dstReceiver_end_;
		::Uml::AssociationRole Receiver::meta_srcReceiver_end_;
		::Uml::AssociationRole Message::meta_referedbyMsgRef;
		::Uml::AssociationRole Message::meta_srcSender, Message::meta_srcSender_rev;
		::Uml::AssociationRole Message::meta_dstReceiver, Message::meta_dstReceiver_rev;
		::Uml::AssociationRole Bus::meta_srcLink, Bus::meta_srcLink_rev;
		::Uml::AssociationRole Sender::meta_srcSender_end_;
		::Uml::AssociationRole Sender::meta_dstSender_end_;
		::Uml::AssociationRole Link::meta_dstLink_end_;
		::Uml::AssociationRole Link::meta_srcLink_end_;
		::Uml::AssociationRole Task::meta_referedbyTaskRef;
		::Uml::CompositionParentRole Assignment::meta_Solution_parent;
		::Uml::CompositionParentRole InstanceRef::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole Solution::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole SearchConfig::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole XPort::meta_PairConstraint_parent;
		::Uml::CompositionParentRole YPort::meta_PairConstraint_parent;
		::Uml::CompositionParentRole Member::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole Constraint::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole ConstraintProblem::meta_RootFolder_parent;
		::Uml::CompositionParentRole FDVar::meta_ConstraintProblem_parent;
		::Uml::CompositionParentRole RootFolder::meta_RootFolder_parent;
		::Uml::CompositionParentRole ReceiverInst::meta_Dependencies_parent;
		::Uml::CompositionParentRole MsgInstance::meta_MsgInstances_parent;
		::Uml::CompositionParentRole SenderInst::meta_Dependencies_parent;
		::Uml::CompositionParentRole MsgInstRef::meta_Dependencies_parent;
		::Uml::CompositionParentRole InstanceModel::meta_RootFolder_parent;
		::Uml::CompositionParentRole TaskInstRef::meta_Dependencies_parent;
		::Uml::CompositionParentRole Dependencies::meta_Bus_parent;
		::Uml::CompositionParentRole TaskInstances::meta_Processor_parent;
		::Uml::CompositionParentRole MsgInstances::meta_Bus_parent;
		::Uml::CompositionParentRole TaskInstance::meta_TaskInstances_parent;
		::Uml::CompositionParentRole Latency::meta_LatencySpec_parent;
		::Uml::CompositionParentRole LatencySpec::meta_TTSystem_parent;
		::Uml::CompositionParentRole Processor::meta_InstanceModel_parent;
		::Uml::CompositionParentRole Processor::meta_TTSystem_parent;
		::Uml::CompositionParentRole MsgRef::meta_MsgInstances_parent;
		::Uml::CompositionParentRole MsgRef::meta_LatencySpec_parent;
		::Uml::CompositionParentRole StartMsg::meta_LatencySpec_parent;
		::Uml::CompositionParentRole TaskRef::meta_TaskInstances_parent;
		::Uml::CompositionParentRole TaskRef::meta_Bus_parent;
		::Uml::CompositionParentRole Dependency::meta_LatencySpec_parent;
		::Uml::CompositionParentRole TimeSync::meta_Bus_parent;
		::Uml::CompositionParentRole EndMsg::meta_LatencySpec_parent;
		::Uml::CompositionParentRole Receiver::meta_Bus_parent;
		::Uml::CompositionParentRole TTSystem::meta_RootFolder_parent;
		::Uml::CompositionParentRole Message::meta_Bus_parent;
		::Uml::CompositionParentRole Bus::meta_InstanceModel_parent;
		::Uml::CompositionParentRole Bus::meta_TTSystem_parent;
		::Uml::CompositionParentRole Sender::meta_Bus_parent;
		::Uml::CompositionParentRole Link::meta_TTSystem_parent;
		::Uml::CompositionParentRole Task::meta_Processor_parent;
		::Uml::CompositionChildRole Solution::meta_Assignment_children;
		::Uml::CompositionChildRole PairConstraint::meta_YPort_children;
		::Uml::CompositionChildRole PairConstraint::meta_XPort_children;
		::Uml::CompositionChildRole ConstraintProblem::meta_FDVar_children;
		::Uml::CompositionChildRole ConstraintProblem::meta_Member_children;
		::Uml::CompositionChildRole ConstraintProblem::meta_Constraint_children;
		::Uml::CompositionChildRole ConstraintProblem::meta_Solution_children;
		::Uml::CompositionChildRole ConstraintProblem::meta_SearchConfig_child;
		::Uml::CompositionChildRole ConstraintProblem::meta_InstanceRef_child;
		::Uml::CompositionChildRole RootFolder::meta_InstanceModel_children;
		::Uml::CompositionChildRole RootFolder::meta_ConstraintProblem_children;
		::Uml::CompositionChildRole RootFolder::meta_TTSystem_children;
		::Uml::CompositionChildRole RootFolder::meta_RootFolder_children;
		::Uml::CompositionChildRole InstanceModel::meta_Bus_children;
		::Uml::CompositionChildRole InstanceModel::meta_Processor_children;
		::Uml::CompositionChildRole Dependencies::meta_TaskInstRef_children;
		::Uml::CompositionChildRole Dependencies::meta_MsgInstRef_children;
		::Uml::CompositionChildRole Dependencies::meta_SenderInst_children;
		::Uml::CompositionChildRole Dependencies::meta_ReceiverInst_children;
		::Uml::CompositionChildRole TaskInstances::meta_TaskInstance_children;
		::Uml::CompositionChildRole TaskInstances::meta_TaskRef_children;
		::Uml::CompositionChildRole MsgInstances::meta_MsgRef_children;
		::Uml::CompositionChildRole MsgInstances::meta_MsgInstance_children;
		::Uml::CompositionChildRole LatencySpec::meta_MsgRef_children;
		::Uml::CompositionChildRole LatencySpec::meta_Latency_children;
		::Uml::CompositionChildRole LatencySpec::meta_StartMsg_children;
		::Uml::CompositionChildRole LatencySpec::meta_Dependency_children;
		::Uml::CompositionChildRole LatencySpec::meta_EndMsg_children;
		::Uml::CompositionChildRole Processor::meta_TaskInstances_child;
		::Uml::CompositionChildRole Processor::meta_Task_children;
		::Uml::CompositionChildRole TTSystem::meta_Processor_children;
		::Uml::CompositionChildRole TTSystem::meta_LatencySpec_children;
		::Uml::CompositionChildRole TTSystem::meta_Bus_children;
		::Uml::CompositionChildRole TTSystem::meta_Link_children;
		::Uml::CompositionChildRole Bus::meta_TaskRef_children;
		::Uml::CompositionChildRole Bus::meta_Sender_children;
		::Uml::CompositionChildRole Bus::meta_Message_children;
		::Uml::CompositionChildRole Bus::meta_Receiver_children;
		::Uml::CompositionChildRole Bus::meta_TimeSync_child;
		::Uml::CompositionChildRole Bus::meta_MsgInstances_child;
		::Uml::CompositionChildRole Bus::meta_Dependencies_child;
		void Creates()
		{
			Assignment::meta = ::Uml::Class::Create(umldiagram);
			InstanceRef::meta = ::Uml::Class::Create(umldiagram);
			Solution::meta = ::Uml::Class::Create(umldiagram);
			SearchConfig::meta = ::Uml::Class::Create(umldiagram);
			Connectable::meta = ::Uml::Class::Create(umldiagram);
			PairConstraint::meta = ::Uml::Class::Create(umldiagram);
			XPort::meta = ::Uml::Class::Create(umldiagram);
			YPort::meta = ::Uml::Class::Create(umldiagram);
			RangeConstraint::meta = ::Uml::Class::Create(umldiagram);
			Member::meta = ::Uml::Class::Create(umldiagram);
			SerialConstraint::meta = ::Uml::Class::Create(umldiagram);
			Constraint::meta = ::Uml::Class::Create(umldiagram);
			ConstraintProblem::meta = ::Uml::Class::Create(umldiagram);
			FDVar::meta = ::Uml::Class::Create(umldiagram);
			InequalityConstraint::meta = ::Uml::Class::Create(umldiagram);
			EqualityConstraint::meta = ::Uml::Class::Create(umldiagram);
			RootFolder::meta = ::Uml::Class::Create(umldiagram);
			MgaObject::meta = ::Uml::Class::Create(umldiagram);
			ReceiverInst::meta = ::Uml::Class::Create(umldiagram);
			MsgInstance::meta = ::Uml::Class::Create(umldiagram);
			SenderInst::meta = ::Uml::Class::Create(umldiagram);
			MsgInstRef::meta = ::Uml::Class::Create(umldiagram);
			InstanceModel::meta = ::Uml::Class::Create(umldiagram);
			TaskInstRef::meta = ::Uml::Class::Create(umldiagram);
			Dependencies::meta = ::Uml::Class::Create(umldiagram);
			TaskInstances::meta = ::Uml::Class::Create(umldiagram);
			MsgInstances::meta = ::Uml::Class::Create(umldiagram);
			TaskInstance::meta = ::Uml::Class::Create(umldiagram);
			Latency::meta = ::Uml::Class::Create(umldiagram);
			LatencySpec::meta = ::Uml::Class::Create(umldiagram);
			Processor::meta = ::Uml::Class::Create(umldiagram);
			MsgRef::meta = ::Uml::Class::Create(umldiagram);
			StartMsg::meta = ::Uml::Class::Create(umldiagram);
			TaskRef::meta = ::Uml::Class::Create(umldiagram);
			Dependency::meta = ::Uml::Class::Create(umldiagram);
			TimeSync::meta = ::Uml::Class::Create(umldiagram);
			EndMsg::meta = ::Uml::Class::Create(umldiagram);
			Receiver::meta = ::Uml::Class::Create(umldiagram);
			TTSystem::meta = ::Uml::Class::Create(umldiagram);
			Message::meta = ::Uml::Class::Create(umldiagram);
			Bus::meta = ::Uml::Class::Create(umldiagram);
			Sender::meta = ::Uml::Class::Create(umldiagram);
			Link::meta = ::Uml::Class::Create(umldiagram);
			Task::meta = ::Uml::Class::Create(umldiagram);
			Assignment::meta_Value = ::Uml::Attribute::Create(Assignment::meta);
			SearchConfig::meta_MaxRunTimeSec = ::Uml::Attribute::Create(SearchConfig::meta);
			SearchConfig::meta_MaxFailures = ::Uml::Attribute::Create(SearchConfig::meta);
			SearchConfig::meta_SearchEngine = ::Uml::Attribute::Create(SearchConfig::meta);
			SearchConfig::meta_MaxSolutions = ::Uml::Attribute::Create(SearchConfig::meta);
			RangeConstraint::meta_UpperBound = ::Uml::Attribute::Create(RangeConstraint::meta);
			RangeConstraint::meta_LowerBound = ::Uml::Attribute::Create(RangeConstraint::meta);
			ConstraintProblem::meta_UpperBound = ::Uml::Attribute::Create(ConstraintProblem::meta);
			ConstraintProblem::meta_LowerBound = ::Uml::Attribute::Create(ConstraintProblem::meta);
			FDVar::meta_UpperBound = ::Uml::Attribute::Create(FDVar::meta);
			FDVar::meta_LowerBound = ::Uml::Attribute::Create(FDVar::meta);
			FDVar::meta_Size = ::Uml::Attribute::Create(FDVar::meta);
			InequalityConstraint::meta_Offset = ::Uml::Attribute::Create(InequalityConstraint::meta);
			EqualityConstraint::meta_Offset = ::Uml::Attribute::Create(EqualityConstraint::meta);
			RootFolder::meta_name = ::Uml::Attribute::Create(RootFolder::meta);
			MgaObject::meta_position = ::Uml::Attribute::Create(MgaObject::meta);
			MgaObject::meta_name = ::Uml::Attribute::Create(MgaObject::meta);
			MsgInstance::meta_RefName = ::Uml::Attribute::Create(MsgInstance::meta);
			MsgInstance::meta_StartTime = ::Uml::Attribute::Create(MsgInstance::meta);
			MsgInstance::meta_InstID = ::Uml::Attribute::Create(MsgInstance::meta);
			InstanceModel::meta_HyperPeriod = ::Uml::Attribute::Create(InstanceModel::meta);
			TaskInstance::meta_RefName = ::Uml::Attribute::Create(TaskInstance::meta);
			TaskInstance::meta_StartTime = ::Uml::Attribute::Create(TaskInstance::meta);
			TaskInstance::meta_InstID = ::Uml::Attribute::Create(TaskInstance::meta);
			Latency::meta_MaxLength = ::Uml::Attribute::Create(Latency::meta);
			TimeSync::meta_MsgLength = ::Uml::Attribute::Create(TimeSync::meta);
			TimeSync::meta_Period = ::Uml::Attribute::Create(TimeSync::meta);
			Message::meta_MsgLength = ::Uml::Attribute::Create(Message::meta);
			Bus::meta_Resolution = ::Uml::Attribute::Create(Bus::meta);
			Task::meta_WCExecTime = ::Uml::Attribute::Create(Task::meta);
			Task::meta_Period = ::Uml::Attribute::Create(Task::meta);
		}
		void InitClassesAttributes()
		{
			Assignment::meta.name() = "Assignment";
			Assignment::meta.isAbstract() = false;
			Assignment::meta.stereotype() = "Atom";
			InstanceRef::meta.name() = "InstanceRef";
			InstanceRef::meta.isAbstract() = false;
			InstanceRef::meta.stereotype() = "Reference";
			Solution::meta.name() = "Solution";
			Solution::meta.isAbstract() = false;
			Solution::meta.stereotype() = "Model";
			SearchConfig::meta.name() = "SearchConfig";
			SearchConfig::meta.isAbstract() = false;
			SearchConfig::meta.stereotype() = "Atom";
			Connectable::meta.name() = "Connectable";
			Connectable::meta.isAbstract() = true;
			Connectable::meta.stereotype() = "FCO";
			PairConstraint::meta.name() = "PairConstraint";
			PairConstraint::meta.isAbstract() = true;
			PairConstraint::meta.stereotype() = "Model";
			XPort::meta.name() = "XPort";
			XPort::meta.isAbstract() = false;
			XPort::meta.stereotype() = "Atom";
			YPort::meta.name() = "YPort";
			YPort::meta.isAbstract() = false;
			YPort::meta.stereotype() = "Atom";
			RangeConstraint::meta.name() = "RangeConstraint";
			RangeConstraint::meta.isAbstract() = false;
			RangeConstraint::meta.stereotype() = "Model";
			Member::meta.name() = "Member";
			Member::meta.isAbstract() = false;
			Member::meta.stereotype() = "Connection";
			SerialConstraint::meta.name() = "SerialConstraint";
			SerialConstraint::meta.isAbstract() = false;
			SerialConstraint::meta.stereotype() = "Model";
			Constraint::meta.name() = "Constraint";
			Constraint::meta.isAbstract() = true;
			Constraint::meta.stereotype() = "Model";
			ConstraintProblem::meta.name() = "ConstraintProblem";
			ConstraintProblem::meta.isAbstract() = false;
			ConstraintProblem::meta.stereotype() = "Model";
			FDVar::meta.name() = "FDVar";
			FDVar::meta.isAbstract() = false;
			FDVar::meta.stereotype() = "Atom";
			InequalityConstraint::meta.name() = "InequalityConstraint";
			InequalityConstraint::meta.isAbstract() = false;
			InequalityConstraint::meta.stereotype() = "Model";
			EqualityConstraint::meta.name() = "EqualityConstraint";
			EqualityConstraint::meta.isAbstract() = false;
			EqualityConstraint::meta.stereotype() = "Model";
			RootFolder::meta.name() = "RootFolder";
			RootFolder::meta.isAbstract() = false;
			RootFolder::meta.stereotype() = "Folder";
			MgaObject::meta.name() = "MgaObject";
			MgaObject::meta.isAbstract() = true;
			ReceiverInst::meta.name() = "ReceiverInst";
			ReceiverInst::meta.isAbstract() = false;
			ReceiverInst::meta.stereotype() = "Connection";
			MsgInstance::meta.name() = "MsgInstance";
			MsgInstance::meta.isAbstract() = false;
			MsgInstance::meta.stereotype() = "Atom";
			SenderInst::meta.name() = "SenderInst";
			SenderInst::meta.isAbstract() = false;
			SenderInst::meta.stereotype() = "Connection";
			MsgInstRef::meta.name() = "MsgInstRef";
			MsgInstRef::meta.isAbstract() = false;
			MsgInstRef::meta.stereotype() = "Reference";
			InstanceModel::meta.name() = "InstanceModel";
			InstanceModel::meta.isAbstract() = false;
			InstanceModel::meta.stereotype() = "Model";
			TaskInstRef::meta.name() = "TaskInstRef";
			TaskInstRef::meta.isAbstract() = false;
			TaskInstRef::meta.stereotype() = "Reference";
			Dependencies::meta.name() = "Dependencies";
			Dependencies::meta.isAbstract() = false;
			Dependencies::meta.stereotype() = "Model";
			TaskInstances::meta.name() = "TaskInstances";
			TaskInstances::meta.isAbstract() = false;
			TaskInstances::meta.stereotype() = "Model";
			MsgInstances::meta.name() = "MsgInstances";
			MsgInstances::meta.isAbstract() = false;
			MsgInstances::meta.stereotype() = "Model";
			TaskInstance::meta.name() = "TaskInstance";
			TaskInstance::meta.isAbstract() = false;
			TaskInstance::meta.stereotype() = "Atom";
			Latency::meta.name() = "Latency";
			Latency::meta.isAbstract() = false;
			Latency::meta.stereotype() = "Atom";
			LatencySpec::meta.name() = "LatencySpec";
			LatencySpec::meta.isAbstract() = false;
			LatencySpec::meta.stereotype() = "Model";
			Processor::meta.name() = "Processor";
			Processor::meta.isAbstract() = false;
			Processor::meta.stereotype() = "Model";
			MsgRef::meta.name() = "MsgRef";
			MsgRef::meta.isAbstract() = false;
			MsgRef::meta.stereotype() = "Reference";
			StartMsg::meta.name() = "StartMsg";
			StartMsg::meta.isAbstract() = false;
			StartMsg::meta.stereotype() = "Connection";
			TaskRef::meta.name() = "TaskRef";
			TaskRef::meta.isAbstract() = false;
			TaskRef::meta.stereotype() = "Reference";
			Dependency::meta.name() = "Dependency";
			Dependency::meta.isAbstract() = false;
			Dependency::meta.stereotype() = "Connection";
			TimeSync::meta.name() = "TimeSync";
			TimeSync::meta.isAbstract() = false;
			TimeSync::meta.stereotype() = "Atom";
			EndMsg::meta.name() = "EndMsg";
			EndMsg::meta.isAbstract() = false;
			EndMsg::meta.stereotype() = "Connection";
			Receiver::meta.name() = "Receiver";
			Receiver::meta.isAbstract() = false;
			Receiver::meta.stereotype() = "Connection";
			TTSystem::meta.name() = "TTSystem";
			TTSystem::meta.isAbstract() = false;
			TTSystem::meta.stereotype() = "Model";
			Message::meta.name() = "Message";
			Message::meta.isAbstract() = false;
			Message::meta.stereotype() = "Atom";
			Bus::meta.name() = "Bus";
			Bus::meta.isAbstract() = false;
			Bus::meta.stereotype() = "Model";
			Sender::meta.name() = "Sender";
			Sender::meta.isAbstract() = false;
			Sender::meta.stereotype() = "Connection";
			Link::meta.name() = "Link";
			Link::meta.isAbstract() = false;
			Link::meta.stereotype() = "Connection";
			Task::meta.name() = "Task";
			Task::meta.isAbstract() = false;
			Task::meta.stereotype() = "Atom";
			
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
			InequalityConstraint::meta_Offset.name() = "Offset";
			InequalityConstraint::meta_Offset.type() = "Integer";
			InequalityConstraint::meta_Offset.min() = 1;
			InequalityConstraint::meta_Offset.max() = 1;
			InequalityConstraint::meta_Offset.nonpersistent() = false;
			InequalityConstraint::meta_Offset.registry() = false;
			InequalityConstraint::meta_Offset.ordered() = false;
			InequalityConstraint::meta_Offset.visibility() = "public";
			InequalityConstraint::meta_Offset.defvalue() = vector<string>();
			EqualityConstraint::meta_Offset.name() = "Offset";
			EqualityConstraint::meta_Offset.type() = "Integer";
			EqualityConstraint::meta_Offset.min() = 1;
			EqualityConstraint::meta_Offset.max() = 1;
			EqualityConstraint::meta_Offset.nonpersistent() = false;
			EqualityConstraint::meta_Offset.registry() = false;
			EqualityConstraint::meta_Offset.ordered() = false;
			EqualityConstraint::meta_Offset.visibility() = "public";
			EqualityConstraint::meta_Offset.defvalue() = vector<string>();
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
			Latency::meta_MaxLength.name() = "MaxLength";
			Latency::meta_MaxLength.type() = "Integer";
			Latency::meta_MaxLength.min() = 1;
			Latency::meta_MaxLength.max() = 1;
			Latency::meta_MaxLength.nonpersistent() = false;
			Latency::meta_MaxLength.registry() = false;
			Latency::meta_MaxLength.ordered() = false;
			Latency::meta_MaxLength.visibility() = "public";
			Latency::meta_MaxLength.defvalue() = vector<string>();
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
			Message::meta_MsgLength.name() = "MsgLength";
			Message::meta_MsgLength.type() = "Integer";
			Message::meta_MsgLength.min() = 1;
			Message::meta_MsgLength.max() = 1;
			Message::meta_MsgLength.nonpersistent() = false;
			Message::meta_MsgLength.registry() = false;
			Message::meta_MsgLength.ordered() = false;
			Message::meta_MsgLength.visibility() = "public";
			Message::meta_MsgLength.defvalue() = vector<string>();
			Bus::meta_Resolution.name() = "Resolution";
			Bus::meta_Resolution.type() = "Integer";
			Bus::meta_Resolution.min() = 1;
			Bus::meta_Resolution.max() = 1;
			Bus::meta_Resolution.nonpersistent() = false;
			Bus::meta_Resolution.registry() = false;
			Bus::meta_Resolution.ordered() = false;
			Bus::meta_Resolution.visibility() = "public";
			Bus::meta_Resolution.defvalue() = vector<string>();
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
			::CSched::InstanceModel::meta_referedbyInstanceRef = ::Uml::AssociationRole::Create(ass);
			::CSched::InstanceModel::meta_referedbyInstanceRef.name() = "referedbyInstanceRef";
			::CSched::InstanceModel::meta_referedbyInstanceRef.min() = 0;
			::CSched::InstanceModel::meta_referedbyInstanceRef.max() = -1;
			::CSched::InstanceModel::meta_referedbyInstanceRef.isNavigable() = true;
			::CSched::InstanceModel::meta_referedbyInstanceRef.isPrimary() = false;
			::CSched::InstanceModel::meta_referedbyInstanceRef.target() = ::CSched::InstanceRef::meta;
			::CSched::InstanceRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::CSched::InstanceRef::meta_ref.name() = "ref";
			::CSched::InstanceRef::meta_ref.min() = 0;
			::CSched::InstanceRef::meta_ref.max() = 1;
			::CSched::InstanceRef::meta_ref.isNavigable() = true;
			::CSched::InstanceRef::meta_ref.isPrimary() = false;
			::CSched::InstanceRef::meta_ref.target() = ::CSched::InstanceModel::meta;
		};
		void InitAssociation1()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Member";
			ass.assocClass() = ::CSched::Member::meta;
			::CSched::Connectable::meta_srcMember = ::Uml::AssociationRole::Create(ass);
			::CSched::Connectable::meta_srcMember.name() = "srcMember";
			::CSched::Connectable::meta_srcMember.min() = 0;
			::CSched::Connectable::meta_srcMember.max() = -1;
			::CSched::Connectable::meta_srcMember.isNavigable() = true;
			::CSched::Connectable::meta_srcMember.isPrimary() = false;
			::CSched::Connectable::meta_srcMember.target() = ::CSched::FDVar::meta;
			::CSched::Member::meta_srcMember_end_ = ::CSched::FDVar::meta_dstMember_rev = ::CSched::Connectable::meta_srcMember;
			::CSched::FDVar::meta_dstMember = ::Uml::AssociationRole::Create(ass);
			::CSched::FDVar::meta_dstMember.name() = "dstMember";
			::CSched::FDVar::meta_dstMember.min() = 0;
			::CSched::FDVar::meta_dstMember.max() = -1;
			::CSched::FDVar::meta_dstMember.isNavigable() = true;
			::CSched::FDVar::meta_dstMember.isPrimary() = false;
			::CSched::FDVar::meta_dstMember.target() = ::CSched::Connectable::meta;
			::CSched::Member::meta_dstMember_end_ = ::CSched::Connectable::meta_srcMember_rev = ::CSched::FDVar::meta_dstMember;
		};
		void InitAssociation2()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			::CSched::MsgInstance::meta_referedbyMsgInstRef = ::Uml::AssociationRole::Create(ass);
			::CSched::MsgInstance::meta_referedbyMsgInstRef.name() = "referedbyMsgInstRef";
			::CSched::MsgInstance::meta_referedbyMsgInstRef.min() = 0;
			::CSched::MsgInstance::meta_referedbyMsgInstRef.max() = -1;
			::CSched::MsgInstance::meta_referedbyMsgInstRef.isNavigable() = true;
			::CSched::MsgInstance::meta_referedbyMsgInstRef.isPrimary() = false;
			::CSched::MsgInstance::meta_referedbyMsgInstRef.target() = ::CSched::MsgInstRef::meta;
			::CSched::MsgInstRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::CSched::MsgInstRef::meta_ref.name() = "ref";
			::CSched::MsgInstRef::meta_ref.min() = 0;
			::CSched::MsgInstRef::meta_ref.max() = 1;
			::CSched::MsgInstRef::meta_ref.isNavigable() = true;
			::CSched::MsgInstRef::meta_ref.isPrimary() = false;
			::CSched::MsgInstRef::meta_ref.target() = ::CSched::MsgInstance::meta;
		};
		void InitAssociation3()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "ReceiverInst";
			ass.assocClass() = ::CSched::ReceiverInst::meta;
			::CSched::MsgInstRef::meta_dstReceiverInst = ::Uml::AssociationRole::Create(ass);
			::CSched::MsgInstRef::meta_dstReceiverInst.name() = "dstReceiverInst";
			::CSched::MsgInstRef::meta_dstReceiverInst.min() = 0;
			::CSched::MsgInstRef::meta_dstReceiverInst.max() = -1;
			::CSched::MsgInstRef::meta_dstReceiverInst.isNavigable() = true;
			::CSched::MsgInstRef::meta_dstReceiverInst.isPrimary() = false;
			::CSched::MsgInstRef::meta_dstReceiverInst.target() = ::CSched::TaskInstRef::meta;
			::CSched::ReceiverInst::meta_dstReceiverInst_end_ = ::CSched::TaskInstRef::meta_srcReceiverInst_rev = ::CSched::MsgInstRef::meta_dstReceiverInst;
			::CSched::TaskInstRef::meta_srcReceiverInst = ::Uml::AssociationRole::Create(ass);
			::CSched::TaskInstRef::meta_srcReceiverInst.name() = "srcReceiverInst";
			::CSched::TaskInstRef::meta_srcReceiverInst.min() = 0;
			::CSched::TaskInstRef::meta_srcReceiverInst.max() = -1;
			::CSched::TaskInstRef::meta_srcReceiverInst.isNavigable() = true;
			::CSched::TaskInstRef::meta_srcReceiverInst.isPrimary() = false;
			::CSched::TaskInstRef::meta_srcReceiverInst.target() = ::CSched::MsgInstRef::meta;
			::CSched::ReceiverInst::meta_srcReceiverInst_end_ = ::CSched::MsgInstRef::meta_dstReceiverInst_rev = ::CSched::TaskInstRef::meta_srcReceiverInst;
		};
		void InitAssociation4()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "SenderInst";
			ass.assocClass() = ::CSched::SenderInst::meta;
			::CSched::MsgInstRef::meta_srcSenderInst = ::Uml::AssociationRole::Create(ass);
			::CSched::MsgInstRef::meta_srcSenderInst.name() = "srcSenderInst";
			::CSched::MsgInstRef::meta_srcSenderInst.min() = 0;
			::CSched::MsgInstRef::meta_srcSenderInst.max() = -1;
			::CSched::MsgInstRef::meta_srcSenderInst.isNavigable() = true;
			::CSched::MsgInstRef::meta_srcSenderInst.isPrimary() = false;
			::CSched::MsgInstRef::meta_srcSenderInst.target() = ::CSched::TaskInstRef::meta;
			::CSched::SenderInst::meta_srcSenderInst_end_ = ::CSched::TaskInstRef::meta_dstSenderInst_rev = ::CSched::MsgInstRef::meta_srcSenderInst;
			::CSched::TaskInstRef::meta_dstSenderInst = ::Uml::AssociationRole::Create(ass);
			::CSched::TaskInstRef::meta_dstSenderInst.name() = "dstSenderInst";
			::CSched::TaskInstRef::meta_dstSenderInst.min() = 0;
			::CSched::TaskInstRef::meta_dstSenderInst.max() = -1;
			::CSched::TaskInstRef::meta_dstSenderInst.isNavigable() = true;
			::CSched::TaskInstRef::meta_dstSenderInst.isPrimary() = false;
			::CSched::TaskInstRef::meta_dstSenderInst.target() = ::CSched::MsgInstRef::meta;
			::CSched::SenderInst::meta_dstSenderInst_end_ = ::CSched::MsgInstRef::meta_srcSenderInst_rev = ::CSched::TaskInstRef::meta_dstSenderInst;
		};
		void InitAssociation5()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			::CSched::TaskInstRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::CSched::TaskInstRef::meta_ref.name() = "ref";
			::CSched::TaskInstRef::meta_ref.min() = 0;
			::CSched::TaskInstRef::meta_ref.max() = 1;
			::CSched::TaskInstRef::meta_ref.isNavigable() = true;
			::CSched::TaskInstRef::meta_ref.isPrimary() = false;
			::CSched::TaskInstRef::meta_ref.target() = ::CSched::TaskInstance::meta;
			::CSched::TaskInstance::meta_referedbyTaskInstRef = ::Uml::AssociationRole::Create(ass);
			::CSched::TaskInstance::meta_referedbyTaskInstRef.name() = "referedbyTaskInstRef";
			::CSched::TaskInstance::meta_referedbyTaskInstRef.min() = 0;
			::CSched::TaskInstance::meta_referedbyTaskInstRef.max() = -1;
			::CSched::TaskInstance::meta_referedbyTaskInstRef.isNavigable() = true;
			::CSched::TaskInstance::meta_referedbyTaskInstRef.isPrimary() = false;
			::CSched::TaskInstance::meta_referedbyTaskInstRef.target() = ::CSched::TaskInstRef::meta;
		};
		void InitAssociation6()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "EndMsg";
			ass.assocClass() = ::CSched::EndMsg::meta;
			::CSched::Latency::meta_srcEndMsg = ::Uml::AssociationRole::Create(ass);
			::CSched::Latency::meta_srcEndMsg.name() = "srcEndMsg";
			::CSched::Latency::meta_srcEndMsg.min() = 0;
			::CSched::Latency::meta_srcEndMsg.max() = -1;
			::CSched::Latency::meta_srcEndMsg.isNavigable() = true;
			::CSched::Latency::meta_srcEndMsg.isPrimary() = false;
			::CSched::Latency::meta_srcEndMsg.target() = ::CSched::MsgRef::meta;
			::CSched::EndMsg::meta_srcEndMsg_end_ = ::CSched::MsgRef::meta_dstEndMsg_rev = ::CSched::Latency::meta_srcEndMsg;
			::CSched::MsgRef::meta_dstEndMsg = ::Uml::AssociationRole::Create(ass);
			::CSched::MsgRef::meta_dstEndMsg.name() = "dstEndMsg";
			::CSched::MsgRef::meta_dstEndMsg.min() = 0;
			::CSched::MsgRef::meta_dstEndMsg.max() = -1;
			::CSched::MsgRef::meta_dstEndMsg.isNavigable() = true;
			::CSched::MsgRef::meta_dstEndMsg.isPrimary() = false;
			::CSched::MsgRef::meta_dstEndMsg.target() = ::CSched::Latency::meta;
			::CSched::EndMsg::meta_dstEndMsg_end_ = ::CSched::Latency::meta_srcEndMsg_rev = ::CSched::MsgRef::meta_dstEndMsg;
		};
		void InitAssociation7()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "StartMsg";
			ass.assocClass() = ::CSched::StartMsg::meta;
			::CSched::Latency::meta_dstStartMsg = ::Uml::AssociationRole::Create(ass);
			::CSched::Latency::meta_dstStartMsg.name() = "dstStartMsg";
			::CSched::Latency::meta_dstStartMsg.min() = 0;
			::CSched::Latency::meta_dstStartMsg.max() = -1;
			::CSched::Latency::meta_dstStartMsg.isNavigable() = true;
			::CSched::Latency::meta_dstStartMsg.isPrimary() = false;
			::CSched::Latency::meta_dstStartMsg.target() = ::CSched::MsgRef::meta;
			::CSched::StartMsg::meta_dstStartMsg_end_ = ::CSched::MsgRef::meta_srcStartMsg_rev = ::CSched::Latency::meta_dstStartMsg;
			::CSched::MsgRef::meta_srcStartMsg = ::Uml::AssociationRole::Create(ass);
			::CSched::MsgRef::meta_srcStartMsg.name() = "srcStartMsg";
			::CSched::MsgRef::meta_srcStartMsg.min() = 0;
			::CSched::MsgRef::meta_srcStartMsg.max() = -1;
			::CSched::MsgRef::meta_srcStartMsg.isNavigable() = true;
			::CSched::MsgRef::meta_srcStartMsg.isPrimary() = false;
			::CSched::MsgRef::meta_srcStartMsg.target() = ::CSched::Latency::meta;
			::CSched::StartMsg::meta_srcStartMsg_end_ = ::CSched::Latency::meta_dstStartMsg_rev = ::CSched::MsgRef::meta_srcStartMsg;
		};
		void InitAssociation8()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Link";
			ass.assocClass() = ::CSched::Link::meta;
			::CSched::Bus::meta_srcLink = ::Uml::AssociationRole::Create(ass);
			::CSched::Bus::meta_srcLink.name() = "srcLink";
			::CSched::Bus::meta_srcLink.min() = 0;
			::CSched::Bus::meta_srcLink.max() = -1;
			::CSched::Bus::meta_srcLink.isNavigable() = true;
			::CSched::Bus::meta_srcLink.isPrimary() = false;
			::CSched::Bus::meta_srcLink.target() = ::CSched::Processor::meta;
			::CSched::Link::meta_srcLink_end_ = ::CSched::Processor::meta_dstLink_rev = ::CSched::Bus::meta_srcLink;
			::CSched::Processor::meta_dstLink = ::Uml::AssociationRole::Create(ass);
			::CSched::Processor::meta_dstLink.name() = "dstLink";
			::CSched::Processor::meta_dstLink.min() = 0;
			::CSched::Processor::meta_dstLink.max() = -1;
			::CSched::Processor::meta_dstLink.isNavigable() = true;
			::CSched::Processor::meta_dstLink.isPrimary() = false;
			::CSched::Processor::meta_dstLink.target() = ::CSched::Bus::meta;
			::CSched::Link::meta_dstLink_end_ = ::CSched::Bus::meta_srcLink_rev = ::CSched::Processor::meta_dstLink;
		};
		void InitAssociation9()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Dependency";
			ass.assocClass() = ::CSched::Dependency::meta;
			::CSched::MsgRef::meta_dstDependency = ::Uml::AssociationRole::Create(ass);
			::CSched::MsgRef::meta_dstDependency.name() = "dstDependency";
			::CSched::MsgRef::meta_dstDependency.min() = 0;
			::CSched::MsgRef::meta_dstDependency.max() = -1;
			::CSched::MsgRef::meta_dstDependency.isNavigable() = true;
			::CSched::MsgRef::meta_dstDependency.isPrimary() = false;
			::CSched::MsgRef::meta_dstDependency.target() = ::CSched::MsgRef::meta;
			::CSched::Dependency::meta_dstDependency_end_ = ::CSched::MsgRef::meta_srcDependency_rev = ::CSched::MsgRef::meta_dstDependency;
			::CSched::MsgRef::meta_srcDependency = ::Uml::AssociationRole::Create(ass);
			::CSched::MsgRef::meta_srcDependency.name() = "srcDependency";
			::CSched::MsgRef::meta_srcDependency.min() = 0;
			::CSched::MsgRef::meta_srcDependency.max() = -1;
			::CSched::MsgRef::meta_srcDependency.isNavigable() = true;
			::CSched::MsgRef::meta_srcDependency.isPrimary() = false;
			::CSched::MsgRef::meta_srcDependency.target() = ::CSched::MsgRef::meta;
			::CSched::Dependency::meta_srcDependency_end_ = ::CSched::MsgRef::meta_dstDependency_rev = ::CSched::MsgRef::meta_srcDependency;
		};
		void InitAssociation10()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			::CSched::MsgRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::CSched::MsgRef::meta_ref.name() = "ref";
			::CSched::MsgRef::meta_ref.min() = 0;
			::CSched::MsgRef::meta_ref.max() = 1;
			::CSched::MsgRef::meta_ref.isNavigable() = true;
			::CSched::MsgRef::meta_ref.isPrimary() = false;
			::CSched::MsgRef::meta_ref.target() = ::CSched::Message::meta;
			::CSched::Message::meta_referedbyMsgRef = ::Uml::AssociationRole::Create(ass);
			::CSched::Message::meta_referedbyMsgRef.name() = "referedbyMsgRef";
			::CSched::Message::meta_referedbyMsgRef.min() = 0;
			::CSched::Message::meta_referedbyMsgRef.max() = -1;
			::CSched::Message::meta_referedbyMsgRef.isNavigable() = true;
			::CSched::Message::meta_referedbyMsgRef.isPrimary() = false;
			::CSched::Message::meta_referedbyMsgRef.target() = ::CSched::MsgRef::meta;
		};
		void InitAssociation11()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			::CSched::TaskRef::meta_ref = ::Uml::AssociationRole::Create(ass);
			::CSched::TaskRef::meta_ref.name() = "ref";
			::CSched::TaskRef::meta_ref.min() = 0;
			::CSched::TaskRef::meta_ref.max() = 1;
			::CSched::TaskRef::meta_ref.isNavigable() = true;
			::CSched::TaskRef::meta_ref.isPrimary() = false;
			::CSched::TaskRef::meta_ref.target() = ::CSched::Task::meta;
			::CSched::Task::meta_referedbyTaskRef = ::Uml::AssociationRole::Create(ass);
			::CSched::Task::meta_referedbyTaskRef.name() = "referedbyTaskRef";
			::CSched::Task::meta_referedbyTaskRef.min() = 0;
			::CSched::Task::meta_referedbyTaskRef.max() = -1;
			::CSched::Task::meta_referedbyTaskRef.isNavigable() = true;
			::CSched::Task::meta_referedbyTaskRef.isPrimary() = false;
			::CSched::Task::meta_referedbyTaskRef.target() = ::CSched::TaskRef::meta;
		};
		void InitAssociation12()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Sender";
			ass.assocClass() = ::CSched::Sender::meta;
			::CSched::TaskRef::meta_dstSender = ::Uml::AssociationRole::Create(ass);
			::CSched::TaskRef::meta_dstSender.name() = "dstSender";
			::CSched::TaskRef::meta_dstSender.min() = 0;
			::CSched::TaskRef::meta_dstSender.max() = -1;
			::CSched::TaskRef::meta_dstSender.isNavigable() = true;
			::CSched::TaskRef::meta_dstSender.isPrimary() = false;
			::CSched::TaskRef::meta_dstSender.target() = ::CSched::Message::meta;
			::CSched::Sender::meta_dstSender_end_ = ::CSched::Message::meta_srcSender_rev = ::CSched::TaskRef::meta_dstSender;
			::CSched::Message::meta_srcSender = ::Uml::AssociationRole::Create(ass);
			::CSched::Message::meta_srcSender.name() = "srcSender";
			::CSched::Message::meta_srcSender.min() = 0;
			::CSched::Message::meta_srcSender.max() = 1;
			::CSched::Message::meta_srcSender.isNavigable() = true;
			::CSched::Message::meta_srcSender.isPrimary() = false;
			::CSched::Message::meta_srcSender.target() = ::CSched::TaskRef::meta;
			::CSched::Sender::meta_srcSender_end_ = ::CSched::TaskRef::meta_dstSender_rev = ::CSched::Message::meta_srcSender;
		};
		void InitAssociation13()
		{
			::Uml::Association ass = ::Uml::Association::Create(umldiagram);
			ass.name() = "Receiver";
			ass.assocClass() = ::CSched::Receiver::meta;
			::CSched::TaskRef::meta_srcReceiver = ::Uml::AssociationRole::Create(ass);
			::CSched::TaskRef::meta_srcReceiver.name() = "srcReceiver";
			::CSched::TaskRef::meta_srcReceiver.min() = 0;
			::CSched::TaskRef::meta_srcReceiver.max() = -1;
			::CSched::TaskRef::meta_srcReceiver.isNavigable() = true;
			::CSched::TaskRef::meta_srcReceiver.isPrimary() = false;
			::CSched::TaskRef::meta_srcReceiver.target() = ::CSched::Message::meta;
			::CSched::Receiver::meta_srcReceiver_end_ = ::CSched::Message::meta_dstReceiver_rev = ::CSched::TaskRef::meta_srcReceiver;
			::CSched::Message::meta_dstReceiver = ::Uml::AssociationRole::Create(ass);
			::CSched::Message::meta_dstReceiver.name() = "dstReceiver";
			::CSched::Message::meta_dstReceiver.min() = 0;
			::CSched::Message::meta_dstReceiver.max() = -1;
			::CSched::Message::meta_dstReceiver.isNavigable() = true;
			::CSched::Message::meta_dstReceiver.isPrimary() = false;
			::CSched::Message::meta_dstReceiver.target() = ::CSched::TaskRef::meta;
			::CSched::Receiver::meta_dstReceiver_end_ = ::CSched::TaskRef::meta_srcReceiver_rev = ::CSched::Message::meta_dstReceiver;
		};
		void InitComposition0()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Assignment::meta_Solution_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Assignment::meta_Solution_parent.name() = "Solution_parent";
			::CSched::Assignment::meta_Solution_parent.isNavigable() = true;
			::CSched::Assignment::meta_Solution_parent.target() = ::CSched::Solution::meta;
			::CSched::Solution::meta_Assignment_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Solution::meta_Assignment_children.name() = "Assignment";
			::CSched::Solution::meta_Assignment_children.min() = 0;
			::CSched::Solution::meta_Assignment_children.max() = -1;
			::CSched::Solution::meta_Assignment_children.isNavigable() = true;
			::CSched::Solution::meta_Assignment_children.target() = ::CSched::Assignment::meta;
		}
		void InitComposition1()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::YPort::meta_PairConstraint_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::YPort::meta_PairConstraint_parent.name() = "PairConstraint_parent";
			::CSched::YPort::meta_PairConstraint_parent.isNavigable() = true;
			::CSched::YPort::meta_PairConstraint_parent.target() = ::CSched::PairConstraint::meta;
			::CSched::PairConstraint::meta_YPort_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::PairConstraint::meta_YPort_children.name() = "YPort";
			::CSched::PairConstraint::meta_YPort_children.min() = 0;
			::CSched::PairConstraint::meta_YPort_children.max() = -1;
			::CSched::PairConstraint::meta_YPort_children.isNavigable() = true;
			::CSched::PairConstraint::meta_YPort_children.target() = ::CSched::YPort::meta;
		}
		void InitComposition2()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::XPort::meta_PairConstraint_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::XPort::meta_PairConstraint_parent.name() = "PairConstraint_parent";
			::CSched::XPort::meta_PairConstraint_parent.isNavigable() = true;
			::CSched::XPort::meta_PairConstraint_parent.target() = ::CSched::PairConstraint::meta;
			::CSched::PairConstraint::meta_XPort_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::PairConstraint::meta_XPort_children.name() = "XPort";
			::CSched::PairConstraint::meta_XPort_children.min() = 0;
			::CSched::PairConstraint::meta_XPort_children.max() = -1;
			::CSched::PairConstraint::meta_XPort_children.isNavigable() = true;
			::CSched::PairConstraint::meta_XPort_children.target() = ::CSched::XPort::meta;
		}
		void InitComposition3()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::FDVar::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::FDVar::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::CSched::FDVar::meta_ConstraintProblem_parent.isNavigable() = true;
			::CSched::FDVar::meta_ConstraintProblem_parent.target() = ::CSched::ConstraintProblem::meta;
			::CSched::ConstraintProblem::meta_FDVar_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::ConstraintProblem::meta_FDVar_children.name() = "FDVar";
			::CSched::ConstraintProblem::meta_FDVar_children.min() = 0;
			::CSched::ConstraintProblem::meta_FDVar_children.max() = -1;
			::CSched::ConstraintProblem::meta_FDVar_children.isNavigable() = true;
			::CSched::ConstraintProblem::meta_FDVar_children.target() = ::CSched::FDVar::meta;
		}
		void InitComposition4()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Member::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Member::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::CSched::Member::meta_ConstraintProblem_parent.isNavigable() = true;
			::CSched::Member::meta_ConstraintProblem_parent.target() = ::CSched::ConstraintProblem::meta;
			::CSched::ConstraintProblem::meta_Member_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::ConstraintProblem::meta_Member_children.name() = "Member";
			::CSched::ConstraintProblem::meta_Member_children.min() = 0;
			::CSched::ConstraintProblem::meta_Member_children.max() = -1;
			::CSched::ConstraintProblem::meta_Member_children.isNavigable() = true;
			::CSched::ConstraintProblem::meta_Member_children.target() = ::CSched::Member::meta;
		}
		void InitComposition5()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Constraint::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Constraint::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::CSched::Constraint::meta_ConstraintProblem_parent.isNavigable() = true;
			::CSched::Constraint::meta_ConstraintProblem_parent.target() = ::CSched::ConstraintProblem::meta;
			::CSched::ConstraintProblem::meta_Constraint_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::ConstraintProblem::meta_Constraint_children.name() = "Constraint";
			::CSched::ConstraintProblem::meta_Constraint_children.min() = 0;
			::CSched::ConstraintProblem::meta_Constraint_children.max() = -1;
			::CSched::ConstraintProblem::meta_Constraint_children.isNavigable() = true;
			::CSched::ConstraintProblem::meta_Constraint_children.target() = ::CSched::Constraint::meta;
		}
		void InitComposition6()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Solution::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Solution::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::CSched::Solution::meta_ConstraintProblem_parent.isNavigable() = true;
			::CSched::Solution::meta_ConstraintProblem_parent.target() = ::CSched::ConstraintProblem::meta;
			::CSched::ConstraintProblem::meta_Solution_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::ConstraintProblem::meta_Solution_children.name() = "Solution";
			::CSched::ConstraintProblem::meta_Solution_children.min() = 0;
			::CSched::ConstraintProblem::meta_Solution_children.max() = -1;
			::CSched::ConstraintProblem::meta_Solution_children.isNavigable() = true;
			::CSched::ConstraintProblem::meta_Solution_children.target() = ::CSched::Solution::meta;
		}
		void InitComposition7()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::SearchConfig::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::SearchConfig::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::CSched::SearchConfig::meta_ConstraintProblem_parent.isNavigable() = true;
			::CSched::SearchConfig::meta_ConstraintProblem_parent.target() = ::CSched::ConstraintProblem::meta;
			::CSched::ConstraintProblem::meta_SearchConfig_child = ::Uml::CompositionChildRole::Create(comp);
			::CSched::ConstraintProblem::meta_SearchConfig_child.name() = "SearchConfig";
			::CSched::ConstraintProblem::meta_SearchConfig_child.min() = 0;
			::CSched::ConstraintProblem::meta_SearchConfig_child.max() = 1;
			::CSched::ConstraintProblem::meta_SearchConfig_child.isNavigable() = true;
			::CSched::ConstraintProblem::meta_SearchConfig_child.target() = ::CSched::SearchConfig::meta;
		}
		void InitComposition8()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::InstanceRef::meta_ConstraintProblem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::InstanceRef::meta_ConstraintProblem_parent.name() = "ConstraintProblem_parent";
			::CSched::InstanceRef::meta_ConstraintProblem_parent.isNavigable() = true;
			::CSched::InstanceRef::meta_ConstraintProblem_parent.target() = ::CSched::ConstraintProblem::meta;
			::CSched::ConstraintProblem::meta_InstanceRef_child = ::Uml::CompositionChildRole::Create(comp);
			::CSched::ConstraintProblem::meta_InstanceRef_child.name() = "InstanceRef";
			::CSched::ConstraintProblem::meta_InstanceRef_child.min() = 0;
			::CSched::ConstraintProblem::meta_InstanceRef_child.max() = 1;
			::CSched::ConstraintProblem::meta_InstanceRef_child.isNavigable() = true;
			::CSched::ConstraintProblem::meta_InstanceRef_child.target() = ::CSched::InstanceRef::meta;
		}
		void InitComposition9()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::InstanceModel::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::InstanceModel::meta_RootFolder_parent.name() = "RootFolder_parent";
			::CSched::InstanceModel::meta_RootFolder_parent.isNavigable() = true;
			::CSched::InstanceModel::meta_RootFolder_parent.target() = ::CSched::RootFolder::meta;
			::CSched::RootFolder::meta_InstanceModel_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::RootFolder::meta_InstanceModel_children.name() = "InstanceModel";
			::CSched::RootFolder::meta_InstanceModel_children.min() = 0;
			::CSched::RootFolder::meta_InstanceModel_children.max() = -1;
			::CSched::RootFolder::meta_InstanceModel_children.isNavigable() = true;
			::CSched::RootFolder::meta_InstanceModel_children.target() = ::CSched::InstanceModel::meta;
		}
		void InitComposition10()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::ConstraintProblem::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::ConstraintProblem::meta_RootFolder_parent.name() = "RootFolder_parent";
			::CSched::ConstraintProblem::meta_RootFolder_parent.isNavigable() = true;
			::CSched::ConstraintProblem::meta_RootFolder_parent.target() = ::CSched::RootFolder::meta;
			::CSched::RootFolder::meta_ConstraintProblem_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::RootFolder::meta_ConstraintProblem_children.name() = "ConstraintProblem";
			::CSched::RootFolder::meta_ConstraintProblem_children.min() = 0;
			::CSched::RootFolder::meta_ConstraintProblem_children.max() = -1;
			::CSched::RootFolder::meta_ConstraintProblem_children.isNavigable() = true;
			::CSched::RootFolder::meta_ConstraintProblem_children.target() = ::CSched::ConstraintProblem::meta;
		}
		void InitComposition11()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::TTSystem::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::TTSystem::meta_RootFolder_parent.name() = "RootFolder_parent";
			::CSched::TTSystem::meta_RootFolder_parent.isNavigable() = true;
			::CSched::TTSystem::meta_RootFolder_parent.target() = ::CSched::RootFolder::meta;
			::CSched::RootFolder::meta_TTSystem_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::RootFolder::meta_TTSystem_children.name() = "TTSystem";
			::CSched::RootFolder::meta_TTSystem_children.min() = 0;
			::CSched::RootFolder::meta_TTSystem_children.max() = -1;
			::CSched::RootFolder::meta_TTSystem_children.isNavigable() = true;
			::CSched::RootFolder::meta_TTSystem_children.target() = ::CSched::TTSystem::meta;
		}
		void InitComposition12()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::RootFolder::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::RootFolder::meta_RootFolder_parent.name() = "RootFolder_parent";
			::CSched::RootFolder::meta_RootFolder_parent.isNavigable() = true;
			::CSched::RootFolder::meta_RootFolder_parent.target() = ::CSched::RootFolder::meta;
			::CSched::RootFolder::meta_RootFolder_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::RootFolder::meta_RootFolder_children.name() = "RootFolder";
			::CSched::RootFolder::meta_RootFolder_children.min() = 0;
			::CSched::RootFolder::meta_RootFolder_children.max() = -1;
			::CSched::RootFolder::meta_RootFolder_children.isNavigable() = true;
			::CSched::RootFolder::meta_RootFolder_children.target() = ::CSched::RootFolder::meta;
		}
		void InitComposition13()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Bus::meta_InstanceModel_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Bus::meta_InstanceModel_parent.name() = "InstanceModel_parent";
			::CSched::Bus::meta_InstanceModel_parent.isNavigable() = true;
			::CSched::Bus::meta_InstanceModel_parent.target() = ::CSched::InstanceModel::meta;
			::CSched::InstanceModel::meta_Bus_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::InstanceModel::meta_Bus_children.name() = "Bus";
			::CSched::InstanceModel::meta_Bus_children.min() = 0;
			::CSched::InstanceModel::meta_Bus_children.max() = -1;
			::CSched::InstanceModel::meta_Bus_children.isNavigable() = true;
			::CSched::InstanceModel::meta_Bus_children.target() = ::CSched::Bus::meta;
		}
		void InitComposition14()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Processor::meta_InstanceModel_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Processor::meta_InstanceModel_parent.name() = "InstanceModel_parent";
			::CSched::Processor::meta_InstanceModel_parent.isNavigable() = true;
			::CSched::Processor::meta_InstanceModel_parent.target() = ::CSched::InstanceModel::meta;
			::CSched::InstanceModel::meta_Processor_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::InstanceModel::meta_Processor_children.name() = "Processor";
			::CSched::InstanceModel::meta_Processor_children.min() = 0;
			::CSched::InstanceModel::meta_Processor_children.max() = -1;
			::CSched::InstanceModel::meta_Processor_children.isNavigable() = true;
			::CSched::InstanceModel::meta_Processor_children.target() = ::CSched::Processor::meta;
		}
		void InitComposition15()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::TaskInstRef::meta_Dependencies_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::TaskInstRef::meta_Dependencies_parent.name() = "Dependencies_parent";
			::CSched::TaskInstRef::meta_Dependencies_parent.isNavigable() = true;
			::CSched::TaskInstRef::meta_Dependencies_parent.target() = ::CSched::Dependencies::meta;
			::CSched::Dependencies::meta_TaskInstRef_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Dependencies::meta_TaskInstRef_children.name() = "TaskInstRef";
			::CSched::Dependencies::meta_TaskInstRef_children.min() = 0;
			::CSched::Dependencies::meta_TaskInstRef_children.max() = -1;
			::CSched::Dependencies::meta_TaskInstRef_children.isNavigable() = true;
			::CSched::Dependencies::meta_TaskInstRef_children.target() = ::CSched::TaskInstRef::meta;
		}
		void InitComposition16()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::MsgInstRef::meta_Dependencies_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::MsgInstRef::meta_Dependencies_parent.name() = "Dependencies_parent";
			::CSched::MsgInstRef::meta_Dependencies_parent.isNavigable() = true;
			::CSched::MsgInstRef::meta_Dependencies_parent.target() = ::CSched::Dependencies::meta;
			::CSched::Dependencies::meta_MsgInstRef_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Dependencies::meta_MsgInstRef_children.name() = "MsgInstRef";
			::CSched::Dependencies::meta_MsgInstRef_children.min() = 0;
			::CSched::Dependencies::meta_MsgInstRef_children.max() = -1;
			::CSched::Dependencies::meta_MsgInstRef_children.isNavigable() = true;
			::CSched::Dependencies::meta_MsgInstRef_children.target() = ::CSched::MsgInstRef::meta;
		}
		void InitComposition17()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::SenderInst::meta_Dependencies_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::SenderInst::meta_Dependencies_parent.name() = "Dependencies_parent";
			::CSched::SenderInst::meta_Dependencies_parent.isNavigable() = true;
			::CSched::SenderInst::meta_Dependencies_parent.target() = ::CSched::Dependencies::meta;
			::CSched::Dependencies::meta_SenderInst_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Dependencies::meta_SenderInst_children.name() = "SenderInst";
			::CSched::Dependencies::meta_SenderInst_children.min() = 0;
			::CSched::Dependencies::meta_SenderInst_children.max() = -1;
			::CSched::Dependencies::meta_SenderInst_children.isNavigable() = true;
			::CSched::Dependencies::meta_SenderInst_children.target() = ::CSched::SenderInst::meta;
		}
		void InitComposition18()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::ReceiverInst::meta_Dependencies_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::ReceiverInst::meta_Dependencies_parent.name() = "Dependencies_parent";
			::CSched::ReceiverInst::meta_Dependencies_parent.isNavigable() = true;
			::CSched::ReceiverInst::meta_Dependencies_parent.target() = ::CSched::Dependencies::meta;
			::CSched::Dependencies::meta_ReceiverInst_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Dependencies::meta_ReceiverInst_children.name() = "ReceiverInst";
			::CSched::Dependencies::meta_ReceiverInst_children.min() = 0;
			::CSched::Dependencies::meta_ReceiverInst_children.max() = -1;
			::CSched::Dependencies::meta_ReceiverInst_children.isNavigable() = true;
			::CSched::Dependencies::meta_ReceiverInst_children.target() = ::CSched::ReceiverInst::meta;
		}
		void InitComposition19()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::TaskInstance::meta_TaskInstances_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::TaskInstance::meta_TaskInstances_parent.name() = "TaskInstances_parent";
			::CSched::TaskInstance::meta_TaskInstances_parent.isNavigable() = true;
			::CSched::TaskInstance::meta_TaskInstances_parent.target() = ::CSched::TaskInstances::meta;
			::CSched::TaskInstances::meta_TaskInstance_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::TaskInstances::meta_TaskInstance_children.name() = "TaskInstance";
			::CSched::TaskInstances::meta_TaskInstance_children.min() = 0;
			::CSched::TaskInstances::meta_TaskInstance_children.max() = -1;
			::CSched::TaskInstances::meta_TaskInstance_children.isNavigable() = true;
			::CSched::TaskInstances::meta_TaskInstance_children.target() = ::CSched::TaskInstance::meta;
		}
		void InitComposition20()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::TaskRef::meta_TaskInstances_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::TaskRef::meta_TaskInstances_parent.name() = "TaskInstances_parent";
			::CSched::TaskRef::meta_TaskInstances_parent.isNavigable() = true;
			::CSched::TaskRef::meta_TaskInstances_parent.target() = ::CSched::TaskInstances::meta;
			::CSched::TaskInstances::meta_TaskRef_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::TaskInstances::meta_TaskRef_children.name() = "TaskRef";
			::CSched::TaskInstances::meta_TaskRef_children.min() = 0;
			::CSched::TaskInstances::meta_TaskRef_children.max() = -1;
			::CSched::TaskInstances::meta_TaskRef_children.isNavigable() = true;
			::CSched::TaskInstances::meta_TaskRef_children.target() = ::CSched::TaskRef::meta;
		}
		void InitComposition21()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::MsgRef::meta_MsgInstances_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::MsgRef::meta_MsgInstances_parent.name() = "MsgInstances_parent";
			::CSched::MsgRef::meta_MsgInstances_parent.isNavigable() = true;
			::CSched::MsgRef::meta_MsgInstances_parent.target() = ::CSched::MsgInstances::meta;
			::CSched::MsgInstances::meta_MsgRef_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::MsgInstances::meta_MsgRef_children.name() = "MsgRef";
			::CSched::MsgInstances::meta_MsgRef_children.min() = 0;
			::CSched::MsgInstances::meta_MsgRef_children.max() = -1;
			::CSched::MsgInstances::meta_MsgRef_children.isNavigable() = true;
			::CSched::MsgInstances::meta_MsgRef_children.target() = ::CSched::MsgRef::meta;
		}
		void InitComposition22()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::MsgInstance::meta_MsgInstances_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::MsgInstance::meta_MsgInstances_parent.name() = "MsgInstances_parent";
			::CSched::MsgInstance::meta_MsgInstances_parent.isNavigable() = true;
			::CSched::MsgInstance::meta_MsgInstances_parent.target() = ::CSched::MsgInstances::meta;
			::CSched::MsgInstances::meta_MsgInstance_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::MsgInstances::meta_MsgInstance_children.name() = "MsgInstance";
			::CSched::MsgInstances::meta_MsgInstance_children.min() = 0;
			::CSched::MsgInstances::meta_MsgInstance_children.max() = -1;
			::CSched::MsgInstances::meta_MsgInstance_children.isNavigable() = true;
			::CSched::MsgInstances::meta_MsgInstance_children.target() = ::CSched::MsgInstance::meta;
		}
		void InitComposition23()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::MsgRef::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::MsgRef::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::CSched::MsgRef::meta_LatencySpec_parent.isNavigable() = true;
			::CSched::MsgRef::meta_LatencySpec_parent.target() = ::CSched::LatencySpec::meta;
			::CSched::LatencySpec::meta_MsgRef_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::LatencySpec::meta_MsgRef_children.name() = "MsgRef";
			::CSched::LatencySpec::meta_MsgRef_children.min() = 0;
			::CSched::LatencySpec::meta_MsgRef_children.max() = -1;
			::CSched::LatencySpec::meta_MsgRef_children.isNavigable() = true;
			::CSched::LatencySpec::meta_MsgRef_children.target() = ::CSched::MsgRef::meta;
		}
		void InitComposition24()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Latency::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Latency::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::CSched::Latency::meta_LatencySpec_parent.isNavigable() = true;
			::CSched::Latency::meta_LatencySpec_parent.target() = ::CSched::LatencySpec::meta;
			::CSched::LatencySpec::meta_Latency_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::LatencySpec::meta_Latency_children.name() = "Latency";
			::CSched::LatencySpec::meta_Latency_children.min() = 0;
			::CSched::LatencySpec::meta_Latency_children.max() = -1;
			::CSched::LatencySpec::meta_Latency_children.isNavigable() = true;
			::CSched::LatencySpec::meta_Latency_children.target() = ::CSched::Latency::meta;
		}
		void InitComposition25()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::StartMsg::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::StartMsg::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::CSched::StartMsg::meta_LatencySpec_parent.isNavigable() = true;
			::CSched::StartMsg::meta_LatencySpec_parent.target() = ::CSched::LatencySpec::meta;
			::CSched::LatencySpec::meta_StartMsg_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::LatencySpec::meta_StartMsg_children.name() = "StartMsg";
			::CSched::LatencySpec::meta_StartMsg_children.min() = 0;
			::CSched::LatencySpec::meta_StartMsg_children.max() = -1;
			::CSched::LatencySpec::meta_StartMsg_children.isNavigable() = true;
			::CSched::LatencySpec::meta_StartMsg_children.target() = ::CSched::StartMsg::meta;
		}
		void InitComposition26()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Dependency::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Dependency::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::CSched::Dependency::meta_LatencySpec_parent.isNavigable() = true;
			::CSched::Dependency::meta_LatencySpec_parent.target() = ::CSched::LatencySpec::meta;
			::CSched::LatencySpec::meta_Dependency_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::LatencySpec::meta_Dependency_children.name() = "Dependency";
			::CSched::LatencySpec::meta_Dependency_children.min() = 0;
			::CSched::LatencySpec::meta_Dependency_children.max() = -1;
			::CSched::LatencySpec::meta_Dependency_children.isNavigable() = true;
			::CSched::LatencySpec::meta_Dependency_children.target() = ::CSched::Dependency::meta;
		}
		void InitComposition27()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::EndMsg::meta_LatencySpec_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::EndMsg::meta_LatencySpec_parent.name() = "LatencySpec_parent";
			::CSched::EndMsg::meta_LatencySpec_parent.isNavigable() = true;
			::CSched::EndMsg::meta_LatencySpec_parent.target() = ::CSched::LatencySpec::meta;
			::CSched::LatencySpec::meta_EndMsg_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::LatencySpec::meta_EndMsg_children.name() = "EndMsg";
			::CSched::LatencySpec::meta_EndMsg_children.min() = 0;
			::CSched::LatencySpec::meta_EndMsg_children.max() = -1;
			::CSched::LatencySpec::meta_EndMsg_children.isNavigable() = true;
			::CSched::LatencySpec::meta_EndMsg_children.target() = ::CSched::EndMsg::meta;
		}
		void InitComposition28()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::TaskInstances::meta_Processor_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::TaskInstances::meta_Processor_parent.name() = "Processor_parent";
			::CSched::TaskInstances::meta_Processor_parent.isNavigable() = true;
			::CSched::TaskInstances::meta_Processor_parent.target() = ::CSched::Processor::meta;
			::CSched::Processor::meta_TaskInstances_child = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Processor::meta_TaskInstances_child.name() = "TaskInstances";
			::CSched::Processor::meta_TaskInstances_child.min() = 0;
			::CSched::Processor::meta_TaskInstances_child.max() = 1;
			::CSched::Processor::meta_TaskInstances_child.isNavigable() = true;
			::CSched::Processor::meta_TaskInstances_child.target() = ::CSched::TaskInstances::meta;
		}
		void InitComposition29()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Task::meta_Processor_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Task::meta_Processor_parent.name() = "Processor_parent";
			::CSched::Task::meta_Processor_parent.isNavigable() = true;
			::CSched::Task::meta_Processor_parent.target() = ::CSched::Processor::meta;
			::CSched::Processor::meta_Task_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Processor::meta_Task_children.name() = "Task";
			::CSched::Processor::meta_Task_children.min() = 0;
			::CSched::Processor::meta_Task_children.max() = -1;
			::CSched::Processor::meta_Task_children.isNavigable() = true;
			::CSched::Processor::meta_Task_children.target() = ::CSched::Task::meta;
		}
		void InitComposition30()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Processor::meta_TTSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Processor::meta_TTSystem_parent.name() = "TTSystem_parent";
			::CSched::Processor::meta_TTSystem_parent.isNavigable() = true;
			::CSched::Processor::meta_TTSystem_parent.target() = ::CSched::TTSystem::meta;
			::CSched::TTSystem::meta_Processor_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::TTSystem::meta_Processor_children.name() = "Processor";
			::CSched::TTSystem::meta_Processor_children.min() = 0;
			::CSched::TTSystem::meta_Processor_children.max() = -1;
			::CSched::TTSystem::meta_Processor_children.isNavigable() = true;
			::CSched::TTSystem::meta_Processor_children.target() = ::CSched::Processor::meta;
		}
		void InitComposition31()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::LatencySpec::meta_TTSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::LatencySpec::meta_TTSystem_parent.name() = "TTSystem_parent";
			::CSched::LatencySpec::meta_TTSystem_parent.isNavigable() = true;
			::CSched::LatencySpec::meta_TTSystem_parent.target() = ::CSched::TTSystem::meta;
			::CSched::TTSystem::meta_LatencySpec_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::TTSystem::meta_LatencySpec_children.name() = "LatencySpec";
			::CSched::TTSystem::meta_LatencySpec_children.min() = 0;
			::CSched::TTSystem::meta_LatencySpec_children.max() = -1;
			::CSched::TTSystem::meta_LatencySpec_children.isNavigable() = true;
			::CSched::TTSystem::meta_LatencySpec_children.target() = ::CSched::LatencySpec::meta;
		}
		void InitComposition32()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Bus::meta_TTSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Bus::meta_TTSystem_parent.name() = "TTSystem_parent";
			::CSched::Bus::meta_TTSystem_parent.isNavigable() = true;
			::CSched::Bus::meta_TTSystem_parent.target() = ::CSched::TTSystem::meta;
			::CSched::TTSystem::meta_Bus_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::TTSystem::meta_Bus_children.name() = "Bus";
			::CSched::TTSystem::meta_Bus_children.min() = 0;
			::CSched::TTSystem::meta_Bus_children.max() = -1;
			::CSched::TTSystem::meta_Bus_children.isNavigable() = true;
			::CSched::TTSystem::meta_Bus_children.target() = ::CSched::Bus::meta;
		}
		void InitComposition33()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Link::meta_TTSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Link::meta_TTSystem_parent.name() = "TTSystem_parent";
			::CSched::Link::meta_TTSystem_parent.isNavigable() = true;
			::CSched::Link::meta_TTSystem_parent.target() = ::CSched::TTSystem::meta;
			::CSched::TTSystem::meta_Link_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::TTSystem::meta_Link_children.name() = "Link";
			::CSched::TTSystem::meta_Link_children.min() = 0;
			::CSched::TTSystem::meta_Link_children.max() = -1;
			::CSched::TTSystem::meta_Link_children.isNavigable() = true;
			::CSched::TTSystem::meta_Link_children.target() = ::CSched::Link::meta;
		}
		void InitComposition34()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::TaskRef::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::TaskRef::meta_Bus_parent.name() = "Bus_parent";
			::CSched::TaskRef::meta_Bus_parent.isNavigable() = true;
			::CSched::TaskRef::meta_Bus_parent.target() = ::CSched::Bus::meta;
			::CSched::Bus::meta_TaskRef_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Bus::meta_TaskRef_children.name() = "TaskRef";
			::CSched::Bus::meta_TaskRef_children.min() = 0;
			::CSched::Bus::meta_TaskRef_children.max() = -1;
			::CSched::Bus::meta_TaskRef_children.isNavigable() = true;
			::CSched::Bus::meta_TaskRef_children.target() = ::CSched::TaskRef::meta;
		}
		void InitComposition35()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Sender::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Sender::meta_Bus_parent.name() = "Bus_parent";
			::CSched::Sender::meta_Bus_parent.isNavigable() = true;
			::CSched::Sender::meta_Bus_parent.target() = ::CSched::Bus::meta;
			::CSched::Bus::meta_Sender_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Bus::meta_Sender_children.name() = "Sender";
			::CSched::Bus::meta_Sender_children.min() = 0;
			::CSched::Bus::meta_Sender_children.max() = -1;
			::CSched::Bus::meta_Sender_children.isNavigable() = true;
			::CSched::Bus::meta_Sender_children.target() = ::CSched::Sender::meta;
		}
		void InitComposition36()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Message::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Message::meta_Bus_parent.name() = "Bus_parent";
			::CSched::Message::meta_Bus_parent.isNavigable() = true;
			::CSched::Message::meta_Bus_parent.target() = ::CSched::Bus::meta;
			::CSched::Bus::meta_Message_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Bus::meta_Message_children.name() = "Message";
			::CSched::Bus::meta_Message_children.min() = 0;
			::CSched::Bus::meta_Message_children.max() = -1;
			::CSched::Bus::meta_Message_children.isNavigable() = true;
			::CSched::Bus::meta_Message_children.target() = ::CSched::Message::meta;
		}
		void InitComposition37()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Receiver::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Receiver::meta_Bus_parent.name() = "Bus_parent";
			::CSched::Receiver::meta_Bus_parent.isNavigable() = true;
			::CSched::Receiver::meta_Bus_parent.target() = ::CSched::Bus::meta;
			::CSched::Bus::meta_Receiver_children = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Bus::meta_Receiver_children.name() = "Receiver";
			::CSched::Bus::meta_Receiver_children.min() = 0;
			::CSched::Bus::meta_Receiver_children.max() = -1;
			::CSched::Bus::meta_Receiver_children.isNavigable() = true;
			::CSched::Bus::meta_Receiver_children.target() = ::CSched::Receiver::meta;
		}
		void InitComposition38()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::TimeSync::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::TimeSync::meta_Bus_parent.name() = "Bus_parent";
			::CSched::TimeSync::meta_Bus_parent.isNavigable() = true;
			::CSched::TimeSync::meta_Bus_parent.target() = ::CSched::Bus::meta;
			::CSched::Bus::meta_TimeSync_child = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Bus::meta_TimeSync_child.name() = "TimeSync";
			::CSched::Bus::meta_TimeSync_child.min() = 0;
			::CSched::Bus::meta_TimeSync_child.max() = 1;
			::CSched::Bus::meta_TimeSync_child.isNavigable() = true;
			::CSched::Bus::meta_TimeSync_child.target() = ::CSched::TimeSync::meta;
		}
		void InitComposition39()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::MsgInstances::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::MsgInstances::meta_Bus_parent.name() = "Bus_parent";
			::CSched::MsgInstances::meta_Bus_parent.isNavigable() = true;
			::CSched::MsgInstances::meta_Bus_parent.target() = ::CSched::Bus::meta;
			::CSched::Bus::meta_MsgInstances_child = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Bus::meta_MsgInstances_child.name() = "MsgInstances";
			::CSched::Bus::meta_MsgInstances_child.min() = 0;
			::CSched::Bus::meta_MsgInstances_child.max() = 1;
			::CSched::Bus::meta_MsgInstances_child.isNavigable() = true;
			::CSched::Bus::meta_MsgInstances_child.target() = ::CSched::MsgInstances::meta;
		}
		void InitComposition40()
		{
			::Uml::Composition comp = ::Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;
			::CSched::Dependencies::meta_Bus_parent = ::Uml::CompositionParentRole::Create(comp);
			::CSched::Dependencies::meta_Bus_parent.name() = "Bus_parent";
			::CSched::Dependencies::meta_Bus_parent.isNavigable() = true;
			::CSched::Dependencies::meta_Bus_parent.target() = ::CSched::Bus::meta;
			::CSched::Bus::meta_Dependencies_child = ::Uml::CompositionChildRole::Create(comp);
			::CSched::Bus::meta_Dependencies_child.name() = "Dependencies";
			::CSched::Bus::meta_Dependencies_child.min() = 0;
			::CSched::Bus::meta_Dependencies_child.max() = 1;
			::CSched::Bus::meta_Dependencies_child.isNavigable() = true;
			::CSched::Bus::meta_Dependencies_child.target() = ::CSched::Dependencies::meta;
		}
		void InitCrossNSCompositions()
		{
		}
		void InitInheritence()
		{
			Connectable::meta.subTypes() +=  ::CSched::XPort::meta;
			Connectable::meta.subTypes() +=  ::CSched::YPort::meta;
			Connectable::meta.subTypes() +=  ::CSched::RangeConstraint::meta;
			Connectable::meta.subTypes() +=  ::CSched::SerialConstraint::meta;
			PairConstraint::meta.subTypes() +=  ::CSched::InequalityConstraint::meta;
			PairConstraint::meta.subTypes() +=  ::CSched::EqualityConstraint::meta;
			Constraint::meta.subTypes() +=  ::CSched::PairConstraint::meta;
			Constraint::meta.subTypes() +=  ::CSched::RangeConstraint::meta;
			Constraint::meta.subTypes() +=  ::CSched::SerialConstraint::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Assignment::meta;
			MgaObject::meta.subTypes() +=  ::CSched::InstanceRef::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Solution::meta;
			MgaObject::meta.subTypes() +=  ::CSched::SearchConfig::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Connectable::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Member::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Constraint::meta;
			MgaObject::meta.subTypes() +=  ::CSched::ConstraintProblem::meta;
			MgaObject::meta.subTypes() +=  ::CSched::FDVar::meta;
			MgaObject::meta.subTypes() +=  ::CSched::ReceiverInst::meta;
			MgaObject::meta.subTypes() +=  ::CSched::MsgInstance::meta;
			MgaObject::meta.subTypes() +=  ::CSched::SenderInst::meta;
			MgaObject::meta.subTypes() +=  ::CSched::MsgInstRef::meta;
			MgaObject::meta.subTypes() +=  ::CSched::InstanceModel::meta;
			MgaObject::meta.subTypes() +=  ::CSched::TaskInstRef::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Dependencies::meta;
			MgaObject::meta.subTypes() +=  ::CSched::TaskInstances::meta;
			MgaObject::meta.subTypes() +=  ::CSched::MsgInstances::meta;
			MgaObject::meta.subTypes() +=  ::CSched::TaskInstance::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Latency::meta;
			MgaObject::meta.subTypes() +=  ::CSched::LatencySpec::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Processor::meta;
			MgaObject::meta.subTypes() +=  ::CSched::MsgRef::meta;
			MgaObject::meta.subTypes() +=  ::CSched::StartMsg::meta;
			MgaObject::meta.subTypes() +=  ::CSched::TaskRef::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Dependency::meta;
			MgaObject::meta.subTypes() +=  ::CSched::TimeSync::meta;
			MgaObject::meta.subTypes() +=  ::CSched::EndMsg::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Receiver::meta;
			MgaObject::meta.subTypes() +=  ::CSched::TTSystem::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Message::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Bus::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Sender::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Link::meta;
			MgaObject::meta.subTypes() +=  ::CSched::Task::meta;
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
			::Uml::SetClass(Assignment::meta, dgr, "Assignment");
			::Uml::SetAttribute(Assignment::meta_Value,Assignment::meta, "Value");
			::Uml::SetClass(InstanceRef::meta, dgr, "InstanceRef");
			::Uml::SetClass(Solution::meta, dgr, "Solution");
			::Uml::SetClass(SearchConfig::meta, dgr, "SearchConfig");
			::Uml::SetAttribute(SearchConfig::meta_MaxRunTimeSec,SearchConfig::meta, "MaxRunTimeSec");
			::Uml::SetAttribute(SearchConfig::meta_MaxFailures,SearchConfig::meta, "MaxFailures");
			::Uml::SetAttribute(SearchConfig::meta_SearchEngine,SearchConfig::meta, "SearchEngine");
			::Uml::SetAttribute(SearchConfig::meta_MaxSolutions,SearchConfig::meta, "MaxSolutions");
			::Uml::SetClass(Connectable::meta, dgr, "Connectable");
			::Uml::SetClass(PairConstraint::meta, dgr, "PairConstraint");
			::Uml::SetClass(XPort::meta, dgr, "XPort");
			::Uml::SetClass(YPort::meta, dgr, "YPort");
			::Uml::SetClass(RangeConstraint::meta, dgr, "RangeConstraint");
			::Uml::SetAttribute(RangeConstraint::meta_UpperBound,RangeConstraint::meta, "UpperBound");
			::Uml::SetAttribute(RangeConstraint::meta_LowerBound,RangeConstraint::meta, "LowerBound");
			::Uml::SetClass(Member::meta, dgr, "Member");
			::Uml::SetClass(SerialConstraint::meta, dgr, "SerialConstraint");
			::Uml::SetClass(Constraint::meta, dgr, "Constraint");
			::Uml::SetClass(ConstraintProblem::meta, dgr, "ConstraintProblem");
			::Uml::SetAttribute(ConstraintProblem::meta_UpperBound,ConstraintProblem::meta, "UpperBound");
			::Uml::SetAttribute(ConstraintProblem::meta_LowerBound,ConstraintProblem::meta, "LowerBound");
			::Uml::SetClass(FDVar::meta, dgr, "FDVar");
			::Uml::SetAttribute(FDVar::meta_UpperBound,FDVar::meta, "UpperBound");
			::Uml::SetAttribute(FDVar::meta_LowerBound,FDVar::meta, "LowerBound");
			::Uml::SetAttribute(FDVar::meta_Size,FDVar::meta, "Size");
			::Uml::SetClass(InequalityConstraint::meta, dgr, "InequalityConstraint");
			::Uml::SetAttribute(InequalityConstraint::meta_Offset,InequalityConstraint::meta, "Offset");
			::Uml::SetClass(EqualityConstraint::meta, dgr, "EqualityConstraint");
			::Uml::SetAttribute(EqualityConstraint::meta_Offset,EqualityConstraint::meta, "Offset");
			::Uml::SetClass(RootFolder::meta, dgr, "RootFolder");
			::Uml::SetAttribute(RootFolder::meta_name,RootFolder::meta, "name");
			::Uml::SetClass(MgaObject::meta, dgr, "MgaObject");
			::Uml::SetAttribute(MgaObject::meta_position,MgaObject::meta, "position");
			::Uml::SetAttribute(MgaObject::meta_name,MgaObject::meta, "name");
			::Uml::SetClass(ReceiverInst::meta, dgr, "ReceiverInst");
			::Uml::SetClass(MsgInstance::meta, dgr, "MsgInstance");
			::Uml::SetAttribute(MsgInstance::meta_RefName,MsgInstance::meta, "RefName");
			::Uml::SetAttribute(MsgInstance::meta_StartTime,MsgInstance::meta, "StartTime");
			::Uml::SetAttribute(MsgInstance::meta_InstID,MsgInstance::meta, "InstID");
			::Uml::SetClass(SenderInst::meta, dgr, "SenderInst");
			::Uml::SetClass(MsgInstRef::meta, dgr, "MsgInstRef");
			::Uml::SetClass(InstanceModel::meta, dgr, "InstanceModel");
			::Uml::SetAttribute(InstanceModel::meta_HyperPeriod,InstanceModel::meta, "HyperPeriod");
			::Uml::SetClass(TaskInstRef::meta, dgr, "TaskInstRef");
			::Uml::SetClass(Dependencies::meta, dgr, "Dependencies");
			::Uml::SetClass(TaskInstances::meta, dgr, "TaskInstances");
			::Uml::SetClass(MsgInstances::meta, dgr, "MsgInstances");
			::Uml::SetClass(TaskInstance::meta, dgr, "TaskInstance");
			::Uml::SetAttribute(TaskInstance::meta_RefName,TaskInstance::meta, "RefName");
			::Uml::SetAttribute(TaskInstance::meta_StartTime,TaskInstance::meta, "StartTime");
			::Uml::SetAttribute(TaskInstance::meta_InstID,TaskInstance::meta, "InstID");
			::Uml::SetClass(Latency::meta, dgr, "Latency");
			::Uml::SetAttribute(Latency::meta_MaxLength,Latency::meta, "MaxLength");
			::Uml::SetClass(LatencySpec::meta, dgr, "LatencySpec");
			::Uml::SetClass(Processor::meta, dgr, "Processor");
			::Uml::SetClass(MsgRef::meta, dgr, "MsgRef");
			::Uml::SetClass(StartMsg::meta, dgr, "StartMsg");
			::Uml::SetClass(TaskRef::meta, dgr, "TaskRef");
			::Uml::SetClass(Dependency::meta, dgr, "Dependency");
			::Uml::SetClass(TimeSync::meta, dgr, "TimeSync");
			::Uml::SetAttribute(TimeSync::meta_MsgLength,TimeSync::meta, "MsgLength");
			::Uml::SetAttribute(TimeSync::meta_Period,TimeSync::meta, "Period");
			::Uml::SetClass(EndMsg::meta, dgr, "EndMsg");
			::Uml::SetClass(Receiver::meta, dgr, "Receiver");
			::Uml::SetClass(TTSystem::meta, dgr, "TTSystem");
			::Uml::SetClass(Message::meta, dgr, "Message");
			::Uml::SetAttribute(Message::meta_MsgLength,Message::meta, "MsgLength");
			::Uml::SetClass(Bus::meta, dgr, "Bus");
			::Uml::SetAttribute(Bus::meta_Resolution,Bus::meta, "Resolution");
			::Uml::SetClass(Sender::meta, dgr, "Sender");
			::Uml::SetClass(Link::meta, dgr, "Link");
			::Uml::SetClass(Task::meta, dgr, "Task");
			::Uml::SetAttribute(Task::meta_WCExecTime,Task::meta, "WCExecTime");
			::Uml::SetAttribute(Task::meta_Period,Task::meta, "Period");
			//composition child roles 
			::Uml::SetParentRole(Assignment::meta_Solution_parent,Assignment::meta,::CSched::Solution::meta, "", "");
			::Uml::SetParentRole(InstanceRef::meta_ConstraintProblem_parent,InstanceRef::meta,::CSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(Solution::meta_ConstraintProblem_parent,Solution::meta,::CSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(SearchConfig::meta_ConstraintProblem_parent,SearchConfig::meta,::CSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(XPort::meta_PairConstraint_parent,XPort::meta,::CSched::PairConstraint::meta, "", "");
			::Uml::SetParentRole(YPort::meta_PairConstraint_parent,YPort::meta,::CSched::PairConstraint::meta, "", "");
			::Uml::SetParentRole(Member::meta_ConstraintProblem_parent,Member::meta,::CSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(Constraint::meta_ConstraintProblem_parent,Constraint::meta,::CSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(ConstraintProblem::meta_RootFolder_parent,ConstraintProblem::meta,::CSched::RootFolder::meta, "", "");
			::Uml::SetParentRole(FDVar::meta_ConstraintProblem_parent,FDVar::meta,::CSched::ConstraintProblem::meta, "", "");
			::Uml::SetParentRole(RootFolder::meta_RootFolder_parent,RootFolder::meta,::CSched::RootFolder::meta, "", "");
			::Uml::SetParentRole(ReceiverInst::meta_Dependencies_parent,ReceiverInst::meta,::CSched::Dependencies::meta, "", "");
			::Uml::SetParentRole(MsgInstance::meta_MsgInstances_parent,MsgInstance::meta,::CSched::MsgInstances::meta, "", "");
			::Uml::SetParentRole(SenderInst::meta_Dependencies_parent,SenderInst::meta,::CSched::Dependencies::meta, "", "");
			::Uml::SetParentRole(MsgInstRef::meta_Dependencies_parent,MsgInstRef::meta,::CSched::Dependencies::meta, "", "");
			::Uml::SetParentRole(InstanceModel::meta_RootFolder_parent,InstanceModel::meta,::CSched::RootFolder::meta, "", "");
			::Uml::SetParentRole(TaskInstRef::meta_Dependencies_parent,TaskInstRef::meta,::CSched::Dependencies::meta, "", "");
			::Uml::SetParentRole(Dependencies::meta_Bus_parent,Dependencies::meta,::CSched::Bus::meta, "", "");
			::Uml::SetParentRole(TaskInstances::meta_Processor_parent,TaskInstances::meta,::CSched::Processor::meta, "", "");
			::Uml::SetParentRole(MsgInstances::meta_Bus_parent,MsgInstances::meta,::CSched::Bus::meta, "", "");
			::Uml::SetParentRole(TaskInstance::meta_TaskInstances_parent,TaskInstance::meta,::CSched::TaskInstances::meta, "", "");
			::Uml::SetParentRole(Latency::meta_LatencySpec_parent,Latency::meta,::CSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(LatencySpec::meta_TTSystem_parent,LatencySpec::meta,::CSched::TTSystem::meta, "", "");
			::Uml::SetParentRole(Processor::meta_InstanceModel_parent,Processor::meta,::CSched::InstanceModel::meta, "", "");
			::Uml::SetParentRole(Processor::meta_TTSystem_parent,Processor::meta,::CSched::TTSystem::meta, "", "");
			::Uml::SetParentRole(MsgRef::meta_MsgInstances_parent,MsgRef::meta,::CSched::MsgInstances::meta, "", "");
			::Uml::SetParentRole(MsgRef::meta_LatencySpec_parent,MsgRef::meta,::CSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(StartMsg::meta_LatencySpec_parent,StartMsg::meta,::CSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(TaskRef::meta_TaskInstances_parent,TaskRef::meta,::CSched::TaskInstances::meta, "", "");
			::Uml::SetParentRole(TaskRef::meta_Bus_parent,TaskRef::meta,::CSched::Bus::meta, "", "");
			::Uml::SetParentRole(Dependency::meta_LatencySpec_parent,Dependency::meta,::CSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(TimeSync::meta_Bus_parent,TimeSync::meta,::CSched::Bus::meta, "", "");
			::Uml::SetParentRole(EndMsg::meta_LatencySpec_parent,EndMsg::meta,::CSched::LatencySpec::meta, "", "");
			::Uml::SetParentRole(Receiver::meta_Bus_parent,Receiver::meta,::CSched::Bus::meta, "", "");
			::Uml::SetParentRole(TTSystem::meta_RootFolder_parent,TTSystem::meta,::CSched::RootFolder::meta, "", "");
			::Uml::SetParentRole(Message::meta_Bus_parent,Message::meta,::CSched::Bus::meta, "", "");
			::Uml::SetParentRole(Bus::meta_InstanceModel_parent,Bus::meta,::CSched::InstanceModel::meta, "", "");
			::Uml::SetParentRole(Bus::meta_TTSystem_parent,Bus::meta,::CSched::TTSystem::meta, "", "");
			::Uml::SetParentRole(Sender::meta_Bus_parent,Sender::meta,::CSched::Bus::meta, "", "");
			::Uml::SetParentRole(Link::meta_TTSystem_parent,Link::meta,::CSched::TTSystem::meta, "", "");
			::Uml::SetParentRole(Task::meta_Processor_parent,Task::meta,::CSched::Processor::meta, "", "");
			// composition parentroles 
			::Uml::SetChildRole(Solution::meta_Assignment_children,Solution::meta,::CSched::Assignment::meta, "", "");
			::Uml::SetChildRole(PairConstraint::meta_YPort_children,PairConstraint::meta,::CSched::YPort::meta, "", "");
			::Uml::SetChildRole(PairConstraint::meta_XPort_children,PairConstraint::meta,::CSched::XPort::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_FDVar_children,ConstraintProblem::meta,::CSched::FDVar::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_Member_children,ConstraintProblem::meta,::CSched::Member::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_Constraint_children,ConstraintProblem::meta,::CSched::Constraint::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_Solution_children,ConstraintProblem::meta,::CSched::Solution::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_SearchConfig_child,ConstraintProblem::meta,::CSched::SearchConfig::meta, "", "");
			::Uml::SetChildRole(ConstraintProblem::meta_InstanceRef_child,ConstraintProblem::meta,::CSched::InstanceRef::meta, "", "");
			::Uml::SetChildRole(RootFolder::meta_InstanceModel_children,RootFolder::meta,::CSched::InstanceModel::meta, "", "");
			::Uml::SetChildRole(RootFolder::meta_ConstraintProblem_children,RootFolder::meta,::CSched::ConstraintProblem::meta, "", "");
			::Uml::SetChildRole(RootFolder::meta_TTSystem_children,RootFolder::meta,::CSched::TTSystem::meta, "", "");
			::Uml::SetChildRole(RootFolder::meta_RootFolder_children,RootFolder::meta,::CSched::RootFolder::meta, "", "");
			::Uml::SetChildRole(InstanceModel::meta_Bus_children,InstanceModel::meta,::CSched::Bus::meta, "", "");
			::Uml::SetChildRole(InstanceModel::meta_Processor_children,InstanceModel::meta,::CSched::Processor::meta, "", "");
			::Uml::SetChildRole(Dependencies::meta_TaskInstRef_children,Dependencies::meta,::CSched::TaskInstRef::meta, "", "");
			::Uml::SetChildRole(Dependencies::meta_MsgInstRef_children,Dependencies::meta,::CSched::MsgInstRef::meta, "", "");
			::Uml::SetChildRole(Dependencies::meta_SenderInst_children,Dependencies::meta,::CSched::SenderInst::meta, "", "");
			::Uml::SetChildRole(Dependencies::meta_ReceiverInst_children,Dependencies::meta,::CSched::ReceiverInst::meta, "", "");
			::Uml::SetChildRole(TaskInstances::meta_TaskInstance_children,TaskInstances::meta,::CSched::TaskInstance::meta, "", "");
			::Uml::SetChildRole(TaskInstances::meta_TaskRef_children,TaskInstances::meta,::CSched::TaskRef::meta, "", "");
			::Uml::SetChildRole(MsgInstances::meta_MsgRef_children,MsgInstances::meta,::CSched::MsgRef::meta, "", "");
			::Uml::SetChildRole(MsgInstances::meta_MsgInstance_children,MsgInstances::meta,::CSched::MsgInstance::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_MsgRef_children,LatencySpec::meta,::CSched::MsgRef::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_Latency_children,LatencySpec::meta,::CSched::Latency::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_StartMsg_children,LatencySpec::meta,::CSched::StartMsg::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_Dependency_children,LatencySpec::meta,::CSched::Dependency::meta, "", "");
			::Uml::SetChildRole(LatencySpec::meta_EndMsg_children,LatencySpec::meta,::CSched::EndMsg::meta, "", "");
			::Uml::SetChildRole(Processor::meta_TaskInstances_child,Processor::meta,::CSched::TaskInstances::meta, "", "");
			::Uml::SetChildRole(Processor::meta_Task_children,Processor::meta,::CSched::Task::meta, "", "");
			::Uml::SetChildRole(TTSystem::meta_Processor_children,TTSystem::meta,::CSched::Processor::meta, "", "");
			::Uml::SetChildRole(TTSystem::meta_LatencySpec_children,TTSystem::meta,::CSched::LatencySpec::meta, "", "");
			::Uml::SetChildRole(TTSystem::meta_Bus_children,TTSystem::meta,::CSched::Bus::meta, "", "");
			::Uml::SetChildRole(TTSystem::meta_Link_children,TTSystem::meta,::CSched::Link::meta, "", "");
			::Uml::SetChildRole(Bus::meta_TaskRef_children,Bus::meta,::CSched::TaskRef::meta, "", "");
			::Uml::SetChildRole(Bus::meta_Sender_children,Bus::meta,::CSched::Sender::meta, "", "");
			::Uml::SetChildRole(Bus::meta_Message_children,Bus::meta,::CSched::Message::meta, "", "");
			::Uml::SetChildRole(Bus::meta_Receiver_children,Bus::meta,::CSched::Receiver::meta, "", "");
			::Uml::SetChildRole(Bus::meta_TimeSync_child,Bus::meta,::CSched::TimeSync::meta, "", "");
			::Uml::SetChildRole(Bus::meta_MsgInstances_child,Bus::meta,::CSched::MsgInstances::meta, "", "");
			::Uml::SetChildRole(Bus::meta_Dependencies_child,Bus::meta,::CSched::Dependencies::meta, "", "");
			// Association roles 
			::Uml::SetAssocRole(InstanceRef::meta_ref,InstanceRef::meta,::CSched::InstanceModel::meta, "referedbyInstanceRef");
			::Uml::SetAssocRole(Connectable::meta_srcMember,Connectable::meta,::CSched::FDVar::meta, "dstMember");
			::CSched::FDVar::meta_dstMember_rev = Connectable::meta_srcMember;
			::Uml::SetAssocRole(FDVar::meta_dstMember,FDVar::meta,::CSched::Connectable::meta, "srcMember");
			::CSched::Connectable::meta_srcMember_rev = FDVar::meta_dstMember;
			::Uml::SetAssocRole(MsgInstance::meta_referedbyMsgInstRef,MsgInstance::meta,::CSched::MsgInstRef::meta, "ref");
			::Uml::SetAssocRole(MsgInstRef::meta_ref,MsgInstRef::meta,::CSched::MsgInstance::meta, "referedbyMsgInstRef");
			::Uml::SetAssocRole(MsgInstRef::meta_dstReceiverInst,MsgInstRef::meta,::CSched::TaskInstRef::meta, "srcReceiverInst");
			::CSched::TaskInstRef::meta_srcReceiverInst_rev = MsgInstRef::meta_dstReceiverInst;
			::Uml::SetAssocRole(MsgInstRef::meta_srcSenderInst,MsgInstRef::meta,::CSched::TaskInstRef::meta, "dstSenderInst");
			::CSched::TaskInstRef::meta_dstSenderInst_rev = MsgInstRef::meta_srcSenderInst;
			::Uml::SetAssocRole(InstanceModel::meta_referedbyInstanceRef,InstanceModel::meta,::CSched::InstanceRef::meta, "ref");
			::Uml::SetAssocRole(TaskInstRef::meta_srcReceiverInst,TaskInstRef::meta,::CSched::MsgInstRef::meta, "dstReceiverInst");
			::CSched::MsgInstRef::meta_dstReceiverInst_rev = TaskInstRef::meta_srcReceiverInst;
			::Uml::SetAssocRole(TaskInstRef::meta_dstSenderInst,TaskInstRef::meta,::CSched::MsgInstRef::meta, "srcSenderInst");
			::CSched::MsgInstRef::meta_srcSenderInst_rev = TaskInstRef::meta_dstSenderInst;
			::Uml::SetAssocRole(TaskInstRef::meta_ref,TaskInstRef::meta,::CSched::TaskInstance::meta, "referedbyTaskInstRef");
			::Uml::SetAssocRole(TaskInstance::meta_referedbyTaskInstRef,TaskInstance::meta,::CSched::TaskInstRef::meta, "ref");
			::Uml::SetAssocRole(Latency::meta_srcEndMsg,Latency::meta,::CSched::MsgRef::meta, "dstEndMsg");
			::CSched::MsgRef::meta_dstEndMsg_rev = Latency::meta_srcEndMsg;
			::Uml::SetAssocRole(Latency::meta_dstStartMsg,Latency::meta,::CSched::MsgRef::meta, "srcStartMsg");
			::CSched::MsgRef::meta_srcStartMsg_rev = Latency::meta_dstStartMsg;
			::Uml::SetAssocRole(Processor::meta_dstLink,Processor::meta,::CSched::Bus::meta, "srcLink");
			::CSched::Bus::meta_srcLink_rev = Processor::meta_dstLink;
			::Uml::SetAssocRole(MsgRef::meta_dstEndMsg,MsgRef::meta,::CSched::Latency::meta, "srcEndMsg");
			::CSched::Latency::meta_srcEndMsg_rev = MsgRef::meta_dstEndMsg;
			::Uml::SetAssocRole(MsgRef::meta_srcStartMsg,MsgRef::meta,::CSched::Latency::meta, "dstStartMsg");
			::CSched::Latency::meta_dstStartMsg_rev = MsgRef::meta_srcStartMsg;
			::Uml::SetAssocRole(MsgRef::meta_dstDependency,MsgRef::meta,::CSched::MsgRef::meta, "srcDependency");
			::CSched::MsgRef::meta_srcDependency_rev = MsgRef::meta_dstDependency;
			::Uml::SetAssocRole(MsgRef::meta_srcDependency,MsgRef::meta,::CSched::MsgRef::meta, "dstDependency");
			::CSched::MsgRef::meta_dstDependency_rev = MsgRef::meta_srcDependency;
			::Uml::SetAssocRole(MsgRef::meta_ref,MsgRef::meta,::CSched::Message::meta, "referedbyMsgRef");
			::Uml::SetAssocRole(TaskRef::meta_ref,TaskRef::meta,::CSched::Task::meta, "referedbyTaskRef");
			::Uml::SetAssocRole(TaskRef::meta_dstSender,TaskRef::meta,::CSched::Message::meta, "srcSender");
			::CSched::Message::meta_srcSender_rev = TaskRef::meta_dstSender;
			::Uml::SetAssocRole(TaskRef::meta_srcReceiver,TaskRef::meta,::CSched::Message::meta, "dstReceiver");
			::CSched::Message::meta_dstReceiver_rev = TaskRef::meta_srcReceiver;
			::Uml::SetAssocRole(Message::meta_referedbyMsgRef,Message::meta,::CSched::MsgRef::meta, "ref");
			::Uml::SetAssocRole(Message::meta_srcSender,Message::meta,::CSched::TaskRef::meta, "dstSender");
			::CSched::TaskRef::meta_dstSender_rev = Message::meta_srcSender;
			::Uml::SetAssocRole(Message::meta_dstReceiver,Message::meta,::CSched::TaskRef::meta, "srcReceiver");
			::CSched::TaskRef::meta_srcReceiver_rev = Message::meta_dstReceiver;
			::Uml::SetAssocRole(Bus::meta_srcLink,Bus::meta,::CSched::Processor::meta, "dstLink");
			::CSched::Processor::meta_dstLink_rev = Bus::meta_srcLink;
			::Uml::SetAssocRole(Task::meta_referedbyTaskRef,Task::meta,::CSched::TaskRef::meta, "ref");
			Member::meta_dstMember_end_ = ::CSched::FDVar::meta_dstMember;
			Member::meta_srcMember_end_ = ::CSched::Connectable::meta_srcMember;
			ReceiverInst::meta_srcReceiverInst_end_ = ::CSched::TaskInstRef::meta_srcReceiverInst;
			ReceiverInst::meta_dstReceiverInst_end_ = ::CSched::MsgInstRef::meta_dstReceiverInst;
			SenderInst::meta_dstSenderInst_end_ = ::CSched::TaskInstRef::meta_dstSenderInst;
			SenderInst::meta_srcSenderInst_end_ = ::CSched::MsgInstRef::meta_srcSenderInst;
			StartMsg::meta_srcStartMsg_end_ = ::CSched::MsgRef::meta_srcStartMsg;
			StartMsg::meta_dstStartMsg_end_ = ::CSched::Latency::meta_dstStartMsg;
			Dependency::meta_srcDependency_end_ = ::CSched::MsgRef::meta_srcDependency;
			Dependency::meta_dstDependency_end_ = ::CSched::MsgRef::meta_dstDependency;
			EndMsg::meta_dstEndMsg_end_ = ::CSched::MsgRef::meta_dstEndMsg;
			EndMsg::meta_srcEndMsg_end_ = ::CSched::Latency::meta_srcEndMsg;
			Receiver::meta_dstReceiver_end_ = ::CSched::Message::meta_dstReceiver;
			Receiver::meta_srcReceiver_end_ = ::CSched::TaskRef::meta_srcReceiver;
			Sender::meta_srcSender_end_ = ::CSched::Message::meta_srcSender;
			Sender::meta_dstSender_end_ = ::CSched::TaskRef::meta_dstSender;
			Link::meta_dstLink_end_ = ::CSched::Processor::meta_dstLink;
			Link::meta_srcLink_end_ = ::CSched::Bus::meta_srcLink;

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
		meta_dn->CreateNew("CSched.mem", "", ::Uml::Diagram::meta, Udm::CHANGES_LOST_DEFAULT);
		umldiagram = ::Uml::Diagram::Cast(meta_dn->GetRootObject());
		umldiagram.name() ="CSched";
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
			Udm::MetaDepository::StoreDiagram("CSched", diagram);
		};
		~_regClass()
		{
			Udm::MetaDepository::RemoveDiagram("CSched");
		};
	} __regUnUsed;

}
// END CSched.cpp
