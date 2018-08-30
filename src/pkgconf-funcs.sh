#### start of pkgconf-funcs

#
# Bourne shell functions for config file in pkg-config style, so that
# we can share such a config file between pkg-config and script
#

#
# get_var: Get the variable value of NAME
#
# Variables are recorded in the shell variables named "VAR_<NAME>"
#
get_var () {
    local name=$1

    eval echo \$VAR_$name
}

#
# get_attr: Get the attribute value of KEY
#
# Attributes are recorded in the shell variables named "ATTR_<KEY>"
#
get_attr () {
    local name=$1

    eval echo \$ATTR_$name
}

# Remove ${varname} part in the beginning of a string.
remove_var_expr () {
    local varname=$1
    shift

    eval echo \"\${@#\\\$\\\{$varname\\\}}\"
}

# Given a string, substitute variables.
substitute_vars () {
    local string="$1"
    local line
    local varname
    local result

    while [ -n "$string" ]; do
	case "$string" in
	    \$\$*)
		result="$result\$"
		string="${string#\$\$}"
		;;
	    \${*}*)
		varname="${string#\$\{}"
		varname="${varname%%\}*}"
		result="$result$(get_var ${varname})"
		string=$(remove_var_expr ${varname} ${string})
		;;
	    *)
		result="${result}$(printf %c "$string")"
		string="${string#$(printf %c "$string")}"
		;;
	esac
    done

    echo "$result"
}

#
# Read a config from stdin
#
# Variables:
# For VAR=VALUE, value is stored in the shell variable VAR_*.
#
# Attributes:
# For KEY: VALUE, value is stored in the shell variable ATTR_*.
#
read_config_from_stdin () {
    local line
    local varname
    local value
    local key

    while read line; do
	case "$line" in
	    *=*)
		varname="${line%%=*}"
		value="${line#*=}"
		VAR_list="$VAR_list VAR_$varname"
		read VAR_$varname <<EOF1
$(substitute_vars "$value")
EOF1
		;;
	    *:\ *)
		key="${line%%:\ *}"
		value="${line#*:\ }"
		ATTR_list="$ATTR_list ATTR_$key"
		read ATTR_$key <<EOF2
$(substitute_vars "$value")
EOF2
		;;
	esac
    done
}


find_file_in_path () {
    local f=$1
    local p=$2
    local IFS=":"		# On Windows it should be ";"???

    for d in $p; do
	if [ -r $d/$f ]; then
	    RESULT="$d/$f"
	    return 0
	fi
    done
    RESULT=""
    return 1
}

read_config_file () {
    local config_file
    local RESULT

    if find_file_in_path $1.pc $2; then
	config_file=$RESULT
    else
	echo "Can't find $1.pc" 1>&2
	exit 1
    fi
    read_config_from_stdin < $config_file
}

cleanup_vars_attrs () {
    eval unset $VAR_list VAR_list
    eval unset $ATTR_list ATTR_list
}

not_listed_yet () {
    local m=$1
    shift

    for arg; do
	if [ $m = $arg ]; then
	    return 1
	fi
    done

    return 0
}

list_only_once () {
    local result=""
    local arg

    for arg; do
	if not_listed_yet $arg "$result"; then
	    result="$result $arg"
	fi
    done

    echo $result
}

list_only_once_for_libs () {
    local result=""
    loca rev_list=""
    local arg

    # Scan the list and eliminate duplicates for non-"-lxxx"
    # the resulted list is in reverse order
    for arg; do
	case "$arg" in
	    -l*)
		# As-is
		rev_list="$arg $rev_list"
		;;
	    *)
		if not_listed_yet $arg "$rev_list"; then
		    rev_list="$arg $rev_list"
		fi
		;;
	esac
    done

    # Scan again
    for arg in $rev_list; do
	case "$arg" in
	    -l*)
		if not_listed_yet $arg "$result"; then
		    result="$arg $result"
		fi
		;;
	    *)
		# As-is
		result="$arg $result"
		;;
	esac
    done

    echo $result
}

#
# Recursively solve package dependencies
#
# XXX: version requirement (version comparison) is not yet supported
#
all_required_config_files () {
    local list="$1"
    local all_list
    local new_list
    local p

    all_list="$list"

    while [ -n "$list" ]; do
	new_list=""
	for p in $list; do
	    read_config_file $p $PKG_CONFIG_PATH
	    new_list="$new_list $(get_attr Requires)"
	    cleanup_vars_attrs
	done
	all_list="$all_list $new_list"
	list="$new_list"
    done

    echo $(list_only_once $all_list)
}

#### end of pkgconf-funcs
