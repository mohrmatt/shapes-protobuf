#!/usr/bin/env bash
set -euo pipefail
exec /usr/local/bin/rtiroutingserviceapp \
    -cfgFile /config/shapesdemo_kafka_protobuf.xml \
    -cfgName shapesdemo_bridge
