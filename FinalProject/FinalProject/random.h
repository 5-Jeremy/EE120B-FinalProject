#ifndef RANDOM_H_
#define RANDOM_H_

unsigned short GenNewSeed(unsigned short delay);

unsigned long long GenRand();

unsigned char GenRandWithinRange (unsigned char min, unsigned char max);

#endif /* RANDOM_H_ */