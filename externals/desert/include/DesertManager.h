#ifndef DESERTMANAGER_H
#define DESERTMANAGER_H

#include <list>
#include <fstream>
#include <exception>

class Desert_exception : public std::exception
{
public:
	Desert_exception() throw();
	Desert_exception(const Desert_exception &a) throw() : description(a.description) { }
	Desert_exception(const std::string &d) throw() : description(d) { }
	Desert_exception(const char *d) throw() : description(d) { }	
	virtual ~Desert_exception() throw() { }
	virtual const char *what() const throw() { return description.c_str(); }

protected:
	std::string description;
};

class DesertManager
{
private:
	std::string importer;
	std::string exporter;
	std::string inputModel;
	std::list<std::string> outputModels;
	std::string desertIfaceFile;
	std::string desertIfaceBackFile;
	std::string constraints;
	bool applyAll;
	int cfgCount;
	std::ofstream logFile;

public:
	DesertManager();
	~DesertManager();

	/* 
	*  register the executable that generates a DesertIface XML file from an input model file
	*  param importer is the name of such executable, e.g. scamla2desert.exe
	*  the usage of importer is: "importer inputModel"
	*/
	void registerImporter(const std::string &importer);
	
	/* 
	*  register the executable that generates an output file with a given input model file, 
	*  a DesertIfaceBack XML file and a configuration number.
	*  param exporter is the name of such executable, e.g. desert2scamla.exe
	*  the usage of exporter is: "exporter inputModel DesertIfaceBack.xml cfgId s"
	*  the argument s is for silent mode without GUI shown up
	*/
	void registerExporter(const std::string &exporter);

	/* 
	* execute the importer on the model file with the given name to generate the DesertIface XML file
	* param modelFile is the name of the source model file that will be processed by the 
	* registered importer and exporter. 
	*/
	void importModel(const std::string &modelFile);
	
	/*
	* insert the constraint to the set of constraints to be applied to run the deserttool.
	* param name is the name of the constraint
	* param expr is the expression of the constraint
	* param path is the full path of the component which contains this inserted constraint
	* the seperator "\" is used in the full path (e.g. com1\com2\com3)
	* tip: if the component name is unique, use the name instead of the full path
	*/
	void addConstraint(const std::string &name, const std::string &expr, const std::string &path);
	
	/*
	* apply the constraint to run the deserttool
	* param constraint_name is the name of the constraint to be applied
	*/
	void applyConstraint(const std::string &constraint_name);

	/*
	* apply the list of constraints to run the deserttool
	* param constraints_list is the string of the list of constraints to be applied
	* the seperator ":" is used in the list of constraint (e.g. constraint1:constraint2)
	*/
	void applyConstraints(const std::string &constraints_list);

	/*
	* apply all the constraints in the constraint set to run the deserttool
	*/
	void applyAllConstraints();

	/*
	* generate the DesertIfaceBack xml file from the DesertIface xml file generated from the 
	* input model file and constraints to be applied, 
	* return the number of configurations in the output DesertIfaceBack XML file
	*/
	int runDesert();

	/*
	* execute the exporter to generate the output model file with the given input model file, 
	* generated DesertIfaceBack xml file and the specified configuration index.
	* param cfgIndex is the index of the configurations in the DesertIfaceBack xml file
	*/
	void exportModel(int cfgIndex);

	/*
	* for all the configurations in the DesertIfaceBack xml file, execute the exporter to generate the
	* corresponding output model with the same given input model file
	*/
	void exportAll();

protected:
	void log(const std::string &info);
};

#endif