/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _PRIMETOKEN_H_RPCGEN
#define _PRIMETOKEN_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define LIMIT 100000

struct Tkn {
	int _numberOfHops;
	quad_t _timeOfCreation;
	int _nextInteger;
	int _numberOfPrimesFound;
	int _highestPrimeFound;
};
typedef struct Tkn Tkn;

typedef Tkn Token;

#define PRIME_COMPUTATION 0x20000001
#define PCOMP_1 1

#if defined(__STDC__) || defined(__cplusplus)
#define START_PRIME_COMPUTATION 1
extern  Token * start_prime_computation_1(Token *, CLIENT *);
extern  Token * start_prime_computation_1_svc(Token *, struct svc_req *);
extern int prime_computation_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define START_PRIME_COMPUTATION 1
extern  Token * start_prime_computation_1();
extern  Token * start_prime_computation_1_svc();
extern int prime_computation_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_Tkn (XDR *, Tkn*);
extern  bool_t xdr_Token (XDR *, Token*);

#else /* K&R C */
extern bool_t xdr_Tkn ();
extern bool_t xdr_Token ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_PRIMETOKEN_H_RPCGEN */