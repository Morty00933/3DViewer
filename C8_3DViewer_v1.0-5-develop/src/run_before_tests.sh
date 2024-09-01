#!/bin/bash
FLDR=objects
SRC=txt
DST=obj
TEST1=nothing
TEST2=pyramid
TEST3=test
mv $FLDR/$TEST1.$SRC $FLDR/$TEST1.$DST
mv $FLDR/$TEST2.$SRC $FLDR/$TEST2.$DST
mv $FLDR/$TEST3.$SRC $FLDR/$TEST3.$DST
chmod +x valgrind.sh