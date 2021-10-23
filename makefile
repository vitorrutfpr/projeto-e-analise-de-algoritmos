all:
	g++ closest.cpp -o closest -lm
	.\genpoints 1000000
	.\closest.exe input.txt
clean:
	rm -f closest;
