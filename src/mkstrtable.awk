# Copyright (C) 2003 g10 Code GmbH
# This file is part of libgpg-error.

# libgpg-error is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# libgpg-error is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with the GNU C Library; if not, write to the Free
# Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307 USA.

# This script outputs a source file that does define the following
# symbols:
#
# static const char msgstr[];
# A string containing all messages in the list.
#
# static const int msgidx[];
# A list of index numbers, one for each message, that points to the
# beginning of the string in msgstr.
#
# msgidxof (code);
# A macro that maps code numbers to idx numbers.  If a DEFAULT MESSAGE
# is provided (see below), its index will be returned for unknown codes.
# Otherwise -1 is returned for codes that do not appear in the list.
# You can lookup the message with code CODE with:
# msgstr + msgidx[msgidxof (code)].
#
# The input file has the following format:
# CODE1	MESSAGE1		(Code number, <tab>, message string)
# CODE2	MESSAGE2		(Code number, <tab>, message string)
# ...
# CODEn	MESSAGEn		(Code number, <tab>, message string)
# 	DEFAULT MESSAGE		(<tab>, fall-back message string)
#
# Comments (starting with # and ending at the end of the line) are removed,
# as is trailing whitespace.  The last line is optional; if no DEFAULT
# MESSAGE is given, msgidxof will return the number -1 for unknown
# index numbers.

BEGIN {
  FS = "\t";
# cpos holds the current position in the message string.
  cpos = 0;
# msg holds the number of messages.
  msg = 0;
  print "/* Output of mkstrtable.awk.  DO NOT EDIT.  */";
  print "";
  print "/* The purpose of this complex string table is to produce";
  print "   optimal code with a minimum of relocations.  */";
  print "";
  print "static const char msgstr[] = ";
}

{
  sub (/\#.+/, "");
  sub (/[ 	]+$/, ""); # Strip trailing space and tab characters.
  if (/^$/)
    next;

# Print the string msgstr line by line.  We delay output by one line to be able
# to treat the last line differently (see END).
  if (last_msgstr)
    print "  gettext_noop (\"" last_msgstr "\") \"\\0\"";
  last_msgstr = $2;

# Remember the error code and msgidx of each error message.
  code[msg] = $1;
  pos[msg] = cpos;
  cpos += length ($2) + 1;
  msg++;

  if ($1 == "")
    {
      has_default = 1;
      exit;
    }
}
END {
  if (has_default)
    coded_msgs = msg - 1;
  else
    coded_msgs = msg;

  print "  gettext_noop (\"" last_msgstr "\");";
  print "";
  print "static const int msgidx[] =";
  print "  {";
  for (i = 0; i < coded_msgs; i++)
    print "    " pos[i] ",";
  print "    " pos[coded_msgs];
  print "  };";
  print "";
  print "#define msgidxof(code) (0 ? -1 \\";

# Gather the ranges.
  skip = code[0];
  start = code[0];
  stop = code[0];
  for (i = 1; i < coded_msgs; i++)
    {
      if (code[i] == stop + 1)
	stop++;
      else
	{
	  print "  : ((code >= " start ") && (code <= " stop ")) ? (code - " \
            skip ") \\";
	  skip += code[i] - stop - 1;
	  start = code[i];
	  stop = code[i];
	}
    }
  print "  : ((code >= " start ") && (code <= " stop ")) ? (code - " \
    skip ") \\";
  if (has_default)
    print "  : " stop + 1 " - " skip ")";
  else
    print "  : -1)";
    
 }
