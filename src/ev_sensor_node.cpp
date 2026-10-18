#include <chrono>
#include <functional>
#include <memory>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

using namespace std::chrono_literals;

class EvSensorNode : public rclcpp::Node
{
public:
  EvSensorNode()
  : Node("ev_sensor_node"), battery_energy_(60.0f), rng_(std::random_device{}()), dist_(15.0f, 21.0f)
  {
    energy_pub_ = this->create_publisher<std_msgs::msg::Float32>("/battery_energy", 10);
    consumption_pub_ = this->create_publisher<std_msgs::msg::Float32>("/avg_consumption", 10);

    timer_ = this->create_wall_timer(
      1000ms, std::bind(&EvSensorNode::timer_callback, this));

    RCLCPP_INFO(
      this->get_logger(),
      "EV Sensor Node started. Initial battery energy: %.1f kWh",
      battery_energy_);
  }

private:
  void timer_callback()
  {
    // Simulate battery drain and recharge cycle
    battery_energy_ -= 1.2f;
    if (battery_energy_ < 3.0f) {
      battery_energy_ = 60.0f;
      RCLCPP_INFO(this->get_logger(), "--- Battery recharged to 60.0 kWh! ---");
    }

    // Generate random average consumption between 15.0 and 21.0 kWh/100km
    float current_consumption = dist_(rng_);

    auto energy_msg = std_msgs::msg::Float32();
    energy_msg.data = battery_energy_;

    auto consumption_msg = std_msgs::msg::Float32();
    consumption_msg.data = current_consumption;

    energy_pub_->publish(energy_msg);
    consumption_pub_->publish(consumption_msg);

    RCLCPP_INFO(
      this->get_logger(),
      "Published sensor data -> Energy: %.1f kWh | Consumption: %.1f kWh/100km",
      energy_msg.data, consumption_msg.data);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr energy_pub_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr consumption_pub_;
  float battery_energy_;
  std::mt19937 rng_;
  std::uniform_real_distribution<float> dist_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<EvSensorNode>());
  rclcpp::shutdown();
  return 0;
}