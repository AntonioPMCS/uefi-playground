#!/bin/bash
 if [ -z "$WORKSPACE" ]; then                   echo "Running edksetup.sh..."                   && source ./edksetup.sh 
                   fi
                   build -a X64 -b DEBUG -t GCC5 -p MdeModulePkg/MdeModulePkg.dsc
