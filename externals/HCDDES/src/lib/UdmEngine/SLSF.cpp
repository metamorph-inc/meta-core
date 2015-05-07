#include "SLSF.h"

#if PARADIGM==ESMoL_PARADIGM

# include	<ESMoL.cpp>

#elif PARADIGM==FCVMS_PARADIGM

# include	<FCSL.cpp>

#elif PARADIGM==SDR_PARADIGM

# include	<SCAML.cpp>

#elif PARADIGM==CyPhyML_PARADIGM

#elif PARADIGM==CyberComposition_PARADIGM

# include   <CyberComposition.cpp>

#else

# include	<ESMoL.cpp>

#endif
