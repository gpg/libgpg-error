#!/bin/sh

PKG_CONFIG_PATH="."

export PKG_CONFIG_PATH

OUTPUT_OLD=$(./gpg-error-config --version)
OUTPUT_NEW=$(./gpg-error-config-new --version)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || exit 99

OUTPUT_OLD=$(./gpg-error-config --libs)
OUTPUT_NEW=$(./gpg-error-config-new --libs)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || exit 99

OUTPUT_OLD=$(./gpg-error-config --cflags)
OUTPUT_NEW=$(./gpg-error-config-new --cflags)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || exit 99

OUTPUT_OLD=$(./gpg-error-config --mt --libs)
OUTPUT_NEW=$(./gpg-error-config-new --mt --libs)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || exit 99

OUTPUT_OLD=$(./gpg-error-config --mt --cflags)
OUTPUT_NEW=$(./gpg-error-config-new --mt --cflags)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || exit 99

OUTPUT_OLD=$(./gpg-error-config --cflags --libs)
OUTPUT_NEW=$(./gpg-error-config-new --cflags --libs)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || exit 99

OUTPUT_OLD=$(./gpg-error-config --mt --cflags --libs)
OUTPUT_NEW=$(./gpg-error-config-new --mt --cflags --libs)
[ "$OUTPUT_OLD" = "$OUTPUT_NEW" ] || exit 99

exit 0
