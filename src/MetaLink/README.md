META-Link
======================

Used by meta to perform interactive component assembly.
The subprojects hierarchy is:

README.md :
This file which describes the project source an build instructions.

meta-link-protobuf :
The protobuf files for posting edits to designs.
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


