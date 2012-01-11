/***************************************************************************
 * Noise.cpp
 *
 *  Random number generation from uniform and normal distributions
 *
 *  Created : 6/17/97 - Matt Harrison
 *
 *  Copyright 2011 Informed Simplifications, LLC
 *  This file is part of NeuroJet.
 *
 *  NeuroJet is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  NeuroJet is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with NeuroJet.  If not, see <http://www.gnu.org/licenses/lgpl.txt>.
 ****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
//
// Noise()
// Noise(unsigned int seed, char type = 't
// void Reset(unsigned int seed, char type = 't')
//
// The above function initialize a random number generator with
// a given seed. Initialization using the default constructor
// is provided for use with arrays of Noises. Calling any
// of the distribution functions before initializing the Noise
// with Reset is undefined.
// The type is always 't' and indicates the underlying
// pseudo-random number generator being used.
// 't' corresponds to the TT800 algorithm and 'm' used to correspond
// to the algorithm found in Numerical Recipes in C.
// Note that if the 'm' flag is used then the seed will be
// cast to an int and negated to make it correspond to the
// old rn1 generator.
//
// The Uniform functions return doubles on [low,high]
//
// double Uniform(double low, double high)
// void Uniform(double *vec, int rows, double low, double high)
// void Uniform(float *vec, int rows, double low, double high)
// void Uniform(double **matrix, int rows, int cols, double low, double high)
// void Uniform(float **matrix, int rows, int cols, double low, double high)
//
// The Normal functions return doubles from Normal(mu,sigma)
// where mu is the mean and sigma is the standard deviation.
//
// double Normal(double mu, double sigma)
// void Normal(double *vec, int rows, double mu, double sigma)
// void Normal(float *vec, int rows, double mu, double sigma)
// void Normal(double **matrix, int rows, int cols, double mu, double sigma)
// void Normal(float **matrix, int rows, int cols, double mu, double sigma)
//
// The RandInt functions return intgers from the set of integers:
// {low, low+1, low+2, ... , high-2, high-1, high}
// The syntax is identical to the Uniform functions except that
// the RandInt functions take integer parameters.
//
// int RandInt(int low, int high)
// void RandInt(int *vec, int rows, int low, int high)
// void RandInt(int **matrix, int rows, int cols, int low, int high)
//
// The Bernoulli functions return zeros and ones where the
// probability of a one is rate.
//
// int Bernoulli(double rate)
// void Bernoulli(int *vec, int rows, double rate)
// void Bernoulli(int **matrix, int rows, int cols, double rate)
// void Bernoulli(bool *vec, int rows, double rate)
// void Bernoulli(bool **matrix, int rows, int cols, double rate)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(NOISE_HPP)
#include "Noise.hpp"
#endif
using namespace std;
#include <cmath>

Noise::Noise(unsigned int seed, char type) {
  Reset(seed, type);
}

void Noise::Reset(unsigned int seed, char type) {
  if ((type == 't') || (type == 'T')) {
    TRandInit(seed);
    // We used to also have another option from Numerical Recipes in C, but
    // removed it in order to make the code open source.
  } else {
    cerr << "Noise type " << type << "unknown. Must be 't'." << endl;
    exit(EXIT_FAILURE);
  }
  IsInit = true;
}

void Noise::Uniform(double *vec, int rows, double low,
                    double high) {
  RandDblVect(vec, rows);
  double range = high - low;
  for (int i = 0; i < rows; i++) {
    vec[i] *= range;
    vec[i] += low;
  }
  return;
}

void Noise::Uniform(float *vec, int rows, double low,
                    double high) {
  TRandFltVect(vec, rows);
  float range = static_cast<float>(high - low);
  float flow = static_cast<float>(low);
  for (int i = 0; i < rows; i++) {
    vec[i] *= range;
    vec[i] += flow;
  }
  return;
}

void Noise::Uniform(double **matrix, int rows, int cols,
                    double low, double high) {
  TRandDblMat(matrix, rows, cols);
  double range = high - low;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      matrix[i][j] *= range;
      matrix[i][j] += low;
    }
  return;
}

void Noise::Uniform(float **matrix, int rows, int cols,
                    double low, double high) {
  TRandFltMat(matrix, rows, cols);
  float range = static_cast<float>(high - low);
  float flow = static_cast<float>(low);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      matrix[i][j] *= range;
      matrix[i][j] += flow;
    }
  return;
}

void Noise::Normal(double *vec, int rows,
                   double mu, double sigma) {
  RandDblVect(vec, rows);
  int numrows = 2 * (rows / 2);
  double temp1, temp2;
  for (int i = 0; i < numrows; i += 2) {
    temp1 = sqrt(-2.0 * log(vec[i]));
    temp2 = 6.28318530718 * vec[i + 1];
    vec[i] = temp1 * cos(temp2);
    vec[i + 1] = temp1 * sin(temp2);
  }
  if (numrows != rows) {
    vec[numrows] = sqrt(-2.0 * log(vec[numrows]))
      * cos(6.28318530718 * RandDbl());
  }
  for (int j = 0; j < rows; j++) {
    vec[j] *= sigma;
    vec[j] += mu;
  }
  return;
}

void Noise::Normal(float *vec, int rows,
                   double mu, double sigma) {
  float fmu = static_cast<float>(mu);
  float fsigma = static_cast<float>(sigma);
  TRandFltVect(vec, rows);
  int numrows = 2 * (rows / 2);
  double temp1, temp2;
  for (int i = 0; i < numrows; i += 2) {
    temp1 = sqrt(-2.0 * log(static_cast<double>(vec[i])));
    temp2 = 6.28318530718 * static_cast<double>(vec[i + 1]);
    vec[i] = static_cast<float>(temp1 * cos(temp2));
    vec[i + 1] = static_cast<float>(temp1 * sin(temp2));
  }
  if (numrows != rows) {
    vec[numrows] = static_cast<float>(sqrt(-2.0 *
                                           log(static_cast<double>(vec[numrows])))
                                      * cos(6.28318530718 * RandDbl()));
  }
  for (int j = 0; j < rows; j++) {
    vec[j] *= fsigma;
    vec[j] += fmu;
  }
}

void Noise::Normal(double **matrix, int rows, int cols,
                   double mu, double sigma) {
  for (int i = 0; i < rows; i++) {
    Normal(matrix[i], cols, mu, sigma);
  }
}

void Noise::Normal(float **matrix, int rows, int cols,
                   double mu, double sigma) {
  for (int i = 0; i < rows; i++) {
    Normal(matrix[i], cols, mu, sigma);
  }
}

void Noise::RandInt(int *vec, int rows, int low,
                    int high) {
  for (int i = 0; i < rows; i++) {
    vec[i] = RandInt(low, high);
  }
}
void Noise::RandInt(int **matrix, int rows, int cols,
                    int low, int high) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = RandInt(low, high);
    }
  }
}

void Noise::Bernoulli(int *vec, int rows, double rate) {
  for (int i = 0; i < rows; i++) {
    vec[i] = Bernoulli(rate);
  }
}

void Noise::Bernoulli(int **matrix, int rows, int cols,
                      double rate) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = Bernoulli(rate);
    }
  }
}

void Noise::Bernoulli(bool * vec, int rows, double rate) {
  for (int i = 0; i < rows; i++) {
    vec[i] = Bernoulli(rate);
  }
}

void Noise::Bernoulli(bool ** matrix, int rows, int cols,
                      double rate) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = Bernoulli(rate);
    }
  }
}

///////////////////////////////////
// End of Distribution Functions //
///////////////////////////////////

// Noise Private Member Functions

// Noise TRandInit, TRandNum, TRandVect, and TRandMat initialize
// and update the pseudo-random number generator. The PRNG is a C++
// adaptation of TT800, which has the following reference:

// A C-program for TT800 : July 8th 1996 Version
// by M. Matsumoto, email: matumoto@math.keio.ac.jp
// generates one pseudorandom number with double precision
// which is uniformly distributed on [0,1]-interval
// for each call. One may choose any initial 25 seeds
// except all zeros.
// See: ACM Transactions on Modelling and Computer Simulation,
// Vol. 4, No. 3, 1994, pages 254-266.

// Noise RandInit function
// Initializes the private data members based on the seed
void Noise::TRandInit(unsigned int Seed) {
  QNNumber = 0;
  QNArray[0] = Seed;
  // QNArray[0] = 0x95f24dabL;
  QNArray[1] = 0x0b685215L + QNArray[0];
  QNArray[2] = 0xe76ccae7L + QNArray[1];
  QNArray[3] = 0xaf3ec239L + QNArray[2];
  QNArray[4] = 0x715fad23L + QNArray[3];
  QNArray[5] = 0x24a590adL + QNArray[4];
  QNArray[6] = 0x69e4b5efL + QNArray[5];
  QNArray[7] = 0xbf456141L + QNArray[6];
  QNArray[8] = 0x96bc1b7bL + QNArray[7];
  QNArray[9] = 0xa7bdf825L + QNArray[8];
  QNArray[10] = 0xc1de75b7L + QNArray[9];
  QNArray[11] = 0x8858a9c9L + QNArray[10];
  QNArray[12] = 0x2da87693L + QNArray[11];
  QNArray[13] = 0xb657f9ddL + QNArray[12];
  QNArray[14] = 0xffdc8a9fL + QNArray[13];
  QNArray[15] = 0x8121da71L + QNArray[14];
  QNArray[16] = 0x8b823ecbL + QNArray[15];
  QNArray[17] = 0x885d05f5L + QNArray[16];
  QNArray[18] = 0x4e20cd47L + QNArray[17];
  QNArray[19] = 0x5a9ad5d9L + QNArray[18];
  QNArray[20] = 0x512c0c03L + QNArray[19];
  QNArray[21] = 0xea857ccdL + QNArray[20];
  QNArray[22] = 0x4cc1d30fL + QNArray[21];
  QNArray[23] = 0x8891a8a1L + QNArray[22];
  QNArray[24] = 0xa6b7aadbL + QNArray[23];
  // this is magic vector `a', don't change
  mag01[0] = 0x0L;
  mag01[1] = 0x8ebfd028L;
  // cycle for a bit
  for (int i = 0; i < 100; i++) {
    TRandDbl();
  }
}

double Noise::TRandDbl() {
  unsigned int y;
  
  // generate 25 words at one time
  if (QNNumber == 25) {
    int kk = 0;
    for (; kk < 18; kk++) {
      QNArray[kk] = QNArray[kk + 7] ^ (QNArray[kk] >> 1)
        ^ mag01[QNArray[kk] % 2];
    }
    for (; kk < 25; kk++) {
      QNArray[kk] = QNArray[kk - 18] ^ (QNArray[kk] >> 1)
        ^ mag01[QNArray[kk] % 2];
    }
    QNNumber = 0;
  }
  
  y = QNArray[QNNumber];
  // s and b, magic vectors
  y ^= (y << 7) & 0x2b5b2500L;
  // t and c, magic vectors
  y ^= (y << 15) & 0xdb8b0000L;
  // you may delete this line if word size = 32
  y &= 0xffffffffL;
  y ^= (y >> 16);
  QNNumber++;
  
  // return the new random value
  return(static_cast<double>(y) / static_cast<unsigned long>(0xffffffffL));
}

void Noise::TRandDblVect(double *vec, int rows) {
  // get a random number for each index
  for (int i = 0; i < rows; i++) {
    unsigned int y;
    // generate 25 words at one time
    if (QNNumber == 25) {
      int kk = 0;
      for (; kk < 18; kk++) {
        QNArray[kk] = QNArray[kk + 7] ^ (QNArray[kk] >> 1)
          ^ mag01[QNArray[kk] % 2];
      }
      for (; kk < 25; kk++) {
        QNArray[kk] = QNArray[kk - 18] ^ (QNArray[kk] >> 1)
          ^ mag01[QNArray[kk] % 2];
      }
      QNNumber = 0;
    }
    y = QNArray[QNNumber];
    // s and b, magic vectors
    y ^= (y << 7) & 0x2b5b2500L;
    // t and c, magic vectors
    y ^= (y << 15) & 0xdb8b0000L;
    // you may delete this line if word size = 32
    y &= 0xffffffffL;
    y ^= (y >> 16);
    QNNumber++;
    // put the new random value in Vect
    vec[i] = (static_cast<double>(y) / static_cast<unsigned long>(0xffffffffL));
  }
}

void Noise::TRandFltVect(float *vec, int rows) {
  // get a random number for each index
  for (int i = 0; i < rows; i++) {
    unsigned int y;
    // generate 25 words at one time
    if (QNNumber == 25) {
      int kk = 0;
      for (; kk < 18; kk++) {
        QNArray[kk] = QNArray[kk + 7] ^ (QNArray[kk] >> 1)
          ^ mag01[QNArray[kk] % 2];
      }
      for (; kk < 25; kk++) {
        QNArray[kk] = QNArray[kk - 18] ^ (QNArray[kk] >> 1)
          ^ mag01[QNArray[kk] % 2];
      }
      QNNumber = 0;
    }
    y = QNArray[QNNumber];
    // s and b, magic vectors
    y ^= (y << 7) & 0x2b5b2500L;
    // t and c, magic vectors
    y ^= (y << 15) & 0xdb8b0000L;
    // you may delete this line if word size = 32
    y &= 0xffffffffL;
    y ^= (y >> 16);
    QNNumber++;
    // put the new random value in Vect
    vec[i] = static_cast<float>(static_cast<double>(y) /
                                static_cast<unsigned long>(0xffffffffL));
  }
}

void Noise::TRandDblMat(double **matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      unsigned int y;
      // generate 25 words at one time
      if (QNNumber == 25) {
        int kk = 0;
        for (; kk < 18; kk++) {
          QNArray[kk] = QNArray[kk + 7] ^ (QNArray[kk] >> 1)
            ^ mag01[QNArray[kk] % 2];
        }
        for (; kk < 25; kk++) {
          QNArray[kk] = QNArray[kk - 18] ^ (QNArray[kk] >> 1)
            ^ mag01[QNArray[kk] % 2];
        }
        QNNumber = 0;
      }
      y = QNArray[QNNumber];
      // s and b, magic vectors
      y ^= (y << 7) & 0x2b5b2500L;
      // t and c, magic vectors
      y ^= (y << 15) & 0xdb8b0000L;
      // you may delete this line if word size = 32
      y &= 0xffffffffL;
      y ^= (y >> 16);
      QNNumber++;
      // put the new random value in Vect
      matrix[i][j] = (static_cast<double>(y) / static_cast<unsigned long>(0xffffffffL));
    }
  }
}

void Noise::TRandFltMat(float **matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      unsigned int y;
      // generate 25 words at one time
      if (QNNumber == 25) {
        int kk = 0;
        for (; kk < 18; kk++) {
          QNArray[kk] = QNArray[kk + 7] ^ (QNArray[kk] >> 1)
            ^ mag01[QNArray[kk] % 2];
        }
        for (; kk < 25; kk++) {
          QNArray[kk] = QNArray[kk - 18] ^ (QNArray[kk] >> 1)
            ^ mag01[QNArray[kk] % 2];
        }
        QNNumber = 0;
      }
      y = QNArray[QNNumber];
      // s and b, magic vectors
      y ^= (y << 7) & 0x2b5b2500L;
      // t and c, magic vectors
      y ^= (y << 15) & 0xdb8b0000L;
      // you may delete this line if word size = 32
      y &= 0xffffffffL;
      y ^= (y >> 16);
      QNNumber++;
      // put the new random value in Vect
      matrix[i][j] = static_cast<float>(static_cast<double>(y) /
                                        static_cast<unsigned long>(0xffffffffL));
    }
  }
}
