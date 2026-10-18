#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

using std::placeholders::_1;

class RangeCalcNode : public rclcpp::Node
{
public:
  RangeCalcNode()
  : Node("range_calc_node"), latest_energy_(60.0f), latest_consumption_(18.0f)
  {
    energy_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      "/battery_energy", 10, std::bind(&RangeCalcNode::energy_callback, this, _1));

    consumption_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      "/avg_consumption", 10, std::bind(&RangeCalcNode::consumption_callback, this, _1));

    range_pub_ = this->create_publisher<std_msgs::msg::Float32>("/estimated_range", 10);

    RCLCPP_INFO(this->get_logger(), "Range Calculator Node started. Waiting for sensor data...");
  }

private:
  void energy_callback(const std_msgs::msg::Float32 & msg)
  {
    latest_energy_ = msg.data;
  }

  void consumption_callback(const std_msgs::msg::Float32 & msg)
  {
    latest_consumption_ = msg.data;
    calculate_and_publish_range();
  }

  void calculate_and_publish_range()
  {
    if (latest_consumption_ <= 0.1f) {
      return;
    }

    // Calculate estimated range: (Energy [kWh] / Consumption [kWh/100km]) * 100
    float estimated_range = (latest_energy_ / latest_consumption_) * 100.0f;

    auto range_msg = std_msgs::msg::Float32();
    range_msg.data = estimated_range;
    range_pub_->publish(range_msg);

    // Log warning if estimated range drops below 50 km
    if (estimated_range < 50.0f) {
      RCLCPP_WARN(
        this->get_logger(),
        "Estimated range: %.1f km -> WARNING: Low battery range! Charging required!",
        estimated_range);
    } else {
      RCLCPP_INFO(
        this->get_logger(),
        "Estimated range: %.1f km (Energy: %.1f kWh, Consumption: %.1f kWh/100km)",
        estimated_range, latest_energy_, latest_consumption_);
    }
  }

  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr energy_sub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr consumption_sub_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr range_pub_;
  float latest_energy_;
  float latest_consumption_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RangeCalcNode>());
  rclcpp::shutdown();
  return 0;
}