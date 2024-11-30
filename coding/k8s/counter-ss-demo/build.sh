#!/bin/bash
set -x

env GOOS=linux GOARCH=amd64 go build main.go
docker build -t docker.io/interma/demo:counter .
docker push interma/demo:counter
exit 0
