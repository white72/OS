#!/bin/perl

use Math::Integral::Romberg 'integral';

sub f {
	my ($x) = $_[0];
	return log(sin($x));
}

$down = @ARGV[0];
$up = @ARGV[1];
$iter = @ARGV[2];

$V = 0;

$x = $down;
$delta = ($up - $down) / $iter;

$x = $x + ($delta / 2);

while ($x < $up) {
	$iter=$iter	- 1;

	$V = f($x);

	$x = $x + $delta;
}

print "\n\n $V \n";
