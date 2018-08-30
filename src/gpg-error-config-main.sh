
myname=${0##*/}
if [ $myname = gpgrt-config ]; then
  myname="gpg-error-config"
fi

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
    # In future, use --variable=mtcflags or --variable=mtlibs
    mt=yes
    shift
fi

modules=""
output_var=""
output_attr=""
opt_cflags=no
opt_libs=no
output=""

while test $# -gt 0; do
    case $1 in
	--prefix)
	    # In future, use --variable=prefix instead.
	    output_var=prefix
	    break
	    ;;
	--exec-prefix)
	    # In future, use --variable=exec_prefix instead.
	    output_var=exec_prefix
	    break
	    ;;
	--version)
	    # In future, use --modversion instead.
	    output_attr=Version
	    break
	    ;;
	--modversion)
	    output_attr=Version
	    break
	    ;;
	--cflags)
	    opt_cflags=yes
	    ;;
	--libs)
	    opt_libs=yes
	    ;;
	--variable=*)
	    output_var=${1#*=}
	    break
	    ;;
	--host)
	    # In future, use --variable=host instead.
	    output_var=host
	    break
	    ;;
	--help)
	    usage 0
	    ;;
	*)
	    modules="$modules $1"
	    ;;
    esac
    shift
done

if [ -z "$modules" ]; then
    modules=${myname%-config}
fi

if [ myname = "gpg-error-config" -a -z "$modules" ]; then
    read_config_file ${myname%-config} $PKG_CONFIG_PATH
    cflags="$(get_attr Cflags)"
    libs="$(get_attr Libs)"

    mtcflags="$(get_var mtcflags)"
    mtlibs="$(get_var mtlibs)"

    requires="$(get_attr Requires)"
    cleanup_vars_attrs
    pkg_list=$(all_required_config_files $requires)
else
    cflags=""
    libs=""
    pkg_list=$(all_required_config_files $modules)
fi

for p in $pkg_list; do
    read_config_file $p $PKG_CONFIG_PATH
    cflags="$cflags $(get_attr Cflags)"
    libs="$libs $(get_attr Libs)"
    cleanup_vars_attrs
done

if [ $opt_cflags = yes ]; then
    output="$output $(list_only_once $cflags)"
    # Backward compatibility to old gpg-error-config
    if [ $mt = yes ]; then
	output="$output $mtcflags"
    fi
fi
if [ $opt_libs = yes ]; then
    output="$output $(list_only_once_for_libs $libs)"
    # Backward compatibility to old gpg-error-config
    if [ $mt = yes ]; then
	output="$output $mtlibs"
    fi
fi

echo $output
