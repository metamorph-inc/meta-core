#include "StdAfx.h"

#include "DesertThread.h"

#include "DesMap.h"
#include "DesBackMap.h"
#include "CyPhyUtil.h"

#include <sstream>

uint32_t crc32(uint32_t crc, const uint8_t *buf, size_t size);

void Notify::reportProgress(const std::string &progress) 
{
	if (!m_quit)
	{
		CString str(progress.c_str());
		m_dlg.SendMessage(SET_PROGRESS, (LPARAM)(LPCTSTR) str);
	}
}

void Notify::reportStatus(StatusID s_id)
{
	if (!m_quit)
	{
		const StatusDefinition * s_def = LookUpStatus(s_id);
		m_dlg.SendMessage(SET_STATUS, (LPARAM)(LPCTSTR) s_def->desc, (LPARAM)s_def->percent);
	}
}

void Notify::finished() 
{
	m_dlg.PostMessage(DESERT_FINISHED, 0);
}

//============================
//DesertThread::DesertThread():m_notify(NULL){}

DesertThread::DesertThread(DesertIface::DesertSystem &dsystem, 
		                   const std::string &constraints,
						   UdmDesertMap &des_map,
					       DesertUdmMap &inv_des_map,
				           BaseNotify* notify, 
				           int stage, long& configCount) :
								m_ds(dsystem),
								m_constraints(constraints),
								m_notify(notify),
								m_stage(stage),
								m_des_map(des_map),
								m_inv_des_map(inv_des_map),
								m_configCount(configCount)
{

}

DesertThread::DesertThread(DesertIfaceBack::DesertBackSystem &dbacksystem, 
				           UdmDesertMap &des_map,
					       DesertUdmMap &inv_des_map,
						   BaseNotify* notify, 
						   int stage, long& configCount) :
								m_dbs(dbacksystem),
								m_notify(notify),
								m_stage(stage),
								m_des_map(des_map),
								m_inv_des_map(inv_des_map),
								m_configCount(configCount)
{

}

void DesertThread::RunDesert()
{
	
		using namespace DesertIface;
		using namespace BackIfaceFunctions;

		if(m_stage==1 || m_stage==0)
		{
			m_notify->reportStatus(SD_PREAPPLY);
			if(DesertFinit_preApply())
			{
				m_notify->reportStatus(SD_APPLY);
				DesertFinit_Apply(utf82cstring(m_constraints));
				m_configCount = Desert_getRealNoOfConfigurations();
			}
			else
			{
				m_notify->m_quit = true;
				m_notify->m_cancel = true;
			}
		}
		if((m_stage==2 || m_stage==0) && m_dbs)
		{			
			DBConfigurations * confs = (DBConfigurations * )DesertFinit_postApply();
	
			if (confs)
			{
				//	m_dbs.SystemName() = m_ds.SystemName();

				//elements, custommembers and natural members will be created
				//on the fly, as it they are participating in an assignment

				int confn = confs->GetCount();
				m_configCount = confs->GetCount();


				POSITION pos = confs->GetHeadPosition();
				//m_notify->reportStatus(SD_PREP);
				int count = 0;

				m_notify->reportStatus(SD_BACK);
				count = 0;
				pos = confs->GetHeadPosition();
				int confSize = confs->GetCount();
				char size_buff[10];
				itoa(confSize, size_buff, 10);
				std::string desc("Writing out Configuration: ");
				while(pos)
				{
					if(m_notify->m_cancel || m_notify->m_quit) {
						break;
					}

					DBConfiguration * config = confs->GetNext(pos);
					if (config)
					{
						char id_buff[10];
						itoa(config->id,id_buff, 10);
						std::string currdesc = desc+id_buff+"/"+size_buff;

						DOEVENTS();
						m_notify->reportProgress(currdesc.c_str());
				
						//create configuration
						DesertIfaceBack::Configuration dib_conf = DesertIfaceBack::Configuration::Create(m_dbs);				
						char buff[10];
						itoa(config->id, buff, 10);
						std::string temp = "Conf. no: "+(std::string)buff;
						dib_conf.name() = temp.c_str(); 
						dib_conf.id() = config->id;

						POSITION pos2 = config->alt_assignments.GetStartPosition();
						while (pos2)
						{
							long alt, alt_of;
							config->alt_assignments.GetNextAssoc(pos2, alt_of, alt);
							BackIfaceFunctions::CreateAlternativeAssignment(m_dbs, dib_conf, m_inv_des_map, alt_of, alt, true);
						};
				
						delete config;
					}//eo if (config)
				}//eo while(pos)
			} else {
				// No configs generated
				m_configCount = 0;
			}

			delete confs;
			set<DesertIfaceBack::Configuration> cfgs = m_dbs.Configuration_kind_children();			
			set<std::string> cfgids;
			int idx=1;
			for(set<DesertIfaceBack::Configuration>::iterator it=cfgs.begin();it!=cfgs.end();++it)
			{
				//set<DesertIfaceBack::Configuration>::iterator it=cfgs.begin();
				DesertIfaceBack::Configuration cfg = *it;
				std::string cfgid = cfg.cfgId();
				if(cfgids.find(cfgid)==cfgids.end())
				{
					cfgids.insert(cfgid);
					cfg.id() = idx;
					idx++;
					uint32_t id_hash = crc32(0, (const uint8_t*)cfgid.c_str(), cfgid.length());
					std::stringstream sstream;
					sstream << std::hex << std::uppercase << id_hash;
					std::string sstream_str = sstream.str();
					int cnt = sstream_str.size();
					std::string pre("");
					if(sstream_str.size()<8)
					{
						for(int i=0; i<8-cnt; i++)
						{
							pre.append("0");
						}
					}
					cfg.cfgId() = pre+sstream_str;
				}
				else
					cfg.DeleteObject();
			}
			//for(set<DesertIfaceBack::Configuration>::iterator it=cfgs.begin();it!=cfgs.end();++it)
			//{
			//	DesertIfaceBack::Configuration cfg = *it;
			//	std::string cfgid = cfg.cfgId();
			//	boost::crc_32_type id_hash;
			//	id_hash.process_bytes(cfgid.c_str(), cfgid.length());
			//	int uid = id_hash.checksum();
			//	std::stringstream sstream;
			//	sstream << std::hex <<uid;
			//	cfg.cfgId() = sstream.str();
			//}
		}//eo if (configs)
		else
		{
			m_notify->m_cancel = true;
			m_notify->m_quit = true;
		}
}
//============================
void DesertThread::operator()()
{
	CoInitialize(0);  //Magic happens here

	try{
		RunDesert();
	}
	catch(udm_exception &exc)
	{
		AfxMessageBox(CString(exc.what()));
		
		if(m_notify) 
		{
			m_notify->m_quit = true;
			m_notify->m_fail = true;
			m_notify->finished();
		}

		CoUninitialize();
		return;
	}
	catch(CDesertException *e)
	{
		e->ReportError();
//		AfxMessageBox(CString(e->GetErrorMessage()));
		if(m_notify) 
		{
			m_notify->m_quit = true;
			m_notify->m_fail = true;
			m_notify->m_invalidConstraint = e->GetConstraintName();
			m_notify->finished();
		}
		e->Delete();
		CoUninitialize();
		return;
	}

	if (m_notify) {
		m_notify->finished();
	}

	CoUninitialize();
}
