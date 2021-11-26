#!/bin/bash

export LOGXI=*
export LOGXI_FORMAT=pretty,happy 

go run client.go protocol.go
