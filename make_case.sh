#!/bin/bash

for f in *.py; do
  echo "Running $f";
  name=$(basename "$f")
  python "$f"
done