#include "UnitUtil.h"
#include "UmlExt.h"

#include <fstream>
#include <math.h>

UnitUtil::DimensionRep UnitUtil::DimensionRep::zeroes = UnitUtil::DimensionRep(0.0);

	UnitUtil::ValueUnitRep::ValueUnitRep(UnitUtil& unitUtil, double aValue, unit& unitRef)
	{
		cyphyRef = unitRef;

		if (cyphyRef != Udm::null)
		{
			// calculate SI exponents, then save SI exponents in table
			siValue = unitUtil.ConvertToSIEquivalent(cyphyRef, aValue, unitRep);
			actualValue = aValue;		// NEW 12/21/11
		}
		else
		{
			siValue = aValue;
			actualValue = aValue;		// NEW 12/21/11
		}
	}


// conversion value from  derived/conversion unit to SI basic units
	double UnitUtil::ConvertToSIEquivalent(unit& cyphyRef, double value, UnitUtil::DimensionRep& siRep)
	{
		if (cyphyRef != Udm::null)
		{
			Uml::Class unitType = cyphyRef.type();
			if (unitType == si_unit::meta)
			{
				GetDimensionRep(si_unit::Cast(cyphyRef), siRep);
				return value;
			}
			else if (unitType == conversion_based_unit::meta)
			{
				return ConvertToSIEquivalent(conversion_based_unit::Cast(cyphyRef), siRep, value);
			}
			else if (unitType == derived_unit::meta)
			{
				return ConvertToSIEquivalent(derived_unit::Cast(cyphyRef), value, siRep);
			}
		}
		
		// else
		return value;
	}


	double UnitUtil::ConvertToSIEquivalent(conversion_based_unit& cunit, DimensionRep& unitRep, double value, double exponent)
	{
		double newValue = value;

		reference_unit refUnit = cunit.reference_unit_child();
		if (refUnit)
		{
			double conversionFactor = refUnit.conversion_factor(), conversionOffset = refUnit.conversion_offset();

			unit namedUnit = refUnit.ref();
			if (namedUnit.type() == conversion_based_unit::meta)
			{				
				newValue = ConvertValue_To_SIEquivalent(newValue, conversionFactor, conversionOffset, exponent);
				newValue = ConvertToSIEquivalent(conversion_based_unit::Cast(namedUnit), unitRep, newValue);
			}
			else if (namedUnit.type() == si_unit::meta)
			{
				GetDimensionRep(si_unit::Cast(namedUnit), unitRep);
				newValue = ConvertValue_To_SIEquivalent(value, conversionFactor, conversionOffset, exponent);
			}
			else		// DY 12/9/11: derived unit
			{
				newValue = ConvertToSIEquivalent(derived_unit::Cast(namedUnit), value, unitRep);
				newValue = ConvertValue_To_SIEquivalent(newValue, conversionFactor, conversionOffset, 1);
			}
		}
		return newValue;
	}


	double UnitUtil::ConvertToSIEquivalent(derived_unit& unit, double value, DimensionRep& siRep)
	{
		double newValue = value;
		set<derived_unit_element> unit_refs = unit.derived_unit_element_kind_children();
		for (set<derived_unit_element>::const_iterator ci = unit_refs.begin(); ci != unit_refs.end(); ci++)
		{
			DimensionRep tmpRep;
			double exponent = ci->exponent();
			named_unit namedUnit = ci->ref();
			if (namedUnit != Udm::null)
			{
				Uml::Class namedUnitType = namedUnit.type();
				if (namedUnitType == si_unit::meta)
				{
					GetDimensionRep(si_unit::Cast(namedUnit), tmpRep);
				}
				else
				{
					newValue = ConvertToSIEquivalent(conversion_based_unit::Cast(namedUnit), tmpRep, newValue, exponent);
				}
				tmpRep.MultiplyByN(exponent);
				siRep += tmpRep;
			}
		}
		return newValue;
	}


	double UnitUtil::ConvertValue_To_SIEquivalent(double value, double conversionFactor, double conversionOffset, double exponent)
	{
		// ((value^(1/exponent))*factor + offset)^exponent
		//convertedValue = pow((pow(value, 1/exponent)*conversionFactor + conversionOffset), exponent);
		double convertedValue = value, invertExp = 1/exponent;		
		
		// ((value^(1/exponent))*factor - offset)^exponent
		convertedValue = pow((pow(value, 1/exponent)*conversionFactor - conversionOffset), exponent);

		return convertedValue;
	}


	void UnitUtil::GetDimensionRep(si_unit& siUnit, DimensionRep& rep)
	{
		map<unit,DimensionRep>::const_iterator ci = DimensionRep_Table.find(siUnit);
		if (ci == DimensionRep_Table.end())
		{
			rep.current = siUnit.electric_current_exponent();
			rep.luminous = siUnit.luminous_intensity_exponent();
			rep.temperature = siUnit.thermodynamic_temperature_exponent();
			rep.mass = siUnit.mass_exponent();
			rep.length = siUnit.length_exponent();
			rep.amountSubstance = siUnit.amount_of_substance_exponent();
			rep.time = siUnit.time_exponent();
			rep.angle = 0;
			if (siUnit.name() == "Radian")
			{
				rep.angle = 1;
			}

			// FIXME this map is not cleared, but Udm::Objects from different project may have the same id!
			// DimensionRep_Table[siUnit] = rep;
		}
		else
		{
			rep = ci->second;
		}
	}

	// conversion value from SI basic units to derived/conversion unit
	double UnitUtil::ConvertFromSIEquivalent(unit& cyphyRef, double value)
	{
		if (cyphyRef != Udm::null)
		{
			Uml::Class unitType = cyphyRef.type();
			if (unitType == si_unit::meta)
			{
				return value;
			}
			else if (unitType == conversion_based_unit::meta)
			{
				return ConvertFromSIEquivalent(conversion_based_unit::Cast(cyphyRef), value);
			}
			else if (unitType == derived_unit::meta)
			{
				return ConvertFromSIEquivalent(derived_unit::Cast(cyphyRef), value);
			}
		}
		
		// else
		return value;
	}


	double UnitUtil::ConvertFromSIEquivalent(conversion_based_unit& cunit, double value, double exponent)
	{
		double newValue = value;

		reference_unit refUnit = cunit.reference_unit_child();
		if (refUnit)
		{
			double conversionFactor = refUnit.conversion_factor(), conversionOffset = refUnit.conversion_offset();

			unit namedUnit = refUnit.ref();
			if (namedUnit.type() == conversion_based_unit::meta)
			{				
				newValue = ConvertValue_From_SIEquivalent(newValue, conversionFactor, conversionOffset, exponent);
				newValue = ConvertFromSIEquivalent(conversion_based_unit::Cast(namedUnit), value);
			}
			else if (namedUnit.type() == si_unit::meta)
			{
				newValue = ConvertValue_From_SIEquivalent(value, conversionFactor, conversionOffset, exponent);
			}
			else		// derived unit
			{
				newValue = ConvertValue_From_SIEquivalent(newValue, conversionFactor, conversionOffset, 1);
				newValue = ConvertFromSIEquivalent(derived_unit::Cast(namedUnit), newValue);
			}
		}
		return newValue;
	}

	double UnitUtil::ConvertFromSIEquivalent(derived_unit& unit, double value)
	{
		double newValue = value;
		set<derived_unit_element> unit_refs = unit.derived_unit_element_kind_children();
		for (set<derived_unit_element>::const_iterator ci = unit_refs.begin(); ci != unit_refs.end(); ci++)
		{
			double exponent = ci->exponent();
			named_unit namedUnit = ci->ref();
			if (namedUnit != Udm::null)
			{
				Uml::Class namedUnitType = namedUnit.type();
				if (namedUnitType == conversion_based_unit::meta)
				{
					newValue = ConvertFromSIEquivalent(conversion_based_unit::Cast(namedUnit), newValue, exponent);
				}
			}
		}
		return newValue;
	}

	double UnitUtil::ConvertValue_From_SIEquivalent(double value, double conversionFactor, double conversionOffset, double exponent)
	{
		//((value^(1/exponent) - offset)/factor)^exponent
		//newValue = pow(((pow(value, 1/exponent) - conversionOffset)/conversionFactor), exponent);
		double newValue = value;		

		//((value^(1/exponent) + offset)/factor)^exponent
		newValue = pow(((pow(value, 1/exponent) + conversionOffset)/conversionFactor), exponent);

		return newValue;
	}

	// New Version Stuff
	void UnitUtil::CreateConverter(unit& cyphyRef, UnitConverter& converter)
	{		
		Uml::Class unitType = cyphyRef.type();
		if (unitType == si_unit::meta)
		{
			GetDimensionRep(si_unit::Cast(cyphyRef), converter.dimensions);
		}
		else if (unitType == conversion_based_unit::meta)
		{
			CreateConverter(conversion_based_unit::Cast(cyphyRef), converter);
		}
		else if (unitType == derived_unit::meta)
		{
			CreateConverter(derived_unit::Cast(cyphyRef), converter);
		}
	}

	void UnitUtil::CreateConverter(conversion_based_unit& cunit, UnitConverter& converter, double exponent)
	{
		reference_unit refUnit = cunit.reference_unit_child();
		if (refUnit)
		{
			double conversionFactor = refUnit.conversion_factor(), conversionOffset = refUnit.conversion_offset();

			unit namedUnit = refUnit.ref();
			if (namedUnit.type() == conversion_based_unit::meta)
			{				
				//newValue = ConvertValue_To_SIEquivalent(newValue, conversionFactor, conversionOffset, exponent);
				//newValue = ConvertToSIEquivalent(conversion_based_unit::Cast(namedUnit), unitRep, newValue);
				
				converter.conversionVector.push_back(ConversionInfo(exponent, conversionOffset, conversionFactor));
				CreateConverter(conversion_based_unit::Cast(namedUnit), converter);
			}
			else if (namedUnit.type() == si_unit::meta)
			{
				//GetDimensionRep(si_unit::Cast(namedUnit), unitRep);				
				//ConvertValue_To_SIEquivalent(value, conversionFactor, conversionOffset, exponent);


				DimensionRep tmp;
				GetDimensionRep(si_unit::Cast(namedUnit), tmp);
				converter.dimensions+=tmp;
				converter.conversionVector.push_back(ConversionInfo(exponent, conversionOffset, conversionFactor));
			}
			else		// DY 12/9/11: derived unit
			{
				//newValue = ConvertToSIEquivalent(derived_unit::Cast(namedUnit), value, unitRep);
				//newValue = ConvertValue_To_SIEquivalent(newValue, conversionFactor, conversionOffset, 1);

				CreateConverter(derived_unit::Cast(namedUnit), converter);
				converter.conversionVector.push_back(ConversionInfo(exponent, conversionOffset, conversionFactor));
			}
		}
	}

	void UnitUtil::CreateConverter(derived_unit& unit, UnitConverter& converter)
	{
		set<derived_unit_element> unit_refs = unit.derived_unit_element_kind_children();
		for (set<derived_unit_element>::const_iterator ci = unit_refs.begin(); ci != unit_refs.end(); ci++)
		{
			double exponent = ci->exponent();
			named_unit namedUnit = ci->ref();
			if (namedUnit != Udm::null)
			{
				Uml::Class namedUnitType = namedUnit.type();
				if (namedUnitType == si_unit::meta)
				{
					DimensionRep tmp;
					GetDimensionRep(si_unit::Cast(namedUnit), tmp);
					converter.dimensions+=tmp;
				}
				else
				{
					CreateConverter(conversion_based_unit::Cast(namedUnit), converter, exponent);
				}
				converter.dimensions.MultiplyByN(exponent);
			}
		}
	}

	double UnitUtil::ConvertToSIEquivalent_WLookup(unit& cyphyRef, double value, DimensionRep& siRep)
	{
		if (cyphyRef != Udm::null)
		{
			map<CyPhyML::unit, UnitConverter>::iterator i = ConverterMap.find(cyphyRef);
			if (i != ConverterMap.end())
			{
				siRep = i->second.dimensions;
				return i->second.ToSI(value);
			}
			else
			{
				// create UnitConverter
				// add UnitConverter to map
				UnitConverter converter;
				CreateConverter(cyphyRef, converter);
				ConverterMap[cyphyRef] = converter;
				siRep = converter.dimensions;
				return converter.ToSI(value);
			}
		}
		else
			return value;
	}

	double UnitUtil::ConvertFromSIEquivalent_WLookup(unit& cyphyRef, double value)
	{
		if (cyphyRef != Udm::null)
		{
			map<CyPhyML::unit, UnitConverter>::iterator i = ConverterMap.find(cyphyRef);
			if (i != ConverterMap.end())
			{
				return i->second.FromSI(value);
			}
			else
			{
				// create UnitConverter
				// add UnitConverter to map
				UnitConverter converter;
				CreateConverter(cyphyRef, converter);
				ConverterMap[cyphyRef] = converter;
				return converter.FromSI(value);
			}
		}
		else
			return value;
	}

