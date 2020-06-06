Notes on the WorldCom MFC library (wcmfc32.zip):

The 16-bit makefiles are no longer included.

Please use the following directory structure:

./Debug - intermediate output files
../Debug - final output files

On my hard drive, this translates as follows:

c:\code\wcmfc32 - WorldCom MFC library
c:\code\wcmfc32\Debug - intermediate output files
c:\code\Debug - final output files

Generally, I like to do the same with the final Release files
by putting them in ../Release (c:\code\Release).  This keeps all of
my executables and libraries in the same output directory.

The files included in wcmfc32.zip are the intellectual property
of Joe Willcoxson and WorldCom.  They are released for use without
any restrictions.  Feel free to compile or distribute the library in
any manner you desire.

Contact Joe:  mailto:chinajoe@bigfoot.com
URL:  htpp://users.aol.com/chinajoe