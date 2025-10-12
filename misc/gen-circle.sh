#!/bin/bash

cd "$(dirname "$(readlink -f "$0")")" || exit 1

set -x
convert -size 1024x1024 xc:none -antialias -fill "rgba(255,255,255,1)" -draw "circle 511.5,511.5 512,1" ../static/circle.webp
