#!/bin/bash

export LOGXI=*
export LOGXI_FORMAT=pretty,happy 

go run client.go protocol.go -n=3 -server=127.0.0.1:6060 
