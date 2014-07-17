#! /usr/bin/perl -s
#
# PERL version of the makedep util for Unix systems
#
# This is a wrapper script for the gcc dependencies generation facilities
#
# Usage : makedep.pl -include="-Ifoo -Ibar" -sources=dir1;dir2;dir3 -makefile=dependencies_file.dep
#
# $Author: StephaneP$
#

use Shell;

# Substitute backslashes with slashes for Unix semantics
$sources =~ s/\\/\//mg;

$makefile = 'makefile.dep' unless $makefile;

@dirs = split(/;/, $sources);
@includes = split(/ /, $include);
@srcext = ("*.c", "*.cc", "*.cpp", "*.cxx", "*.asm"); 

$olddir = `pwd`;
chop($olddir);
foreach $i (@dirs) {
  &process_dir ($i);  
  chdir ($olddir);
}

foreach $i (@dirs) {
  $include_sources .= " -I" . $i;
}

# Makes the command actually called

my($tmp) = "gcc -MM " .  join(" ", @includes) . $include_sources . " " . join(" ",@all_files) . "> $makefile";
system($tmp);

### End of program

# Parses a directory for C files

sub process_dir {
  my ($dir) = @_;

  chdir ($dir);
  foreach $ext (@srcext) {
    my @files = glob($ext);
    foreach $j (@files) {
      push @all_files, $dir . "/" . $j;
    }
  }
}
