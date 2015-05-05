META-Link
======================

Used by meta to perform interactive component assembly.
The subprojects hierarchy is:

README.md :
This file which describes the project source an build instructions.

meta-schema : 
The schema for the serialized objects.
This derives a protobuf file from an xsd file created 
by the GME CyPhy UML model.
The generation from the protobuf file from the xsd is
performed (mostly) by https://github.com/phreed/protomak.

Protomak has some issues but appears to be 
adequate for our purposes.
Some hand edits of the generated artifacts are required.
The artifact produced is used by 
* meta-link-protobuf.

meta-link-protobuf :
The protobuf files for posting edits to designs.
Contains files generated (in part) by meta-schema.
The components use protobuf for message passing.
First we need to run the appropriate protobuf code generators.
Three languages are currently generated with two generators:
* protoc
   * java - builds maven jar artifact
   * c++ - builds 
* protogen (only runs on .Net)
   * c#

The artifacts produced is used by 
* meta-bridge
* meta-link-creo
* meta-link-cyphy
* meta-link-test

meta-link-creo :
The files for c++ framing, used by Creo.
Depends on meta-link-protobuf.

meta-link-csharp :
The files for c# framing, used by CyPhy.
Depends on meta-link-protobuf.

meta-bridge :
The files for java framing, used by the meta-bridge.
meta-bridge is the edit message bridge server,
it distributes the posted edit messages to all clients
who have expressed interest in the particular edit message topics.
Depends on meta-protobuf.

meta-link-test :
Primarily used for testing.
Depends on the java framing found in meta-bridge.



Build Bridge 
======================

pushd meta-protobuf
mvn clean install
popd

mvn install 

push meta-bridge
mvn clean install
popd



Running
=======================


