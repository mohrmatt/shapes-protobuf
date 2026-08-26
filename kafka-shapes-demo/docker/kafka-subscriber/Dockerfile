FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# libprotobuf23 required by the shapes_kafka_subscriber binary
RUN apt-get update && apt-get install -y --no-install-recommends \
    libprotobuf23 \
    && rm -rf /var/lib/apt/lists/*

# librdkafka shared library (staged by build.sh)
COPY libs/ /usr/local/lib/
RUN ldconfig

# Kafka subscriber binary (staged by build.sh)
COPY bin/shapes_kafka_subscriber /usr/local/bin/shapes_kafka_subscriber
RUN chmod +x /usr/local/bin/shapes_kafka_subscriber

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

# Default args: broker  topic
CMD ["localhost:9092", "Square"]

ENTRYPOINT ["/entrypoint.sh"]
