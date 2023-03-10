#!/bin/bash

# bkp folder
mkdir www_bkp20170222

# copy files
rsync -av --exclude 'pic' --exclude 'pic_del' www/ www_bkp20170222/ 
