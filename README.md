Server für die Steuerung von LEDs am Raspberry Pi - Franz Board
von Florian Reisinger - 12.06.2019

Eingabe:	Zahl zwischen 0 und 15 (Die Leds geben die eingegebene Zahl im Binärformat wieder.)
          q = quit/beenden

Server starten:	./LEDS
			           Sollte dies nicht funktionieren zuerst mit "gcc LEDS.c -o LEDS -Wall -lpthread -lwiringPi" kompilieren.

zum Server verbinden:  nc "ip" "port"
			                 ip = "if config" am raspberry ins Terminal eingeben
			                 Port = in Programm in Zeile 15 ablesen (3000)
