#!/bin/bash
LD_LIBRARY_PATH=./
CMD_LINE="./imageOutliner $@;"
echo $CMD_LINE;
eval $CMD_LINE;
exit $?;
