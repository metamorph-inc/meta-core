#ifndef DATUM_EDITOR_ROUTINES_H
#define DATUM_EDITOR_ROUTINES_H
#include <isis_application_exception.h>
#include "UdmBase.h"
#include <CADDatumEditor.h>
#include <string>

namespace isis
{
	struct ErrorStatus
	{
		bool warningsOccurred;
		bool errorsOccurred;

		ErrorStatus(): warningsOccurred(false), errorsOccurred(false){};
	};

	std::string ErrorStatusMessage( const ErrorStatus &in_ErrorStatus);
	//int ErrorStatusCode( const ErrorStatus &in_ErrorStatus);

	enum e_DatumEditorFunction
	{
		CAD_ADD_DATUM,
		CAD_DELETE_DATUM,
		CAD_ADD_COORDINATE_SYSTEM,
		CAD_PARTS_LIBRARY
	};


//	enum e_OperationType
//	{
//		INTERNAL_OPERATION_POPULATE_LIB_MAP,
//		INTERNAL_OPERATION_ADD_DATUM,
//		INTERNAL_OPERATION_DELETE_DATUM
//	};

	class DatumEditorDiagram_functor
	{
		public:
			virtual void operator() ( e_DatumEditorFunction in_DatumEditorFunction, 
									  Udm::Object &in_UdmObject_function,
									  Udm::Object *in_UdmObject_component = NULL)=0;
	};


	class Log_DatumEditorDiagram_functor: public DatumEditorDiagram_functor
	{
		public:

			virtual void operator() ( e_DatumEditorFunction in_DatumEditorFunction, 
									  Udm::Object &in_UdmObject_function,
									  Udm::Object *in_UdmObject_component = NULL)
									  throw (isis::application_exception );
	};

	class  CheckDataValidity_DatumEditorDiagram_functor: public DatumEditorDiagram_functor
	{
		public:

			virtual void operator() ( e_DatumEditorFunction in_DatumEditorFunction, 
									  Udm::Object &in_UdmObject_function,
									  Udm::Object *in_UdmObject_component = NULL)
									  throw (isis::application_exception );
	};

	class  PopulateLibraryMap_DatumEditorDiagram_functor: public DatumEditorDiagram_functor
	{

		public:
			std::map<std::string, std::string> libID_to_DirectoryPath_map;
			//e_OperationType operationType; 

			virtual void operator() ( e_DatumEditorFunction in_DatumEditorFunction, 
									  Udm::Object &in_UdmObject_function,
									  Udm::Object *in_UdmObject_component = NULL)
									  throw (isis::application_exception );
	};

	//void TraverseXMLDocument(const std::string &in_DatumXML_PathAndFileName);
   void TraverseXMLDocument(Udm::SmartDataNetwork &in_parsedXMLDiagram,
							 DatumEditorDiagram_functor &in_DatumEditorDiagram_functor ) throw (isis::application_exception );

   void DeleteAndCreateCSysAndDatums(	Udm::SmartDataNetwork	&in_parsedXMLDiagram,
								const std::map<std::string, std::string> &in_LibID_to_DirectoryPath_map,
								const std::string		&in_StartingDirectory,
								ErrorStatus				&out_ErrorStatus)
								throw (isis::application_exception );

} // End namespace isis

#endif