
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
delimiter=" "

while test $# -gt 0; do
    case $1 in
	--prefix)
	    # In future, use --variable=prefix instead.
	    output_var=prefix
	    ;;
	--exec-prefix)
	    # In future, use --variable=exec_prefix instead.
	    output_var=exec_prefix
	    ;;
	--version)
	    # In future, use --modversion instead.
	    output_attr=Version
	    delimiter="
"
	    ;;
	--modversion)
	    output_attr=Version
	    delimiter="
"
	    ;;
	--cflags)
	    opt_cflags=yes
	    ;;
	--libs)
	    opt_libs=yes
	    ;;
	--variable=*)
	    output_var=${1#*=}
	    ;;
	--host)
	    # In future, use --variable=host instead.
	    output_var=host
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

if [ $myname = "gpg-error-config" -a -z "$modules" ]; then
    read_config_file ${myname%-config} $PKG_CONFIG_PATH
    if [ -n "$output_var" ]; then
	output="$output${output:+ }$(get_var $output_var)"
    elif [ -n "$output_attr" ]; then
	output="$output${output:+ }$(get_attr $output_attr)"
    else
	cflags="$(get_attr Cflags)"
	libs="$(get_attr Libs)"

	mtcflags="$(get_var mtcflags)"
	mtlibs="$(get_var mtlibs)"
    fi

    requires="$(get_attr Requires)"
    cleanup_vars_attrs
    pkg_list=$(all_required_config_files $requires)
else
    if [ -z "$modules" ]; then
	modules=${myname%-config}
    fi
    cflags=""
    libs=""
    pkg_list=$(all_required_config_files $modules)
fi

for p in $pkg_list; do
    read_config_file $p $PKG_CONFIG_PATH
    if [ -n "$output_var" ]; then
	output="$output${output:+$delimiter}$(get_var $output_var)"
    elif [ -n "$output_attr" ]; then
	output="$output${output:+$delimiter}$(get_attr $output_attr)"
    else
	cflags="$cflags${cflags:+ }$(get_attr Cflags)"
	libs="$libs${libs:+ }$(get_attr Libs)"
    fi
    cleanup_vars_attrs
done

if [ -z "$output_var" -a -z "$output_attr" ]; then
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
fi

echo "$output"
