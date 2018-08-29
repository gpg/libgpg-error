
if echo "$0" | grep gpg-error-config 2>/dev/null >/dev/null; then
  myname="gpg-error-config"
else
  myname="gpgrt-config"
fi

if find_file_in_path ${myname%-config}.pc $PKG_CONFIG_PATH; then
    CONFIG_FILE=$RESULT
else
    echo "Can't find ${myname%-config}.pc" 1>&2
    exit 1
fi

output=""

usage()
{
    cat <<EOF
Usage: $myname [OPTIONS]
Options:
        [--mt]       (must be the first option)
	[--prefix]
	[--exec-prefix]
	[--version]
	[--libs]
	[--cflags]
EOF
    exit $1
}

if test $# -eq 0; then
    usage 1 1>&2
fi

if [ "$1" != "--mt" ]; then
    mt=no
else
    mt=yes
    shift
fi

read_config_file < "$CONFIG_FILE"

while test $# -gt 0; do
    case "$1" in
	-*=*)
	    optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'`
	    ;;
	*)
	    optarg=
	    ;;
    esac

    case $1 in
	--prefix)
	    output="$output $(get_var prefix)"
	    ;;
	--exec-prefix)
	    output="$output $(get_var exec_prefix)"
	    ;;
	--version)
	    echo "$(get_attr Version)"
	    exit 0
	    ;;
	--cflags)
	    output="$output $(get_attr Cflags)"
            if test $mt = yes ; then
                output="$output $(get_var mtcflags)"
            fi
	    ;;
	--libs)
	    output="$output $(get_attr Libs)"
            if test $mt = yes ; then
                output="$output $(get_var mtlibs)"
            fi
	    ;;
	--variable=*)
	    echo "$(get_var ${1#*=})"
	    exit 0
	    ;;
	--host)
	    echo "$(get_var host)"
	    exit 0
	    ;;
	*)
	    usage 1 1>&2
	    ;;
    esac
    shift
done

#
# Clean up
#
# eval unset $VAR_list VAR_list
# eval unset $ATTR_list ATTR_list
#

echo $output
