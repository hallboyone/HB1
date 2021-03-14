#ifndef PID_CONTROLLER
#define PID_CONTROLLER

#include <limits>

namespace HB1{
  /**
   * An implemenation of a PID controller. 
   */
  class PIDController{
  private:
    const double P_;
    const double I_;
    const double D_;

    double windup_bound_;
    double input_min_;
    double input_max_;
    double set_point_;

    double err_sum_;
    double err_change_;
    double err_;

    double clamp(double val, double min, double max){
      if (val < min) return min;
      else if (val > max) return max;
      else return val;
    }
    
  public:
    PIDController(const double P,
		  const double I,
		  const double D,
		  const double set_point = 0):
      P_(P), I_(I), D_(D), set_point_(set_point){
      prev_err_ = 0;
      prev_err_avg_ = 0;
      err_sum_ = 0;
      windup_bound_ = std::numeric_limits<double>::infinity;
      input_min_ = -std::numeric_limits<double>::infinity;
      input_max_ = std::numeric_limits<double>::infinity;
    }

    void setSetPoint(const double set_point){
      set_point_ = set_point;
    }
    void setWindupBound(const double bound){
      windup_bound_ = bound;
    }
    void setInputBounds(const double min, const double max){
      if (max < min) throw "Max input value must be greater than min";
      input_max_ = max;
      input_min_ = min;
    }

    double addSample(const double sample){
      double new_err = set_point_ - sample;
      err_sum_ = clamp(err_sum_ + new_err, -windup_bound_, windup_bound_);
      double err_change = new_err - err_;
      err_ = new_err;

      return input();
    }

    double input(){
      return clamp(P_*err_ + I_*err_sum_ + D_*err_change, input_min_, input_max_);
  }
}

#endif
