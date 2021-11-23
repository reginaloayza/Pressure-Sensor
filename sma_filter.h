//@author: reginaloayza

class SMA_Filter
{
public:
  SMA_Filter(const unsigned int lookback_period)
  {
    T = lookback_period;
    memory = new float[lookback_period];
    for (size_t i = 0; i < T; i++)
      memory[i] = 0;
  }

  ~SMA_Filter() { delete[] memory; }

  void push_back(const float value)
  {
    sum += value - memory[T-1];
    for (size_t i = T - 1; i >= 1; i--)
        memory[i] = memory[i - 1];
    memory[0] = value;
  }

  float get_average()
  {
    return sum / (float)T;
  }

  float operator()(const float value)
  {
    push_back(value);
    return get_average();
  }
  
private:
  float* memory;
  unsigned int T;
  float sum = 0;
};