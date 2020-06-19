#@since 3/1/2020
#@file Makefile

all: generateRpc client server


generateRpc: primeToken.x
	rpcgen -C primeToken.x

client: primeToken.h primeToken_client.c primeToken_clnt.c primeToken_xdr.c
	gcc primeToken.h primeToken_client.c primeToken_clnt.c primeToken_xdr.c -o client
	
server: primeToken.h primeToken_server.c primeToken_svc.c primeToken_xdr.c
	gcc primeToken.h primeToken_server.c primeToken_svc.c primeToken_xdr.c -o server
	
clean:
	rm -f client
	rm -f server
	rm -f primeToken_clnt.c
	rm -f primeToken_svc.c
	rm -f primeToken_xdr.c
	rm -f primeToken.h
	