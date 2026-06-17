#!/bin/bash

DEVICE="/dev/rfcomm0"
LOG_FILE="$(date +%Y-%m-%d)_log.txt"

# ==========================================
# 1. tmux sub process (The Clean UI)
# ==========================================

if [ "$1" == "top_pane" ]; then
    clear
    echo '======== COMMAND INPUT ========'
    while true; do 
        read -p 'Send > ' line

        if [ "$line" == 'STOP' ]; then
            line='$PCREF,0,0*'            
        fi
        
        echo -ne "$line\r\n" > "$DEVICE"
    done
    exit 0
fi

if [ "$1" == "bottom_pane" ]; then
    clear
    echo '======== DEVICE LOG ========'
    echo "--- WAITING FOR INCOMING DATA ---"
    
    if [[ "$2" == "y" || "$2" == "Y" ]]; then
        echo "--- Logging data to $LOG_FILE ---"
        cat "$DEVICE" | tee -a "$LOG_FILE" || read -p 'Error reading. Press Enter to exit...'
    else
        echo "--- Logging is DISABLED ---"
        cat "$DEVICE" || read -p 'Error reading. Press Enter to exit...'
    fi
    exit 0
fi

# ==========================================
# 2. main setup (standard terminal)
# ==========================================

echo "=== Bluetooth Serial Interface Setup ==="

read -p "Log data? (y/n): " LOG_CHOICE

if [ ! -c "$DEVICE" ]; then
    echo "Device $DEVICE not found. Binding..."
    sudo rfcomm bind rfcomm0 98:D3:34:90:A8:40
    sleep 2
    if [ ! -c "$DEVICE" ]; then
        echo "Failed to bind the device. Exiting."
        exit 1
    fi
fi

echo "Stopping ModemManager..."
sudo systemctl stop ModemManager

echo "Setting port permissions..."
sudo chmod 666 "$DEVICE"

echo "Configuring port..."
stty -F "$DEVICE" 9600 raw -echo

echo "Starting tmux..."

SCRIPT_PATH="$(realpath "$0")"

tmux kill-session -t bt_session 2>/dev/null

tmux new-session -d -s bt_session "\"$SCRIPT_PATH\" top_pane"
tmux split-window -v -t bt_session:0 "\"$SCRIPT_PATH\" bottom_pane \"$LOG_CHOICE\""

tmux select-pane -t bt_session:0.0
tmux attach-session -t bt_session
