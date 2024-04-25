server: server.c
	gcc -Isrc -Ilib -o build/server server.c build/*.o lib/*.c -DASN_DISABLE_OER_SUPPORT

client: client.c
	gcc -Isrc -Ilib -o build/client client.c build/*.o lib/*.c -DASN_DISABLE_OER_SUPPORT

reset:
	rm -rf src
	rm -rf build
	mkdir src
	mkdir build

asn:
	asn1c -D src -no-gen-OER -no-gen-example -fcompound-names rrc.asn1
	chmod +x ./scripts/runner.sh && ./scripts/runner.sh

build: reset asn server client