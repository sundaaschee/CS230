#! /bin/bash

# This script creates a submission tarball suitable for submission.

rm -rf shell-etc-proj-student-submit 
mkdir shell-etc-proj-student-submit
rsync -av --exclude='shell-etc-proj-student-submit' --exclude='*.class' --exclude='submit.sh' . shell-etc-proj-student-submit
tar czf shell-etc-proj-student-submit.tgz shell-etc-proj-student-submit
rm -rf shell-etc-proj-student-submit