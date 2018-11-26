#! /bin/bash

# This script creates a submission tarball suitable for submission.

rm -rf bits-and-bytes-proj-student-submit 
mkdir bits-and-bytes-proj-student-submit
rsync -av --exclude='bits-and-bytes-proj-student-submit' --exclude='submit.sh' . bits-and-bytes-proj-student-submit
tar czf bits-and-bytes-proj-student-submit.tgz bits-and-bytes-proj-student-submit
rm -rf bits-and-bytes-proj-student-submit