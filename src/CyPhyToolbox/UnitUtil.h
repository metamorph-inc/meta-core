#ifndef UNIT_UTIL_H
#define UNIT_UTIL_H

#include "CyPhyML.h"
#include <map>
#include <math.h>

using namespace CyPhyML;

struct UnitUtil
{
	// concept of compound si

	struct DimensionRep
	{
		DimensionRep():
			length(std::numeric_limits<double>::quiet_NaN()),
			mass(std::numeric_limits<double>::quiet_NaN()),
			time(std::numeric_limits<double>::quiet_NaN()),
			current(std::numeric_limits<double>::quiet_NaN()),
			temperature(std::numeric_limits<double>::quiet_NaN()),
			amountSubstance(std::numeric_limits<double>::quiet_NaN()),
			luminous(std::numeric_limits<double>::quiet_NaN()),
			angle(std::numeric_limits<double>::quiet_NaN()) {}

		double length;		// length
		double mass;		// mass
		double time;		// time
		double current;		// electric current
		double temperature;		// temperature
		double amountSubstance;		// amount sustance
		double luminous;		// luminous intensity
		double angle; // SI defines radian as a "derived unit" m/m, but be pragmatic here instead

	private: DimensionRep(double) :
			length(0),
			mass(0),
			time(0),
			current(0),
			temperature(0),
			amountSubstance(0),
			luminous(0),
			angle(0) {}

	public:
		static DimensionRep zeroes;

		DimensionRep& operator=(const DimensionRep &rhs)
		{
			current = rhs.current;
			amountSubstance = rhs.amountSubstance;
			length = rhs.length;
			luminous = rhs.luminous;
			mass = rhs.mass;
			time = rhs.time;
			temperature = rhs.temperature;
			angle = rhs.angle;
			return *this;
		}

		bool operator==(const DimensionRep &rhs) const
		{
			auto equal = [](double a, double b) {
				return (_isnan(a) && _isnan(b)) || a == b;
			};
			return equal(rhs.amountSubstance, amountSubstance)
				&& equal(rhs.current, current)
				&& equal(rhs.length, length)
				&& equal(rhs.luminous, luminous)
				&& equal(rhs.mass, mass)
				&& equal(rhs.temperature, temperature)
				&& equal(rhs.time, time)
				&& equal(rhs.angle, angle);
		}

		bool operator!=(const DimensionRep &rhs) const
		{
			return !(this->operator==(rhs));
		}

		DimensionRep & DimensionRep::operator+=(const DimensionRep &rhs) 
		{
			auto add = [](double& a, double b) {
				if (_isnan(a) && _isnan(b)) {
					return;
				}
				a = (_isnan(a) ? 0 : a) + (_isnan(b) ? 0 : b);
			};
			add(current, rhs.current);
			add(luminous, rhs.luminous);
			add(temperature, rhs.temperature);
			add(mass, rhs.mass);
			add(length, rhs.length);
			add(amountSubstance, rhs.amountSubstance);
			add(time, rhs.time);
			add(angle, rhs.angle);
		    return *this;
		}

		void MultiplyByN(double n)
		{
			current *= n;
			luminous *= n;
			temperature *= n;
			mass *= n;
			length *= n;
			amountSubstance *= n;
			time *= n;
			angle *= n;
		}

	};

	struct ValueUnitRep
	{
		ValueUnitRep() :
			siValue(0),
			actualValue(0){}		
		ValueUnitRep(double v) :
			siValue(v),
			actualValue(0){}
		ValueUnitRep(UnitUtil& unitUtil, double, unit&);

		bool operator==(const ValueUnitRep &rhs) const {
			return siValue == rhs.siValue
				&& actualValue == rhs.actualValue
				&& cyphyRef == rhs.cyphyRef
				&& unitRep == rhs.unitRep
				&& strValue == rhs.strValue
				&& type == rhs.type;
		}

		double siValue;
		double actualValue;
		CyPhyML::unit cyphyRef;
		DimensionRep unitRep;

		std::string strValue;
		enum { DOUBLE, STRING } type;
	};



	// map<string, int> PrefixTable;
	// map<unit, double> SI_ConversionFactorMap;

	map<unit,DimensionRep> DimensionRep_Table;

	// conversion f() to SI base units
	double ConvertToSIEquivalent(unit&, double, DimensionRep&);
	double ConvertToSIEquivalent(conversion_based_unit&, DimensionRep&, double, double exponent = 1);
	double ConvertToSIEquivalent(derived_unit&, double, DimensionRep&);
	double ConvertValue_To_SIEquivalent(double value, double conversionFactor, double conversionOffset, double exponent);			// conversion function

	// conversion f() from SI base units
	double ConvertFromSIEquivalent(unit&, double);
	double ConvertFromSIEquivalent(conversion_based_unit&, double, double exponent = 1);
	double ConvertFromSIEquivalent(derived_unit&, double);
	double ConvertValue_From_SIEquivalent(double value, double conversionFactor, double conversionOffset, double exponent);			// conversion function


	void GetDimensionRep(si_unit&, DimensionRep&);

	// New Version
	struct ConversionInfo
	{
		ConversionInfo(double exp, double off, double factor):
			exponent(exp),
			offset(off),
			conversion_factor(factor){}
		double exponent;
		double offset;
		double conversion_factor;
	};

	class UnitConverter
	{
	public:
		double ToSI(double input)
		{
			double output = input;
			for (unsigned int i = 0; i<conversionVector.size(); i++)
			{
				output = pow((pow(output, 1/conversionVector[i].exponent)*conversionVector[i].conversion_factor - conversionVector[i].offset), conversionVector[i].exponent);
			}
			return output;
		}
		double FromSI(double input)
		{
			double output = input;
			for (unsigned int i = 0; i<conversionVector.size(); i++)
			{
				output = pow(((pow(output, 1/conversionVector[i].exponent) + conversionVector[i].offset)/conversionVector[i].conversion_factor), conversionVector[i].exponent);
			}
			return output;
		}

		vector<ConversionInfo> conversionVector;
		DimensionRep dimensions;

	};	
	
	map<CyPhyML::unit, UnitConverter> ConverterMap;

	double ConvertToSIEquivalent_WLookup(unit&, double, DimensionRep&);
	double ConvertFromSIEquivalent_WLookup(unit&, double);
	void CreateConverter(unit&, UnitConverter&);
	void CreateConverter(conversion_based_unit&, UnitConverter&, double exponent = 1);
	void CreateConverter(derived_unit&, UnitConverter&);


};

#endif