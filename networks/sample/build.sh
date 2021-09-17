#!/bin/bash
export GOPATH=`pwd`
go get github.com/mgutz/logxi/v1
go get github.com/skorobogatov/input
go install ./src/client
go install ./src/server
