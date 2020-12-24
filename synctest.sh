#!/bin/sh

DIR=.
DEST=/mnt/d/df/temp/profuse-sync

while inotifywait -r -e modify,create,delete,move $DIR; do
    rsync -avz --exclude='.git/' $DIR $DEST
done
