#!/bin/bash
export GOPATH=`pwd`
go get github.com/mgutz/logxi/v1
go get golang.org/x/net/html
go install ./src/lenta
