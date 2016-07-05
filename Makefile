all: daemon access

daemon: main.cpp
	g++ -g -Wall main.cpp -lrt -o daemon

daemon_fits: main_fits.cpp
	g++-4.8 -g -Wall main_fits.cpp -std=c++11 -g -Wall -lrt -I${HEADAS}/include -L${HEADAS}/lib -lCCfits_2.4 -o daemon

access: access.cpp
	g++ access.cpp -g -Wall -lrt -o access

access_fits: access_fits.cpp
	g++-4.8 access_fits.cpp -std=c++11 -g -Wall -lrt -I${HEADAS}/include -L${HEADAS}/lib -lCCfits_2.4 -o access_fits

clean:
	rm access daemon