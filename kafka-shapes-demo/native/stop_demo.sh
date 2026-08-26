#!/usr/bin/env bash
# Stops all processes started by start_demo.sh.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ -f "$SCRIPT_DIR/.env" ]; then
    set -o allexport
    # shellcheck source=/dev/null
    source "$SCRIPT_DIR/.env"
    set +o allexport
fi

: "${KAFKA_DIR:?KAFKA_DIR is not set. Copy env.example to .env and fill it in.}"

PID_FILE="$SCRIPT_DIR/.demo_pids"

log() { echo "[$(date '+%H:%M:%S')] $*"; }

# ---- 1. Stop everything except Kafka ----
log "Stopping Routing Service..."
pkill -f "rtiroutingservice" 2>/dev/null || true

log "Stopping Shapes Demo..."
pkill -f "rtishapesdemo" 2>/dev/null || true

log "Stopping Kafka publishers..."
pkill -f "shapes_kafka_publisher" 2>/dev/null || true

log "Stopping Kafka subscribers..."
pkill -f "shapes_kafka_subscriber" 2>/dev/null || true

# Kill any tracked gnome-terminal PIDs (closes windows)
if [[ -f "$PID_FILE" ]]; then
    while read -r name pid; do
        [[ "$name" == "kafka-server" ]] && continue
        kill "$pid" 2>/dev/null || true
    done < "$PID_FILE"
fi

# ---- 2. Delete topics while Kafka is still running ----
log "Deleting Kafka topics..."
for TOPIC in Square Triangle; do
    "$KAFKA_DIR/bin/kafka-topics.sh" --delete \
        --topic "$TOPIC" \
        --bootstrap-server localhost:9092 2>/dev/null \
        && log "Deleted topic: $TOPIC" \
        || log "Could not delete topic: $TOPIC"
done

# ---- 3. Stop Kafka ----
log "Sending Kafka graceful shutdown..."
"$KAFKA_DIR/bin/kafka-server-stop.sh" 2>/dev/null || true

[[ -f "$PID_FILE" ]] && rm -f "$PID_FILE"
log "Done."
