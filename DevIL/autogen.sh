#!/bin/sh
# Author: Meloni Dario <mellon85 -at- gmail.com>

# use the -v flag to get all the output
if [ "$1" = "-v" ] ; then
	VERBOSE=yes
fi

log() {
	if [ "$VERBOSE" = "yes" ] ; then
		$1
		output=""
	else
		output=$($1 2>&1)
	fi
	
	# show output only in case of errors
	if [ $? -ne 0 ] ; then
		echo $output
		exit 1
	fi
}

log "aclocal -I ."
log "autoheader"
log "automake --foreign --include-deps --add-missing --copy"
log "autoconf"
exit 0
