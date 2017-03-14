#! /bin/sh

#=============================================================================#
#                                                                             #
#                                 acgmake                                     #
#      Copyright (C) 2001-2007 by Computer Graphics Group, RWTH Aachen        #
#                          www.rwth-graphics.de                               #
#                                                                             #
#=============================================================================#


# Mandrake
if [ -f /etc/mandrake-release ] ; then
    echo `awk '{print $2 $4}' /etc/mandrake-release`

# SuSE
elif [ -f /etc/SuSE-release ] ; then
    echo `cat /etc/SuSE-release | grep "VERSION" | awk '{print "SuSE" $3}'`
    exit 0

# Debian
elif [ -f /etc/debian_version ] ; then
    if [ `uname -m` = "x86_64" ]; then
    	echo "Debian64"
    else
    	echo "Debian"
    fi
    exit 0

# default
else 
    echo `uname -s`
fi
