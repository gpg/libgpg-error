
This is a small list of steps in order to build libgpg-error into a msvc DLL and lib file.

The project contains Release and Debug builds for static lib files (Debug/Release)
  as well as dynamic shared dll files (DebugDLL/ReleaseDLL).
Choose whichever project configuration meets your requirements.


*** Generating H files ***

The build system requires several of the .h files to be built automatically. This
should have already been done and the resulting source files will be found in the
SMC\ directory. If these files are missing then the project will try and
automatically generate new ones. If the files need to be forced to update then
they can be simply deleted which will result in new files being generated the next
time the project is built.

In order for new files to be generated the project needs access to 'awk'. Ensure
that Gawk is accessible by Visual Studio.

Gawk can be downloaded from here:
http://gnuwin32.sourceforge.net/packages/gawk.htm

Pre-built windows binaries can also be found in the above link. To enable generation
of new files awk.exe should be extracted and added somewhere accessible to the Visual
Studio path (e.g. extract it into the Visual Studio) project folder.