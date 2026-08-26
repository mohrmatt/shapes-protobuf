#!/usr/bin/env bash
# Stop and remove all demo containers, and kill the tmux log viewer if running.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOCKER_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
SESSION="kafka-dds-demo"

docker compose -f "$DOCKER_DIR/docker-compose.yml" down --remove-orphans

pkill -f "rtishapesdemo" 2>/dev/null && echo "rtishapesdemo stopped." || true

if tmux has-session -t "$SESSION" 2>/dev/null; then
    tmux kill-session -t "$SESSION"
    echo "tmux session '$SESSION' closed."
fi

echo "Demo containers stopped."
