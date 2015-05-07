#ifndef DECKCONVERTER_H
#define DECKCONVERTER_H

#include "../CADCommonFunctions/Nastran.h"

namespace isis
{
	class DeckConverterBase
	{
	public:
		virtual void ConvertNastranDeck(isis_CADCommon::NastranDeck&, const std::string& ) = 0;
		static std::string StaticStep;
		static std::string FreqStep;
		static std::string BuckleStep;
		static std::string SteadyStep;
		static std::string DynamicStep;
	};


	class CalculixConverter
	{
	public:
		void DetermineElementFace(std::vector<int> faceNodes, int &face);
		void ConvertNastranDeck(isis_CADCommon::NastranDeck&, const std::string& );

	protected:
		void RectangularCordTransformation();

	};

// class ElmerConverter
	class ElmerConverter
	{
	public:
		void ConvertNastranDeck(isis_CADCommon::NastranDeck&, const std::string& );
	};
}




#endif