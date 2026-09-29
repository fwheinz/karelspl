#!/bin/bash

WORKSPACE=/workspaces/$RepositoryName
BUILDDIR=$WORKSPACE/build
mkdir -p $BUILDDIR
cd $BUILDDIR
cmake $WORKSPACE >/dev/null 2>&1

LASTDIR=$(dirname $(echo $WORKSPACE/exercises/*/*.c | xargs ls -at | head -n 1))
PROJECT=$(echo $LASTDIR | cut -d/ -f 5)
make $PROJECT
./$PROJECT
