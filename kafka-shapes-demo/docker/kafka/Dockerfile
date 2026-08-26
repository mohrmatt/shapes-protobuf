FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    openjdk-21-jre-headless \
    && rm -rf /var/lib/apt/lists/*

ENV KAFKA_VERSION=4.1.1 \
    KAFKA_SCALA_VERSION=2.13 \
    KAFKA_HOME=/opt/kafka

# Tarball is staged into the build context by build.sh (from ~/Downloads or archive.apache.org).
COPY kafka.tgz /tmp/kafka.tgz
RUN tar -xzf /tmp/kafka.tgz -C /opt \
    && mv /opt/kafka_${KAFKA_SCALA_VERSION}-${KAFKA_VERSION} /opt/kafka \
    && rm /tmp/kafka.tgz

COPY server.properties /opt/kafka/config/kraft/server.properties
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

VOLUME ["/opt/kafka/data"]
EXPOSE 9092 9093

ENTRYPOINT ["/entrypoint.sh"]
