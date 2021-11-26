#!/bin/bash

export LOGXI=*
export LOGXI_FORMAT=pretty,happy 

go run server.go protocol.go
