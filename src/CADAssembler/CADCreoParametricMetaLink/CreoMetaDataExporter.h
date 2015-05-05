#ifndef _CREOMETADATAEXPORTER_
#define _CREOMETADATAEXPORTER_

#include "ProAssembly.h"
#include "ProMdl.h"
#include "ProObjects.h"

struct FeatVisitID
{
    int id;
    ProFeature *result;
};

struct ConstraintData
{
    std::string compid1;
    std::string compid2;
    std::string dname1;
    std::string dname2;
    int attr;
};

struct GeomItemSearch
{
    GeomItemSearch()
    {
        found = false;
    }
    int id;
    bool found;
};

class CreoMetaDataExporter
{
private:
    ProMdl assembly;
    void ProcessComponent(ProFeature* comp, ProAsmcomppath *comp_path, std::vector<ConstraintData>& cdatav);
public:
    CreoMetaDataExporter(ProMdl assembly);
    void Export(std::string filename);
};


#endif