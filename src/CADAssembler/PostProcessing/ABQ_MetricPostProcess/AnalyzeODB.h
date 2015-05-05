#ifndef ANALYZE_ODB_H
#define ANALYZE_ODB_H

#include <string>
#include <map>
#include <vector>
#include <sstream>


using namespace std;


struct StructuralResults
{
	StructuralResults():
	mises(0),
	tresca(0),
	press(0),
	maxPrinciple(0),
	minPrinciple(0),
	midPrinciple(0),
	maxInPlane(0),
	minInPlane(0),
	outPlane(0),
	hasMises(0),
	hasTresca(0),
	hasPress(0),
	hasMaxPrinciple(0),
	hasMinPrinciple(0),
	hasMidPrinciple(0),
	hasMaxInPlane(0),
	hasMinInPlane(0),
	hasOutPlane(0),
	hasDisplacement(0){}

	bool hasMises;
	bool hasTresca;
	bool hasPress;
	bool hasMaxPrinciple;
	bool hasMinPrinciple;
	bool hasMidPrinciple;
	bool hasMaxInPlane;
	bool hasMinInPlane;
	bool hasOutPlane;
	bool hasDisplacement;

	float mises;
	float tresca;
	float press;
	float maxPrinciple;
	float minPrinciple;
	float midPrinciple;
	float maxInPlane;
	float minInPlane;
	float outPlane;
	float maxDisplacement;
};

struct Material
{
	Material():
		mises(0),
		bearing(0),
		shear(0) {}
	float mises;		// mises
	float bearing;		// press
	float shear;		// shear
};

enum MetricType
{
	FACTOR_OF_SAFETY,
	RESULT_QUALITY,
	PART_MAX_VON_MISES,
	PART_MAX_BEARING,
	PART_MAX_SHEAR,
	DISPLACEMENT
};

struct Metric
{
	MetricType type;
	string MetricID;
	string MetricValue;
};



float AnalyzeFEAResults();
bool FindStressFromODB(map<string, StructuralResults>&);
void WriteMetricsFile();


// helper
void ParseElementIDs(string);
void ParseMetricIDs(string);
void ParseMaterials(string);
void ParseMetricPairs(string);
void Tokenizer(const string& str, vector<string>& tokens, const string& delimiters = " ");

template <
	class CharType, 
	class Traits, 
	class Allocator,
	class SourceType
>
void to_string (std::basic_string <CharType, Traits, Allocator>& dst, SourceType src)
{
	std::basic_stringstream <CharType, Traits, Allocator> sstream;
	sstream << src;
	if(!sstream)
		throw std::bad_cast();

	dst=sstream.str();
}


class analyzer_exception : public std::exception
{
public:
		analyzer_exception(const char *message) throw()
			: exception(message),
			description(message)	
		{}

		analyzer_exception(const std::string &message) throw()
			: exception(message.c_str()),
			description(message)
		{}

		virtual ~analyzer_exception() throw() 
		{}

		const char* what() const throw() 
		{ 
			return description.c_str(); 
		}

protected:
	std::string description;

};




#endif  // ANALYZE_ODB_H
