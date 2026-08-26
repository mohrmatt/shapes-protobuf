# shapes-protobuf

Demonstrates using Google Protocol Buffers (protobuf) with RTI Connext DDS,
and bridging DDS data to Apache Kafka using RTI Routing Service.

## Demos

| Directory | Description |
|-----------|-------------|
| [`protobuf-dds-demo/`](protobuf-dds-demo/README.md) | Protobuf type definitions for DDS ShapeType - three build variants showing the impact of DDS key and extensibility annotations |
| [`kafka-shapes-demo/`](kafka-shapes-demo/README.md) | Kafka <-> DDS bridge using RTI Routing Service with protobuf serialization; runnable natively or fully containerized with Docker |
