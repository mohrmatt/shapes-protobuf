#!/usr/bin/env bash
# Opens a tmux session with one window per container showing live logs.
# If the session already exists, re-attaches to it.
set -euo pipefail

SESSION="kafka-dds-demo"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOCKER_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
COMPOSE="docker compose -f $DOCKER_DIR/docker-compose.yml logs -f"

if ! command -v tmux &>/dev/null; then
    echo "tmux is required. Install it with: sudo apt-get install tmux"
    exit 1
fi

if tmux has-session -t "$SESSION" 2>/dev/null; then
    tmux attach-session -t "$SESSION"
    exit 0
fi

# Window 0 - Kafka broker
tmux new-session -d -s "$SESSION" -n "kafka-broker"
tmux send-keys -t "$SESSION:0" "$COMPOSE kafka-broker" Enter

# Window 1 - Routing Service
tmux new-window -t "$SESSION" -n "routing-service"
tmux send-keys -t "$SESSION:1" "$COMPOSE routing-service" Enter

# Window 2 - Square publisher (left) and subscriber (right)
tmux new-window -t "$SESSION" -n "Square pub/sub"
tmux split-window -t "$SESSION:2" -h
tmux send-keys -t "$SESSION:2.0" "$COMPOSE kafka-publisher-square" Enter
    tmux send-keys -t "$SESSION:2.1" "$COMPOSE kafka-subscriber-square" Enter

# Window 3 - Triangle publisher (left) and subscriber (right)
tmux new-window -t "$SESSION" -n "Triangle pub/sub"
tmux split-window -t "$SESSION:3" -h
tmux send-keys -t "$SESSION:3.0" "$COMPOSE kafka-publisher-triangle" Enter
tmux send-keys -t "$SESSION:3.1" "$COMPOSE kafka-subscriber-triangle" Enter

tmux select-window -t "$SESSION:0"
echo "Attaching to tmux session '$SESSION'  -  press Ctrl-b d to detach without stopping containers."
tmux attach-session -t "$SESSION"
