#!/bin/sh
b58=$(echo 'ff5a1fc5dd9e6f03819fca94a2d89669469667f9a0' | xxd -r -p | base58 -c)
test x$b58 = x2mkQLxaN3Y4CwN5E9rdMWNgsXX7VS6UnfeT
