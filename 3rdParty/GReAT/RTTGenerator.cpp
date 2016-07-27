#include "RTTGenerator.h"

#include <UdmBase.h>
#include <UdmGME.h>
#include <Uml.h>

using namespace std;

RTTGenerator* RTTGenerator::_instance = 0;
std::string RTTGenerator::_rttRootName = "";

RTTGenerator* RTTGenerator::Instance()
{
    if( _instance == 0)
    {
		_instance = new RTTGenerator();
	}
    return _instance;
}

RTTGenerator* RTTGenerator::Instance(const std::string &file_path, const std::string& rttRoot)
{
    _rttRootName = rttRoot;
	if( _instance == 0)
    {
		_instance = new RTTGenerator(file_path);
	}
    return _instance;
}

RTTGenerator::RTTGenerator() : _ruleCounter(0), _rttdn(0)
{
	if(_filePath.empty()) return;
	else RTTGenerator(_filePath);
}

RTTGenerator::RTTGenerator(const std::string &file_path) : _filePath(file_path), _ruleCounter(0), _rttdn(0)
{
	_rttdn = new Udm::SmartDataNetwork(RTT::diagram);
	_rttdn->CreateNew(_filePath,"RTT",RTT::RootFolder::meta,Udm::CHANGES_LOST_DEFAULT);
	_rttRoot = RTT::RTTRoot::Create(RTT::RootFolder::Cast(_rttdn->GetRootObject()));
	_rttRoot.name() = _rttRootName;
}

void RTTGenerator::close()
{
	if(_rttdn)
	{
		//update the uniqueId if udmCopy applied
		if(!_dnUdmCopyMap.empty())
		{
			set<RTT::ExecuteRule> rules = _rttRoot.ExecuteRule_kind_children();
			for(set<RTT::ExecuteRule>::iterator i=rules.begin();i!=rules.end();++i)
			{
				set<RTT::UdmObject> objs = (*i).UdmObject_kind_children();
				for(set<RTT::UdmObject>::iterator obj_it=objs.begin();obj_it!=objs.end();++obj_it)
				{
					RTT::UdmObject obj = *obj_it;
					if( _dnUdmCopyMap.find(obj.dataNetworkId())!= _dnUdmCopyMap.end())
					{
						map<int, int> copyMap =  _dnUdmCopyMap[obj.dataNetworkId()];
						map<int, int>::iterator pos = copyMap.find(obj.uniqueObjId());
						if(pos!=copyMap.end())
							obj.uniqueObjId() = (*pos).second;
					}
				}
			}
			 _dnUdmCopyMap.clear();
		}
		_rttdn->CloseWithUpdate();
		delete _rttdn, _rttdn=0;
		_currRule = RTT::ExecuteRule::Cast(Udm::null);
	}
}

void RTTGenerator::convertUdmCopyMap(const UdmUtil::copy_assoc_map& copyMap, map<int, int>& copyIdMap, bool reverse)
{
	for(UdmUtil::copy_assoc_map::const_iterator pos=copyMap.begin();pos!=copyMap.end();++pos)
	{
		if(!reverse)
			copyIdMap[(*pos).first.uniqueId()] = (*pos).second.uniqueId();
		else
			copyIdMap[(*pos).second.uniqueId()] = (*pos).first.uniqueId();
	}
}

void RTTGenerator::addUdmCopyMap(int workDNId, map<int, int>& udmCopyMap)
{
	_dnUdmCopyMap[workDNId] = udmCopyMap;
}

void RTTGenerator::generateDataNetwork(int id, const std::string &filepath, const std::string& name)
{
	if(!_rttdn) return;
	RTT::DataNetwork dn = RTT::DataNetwork::Create(_rttRoot);
	dn.dataNetworkId() = id;
	dn.filePath() = filepath;
	dn.name() = name;
}

void RTTGenerator::generateRule(int id, const std::string &name)
{
	if(!_rttdn)
	{
		_currRule = RTT::ExecuteRule::Cast(Udm::null);
		return;
	}
	RTT::ExecuteRule rule = RTT::ExecuteRule::Create(_rttRoot);
	rule.uniqueObjId() = id;
	rule.name() = name;
	if(_currRule!=Udm::null)
	{
		RTT::Sequence seq = RTT::Sequence::Create(_rttRoot);
		seq.srcSequence_end() = _currRule;
		seq.dstSequence_end() = rule;
	}
	_currRule = rule;
	_currObjMap.clear();
}

void RTTGenerator::generateRTT(int srcId, int srcDNId, const std::string& srcName,
							   int dstId, int dstDNId, const std::string& dstName)
{
	if(_currRule==Udm::null) return;
	RTT::UdmObject srcObj, dstObj;
	map< pair<int, int>, RTT::UdmObject>::iterator pos = _currObjMap.find(make_pair(srcId, srcDNId));
	if(pos!=_currObjMap.end())
		srcObj = (*pos).second;
	else
	{
		srcObj = RTT::UdmObject::Create(_currRule);
		srcObj.name() = srcName;
		srcObj.uniqueObjId() = srcId;
		srcObj.dataNetworkId() = srcDNId;
		_currObjMap[make_pair(srcId, srcDNId)] = srcObj;
	}

	pos = _currObjMap.find(make_pair(dstId, dstDNId));
	if(pos!=_currObjMap.end())
		dstObj = (*pos).second;
	else
	{
		dstObj = RTT::UdmObject::Create(_currRule);
		dstObj.name() = dstName;
		dstObj.uniqueObjId() = dstId;
		dstObj.dataNetworkId() = dstDNId;
		_currObjMap[make_pair(dstId, dstDNId)] = dstObj;
	}
	
	RTT::TracesTo tracesto = RTT::TracesTo::Create(_currRule);
	tracesto.srcTracesTo_end() = srcObj;
	tracesto.dstTracesTo_end() = dstObj;
}