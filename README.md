# ESP32 Real-Time CPU & Task Monitor 🚀

![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v4.4+-blue?logo=espressif)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-Enabled-green?logo=freertos)
![MQTT](https://img.shields.io/badge/MQTT-Integrated-orange?logo=eclipse-mosquitto)
![Dual Core](https://img.shields.io/badge/Dual%20Core-Supported-9cf)

## 🌟 Project Vision

Empower ESP32 developers with real-time, actionable insights into system performance. This project provides a professional, plug-and-play ESP-IDF component for monitoring CPU usage, task statistics, and system health, with seamless telemetry for remote diagnostics and visualization. The goal is to simplify performance tuning, debugging, and long-term monitoring for embedded applications.

## 🏗️ Architecture Overview

The system is structured as a modular ESP-IDF component that integrates with FreeRTOS and optional MQTT telemetry. Its main architectural elements are:

- **CPU & Task Monitor Core**: Collects per-core and per-task statistics using FreeRTOS APIs.
- **Heap Monitor**: Tracks memory usage and fragmentation.
- **MQTT Telemetry Module**: Publishes metrics in JSON format to a configurable broker/topic.
- **Configuration Layer**: Allows flexible setup via `cpu_monitor_config_t`.
- **Visualization Integration**: Data is ready for Grafana/Prometheus dashboards.

**Component Interaction Diagram:**

```
+-------------------+
|   Application     |
+-------------------+
          |
          v
+-------------------+
| cpu_monitor_init  |
+-------------------+
          |
          v
+-------------------+
|  CPU/Task Monitor |<---+---+---+
+-------------------+    |   |   |
          |              |   |   |
          v              |   |   |
+-------------------+    |   |   |
|   Heap Monitor    |    |   |   |
+-------------------+    |   |   |
          |              |   |   |
          v              |   |   |
+-------------------+    |   |   |
|  MQTT Telemetry   |----+   |   |
+-------------------+        |   |
          |                  |   |
          v                  |   |
+-------------------+        |   |
|   MQTT Broker     |<-------+   |
+-------------------+            |
          |                      |
          v                      |
+-------------------+            |
| Visualization     |<-----------+
| (Grafana, etc.)   |
+-------------------+
```

---

A professional-grade ESP-IDF component for real-time system monitoring on ESP32 (single/dual-core). Provides per-task CPU usage, core affinity tracking, heap analysis, and MQTT telemetry.

![Dashboard Preview](docs/assets/dashboard-preview.png) *(Example Grafana Dashboard)*

## ✨ Features

- **Multi-Core Monitoring**
  - Tracks CPU% per core (ESP32 dual-core)
  - Identifies core affinity for each task
- **Task-Level Insights**
  - CPU% per FreeRTOS task
  - Stack high-water marks
- **System Health**
  - Heap memory usage & fragmentation
  - Minimum ever free memory
- **Live Telemetry**
  - MQTT publishing (JSON format)
  - Configurable update intervals
- **Visualization Ready**
  - Grafana dashboard templates included
  - Prometheus/InfluxDB compatible

## 📦 Installation

### As an ESP-IDF Component

```bash
cd your_project/components
git clone https://github.com/Muditha-Kumara/freertos-cpu-monitor.git
```

### Via IDF Component Registry

```yaml
# idf_component.yml
dependencies:
  esp32-cpu-monitor:
    git: https://github.com/Muditha-Kumara/freertos-cpu-monitor.git
```

## 🚀 Quick Start

```c
#include "cpu_monitor.h"

void app_main() {
    cpu_monitor_config_t config = {
        .sample_rate_ms = 1000,
        .mqtt_broker = "mqtt://broker.ip",
        .mqtt_topic = "esp32/metrics",
        .enable_heap_mon = true
    };
    
    ESP_ERROR_CHECK(cpu_monitor_init(&config));
    cpu_monitor_start();
}
```

## 📊 MQTT Topics & Payloads

| Topic                 | Payload Example                                         | Description         |
|----------------------|--------------------------------------------------------|---------------------|
| esp32/metrics/cpu    | {"core0":72.5,"core1":65.3}                           | Per-core CPU usage  |
| esp32/metrics/tasks  | [{"name":"wifi","core":0,"cpu":45.2,"stack":512}]      | Task statistics     |
| esp32/metrics/heap   | {"free":42128,"min_free":40192,"frag":8}             | Heap metrics        |
| esp32/metrics/alerts | {"event":"high_cpu","core":0,"value":92.5}          | Threshold alerts    |

## 📈 Grafana Integration

1. Set up data source (InfluxDB/Prometheus)
2. Import dashboard:

   ```bash
   grafana-cli admin import-dashboard docs/grafana/dashboard.json
   ```
3. Sample Dashboard:

   ![Grafana Screenshot](docs/assets/grafana-screenshot.png)

## ⚙️ Configuration

**cpu_monitor_config_t**

| Parameter         | Default           | Description                |
|-------------------|------------------|----------------------------|
| sample_rate_ms    | 1000             | Sampling interval (ms)     |
| mqtt_broker       | NULL              | MQTT broker URI            |
| mqtt_topic        | "esp32/metrics"  | Base MQTT topic            |
| enable_heap_mon   | true             | Enable heap monitoring     |
| alert_threshold   | 90.0             | CPU% alert threshold       |

## 🧠 Technical Details

**CPU Usage Calculation**

```
CPU% = (TaskElapsedTime / (TotalElapsedTime * NumberOfCores)) * 100
```

- Samples FreeRTOS idle task runtime
- Accounts for dual-core scheduling

## 🖨️ Example Output

The actual output from the current implementation (see logs via serial monitor) will look like:

```
I (12345) cpu_monitor: 

Getting real time stats over 1000 ticks
I (12345) cpu_monitor: | Task             | Core       | Run Time   | Percent   | Stack Watermark   | State  | CurPrio  | BasePrio |
I (12345) cpu_monitor: |------------------|------------|------------|-----------|-------------------|--------|----------|----------|
I (12345) cpu_monitor: | IDLE0            | 0          | 206251     | 10%       | 1024              | RUN    | 0        | 0        |
I (12345) cpu_monitor: | IDLE1            | 1          | 464785     | 23%       | 1024              | RUN    | 0        | 0        |
I (12345) cpu_monitor: | wifi             | 0          | 45213      | 2%        | 512               | RDY    | 1        | 1        |
I (12345) cpu_monitor: | main             | 0          | 120000     | 6%        | 2048              | RDY    | 1        | 1        |
I (12345) cpu_monitor: | eventTask        | 1          | 80000      | 4%        | 1024              | BLK    | 1        | 1        |
I (12345) cpu_monitor: | Tmr Svc          | 0          | 0          | 0%        | 1024              | SUS    | 1        | 1        |
I (12345) cpu_monitor: | esp_timer        | 1          | 0          | 0%        | 1024              | SUS    | 1        | 1        |
I (12345) cpu_monitor: | ipc1             | 1          | 0          | 0%        | 1024              | SUS    | 1        | 1        |
I (12345) cpu_monitor: | ipc0             | 0          | 0          | 0%        | 1024              | SUS    | 1        | 1        |
I (12345) cpu_monitor: 
Per-core overall CPU usage:
I (12345) cpu_monitor: Core 0: 18%
I (12345) cpu_monitor: Core 1: 27%
I (12345) cpu_monitor: Real time stats obtained
```

- The log prefix (e.g., `I (12345) cpu_monitor:`) is from ESP-IDF logging.
- The table includes task name, core, run time, percent, stack watermark, state, current and base priority.
- Per-core CPU usage is shown after the table.
- Task names, run times, and percentages will vary depending on your running tasks and system state.

### Memory Footprint

| Feature           | Flash Usage | RAM Usage         |
|-------------------|-------------|-------------------|
| Core Monitoring   | ~2.5KB      | 500B              |
| Task Stats        | +1.2KB      | +1KB per task     |
| MQTT              | +3KB        | +2KB buffer       |

## 📚 Documentation
- API Reference
- Theory of Operation
- Troubleshooting Guide

## 🤝 Contributing
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Open a Pull Request

## 📜 License
Apache 2.0 © Muditha-Kumara 2023

## Ready to optimize your ESP32? Clone and integrate today!

```bash
git clone https://github.com/Muditha-Kumara/freertos-cpu-monitor.git
```

