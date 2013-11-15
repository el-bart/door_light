#ifndef INCLUDE_SAMPLER_HPP_FILE
#define INCLUDE_SAMPLER_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

/** @brief collection of samples.
 */
template<typename T, uint8_t N>
class Sampler
{
public:
  explicit Sampler(const T init):
    idx_{0}
  {
    for(auto& e: samples_)
      e = init;
  }

  void add(const T sample)
  {
    if(idx_>=N)
      idx_=0;
    samples_[idx_] = sample;
  }

  T min(void) const
  {
    return find( [](T const& lhs, T const& rhs){ return lhs<rhs; } );
  }

  T max(void) const
  {
    return find( [](T const& lhs, T const& rhs){ return lhs>rhs; } );
  }

private:
  template<typename F>
  T find(F f) const
  {
    T out=samples_[0];
    for(auto const& e: samples_)
      if( f(e,out) )
        out=e;
    return out;
  }

  uint8_t idx_;
  T       samples_[N];
};

#endif
