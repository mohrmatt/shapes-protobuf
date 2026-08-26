#!/usr/bin/env bash
# Start all demo containers and rtishapesdemo.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOCKER_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

if [ -f "$DOCKER_DIR/.env" ]; then
    set -o allexport
    # shellcheck source=/dev/null
    source "$DOCKER_DIR/.env"
    set +o allexport
fi

: "${NDDSHOME:?NDDSHOME is not set. Copy env.example to .env and fill it in.}"

docker compose -f "$DOCKER_DIR/docker-compose.yml" up -d

echo ""
echo "Starting RTI Shapes Demo..."
"$NDDSHOME/bin/rtishapesdemo" -compact -dataType Shape -pubInterval 1000 >/dev/null 2>&1 &

echo ""
echo "Demo containers started."
echo ""
echo "  kafka-broker                - Kafka 4.1.1 on localhost:9092"
echo "  routing-service             - RTI Routing Service bridging DDS <-> Kafka"
echo "  kafka-publisher-square      - Publishes GREEN squares to Kafka topic 'Square'"
echo "  kafka-publisher-triangle    - Publishes BLUE triangles to Kafka topic 'Triangle'"
echo "  kafka-subscriber-square     - Subscribes to Kafka topic 'Square'"
echo "  kafka-subscriber-triangle   - Subscribes to Kafka topic 'Triangle'"
echo "  rtishapesdemo               - Shapes Demo GUI on domain 0"
echo ""
echo "Run ./scripts/view.sh for a live split-pane log view."
echo "Run ./scripts/stop.sh to tear everything down."
