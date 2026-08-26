#!/usr/bin/env bash
set -euo pipefail

DATA_DIR=/opt/kafka/data
CLUSTER_ID_FILE="$DATA_DIR/.cluster_id"

# Format the log directory on first start
if [ ! -f "$CLUSTER_ID_FILE" ]; then
    mkdir -p "$DATA_DIR"
    CLUSTER_ID=$(/opt/kafka/bin/kafka-storage.sh random-uuid)
    echo "$CLUSTER_ID" > "$CLUSTER_ID_FILE"
    /opt/kafka/bin/kafka-storage.sh format \
        -t "$CLUSTER_ID" \
        -c /opt/kafka/config/kraft/server.properties \
        --ignore-formatted
fi

/opt/kafka/bin/kafka-server-start.sh /opt/kafka/config/kraft/server.properties &
KAFKA_PID=$!

# Forward SIGTERM/SIGINT to the Kafka process so docker stop works cleanly
trap "kill -TERM $KAFKA_PID" TERM INT

# Wait for the broker to be ready before creating topics
echo "Waiting for Kafka broker to be ready..."
until /opt/kafka/bin/kafka-topics.sh --bootstrap-server localhost:9092 --list >/dev/null 2>&1; do
    sleep 1
done

# Pre-create demo topics so the routing service never races against topic creation
for topic in Square Triangle; do
    /opt/kafka/bin/kafka-topics.sh --bootstrap-server localhost:9092 \
        --create --if-not-exists \
        --topic "$topic" \
        --partitions 3 \
        --replication-factor 1
    echo "Topic '$topic' ready."
done

wait $KAFKA_PID
