#!/bin/bash

brew_available=`which brew`

if [ -z "$brew_available" ]
then
    echo "Missing Homebrew, so will install it."
    ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
fi

brew install osx-cpu-temp protobuf grpc