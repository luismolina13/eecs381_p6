#!/usr/bin/perl

print "=======================Status Test====================\n";
system("./p6exe<testfiles/status_in.txt>testfiles/status_out_saam.txt");
system("diff testfiles/status_out_saam.txt testfiles/status_out.txt");

print "=======================Cruise Test====================\n";
system("./p6exe<testfiles/cruise_in.txt>testfiles/cruise_out_saam.txt");
system("diff testfiles/cruise_out_saam.txt testfiles/cruise_out.txt");

print "=======================Views Test====================\n";
system("./p6exe<testfiles/views_in.txt>testfiles/views_out_saam.txt");
system("diff testfiles/views_out_saam.txt testfiles/views_out.txt");

