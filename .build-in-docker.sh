#!/bin/bash
set -e

# Robust git settings to avoid HTTP/2 / TLS flakiness on west update
git config --global http.version HTTP/1.1
git config --global http.postBuffer 524288000
git config --global http.lowSpeedLimit 0
git config --global http.lowSpeedTime 999999
git config --global core.compression 0

cd /workspaces/zmk

# Retry west update up to 5 times if it fails
for attempt in 1 2 3 4 5; do
    echo "=== west update attempt ${attempt} ==="
    if west update; then
        echo "=== west update succeeded on attempt ${attempt} ==="
        break
    fi
    if [ "${attempt}" = "5" ]; then
        echo "=== west update failed after 5 attempts ==="
        exit 1
    fi
    echo "=== retrying in 5 seconds ==="
    sleep 5
done

west zephyr-export

west build -s app -d build/keychron_b1_jis -b keychron -- -DSHIELD=keychron_b1_jis
