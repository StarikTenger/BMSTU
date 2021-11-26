#!/bin/bash
export GOPATH=`pwd`
go get github.com/mgutz/logxi/v1
go install ./src/client
go install ./src/server
go install ./src/proxy
