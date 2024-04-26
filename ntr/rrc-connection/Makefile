server: server.cpp
	gcc -std=c++20 -Isrc -Ilib -o build/server server.cpp build/*.o lib/*.cpp -DASN_DISABLE_OER_SUPPORT

client: client.cpp
	g++ -std=c++20 -Ilib -o build/client client.cpp build/*.o lib/*.cpp -DASN_DISABLE_OER_SUPPORT

reset:
	rm -rf src
	rm -rf build
	mkdir src
	mkdir build

asn:
	asn1c -D src -no-gen-OER -no-gen-example -fcompound-names -fincludes-quoted rrc.asn1
	chmod +x ./scripts/runner.sh && ./scripts/runner.sh

build: reset asn server client

# 1. gcc -Isrc -c src/*.c -DASN_DISABLE_OER_SUPPORT # объектные файлы для кодгена
# 2. gcc -std=c++20 -Isrc -c server.cpp -DASN_DISABLE_OER_SUPPORT # объектный файл для сервера
# 3. g++ *.o -o build/server # линкуешь всё