#!/bin/bash

PROTO_PATH=../protobuf
protoc --java_out=../src/main/java \
	--proto_path=${PROTO_PATH} \
	${PROTO_PATH}/CdbMsg.proto

protoc --java_out=../src/main/java \
	--proto_path=${PROTO_PATH} \
	${PROTO_PATH}/AssemblyInterface.proto

protoc --java_out=../src/main/java \
	--proto_path=${PROTO_PATH} \
	${PROTO_PATH}/XsdTypes.proto
