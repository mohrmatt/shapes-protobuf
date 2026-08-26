#!/usr/bin/env bash
# Copies the required RTI Connext DDS and gateway binaries/libraries into the
# Docker build contexts, then builds all images with docker compose.
#
# Prerequisites: copy env.example to .env and fill in NDDSHOME and
# RTI_GATEWAY_HOME before running this script.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOCKER_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
ENV_FILE="$DOCKER_DIR/.env"

# Source .env if present
if [ -f "$ENV_FILE" ]; then
    set -o allexport
    # shellcheck source=/dev/null
    source "$ENV_FILE"
    set +o allexport
fi

# Validate required variables
: "${NDDSHOME:?NDDSHOME is not set. Copy env.example to .env and fill it in.}"
: "${RTI_GATEWAY_HOME:?RTI_GATEWAY_HOME is not set. Copy env.example to .env and fill it in.}"

CONNEXTDDS_ARCH="${CONNEXTDDS_ARCH:-x64Linux4gcc8.5.0}"

NDDS_LIB="$NDDSHOME/lib/$CONNEXTDDS_ARCH"
NDDS_APP_BIN="$NDDSHOME/resource/app/bin/$CONNEXTDDS_ARCH"
GATEWAY_LIB="$RTI_GATEWAY_HOME/lib"
GATEWAY_EXAMPLE_BIN="$RTI_GATEWAY_HOME/examples/kafka/kafka-shapes-protobuf/bin"

# Verify paths exist before proceeding
for dir in "$NDDS_LIB" "$NDDS_APP_BIN" "$GATEWAY_LIB" "$GATEWAY_EXAMPLE_BIN"; do
    if [ ! -d "$dir" ]; then
        echo "ERROR: directory not found: $dir"
        exit 1
    fi
done

KAFKA_VERSION="${KAFKA_VERSION:-4.1.1}"
KAFKA_SCALA_VERSION="${KAFKA_SCALA_VERSION:-2.13}"
KAFKA_TGZ="kafka_${KAFKA_SCALA_VERSION}-${KAFKA_VERSION}.tgz"
KAFKA_DEST="$DOCKER_DIR/kafka/kafka.tgz"

echo "==> Staging Kafka tarball..."
if [ -f "$HOME/Downloads/$KAFKA_TGZ" ]; then
    echo "    Using local copy from ~/Downloads/$KAFKA_TGZ"
    cp "$HOME/Downloads/$KAFKA_TGZ" "$KAFKA_DEST"
else
    echo "    Local tarball not found; downloading from archive.apache.org..."
    wget -q --show-progress \
        "https://archive.apache.org/dist/kafka/${KAFKA_VERSION}/${KAFKA_TGZ}" \
        -O "$KAFKA_DEST"
fi

echo ""
echo "==> Staging routing service configuration..."
mkdir -p "$DOCKER_DIR/routing-service/config"
cp "$DOCKER_DIR/../config/shapesdemo_kafka_protobuf.xml" \
   "$DOCKER_DIR/routing-service/config/shapesdemo_kafka_protobuf.xml"

echo ""
echo "==> Staging libraries for routing-service..."
mkdir -p "$DOCKER_DIR/routing-service/libs" "$DOCKER_DIR/routing-service/bin"

# RTI Connext DDS core libraries required by the routing service and its plugins
for lib in \
    librtiroutingservice.so \
    libnddscore.so \
    libnddsc.so \
    librtidlc.so \
    libnddsmetp.so \
    librtiapputilsc.so \
    librtixml2.so \
    librticonnextmsgc.so; do
    cp -Pv "$NDDS_LIB/$lib" "$DOCKER_DIR/routing-service/libs/"
done

# Gateway adapter and transformation plugin libraries
for lib in \
    librtikafkaadapter.so \
    librtiprotobuftransf.so \
    librtifieldtransf.so \
    librdkafka.so \
    librdkafka.so.1 \
    "librdkafka++.so" \
    "librdkafka++.so.1"; do
    cp -Pv "$GATEWAY_LIB/$lib" "$DOCKER_DIR/routing-service/libs/"
done

cp -v "$NDDS_APP_BIN/rtiroutingserviceapp" "$DOCKER_DIR/routing-service/bin/"

echo ""
echo "==> Staging libraries for kafka-publisher..."
mkdir -p "$DOCKER_DIR/kafka-publisher/libs" "$DOCKER_DIR/kafka-publisher/bin"
for lib in librdkafka.so librdkafka.so.1; do
    cp -Pv "$GATEWAY_LIB/$lib" "$DOCKER_DIR/kafka-publisher/libs/"
done
cp -v "$GATEWAY_EXAMPLE_BIN/shapes_kafka_publisher" "$DOCKER_DIR/kafka-publisher/bin/"

echo ""
echo "==> Staging libraries for kafka-subscriber..."
mkdir -p "$DOCKER_DIR/kafka-subscriber/libs" "$DOCKER_DIR/kafka-subscriber/bin"
for lib in librdkafka.so librdkafka.so.1; do
    cp -Pv "$GATEWAY_LIB/$lib" "$DOCKER_DIR/kafka-subscriber/libs/"
done
cp -v "$GATEWAY_EXAMPLE_BIN/shapes_kafka_subscriber" "$DOCKER_DIR/kafka-subscriber/bin/"

echo ""
echo "==> Building Docker images (requires internet access for the Kafka base image)..."
docker compose -f "$DOCKER_DIR/docker-compose.yml" build

echo ""
echo "Build complete. Run ./scripts/start.sh to launch the demo."
