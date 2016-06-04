#!/bin/perl

sub log_N {
	my $num = shift;
    my $base = shift;
    return log($num)/log($base);
}


#print "Podaj dolny przedział\n";
#chomp ($down=<STDIN>);

#print "Podaj górny przedział \n";
#chomp ($up=<STDIN>);

#print "Podaj liczbę iteracji \n";
#chomp ($iter=<STDIN>);

$down = @ARGV[0];
$up = @ARGV[1];
$iter = @ARGV[2];

#print "down: $down, up: $up, iter: $iter \n\n";

$V = 0;

$x = $down;
$delta = ($up - $down) / $iter;

$x = $x + ($delta / 2);

while ($x < $up) {
	$iter=$iter	- 1;

	$sinV = sin($x);

#	print "SinV $sinV  x: $x\n";

	$y = log_N(sin($x), 2.718282);

	$V = $V + ($delta * $y);

	$x = $x + $delta;
}

print "\n\n $V \n";
