/**
 * @file sc_random.h
 * @author ForNeVeR.
 * sc_random class header.
 * Defines properties and methods of sc_random class.
 */

#ifndef SC_RANDOM_H
#define SC_RANDOM_H

int sc_random::state[624] = {0};
int sc_random::index = 0;

/**
 * (Re)-seeds the generator with specified integer.
 */
void sc_random::seed(int new_seed)
{
  index = 0;
  state[0] = new_seed;
  for(int i = 1; i < 624; ++i)
  {
    state[i] = 0x6c078965 * (state[i - 1] ^ (state[i-1] >> 30)) + i;
  }
}

/**
 * Returns next raw random number.
 * @return Pseudorandom int value.
 */
int sc_random::get_next()
{
  if(index == 0)
  {
    generate();
  }
  
  int y = state[index];
  y = y ^ (y >> 11);
  y = y ^ ((y << 7) & 0x9d2c5680);
  y = y ^ ((y << 15) & 0xefc60000);
  y = y ^ (y >> 18);

  ++index;
  if(index == 624)
      index = 0;

  return y;
}

/**
 * Returns next random number within specified range.
 * @param max_value Maximal possible value of generated number.
 * @return int value between 0 and max_value.
 */
int sc_random::get_next(int max_value)
{
  int random_number = get_next();
  if(random_number < 0)
      random_number *= -1;

  return random_number % (max_value + 1);
}

/**
 * Returns next random number within specified range.
 * @param min_value Minimal possible value of generated number.
 * @param max_value Maximal possible value of generated number.
 * @return int value between min_value and max_value.
 */
int sc_random::get_next(int min_value, int max_value)
{
  return min_value + get_next(max_value - min_value);
}

/**
 * Generates next portion of 624 random numbers and puts them into a state
 * array.
 */
void sc_random::generate()
{
  for(int i = 0; i < 624; ++i)
  {
    int y = (state[i] >> 31) + (state[(i + 1) % 624] >> 1);
    state[i] = state[(i + 397) % 624] ^ (y >> 1);
    if((y % 2) == 1)
    {
      state[i] = state[i] ^ 0x9908b0df;
    }
  }
}

#endif
