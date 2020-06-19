const LIMIT = 100000;

struct Tkn{
    int _numberOfHops;
    hyper int _timeOfCreation;
    int _nextInteger;
    int _numberOfPrimesFound;
    int _highestPrimeFound;
};

typedef struct Tkn Token;

program PRIME_COMPUTATION{
    version PCOMP_1{
        Token START_PRIME_COMPUTATION(Token) = 1;
    } = 1;
} = 0x20000001;

