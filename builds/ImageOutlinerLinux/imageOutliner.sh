#!/bin/bash
export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
CMD_LINE="./imageOutliner $@;"
echo "executing "$CMD_LINE;
eval $CMD_LINE;
exit $?;
