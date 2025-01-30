#!/usr/bin/perl
#
# Use "--help" to learn how this script works and what it does.
#
$dont_run   = 0;
$event_file = "events.txt";
$trigger    = "numi";
$detector   = "NDOS";
$file_prefix= "ndos";
$release    = "S12-12-12";
for ($i=0; $i<=$#ARGV; ++$i) {
    if (($ARGV[0] eq "-h") || ($ARGV[0] eq "--help")) {
	print "This script will take a list of events in [run] [subrun] [event] format.\n";
	print "The script will find the files (assuming you are working on the FNAL gpvm's)\n";
	print "and run an ART job to extract the listed events. The event file output is hard-wired\n";
	print "to runevtfilt-evt.root\n";
	print "\n";
	print "Usage: find-events --[opt]=[value]\n";
	print "  Where [opt] is one of\n";
	print "   --event-file=[file] : File that lists the events you want\n";
	print "                         Format [run] [subrun] [event]. Default = $event_file\n";
	print "   --trigger=[trig]    : Trigger selection (cosmic, bnb, numi). Default = $trigger\n";
	print "   --detector=[det]    : Detector selection (NDOS only at moment). Default = $detector\n";
	print "   --prefix=[prefix]   : File prefix (ndos,reco). Default=$prefix\n";
	print "   --release=[rel]     : Software release. Default=$release\n";
	print "   --dont-run          : Print what this would do, but don't actually do it\n";
	print "   --help              : Prints this message\n";
	exit(0);
    }
    if ($ARGV[0] eq "--dont-run") { $dont_run = 1; }
    
    ($opt,$value) = split('=',$ARGV[$i]);
    if ($opt eq "--event-file") { $event_file  = $value; }
    if ($opt eq "--trigger")    { $trigger     = $value; }
    if ($opt eq "--detector")   { $detector    = $value; }
    if ($opt eq "--prefix")     { $file_prefix = $value; }
    if ($opt eq "--release")    { $release     = $value; }
}
#.......................................................................

$base_dir = "/nova/data/novaroot/$detector/$release";
print "Searching for files in base directory $base_dir\n";

$file_list  = "files-list.txt";
$event_list = "event-list.txt";

#
# Open the file listing the run, subrun, and event numbers
#
open (FILE, $event_file);
open (FILE_LIST,">$file_list");
open (EVT_LIST,">$event_list");

while ( <FILE> ) {
    chomp($_);
    ($run,$subrun,$event) = split(" ",$_);
    #
    # Construct a file name wild card which will select the file which contains the event we want
    #
    $dir = sprintf("%s/%.6d/%.5d/%s/%s*s%.2d*%s*.reco.root",
		   $base_dir,
		   int($run/100.),
		   $run,
		   $trigger,
		   $file_prefix,
		   $subrun,
		   $release);
    print "Looking for files matching $dir...\n";
    print FILE_LIST `ls $dir`;
    print EVT_LIST "$run $event\n";
}
close(FILE);
close(FILE_LIST);
close(EVT_LIST);

#
# Make sure the file list contains files only once
#
`sort $file_list > files-list.tmp`;
open (TMP_LIST,"files-list.tmp");
open (FILE_LIST,">$file_list");
$last_line = "";
while ( <TMP_LIST> ) {
    if ($_ ne $last_line) {
	print FILE_LIST $_;
	$last_line = $_;
    }
}
close (TMP_LIST);
close (FILE_LIST);

$cmd = "nova -c runeventfiltjob.fcl -S $file_list";
if ($dont_run==1) {
  print $cmd,"\n";
}
else {
  print "Running $cmd\n";
  system($cmd);
}

########################################################################
