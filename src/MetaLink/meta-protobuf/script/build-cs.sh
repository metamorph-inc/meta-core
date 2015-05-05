#!/bin/bash

pushd ../src/main/protobuf
/cygdrive/c/Users/hnine/Downloads/ProtobufCSharp/ProtoGen/protogen.exe -d -i:CdbMsg.proto -o:"../../../../cdb-csharp/CdbMsg.cs"

