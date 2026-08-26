#!/usr/bin/env bash
# Starts Kafka, RTI Shapes Demo, Routing Service, and Kafka publishers/subscribers.
# Run stop_demo.sh to tear everything down.
#
# Configuration is read from .env in this directory. Copy env.example to .env
# and fill in the required paths before running.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ -f "$SCRIPT_DIR/.env" ]; then
    set -o allexport
    # shellcheck source=/dev/null
    source "$SCRIPT_DIR/.env"
    set +o allexport
fi

: "${NDDSHOME:?NDDSHOME is not set. Copy env.example to .env and fill it in.}"
: "${RTI_GATEWAY_HOME:?RTI_GATEWAY_HOME is not set.}"
: "${KAFKA_DIR:?KAFKA_DIR is not set.}"

CONNEXTDDS_ARCH="${CONNEXTDDS_ARCH:-x64Linux4gcc8.5.0}"
RS_CONFIG_DIR="$RTI_GATEWAY_HOME/examples/kafka/kafka-shapes-protobuf"
KAFKA_BOOTSTRAP="${KAFKA_BOOTSTRAP:-localhost:9092}"
LOG_DIR="$SCRIPT_DIR/logs"
PID_FILE="$SCRIPT_DIR/.demo_pids"

mkdir -p "$LOG_DIR"
> "$PID_FILE"

log()       { echo "[$(date '+%H:%M:%S')] $*"; }
track_pid() { echo "$1 $2" >> "$PID_FILE"; }

# ---- 1. Start Kafka (KRaft - format step required each boot) ----
log "Formatting Kafka storage..."
CLUSTER_ID=$("$KAFKA_DIR/bin/kafka-storage.sh" random-uuid)
"$KAFKA_DIR/bin/kafka-storage.sh" format \
    -t "$CLUSTER_ID" \
    -c "$KAFKA_DIR/config/server.properties" \
    --standalone \
    > "$LOG_DIR/kafka-format.log" 2>&1

log "Starting Kafka server..."
"$KAFKA_DIR/bin/kafka-server-start.sh" "$KAFKA_DIR/config/server.properties" \
    > "$LOG_DIR/kafka-server.log" 2>&1 &
track_pid kafka-server $!

log "Waiting for Kafka to be ready..."
until "$KAFKA_DIR/bin/kafka-topics.sh" --list \
    --bootstrap-server "$KAFKA_BOOTSTRAP" > /dev/null 2>&1; do
    sleep 1
done
log "Kafka is ready."

# ---- 2. Create Kafka topics ----
for TOPIC in Square Triangle; do
    "$KAFKA_DIR/bin/kafka-topics.sh" --create \
        --topic "$TOPIC" \
        --bootstrap-server "$KAFKA_BOOTSTRAP" \
        --partitions 1 \
        --replication-factor 1 > /dev/null 2>&1 \
        && log "Created topic: $TOPIC" \
        || log "Topic already exists: $TOPIC"
done

# ---- 3. Start RTI Shapes Demo ----
log "Starting RTI Shapes Demo..."
"$NDDSHOME/bin/rtishapesdemo" -dataType Shape -pubInterval 1000 > /dev/null 2>&1 &
track_pid rtishapesdemo $!

# ---- 4. Start RTI Routing Service ----
# cd to the gateway example dir so the relative descriptor_file path in the XML resolves
log "Starting RTI Routing Service..."
gnome-terminal --title="RTI Routing Service" -- bash -c "
    export NDDSHOME=\"$NDDSHOME\"
    export LD_LIBRARY_PATH=\"$RTI_GATEWAY_HOME/lib:$NDDSHOME/lib/$CONNEXTDDS_ARCH\"
    cd \"$RTI_GATEWAY_HOME/examples/kafka/kafka-shapes-protobuf\"
    exec \"$NDDSHOME/bin/rtiroutingservice\" \
        -cfgFile \"$SCRIPT_DIR/../config/shapesdemo_kafka_protobuf.xml\" \
        -cfgName shapesdemo_bridge 2>&1 | tee \"$LOG_DIR/routing-service.log\"
" &
track_pid rtiroutingservice $!

log "Waiting for Routing Service to initialize..."
sleep 5

# ---- 5. Start Kafka publishers (each in own terminal) ----
log "Starting Kafka publisher: GREEN Square..."
gnome-terminal --title="Kafka Publisher: GREEN Square" -- bash -c "
    export LD_LIBRARY_PATH=\"$RTI_GATEWAY_HOME/lib\"
    exec \"$RS_CONFIG_DIR/bin/shapes_kafka_publisher\" \"$KAFKA_BOOTSTRAP\" GREEN Square
" &
track_pid kafka-pub-green $!

log "Starting Kafka publisher: ORANGE Triangle..."
gnome-terminal --title="Kafka Publisher: ORANGE Triangle" -- bash -c "
    export LD_LIBRARY_PATH=\"$RTI_GATEWAY_HOME/lib\"
    exec \"$RS_CONFIG_DIR/bin/shapes_kafka_publisher\" \"$KAFKA_BOOTSTRAP\" ORANGE Triangle
" &
track_pid kafka-pub-orange $!

# ---- 6. Start Kafka subscribers (each in own terminal) ----
log "Starting Kafka subscriber: Square..."
gnome-terminal --title="Kafka Subscriber: Square" -- bash -c "
    export LD_LIBRARY_PATH=\"$RTI_GATEWAY_HOME/lib\"
    exec \"$RS_CONFIG_DIR/bin/shapes_kafka_subscriber\" \"$KAFKA_BOOTSTRAP\" Square
" &
track_pid kafka-sub-square $!

log "Starting Kafka subscriber: Triangle..."
gnome-terminal --title="Kafka Subscriber: Triangle" -- bash -c "
    export LD_LIBRARY_PATH=\"$RTI_GATEWAY_HOME/lib\"
    exec \"$RS_CONFIG_DIR/bin/shapes_kafka_subscriber\" \"$KAFKA_BOOTSTRAP\" Triangle
" &
track_pid kafka-sub-triangle $!

log "Demo running. Logs: $LOG_DIR/"
log "Run ./stop_demo.sh to stop everything."

sleep 2  # allow gnome-terminal child processes to register before checking PIDs

check_proc() {
    local label="$1" pattern="$2" pid
    pid=$(pgrep -f "$pattern" | head -1)
    if [[ -n "$pid" ]]; then
        printf "%-35s %-8s %s\n" "$label" "$pid" "RUNNING"
    else
        printf "%-35s %-8s %s\n" "$label" "-" "NOT RUNNING"
    fi
}

echo ""
printf "%-35s %-8s %s\n" "Component" "PID" "Status"
printf "%-35s %-8s %s\n" "---------" "---" "------"
check_proc "Kafka Server"                  "kafka.Kafka"
check_proc "RTI Shapes Demo"               "rtishapesdemo"
check_proc "RTI Routing Service"           "rtiroutingserviceapp"
check_proc "Kafka Publisher: GREEN Square" "shapes_kafka_publisher.*GREEN"
check_proc "Kafka Publisher: ORANGE Triangle" "shapes_kafka_publisher.*ORANGE"
check_proc "Kafka Subscriber: Square"      "shapes_kafka_subscriber.*Square"
check_proc "Kafka Subscriber: Triangle"    "shapes_kafka_subscriber.*Triangle"
echo ""
