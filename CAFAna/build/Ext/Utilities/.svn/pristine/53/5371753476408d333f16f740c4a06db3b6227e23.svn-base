#!/usr/bin/perl
#
# Use "--help" to learn how this script works and what it does.
#
$event_file    = "events.txt";
$tempfile      = "tempfile.root";
$endfile       = "runevtfilt-evt.root";
$data_def      = "";
$have_data_def = 0;
$data_mc       = 0;
$is_mc         = 0;
for ($i=0; $i<=$#ARGV; ++$i) {
    if (($ARGV[0] eq "-h") || ($ARGV[0] eq "--help")) {
	print "\n";
	print "This script will take a list of events in [run] [subrun] [event] [slice] format.\n";
	print "The script will generate file names and run an ART job to extract the listed\n";
	print "events assuming the files are in SAM though xrootd.\n";
	print "The event file output is hard-wired to runevtfilt-evt.root\n";
	print "\n";
	print "Usage: find-events-sam.pl --[opt]=[value]\n";
	print "  Where [opt] is one of\n";
	print "   --event-file=[file]  : File that lists the events you want\n";
	print "                          Format [run] [subrun] [event] [slice] for Data\n";
	print "                          Format [run] [subrun] [cycle] [event] [slice] for MC.\n";
	print "                          Default = $event_file\n";
	print "   --data-def=[data def]: a SAM data set definition eg. 'prod_pidpart_R16-03-03-prod2reco.a_fd_cosmic_full_nueveto_v1_goodruns'\n";
	print "   --ismc=[value]       : 0=data, 1=mc\n";
	print "   --help               : Prints this message\n";
	exit(0);
    }
    
    ($opt,$value) = split('=',$ARGV[$i]);
    if ($opt eq "--event-file") { $event_file  = $value; }
    if ($opt eq "--data-def")   { $data_def    = $value; $have_data_def = 1; }
    if ($opt eq "--ismc")       { $is_mc       = $value; $data_mc = 1;}
}

if ($have_data_def != 1 or $data_mc !=1) {
    die("Please specify --ismc=[value] and --data-def=[data def]");
}
#.......................................................................

#
# Given a SAM data set definition, get a list of all the
# files that match that data set definition. Our events will be there.
#
print "Finding the full data set file list...";
if ($have_data_def==1) {
    `samweb list-files "dataset_def_name_newest_snapshot $data_def" > samfiles.txt`;
    if (!-s "samfiles.txt") {
	print "SAM query fails for the newest snapshot for this dataset.Finding the full data set continues...";
	`samweb list-definition-files "$data_def" > samfiles.txt`;
    }
}
print "done.\n";

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
    if ($is_mc == 1) {
	($run,$subrun,$cycle,$event,$slice) = split(" ",$_);
    }
    else {
	($run,$subrun,$event,$slice) = split(" ",$_);
    }
    #
    # Construct a file name wild card which will select the file which 
    # contains the event we want
    #
    if ($have_data_def==1) {
	$run_text    = sprintf("r%.8d",$run);
	$subrun_text = sprintf("s%02d",$subrun);
	$cycle_text = sprintf("c%03d",$cycle);
	if($is_mc == 1) {
	    $fullname = `grep $run_text samfiles.txt | grep $subrun_text | grep $cycle_text`;
	}
	else {
	    $fullname = `grep $run_text samfiles.txt | grep $subrun_text`;
	}
    }
    print FILE_LIST "$fullname";
    if ($is_mc == 1) {
	print EVT_LIST "$run $subrun $cycle $event $slice\n";
    }
    else {
	print EVT_LIST "$run $subrun $event $slice\n";
    }
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

#
# Get ticket to be able to stream files from SAM with samweb2xrootd
#
system("kx509");
system("kxlist -p");
system("voms-proxy-init --rfc --voms=fermilab:/fermilab/nova/Role=Analysis --noregen");

open (FILE_LIST, $file_list);
while ( <FILE_LIST> ) {
  my $tvar = $_;
  chomp $tvar;
  if ($is_mc==1) {
      $cmd = "nova -c runeventfiltmcjob.fcl -o $tvar `samweb2xrootd $_`";
  }
  else {
      $cmd = "nova -c runeventfiltjob.fcl -o $tvar `samweb2xrootd $_`";
  }
  print "\n";
  print "Running: $cmd";
  print "\n";
  system($cmd);
}
close (FILE_LIST);

#
# Need to combine small files together
#
print "\n";
print "Concatenating the resultant filtered small files...\n";
$cmd2 = "nova -c concat_files.fcl -o $endfile -S $file_list";
system($cmd2);

#
# Remove small files now that we are done
#
open (FILE_LIST, $file_list);
while ( <FILE_LIST> ) {
  system("rm $_");
}
close (FILE_LIST);

########################################################################
