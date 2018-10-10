#!/bin/sh

PKG_CONFIG_PATH="."

export PKG_CONFIG_PATH

if [ "$1" = --old-new ]; then
    PKG_CONFIG_CMD=./gpg-error-config-old
else
    PKG_CONFIG_SYSROOT_DIR="/var/example-target"
    export PKG_CONFIG_SYSROOT_DIR
    PKG_CONFIG_CMD="pkg-config gpg-error"
    if ! $PKG_CONFIG_CMD --exists >/dev/null; then
	exit 77			# Skip tests
    fi
fi

test_failed=""

failure () {
    (
	echo "Test result: $*"
	echo "====================: $PKG_CONFIG_CMD"
	echo "$OUTPUT_OLD"
	echo "====================: gpg-error-config-new"
	echo "$OUTPUT_NEW"
	echo "===================="
    ) >> gpg-error-config-test.log
    test_failed=yes
}

rm -f gpg-error-config-test.log

OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --libs))
OUTPUT_NEW=$(./gpg-error-config-new --libs)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --libs

OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --cflags))
OUTPUT_NEW=$(./gpg-error-config-new --cflags)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --cflags

OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --cflags --libs))
OUTPUT_NEW=$(./gpg-error-config-new --cflags --libs)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --cflags --libs

if [ "$PKG_CONFIG_CMD" = ./gpg-error-config-old ]; then
    OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --version))
    OUTPUT_NEW=$(./gpg-error-config-new --version)
    [ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --version

    OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --mt --libs))
    OUTPUT_NEW=$(./gpg-error-config-new --mt --libs)
    [ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --mt --libs

    OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --mt --cflags))
    OUTPUT_NEW=$(./gpg-error-config-new --mt --cflags)
    [ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --mt --cflags

    OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --cflags --libs))
    OUTPUT_NEW=$(./gpg-error-config-new --cflags --libs)
    [ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --cflags --libs

    OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --mt --cflags --libs))
    OUTPUT_NEW=$(./gpg-error-config-new --mt --cflags --libs)
    [ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --mt --cflags --libs

    OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --variable=mtcflags))
    OUTPUT_NEW=$(./gpg-error-config-new --variable=mtcflags)
    [ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --variable=mtcflags

    OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --variable=mtlibs))
    OUTPUT_NEW=$(./gpg-error-config-new --variable=mtlibs)
    [ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --variable=mtlibs

    OUTPUT_OLD=$(echo $($PKG_CONFIG_CMD --variable=host))
    OUTPUT_NEW=$(./gpg-error-config-new --variable=host)
    [ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || failure --variable=host
fi

if [ -n "$test_failed" ]; then
    OUTPUT_OLD=$($PKG_CONFIG_CMD --version)
    OUTPUT_NEW=$(./gpg-error-config-new --version)
    failure --version

    exit 99
fi

exit 0
