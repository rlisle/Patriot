#!/bin/sh
echo "DEBUG: ci_post_xcodebuild"
echo "pwd"
pwd
echo "CI = $CI"
echo "ls -al"
ls -al
#echo "ls -al Snapshots"
#ls -al Snapshots
#echo "ls -al Snapshots/PatriotTests"
#ls -al Snapshots/PatriotTests
echo "ls -al Snapshots/PatriotTests/__Snapshots__/MenuTests"
ls -al Snapshots/PatriotTests/__Snapshots__/MenuTests
echo "ls -al /Volumes/workspace/repository"
ls -al /Volumes/workspace/repository
#echo "ls -al /Volumes/workspace/repository/iOS"
#ls -al /Volumes/workspace/repository/iOS
echo "ls -al /Volumes/workspace/repository/ci_scripts"
ls -al /Volumes/workspace/repository/ci_scripts
echo "env"
env
echo "Done"
