#
# This is a sed script to patch the generated libtool,
# which works well against both of libtool 2.4.2 and 2.4.7.
#
/^[ \t]*# bleh windows$/{
:loop0
s/^[ \t]*esac$/\0/
t done0
s/^[ \t]*case \$host in$/\0/
t insert0
n
b loop0
:insert0 n
i\
	      x86_64-*mingw32*)
i\
		func_arith $current - $age
i\
		major=$func_arith_result
i\
		versuffix="6-$major"
i\
		;;
b loop0
:done0
}
/^[ \t]*# extension on DOS 8.3 file.*systems.$/{
:loop1
s/^[ \t]*\(versuffix=\)\(.*\)\(-$major\)\(.*\)$/\t  case \$host in\n\t  x86_64-*mingw32*)\n\t    \1\26\3\4\n\t    ;;\n\t  *)\n\t    \1\2\3\4\n\t    ;;\n\t  esac/
t done1
n
b loop1
:done1
}
