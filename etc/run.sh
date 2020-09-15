#!/bin/sh

export PATH=$PATH:../bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib

nb.test.tests [ParserDataScene]&
