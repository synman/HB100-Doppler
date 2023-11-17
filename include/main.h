/***************************************************************************
Copyright © 2023 Shell M. Shrader <shell at shellware dot com>
----------------------------------------------------------------------------
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
****************************************************************************/
#include "Bootstrap.h"
#include <ArduinoHA.h>

#ifdef BS_USE_TELNETSPY
    TelnetSpy SerialAndTelnet;
    Bootstrap bs = Bootstrap(PROJECT_NAME, &SerialAndTelnet, 1500000);
#else
    Bootstrap bs = Bootstrap(PROJECT_NAME);
#endif

#define MQTT_SERVER                    "mqtt_server"
#define MQTT_USER                      "mqtt_user"
#define MQTT_PWD                       "mqtt_pwd"
#define PRESENCE_COOLDOWN_SECONDS      "presence_cooldown_seconds"
#define MINIMUM_MOTION_THRESHOLD       "minimum_motion_threshold"

#define MOVEMENT                       "movement"
#define PRESENCE                       "presence"

#define MQTT_SERVER_LEN                16
#define MQTT_USER_LEN                  16
#define MQTT_PWD_LEN                   32

#define DEFAULT_COOLDOWN               60
#define DEFAULT_MIN_MOTION_THRESHOLD    1

typedef struct hb100_config_type : config_type {
    tiny_int      mqtt_server_flag;
    char          mqtt_server[MQTT_SERVER_LEN];
    tiny_int      mqtt_user_flag;
    char          mqtt_user[MQTT_USER_LEN];
    tiny_int      mqtt_pwd_flag;
    char          mqtt_pwd[MQTT_PWD_LEN];
    tiny_int      presence_cooldown_seconds_flag;
    short         presence_cooldown_seconds;
    tiny_int      minimum_motion_threshold_flag;
    tiny_int      minimum_motion_threshold;
} HB100_CONFIG_TYPE;

HB100_CONFIG_TYPE hb100_config;

tiny_int        _movement = 0;
bool            _presence = false;

HADevice device;
WiFiClient wifiClient;
HAMqtt mqtt(wifiClient, device, 10);

byte deviceId[40];
char deviceName[40];

char movementSensorName[80];
char presenceSensorName[80];
char ipAddressSensorName[80];

HASensorNumber* movementSensor;
HASensor* presenceSensor;
HASensor* ipAddressSensor;

const String escParam(const char *param_name);
volatile tiny_int tripped = 0;
void IRAM_ATTR stateChange();

