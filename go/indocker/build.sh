#!/bin/bash
set -x

export HAWQ_DOCKER_TAG="e2e-testing"

GOOS=linux GOARCH=amd64 CGO_ENABLED=0 go build main.go

docker build --no-cache -f ./Dockerfile -t docker.io/interma/demo:arg --build-arg tag=$HAWQ_DOCKER_TAG ./

docker run interma/demo:arg


