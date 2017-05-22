#!/usr/bin/env bash

java_source_dir="vire"
java_sources=$(find vire/ -name "*.java")
find vire/ -name "*.java" > .tmp_java_sources.lis
echo "README.rst" >> .tmp_java_sources.lis


echo >&2 "NOTICE: Building archive..."
tar cvzf VireJavaSrc.tar.gz -T .tmp_java_sources.lis

echo >&2 "NOTICE: List Java source files:"
tar tvzf VireJavaSrc.tar.gz

exit 0

# end
