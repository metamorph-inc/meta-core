#ifndef DESERT2CYPHY_H
#define DESERT2CYPHY_H

#include "CyPhyML.h"
#include "DesertIfaceBack.h"
#include "UdmGme.h"
#include <Mga.h>



using namespace CyPhyML;
using namespace DesertIfaceBack;

typedef map<Configuration, std::unique_ptr<set<CyPhyML::DesignEntity> > > MorphMatrix;

void generateCyPhy(RootFolder &cyphy_rf, DesertBackSystem &desert_top, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);
void generateCyPhy(RootFolder &cyphy_rf, DesertBackSystem &desert_top, Configuration &cfg, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);
CyPhyML::CWC generateCyPhy(RootFolder &cyphy_rf, CyPhyML::Configurations &cfgModels, DesertBackSystem &desert_top, Configuration &cfg, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);

// MorphMatrix only (doesn't export to GME)
void generateMorphMatrixCfgsForGivenDesignSpace(RootFolder &cyphy_rf, DesertBackSystem &desert_top, Configuration &cfg, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);
void generateMorphMatrixCfgsForDesignSpace(RootFolder &cyphy_rf, DesertBackSystem &desert_top, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);
void generateMorphMatrixCfgsForGivenConfigs(RootFolder &cyphy_rf, CyPhyML::Configurations &cfgModels, DesertBackSystem &desert_top, Configuration &cfg, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);

#endif
