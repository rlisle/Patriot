#!/bin/sh
echo "DEBUG: ci_post_xcodebuild"
echo $SOURCE_ROOT
echo $CI
echo "ls -al"
ls -al
echo "ls -al Snapshots"
ls -al Snapshots
echo "ls -al /Volumes/workspace/repository/iOS/Patriot/ci_scripts/"
ls -al /Volumes/workspace/repository/iOS/Patriot/ci_scripts/
echo "env"
env
echo "Done"
