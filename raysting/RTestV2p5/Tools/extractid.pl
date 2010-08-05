use strict;
die unless open(FILE, shift(@ARGV));

while(<FILE>)
{
	print "Z$1,"	if(/id="Z(\d+)"/);
	print "Z$1,"	if(/id='Z(\d+)'/);
}
close(FILE);
