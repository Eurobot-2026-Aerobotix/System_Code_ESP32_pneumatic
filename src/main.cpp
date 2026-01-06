#include <Arduino.h>
#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>

// micro-ROS variables
rcl_subscription_t forward_subscriber;
rcl_subscription_t backward_subscriber;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
std_msgs__msg__Int32 forward_msg;
std_msgs__msg__Int32 backward_msg;

// Function declarations
void forward_pneumatic_callback(const void *msgin);
void backward_pneumatic_callback(const void *msgin);

int front_suction_cup_pins[] = {16, 17, 12, 13};
int back_suction_cup_pins[] = {5, 21, 22, 33};
int front_pump_pin = 19;
int back_pump_pin = 18;

// Callback function for /forward/pneumatic topic
void forward_pneumatic_callback(const void *msgin) {
  const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msgin;
  int value = msg->data;
  
  Serial.println("Forward pneumatic command received: " + String(value));
}

// Callback function for /backward/pneumatic topic
void backward_pneumatic_callback(const void *msgin) {
  const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msgin;
  int value = msg->data;
  Serial.println("Backward pneumatic command received: " + String(value));
}

void activateSuctionCup(int front_0_back_1, int cup_index) {
  if (front_0_back_1 == 0) {
    digitalWrite(front_suction_cup_pins[cup_index], HIGH);
  } else {
    digitalWrite(back_suction_cup_pins[cup_index], HIGH);
  }
}
void activateAllSuctionCups(int front_0_back_1) {
  if (front_0_back_1 == 0) {
    for (int pin : front_suction_cup_pins) {
      digitalWrite(pin, HIGH);
    }
  } else {
    for (int pin : back_suction_cup_pins) {
      digitalWrite(pin, HIGH);
    }
  }
}
void deactivateSuctionCup(int front_0_back_1, int cup_index) {
  if (front_0_back_1 == 0) {
    digitalWrite(front_suction_cup_pins[cup_index], LOW);
  } else {
    digitalWrite(back_suction_cup_pins[cup_index], LOW);
  }
}
void deactivateAllSuctionCups(int front_0_back_1) {
  if (front_0_back_1 == 0) {
    for (int pin : front_suction_cup_pins) {
      digitalWrite(pin, LOW);
    }
  } else {
    for (int pin : back_suction_cup_pins) {
      digitalWrite(pin, LOW);
    }
  }
}
void activatePump(int front_0_back_1) {
  if (front_0_back_1 == 0) {
    digitalWrite(front_pump_pin, HIGH);
  } else {
    digitalWrite(back_pump_pin, HIGH);
  }
}
void deactivatePump(int front_0_back_1) {
  if (front_0_back_1 == 0) {
    digitalWrite(front_pump_pin, LOW);
  } else {
    digitalWrite(back_pump_pin, LOW);
  }
}

void test_with_serial(){
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s.length() > 0) {
      int n = s.toInt();
      Serial.print("Received: ");
      Serial.println(n);

      if (n >= 0 && n <= 3) {
        Serial.println("Activating front suction cup");
        activateSuctionCup(0, n);
        delay(500);
        deactivateSuctionCup(0, n);
      } else if (n >= 4 && n <= 7) {
        int idx = n - 4;
        Serial.println("Activating back suction cup");
        activateSuctionCup(1, idx);
        delay(500);
        deactivateSuctionCup(1, idx);
      } else if (n == 20) {
        Serial.println("Activating all front suction cups");
        activateAllSuctionCups(0);
      } else if (n == 21) {
        Serial.println("Activating all back suction cups");
        activateAllSuctionCups(1);
      } else if (n == 30) {
        Serial.println("Activating front pump");
        activatePump(0);
      } else if (n == 31) {
        Serial.println("Activating back pump");
        activatePump(1);
      } else if (n == 50) {
        Serial.println("Deactivating front pump");
        deactivatePump(0);
      } else if (n == 51) {
        Serial.println("Deactivating back pump");
        deactivatePump(1);
      } else if (n == 40) {
        Serial.println("Deactivating all front suction cups");
        deactivateAllSuctionCups(0);
      } else if (n == 41) {
        Serial.println("Deactivating all back suction cups");
        deactivateAllSuctionCups(1);
      } else {
        Serial.println("Unknown command");
      }
    }
  }
}
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Starting ESP32 Pneumatic Node...");
  set_microros_transports();
  digitalWrite(2, HIGH); // Turn on built-in LED to indicate startup
  // Initialize GPIO pins
  for (int pin : front_suction_cup_pins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  for (int pin : back_suction_cup_pins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  pinMode(front_pump_pin, OUTPUT);
  digitalWrite(front_pump_pin, LOW);
  pinMode(back_pump_pin, OUTPUT);
  digitalWrite(back_pump_pin, LOW);

  // Wait for micro-ROS agent
  Serial.println("Waiting for micro-ROS agent...");
  delay(3000);

  // Initialize micro-ROS with error checking and retry
  allocator = rcl_get_default_allocator();
  
  // Retry connection up to 10 times
  rcl_ret_t ret;
  int retry_count = 0;
  do {
      ret = rclc_support_init(&support, 0, NULL, &allocator);
      if (ret != RCL_RET_OK) {
          Serial.println("Failed to connect to micro-ROS agent, retrying...");
          delay(1000);
          retry_count++;
      }
  } while (ret != RCL_RET_OK && retry_count < 10);
  
  if (ret != RCL_RET_OK) {
      Serial.println("ERROR: Failed to initialize micro-ROS!");
      while(1) { delay(100); }
  }
  Serial.println("✓ Support initialized");

  // Create node
  ret = rclc_node_init_default(&node, "esp32_pneumatic_node", "", &support);
  if (ret != RCL_RET_OK) {
      Serial.println("ERROR: Failed to init node!");
      while(1) { delay(100); }
  }
  Serial.println("✓ Node created");

  // Create subscriptions
  ret = rclc_subscription_init_default(
      &forward_subscriber,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      "/front/pneumatic");
  if (ret != RCL_RET_OK) {
      Serial.println("ERROR: Failed to create forward subscription!");
  } else {
      Serial.println("✓ Forward subscription created");
  }

  ret = rclc_subscription_init_default(
      &backward_subscriber,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      "/backward/pneumatic");
  if (ret != RCL_RET_OK) {
      Serial.println("ERROR: Failed to create backward subscription!");
  } else {
      Serial.println("✓ Backward subscription created");
  }

  // Create executor
  ret = rclc_executor_init(&executor, &support.context, 2, &allocator);
  if (ret != RCL_RET_OK) {
      Serial.println("ERROR: Failed to init executor!");
  } else {
      Serial.println("✓ Executor initialized");
  };
  rclc_executor_add_subscription(&executor, &forward_subscriber, &forward_msg,
                                  &forward_pneumatic_callback, ON_NEW_DATA);
  rclc_executor_add_subscription(&executor, &backward_subscriber, &backward_msg,
                                  &backward_pneumatic_callback, ON_NEW_DATA);
  
  Serial.println("✓✓✓ ESP32 Pneumatic Node Ready! ✓✓✓");
}

void loop() {
  // Spin executor to process incoming messages
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
  delay(10);
}
