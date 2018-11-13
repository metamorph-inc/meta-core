#include <map>
#include "CreoModelToCyphyXML.h"
#include "ProMdl.h"
#include "ProSolid.h"
#include "ProFeatType.h"
#include "ProParameter.h"
#include "ProAsmcomp.h"
#include "cc_CommonDefinitions.h"   

// Boost-related includes
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>
#include <windows.h>


namespace isis
{
	void Component::write(ostream &s, int tab) const 
	{
		s << "<?xml version=\"1.0\"?>\n";
		s << string(tab,'\t') << "<ns1:Component xmlns:ns1=\"avm\" xmlns:ns2=\"modelica\" xmlns:ns3=\"cad\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ID=\"" << avmid << "\" Name=\"" << name << "\" Version=\"" << version << "\">" << endl;
		cadmodel.write(s, tab+1);
		for (vector<ResourceDep>::const_iterator it = resources.begin(); it != resources.end(); ++it)
		it->write(s, tab+1);
		s << string(tab,'\t') << "</ns1:Component>" << endl;
	}

	void CADModel::write(ostream &s, int tab) const
	{
		s << string(tab,'\t') << "<DomainModel Notes=\"" << notes << "\" xsi:type=\"ns3:CADModel\" UsesResource=\"" << mainresourceid << "\">" << endl;
		for (vector<Datum>::const_iterator it = datums.begin(); it != datums.end(); ++it)
			it->write(s, tab+1);
		for (vector<Parameter>::const_iterator it = parameters.begin(); it != parameters.end(); ++it)
			it->write(s, tab+1);
		for (vector<Parameter>::const_iterator it = modelmetrics.begin(); it != modelmetrics.end(); ++it)
			it->write(s, tab+1);
		s << string(tab,'\t') << "</DomainModel>" << endl;
	}

	void Datum::write(ostream &s, int tab) const
	{
		s << string(tab,'\t') << "<Datum Name=\"" << name << "\" ID=\"" << ID << "\" DatumName=\"" << datumname << "\" xsi:type=\"" << type << "\"/>" << endl;
	}

	void Parameter::write(ostream &s, int tab) const
	{
		s << string(tab,'\t') << "<" << keyw << " Name=\"" << name << "\">" << endl;
		value.write(s, tab+1);
		s << string(tab,'\t') << "</" << keyw << ">" << endl;
	}

	void FixedValue::write(ostream &s, int tab) const
	{
		s << string(tab,'\t') << "<Value DataType=\"" << datatype << "\" ID=\"" << id << "\" Unit=\"" << unit << "\">" << endl;
		s << string(tab+1,'\t') << "<ValueExpression xsi:type=\"ns1:FixedValue\">" << endl;
		s << string(tab+2,'\t') << "<Value>" << value << "</Value>" << endl;
		s << string(tab+1,'\t') << "</ValueExpression>" << endl;
		s << string(tab,'\t') << "</Value>" << endl;
	}

	void ResourceDep::write(ostream &s, int tab) const
	{
		s << string(tab, '\t') << "<ResourceDependency ID=\"" << id << "\" Name=\"" << name << "\" Notes=\"" << notes << "\" Path=\"" << path << "\"/>" << endl;
	}

	static map<string,string> accessmap;

	string getStrFromWstr(const wchar_t* ws)
	{
		wstring wstr(ws);
		return string(wstr.begin(),wstr.end());
	}

	static string GetDatumTypeStr(ProFeattype type)
	{
		switch (type)
		{
			case PRO_FEAT_DATUM:
				return "ns3:Plane";
			case PRO_FEAT_DATUM_AXIS:
				return "ns3:Axis";
			case PRO_FEAT_DATUM_POINT:
				return "ns3:Point";
			case PRO_FEAT_CSYS:
				return "ns3:CoordinateSystem";
			default:
				return "unknown";
		}
	}

	static string GetParamTypeStr(ProParamvalueType type)
	{
		switch (type)
		{
			case PRO_PARAM_DOUBLE:
				return "Real";
				break;
			case PRO_PARAM_STRING:
				return "String";
				break;
			case PRO_PARAM_INTEGER:
				return "Integer";
				break;
			case PRO_PARAM_BOOLEAN:
				return "Boolean";
				break;
			default:
				return "unknown";
				break;
		}
	}

	static string getExtension(ProMdlType p_type)
	{
		switch (p_type)
		{
		case PRO_MDL_ASSEMBLY:
			return ".ASM";
		case PRO_MDL_PART:
			return ".PRT";
		default:
			return "";
		}
	}

	bool IsDatumType(ProFeattype type)
	{
		return (type == PRO_FEAT_DATUM || type == PRO_FEAT_DATUM_AXIS || type == PRO_FEAT_DATUM_POINT || type == PRO_FEAT_CSYS);
	}

	static std::string getAbsolutePath(ProMdl& mdl) {
		ProMdlType p_type;
		ProMdlTypeGet(mdl, &p_type);
		if (p_type == PRO_MDL_ASSEMBLY || p_type == PRO_MDL_PART)
		{
			ProName name;
			ProMdlNameGet(mdl, name);
			ProMdldata p_data;

			ProMdlDataGet(mdl, &p_data);
			if (wcslen(p_data.device)) {
				return getStrFromWstr(p_data.device) + ":" + getStrFromWstr(p_data.path) + getStrFromWstr(name) + getExtension(p_type);
			}
			else {
				wchar_t currentDir[MAX_PATH] = { 0 };
				GetCurrentDirectoryW(_countof(currentDir), currentDir);
				return getStrFromWstr(currentDir) + "\\" + getStrFromWstr(p_data.path) + getStrFromWstr(name) + getExtension(p_type);
			}
		}
		return "";
	}

	static ProError FeatVisitCompOnly (ProFeature* p_feature, ProError status, ProAppData app_data)
	{
		Component *comp = (Component*)app_data;
		ProFeattype type;
		ProFeatureTypeGet(p_feature, &type);
		if (type==PRO_FEAT_COMPONENT) {
			ProMdl mdl;
			ProError err;
			err = ProAsmcompMdlGet(p_feature, &mdl);
			if (err == PRO_TK_NO_ERROR)
			{
				ProMdlType p_type;
				ProMdlTypeGet (mdl, &p_type);
				if (p_type==PRO_MDL_ASSEMBLY || p_type==PRO_MDL_PART)
				{
					ProName name;
					ProMdlNameGet (mdl, name);

					if (!comp->resourceExists(getStrFromWstr(name)+getExtension(p_type)))
					{
						boost::uuids::uuid guid = boost::uuids::random_generator()();
						comp->addResource(ResourceDep("id-"+boost::uuids::to_string(guid), getStrFromWstr(name)+getExtension(p_type), "", getAbsolutePath(mdl)));
					}
					ProSolidFeatVisit(ProMdlToSolid(mdl), FeatVisitCompOnly, NULL, comp);
				}
			}
		}
		return PRO_TK_NO_ERROR;
	}


	static ProError FeatVisit (ProFeature* p_feature, ProError status, ProAppData app_data)
	{
		Component *comp = (Component*)app_data;
		ProFeattype type;
		ProFeatureTypeGet(p_feature, &type);
		if (IsDatumType(type))
		{
			ProElempath path;
			ProValue value;
			ProValueData  value_data;
			ProElempathItem constr_type[3]; 

			constr_type[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
			constr_type[0].path_item.elem_id = PRO_E_STD_FEATURE_NAME;

			ProElempathAlloc(&path);
			ProElempathDataSet(path, constr_type, 1);
			ProError error = ProFeatureElemValueGet(p_feature, path, &value);
			wstring wname;
			ostringstream datumid;
			if (error == PRO_TK_NO_ERROR)
			{
				ProValueDataGet(value, &value_data);
			
				wname = wstring(value_data.v.w);

				if (wname==L"no_name" || wname==L"PTC_MATERIAL_NAME" || wname==L"MODELED_BY" || wname==L"DESCRIPTION") return PRO_TK_NO_ERROR;
			} else {
				 return PRO_TK_NO_ERROR;
			}
			datumid << "cad." << string(wname.begin(),wname.end());

			string datumname;

			// Get CYPHY_NAME as datumname, if set
			ProParameter datumNameParam;
			ProParamvalue datumNameValue;
			error = ProParameterInit (p_feature, const_cast<wchar_t*>(wstring(CYPHY_NAME.begin(),CYPHY_NAME.end()).c_str()), &datumNameParam);
			if (error == PRO_TK_NO_ERROR)
			{
				error = ProParameterValueGet(&datumNameParam, &datumNameValue);
				if (error == PRO_TK_NO_ERROR)
				{
					if (datumNameValue.type == PRO_PARAM_STRING)
					{
						wstring wstr(datumNameValue.value.s_val);
						datumname = string(wstr.begin(),wstr.end());
					}
				}
			}

			Datum datum(datumid.str(), string(wname.begin(),wname.end()), GetDatumTypeStr(type), datumname);
			comp->getModel().addDatum(datum);
		} else if (type==PRO_FEAT_COMPONENT) {
			ProMdl mdl;
			ProError err;
			err = ProAsmcompMdlGet(p_feature, &mdl);
			if (err == PRO_TK_NO_ERROR)
			{
				ProMdlType p_type;
				ProMdlTypeGet (mdl, &p_type);
				if (p_type==PRO_MDL_ASSEMBLY || p_type==PRO_MDL_PART)
				{
					ProName name;
					ProMdlNameGet (mdl, name);

					if (!comp->resourceExists(getStrFromWstr(name)+getExtension(p_type)))
					{
						boost::uuids::uuid guid = boost::uuids::random_generator()();
						comp->addResource(ResourceDep("id-"+boost::uuids::to_string(guid), getStrFromWstr(name)+getExtension(p_type), "", getAbsolutePath(mdl)));
					}
					ProSolidFeatVisit(ProMdlToSolid(mdl), FeatVisitCompOnly, NULL, comp);
				}
			}
		}
		return PRO_TK_NO_ERROR;
	}

	string GetParamValueStr(ProParamvalue *value, ProParamvalueType type)
	{
		ostringstream valuestr;
		int intval;
		double doubleval;
		ProLine strval;
		*strval = 0;
		short boolval;
		switch (type)
		{
		case PRO_PARAM_DOUBLE:
			ProParamvalueValueGet (value, type, &doubleval);
			valuestr << doubleval;
			break;
		case PRO_PARAM_STRING:
			ProParamvalueValueGet (value, type, strval);
			valuestr << getStrFromWstr(strval);
			break;
		case PRO_PARAM_INTEGER:
			ProParamvalueValueGet (value, type, &intval);
			valuestr << intval;
			break;
		case PRO_PARAM_BOOLEAN:
			ProParamvalueValueGet (value, type, &boolval);
			valuestr << boolval;
			break;
		default:
			valuestr << "unknown";
			break;
		}
		return valuestr.str();
	}

	ProError ParamAction(ProParameter *handle, ProError status, ProAppData data)
	{
		Component *comp = (Component*)data;
		string paramname = getStrFromWstr(handle->id);
		// Skip Meta-Link params
		if (paramname.size()>=3 && paramname.substr(0, 3)=="ML_")
		{
			return PRO_TK_NO_ERROR;
		}
		ProParamvalue value;
		ProParamvalueType type;
		ProUnititem units;
		ProParameterValueGet(handle, &value);
		ProError err = ProParamvalueTypeGet (&value, &type);
		/*if (err != PRO_TK_NO_ERROR)
		{
			ErrorDialog(L"Error getting parameter type");
		}*/


		err = ProParameterUnitsGet (handle, &units);
		string paramunit;
		if (err == PRO_TK_NO_ERROR)
			paramunit = getStrFromWstr(units.name);

		string valuestr = GetParamValueStr(&value, type);

		string typestr = GetParamTypeStr(type);


		bool isparam=true;
		string kw = "Parameter";
		if (accessmap[paramname]!="Full")
		{
			kw = "ModelMetric";
			isparam=false;
		}
		FixedValue val(GetParamTypeStr(type), "cad."+paramname, paramunit, valuestr);
		Parameter param(kw, paramname, val);
		if (isparam)
		{
			comp->getModel().addParam(param);
		} else {
			comp->getModel().addMMetric(param);
		}

		return PRO_TK_NO_ERROR;
	}


	void ReadParamTable(const string filename, map<string,string> &access)
	{
		ifstream file; 
		file.open(filename.c_str(), ifstream::in|ifstream::binary);
		file.seekg(0, std::ifstream::end);
		std::streamoff size = file.tellg();
		file.seekg(0, std::ifstream::beg);
		wchar_t *buffer = new wchar_t[size/2+1]; 
		file.seekg(2);
		file.read((char*)buffer, size);
		file.close();
		wstringstream wstr(buffer);
		while (!wstr.eof())
		{
			wstring wline;
			getline(wstr, wline);
			string b(wline.begin(),wline.end());
			if (!b.size() || b.find(',')==string::npos) continue;
			string name = b.substr(0, b.find(','));
			string acc = b.substr(name.size()+1);
			access[name] = acc;
		}

		return;
	}

	string SaveParamTable(ProModelitem p_model_item, ProMdl mdl)
	{
		char tmppath[255];
		char tmpfile[255];
		DWORD res = GetTempPath(255, tmppath);
		res = GetTempFileName(tmppath, "", 1234, tmpfile);
		string tmpfilestr(tmpfile);
		wstring tmpfilestrw(tmpfilestr.begin(),tmpfilestr.end());

		ProError err = ProParameterTableExport (&p_model_item, mdl, PRO_PARAMSELECT_MODEL, PRO_PARAMTABLE_EXPORT_CSV, (wchar_t*)tmpfilestrw.c_str(), (ProParamColumn)(PRO_PARAM_NAME_COLUMN|PRO_PARAM_ACCESS_COLUMN));
/*		if (err != PRO_TK_NO_ERROR)
		{
			ostringstream ostr;
			ostr << "Error: " << err;
			wstring werr(ostr.str().begin(),ostr.str().end());
			ErrorDialog(werr.c_str());
		}*/

		return tmpfilestr;
	}

	int FillComponentStructure(ProMdl mdl, Component &comp)
	{
		ProName mdlname;
		ProMdlNameGet(mdl, mdlname);
		ProModelitem p_model_item;
		CADModel model;
		comp.setCADModel(model);

		// Add the model itself as a resource
		ProMdlType p_type;
		ProMdlTypeGet (mdl, &p_type);
		if (p_type==PRO_MDL_ASSEMBLY || p_type==PRO_MDL_PART)
		{
			ProName name;
			ProMdlNameGet (mdl, name);
			ProMdldata p_data;
				
			ProMdlDataGet(mdl, &p_data);
			if (!comp.resourceExists(getStrFromWstr(name)+getExtension(p_type)))
			{
				boost::uuids::uuid guid = boost::uuids::random_generator()();
				comp.addResource(ResourceDep("id-"+boost::uuids::to_string(guid), getStrFromWstr(name)+getExtension(p_type), "", getAbsolutePath(mdl)));
			}
		}

		comp.setName(getStrFromWstr(mdlname)+getExtension(p_type));

		ProSolidFeatVisit(ProMdlToSolid(mdl), FeatVisit, NULL, (ProAppData)&comp);
		ProMdlToModelitem(mdl, &p_model_item);

		string tempfilename = SaveParamTable(p_model_item, mdl);

		accessmap.clear();
		ReadParamTable(tempfilename,accessmap);

		ProParameterVisit (&p_model_item, NULL, ParamAction, (ProAppData)&comp);

		return 0;
	}


	string CreoModelToCyphyXML(ProMdl mdl)
	{
		Component comp;
		FillComponentStructure(mdl, comp);

		ostringstream str;
	
		comp.write(str);

		return str.str();

	}
}