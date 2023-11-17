/***************************************************************************
Copyright © 2023 Shell M. Shrader <shell at shellware dot com>
----------------------------------------------------------------------------
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
****************************************************************************/
#include "main.h"

#ifdef BS_USE_TELNETSPY
void setExtraRemoteCommands(char c) {
  if (c == '?') {
    LOG_PRINTLN(bs.builtInRemoteCommandsMenu + "G = Get Station ID\n? = This menu\n");
  }
  // if (c == 'G') {
  //   LOG_PRINTF("\nStation ID = [%s]\n", my_config.station_id);
  // }
}
#endif
void updateExtraConfigItem(const String item, String value) {
  if (item == MQTT_SERVER) {
    memset(hb100_config.mqtt_server, CFG_NOT_SET, MQTT_SERVER_LEN);
    if (value.length() > 0) {
        value.toCharArray(hb100_config.mqtt_server, MQTT_SERVER_LEN);
        hb100_config.mqtt_server_flag = CFG_SET;
    } else {
        hb100_config.mqtt_server_flag = CFG_NOT_SET;
    }
    return;
  }
  if (item == MQTT_USER) {
    memset(hb100_config.mqtt_user, CFG_NOT_SET, MQTT_USER_LEN);
    if (value.length() > 0) {
        value.toCharArray(hb100_config.mqtt_user, MQTT_USER_LEN);
        hb100_config.mqtt_user_flag = CFG_SET;
    } else {
        hb100_config.mqtt_user_flag = CFG_NOT_SET;
    }
    return;
  }
  if (item == MQTT_PWD) {
    memset(hb100_config.mqtt_pwd, CFG_NOT_SET, MQTT_PWD_LEN);
    if (value.length() > 0) {
        value.toCharArray(hb100_config.mqtt_pwd, MQTT_PWD_LEN);
        hb100_config.mqtt_pwd_flag = CFG_SET;
    } else {
        hb100_config.mqtt_pwd_flag = CFG_NOT_SET;
    }
    return;
  }
  if (item == PRESENCE_COOLDOWN_SECONDS) {
    const short cooldown_seconds = value.toInt();
    if (cooldown_seconds > 0) {
        hb100_config.presence_cooldown_seconds = cooldown_seconds;
        hb100_config.presence_cooldown_seconds_flag = CFG_SET;
    } else {
        hb100_config.presence_cooldown_seconds_flag = CFG_NOT_SET;
        hb100_config.presence_cooldown_seconds = DEFAULT_COOLDOWN;
    }
    return;
  }
  if (item == MINIMUM_MOTION_THRESHOLD) {
    const short min_motion_threshold = value.toInt();
    if (min_motion_threshold > DEFAULT_MIN_MOTION_THRESHOLD) {
        hb100_config.minimum_motion_threshold = min_motion_threshold;
        hb100_config.minimum_motion_threshold_flag = CFG_SET;
    } else {
        hb100_config.minimum_motion_threshold_flag = CFG_NOT_SET;
        hb100_config.minimum_motion_threshold = DEFAULT_MIN_MOTION_THRESHOLD;
    }
    return;
  }
}
void updateExtraHtmlTemplateItems(String *html) {
  while (html->indexOf(escParam(MQTT_SERVER), 0) != -1) {
    html->replace(escParam(MQTT_SERVER), String(hb100_config.mqtt_server));
  }
  while (html->indexOf(escParam(MQTT_USER), 0) != -1) {
    html->replace(escParam(MQTT_USER), String(hb100_config.mqtt_user));
  }
  while (html->indexOf(escParam(MQTT_PWD), 0) != -1) {
    html->replace(escParam(MQTT_PWD), String(hb100_config.mqtt_pwd));
  }
  while (html->indexOf(escParam(PRESENCE_COOLDOWN_SECONDS), 0) != -1) {
    html->replace(escParam(PRESENCE_COOLDOWN_SECONDS), String(hb100_config.presence_cooldown_seconds));
  }
  while (html->indexOf(escParam(MINIMUM_MOTION_THRESHOLD), 0) != -1) {
    html->replace(escParam(MINIMUM_MOTION_THRESHOLD), String(hb100_config.minimum_motion_threshold));
  }
  while (html->indexOf(escParam(PRESENCE), 0) != -1) {
    html->replace(escParam(PRESENCE), _presence ? "Somebody" : "Nobody");
  }
  while (html->indexOf(escParam(MOVEMENT), 0) != -1) {
    html->replace(escParam(MOVEMENT), String(_movement));
  }
}

void setup() {
  #ifdef BS_USE_TELNETSPY
    bs.setExtraRemoteCommands(setExtraRemoteCommands);
  #endif

  bs.setConfig(&hb100_config, sizeof(hb100_config));
  bs.updateExtraConfigItem(updateExtraConfigItem);
  bs.updateExtraHtmlTemplateItems(updateExtraHtmlTemplateItems);
  
  if (!bs.setup()) return;

  // initialize our extended config struct if values are not set
  updateExtraConfigItem(MQTT_SERVER, String(hb100_config.mqtt_server));
  updateExtraConfigItem(MQTT_USER, String(hb100_config.mqtt_user));
  updateExtraConfigItem(MQTT_PWD, String(hb100_config.mqtt_pwd));
  updateExtraConfigItem(PRESENCE_COOLDOWN_SECONDS, String(hb100_config.presence_cooldown_seconds_flag == CFG_SET ? hb100_config.presence_cooldown_seconds : DEFAULT_COOLDOWN));
  updateExtraConfigItem(MINIMUM_MOTION_THRESHOLD, String(hb100_config.minimum_motion_threshold_flag == CFG_SET ? hb100_config.minimum_motion_threshold : DEFAULT_MIN_MOTION_THRESHOLD));
  
  bs.updateSetupHtml();
  bs.updateIndexHtml();

  // wire up our interupt for the HB100 sensor
  pinMode(D2, INPUT);
  attachInterrupt(D2, stateChange, FALLING);

  // set device details
  String uniqueId = String(hb100_config.hostname);
  std::replace(uniqueId.begin(), uniqueId.end(), '-', '_');

  for (tiny_int i = 0; i < uniqueId.length(); i++) {
    deviceId[i] = (byte)uniqueId[i];
  }

  strcpy(deviceName, uniqueId.c_str());

  device.setUniqueId(deviceId, uniqueId.length());
  device.setName(deviceName);
  device.setSoftwareVersion("1.0.0");
  device.setManufacturer("Shell M. Shrader");
  device.setModel("HB100");

  // configure sensors
  strcpy(movementSensorName,    (uniqueId + "_movement_sensor").c_str());
  strcpy(presenceSensorName,     (uniqueId + "_presence_sensor").c_str());
  strcpy(ipAddressSensorName,    (uniqueId + "_ip_address_sensor").c_str());

  movementSensor    = new HASensorNumber(movementSensorName, HASensorNumber::PrecisionP0);
  presenceSensor     = new HASensorNumber(presenceSensorName);
  ipAddressSensor    = new HASensor(ipAddressSensorName);
  
  movementSensor->setIcon("mdi:run-fast");
  movementSensor->setName("Movement");

  presenceSensor->setIcon("mdi:human-male-female-child");
  presenceSensor->setName("Presence");

  ipAddressSensor->setIcon("mdi:ip");
  ipAddressSensor->setName("IP Address");

  // fire up mqtt client if in station mode and mqtt server configured
  if (bs.wifimode == WIFI_STA && hb100_config.mqtt_server_flag == CFG_SET) {
    mqtt.begin(hb100_config.mqtt_server, hb100_config.mqtt_user, hb100_config.mqtt_pwd);

    mqtt.loop();
    movementSensor->setValue(0);
    presenceSensor->setValue("Nobody");
    ipAddressSensor->setValue(WiFi.localIP().toString().c_str());
    mqtt.loop();

    LOG_PRINTLN("MQTT started");
  }

  // setup done
  LOG_PRINTLN("\nSystem Ready\n");
}

void loop() {
  bs.loop();

  if (bs.wifimode == WIFI_STA && hb100_config.mqtt_server_flag == CFG_SET) {
    // handle MQTT
    mqtt.loop();
  }

  static unsigned long lastUpdate = 0;
  static unsigned long cooldown = 0;

  const unsigned long sysmillis = millis();

  // we wrapped
  if (lastUpdate > sysmillis) {
    lastUpdate = 0;
  }

  // evaluate tripped counter once a second
  if (sysmillis > 1000 && sysmillis - 1000 > lastUpdate) {
    if (tripped > hb100_config.minimum_motion_threshold) {
      _movement = tripped;
      _presence = true;
      LOG_PRINTF("movement on=[%d]\n", _movement);
      tripped = 0;
      cooldown = 0;
      movementSensor->setValue(_movement);
      presenceSensor->setValue("Somebody");
      bs.updateHtmlTemplate("/index.template.html", false);
    } else if (_movement != 0) {
      _movement = 0;
      LOG_PRINTF("movement off=[%d]\n", _movement);
      movementSensor->setValue(_movement);
      cooldown = sysmillis;
      bs.updateHtmlTemplate("/index.template.html", false);
    }
    lastUpdate = sysmillis;
  }  

  // we reached our cooldown threshold
  if (cooldown != 0 && cooldown + hb100_config.presence_cooldown_seconds * 1000 < sysmillis) {
    LOG_PRINTF("cooldown=[%lu] calc=[%lu] sysmillis=[%lu]\n", cooldown, cooldown + hb100_config.presence_cooldown_seconds * 1000, sysmillis);
    _presence = false;
    presenceSensor->setValue("Nobody");
    cooldown = 0;
    bs.updateHtmlTemplate("/index.template.html", false);
  }
}

const String escParam(const char * param_name) {
  char buf[64];
  sprintf(buf, "{%s}", param_name);
  return String(buf);
}

void IRAM_ATTR stateChange() {
  tripped++;
}
