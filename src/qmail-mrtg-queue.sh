#!/bin/sh
cd HOME
echo `find queue/mess -type f -print | wc -l`
echo `find queue/todo/* -type f -print | wc -l`
