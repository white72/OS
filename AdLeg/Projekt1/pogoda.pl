#!/bin/perl

use Weather::OpenWeatherMap;

print "
 _____          _                 _       
|  __ \\        | |               | |      
| |__) |__ _ __| | __ _  ___   __| | __ _ 
|  ___/ _ \\ '__| |/ _` |/ _ \\ / _` |/ _` |
| |  |  __/ |  | | (_| | (_) | (_| | (_| |
|_|   \\___|_|  |_|\\__/ |\\___/ \\__,_|\\__,_|
                   __/ |                  
                  |___/                   

\n";

print "Wprowadź nazwę miasta\n";

chomp($city=<STDIN>); #Usunięcie znaku \n na końcu


my $api_key = 'e8cbe9eedf5d23af85d801bded1f2d46'; #ApiKey

my $_units = 'metric'; #metr / metr na sekundę / celcjusz

my $wx = Weather::OpenWeatherMap->new(
	api_key => $api_key,
);

my $current = $wx->get_weather(
	location => $city,
	_units => 'metric',
);

my $temp = $current->temp_f; #metric wymusza 'C
my $wind = $current->wind_speed_mph; #metric wymusza m/s
my $direc = $current->wind_direction; #kierunek wiatru

print "Pogoda dla miasta $city \n";#, $place\n";
print "Temperatura: $temp ℃\n";
print "Wiatr: $wind m/s\n";
print "Kierunek wiatru: $direc\n\n";
