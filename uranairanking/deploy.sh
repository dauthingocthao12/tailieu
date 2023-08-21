#!/bin/bash

##############################################
#
# デプロイ自動化シェル
#
# この方の作ったスクリプトを拝借:
#   https://gist.github.com/kensei/3449277
#
# Usage:
#  deploy.sh [identity file]
#
#  - identity fileはパスワードなしで接続したいときのオプションとして秘密鍵ファイルのパスを指定可
#
# 鍵:
#  \\Azetserver\社内システム共有資料\06_星座占いランキング\01_ログイン情報各種\ssh鍵
#
##############################################

IDENTITY_FILE=
if [ -z "$1" ]
then
	echo "[ERROR] key file must be specified."
	echo
	echo "Usage:"
	echo " ./deploy.sh /path/to/key/file"
	echo
	echo "Example:"
	echo "./deploy.sh /c/Users/yamada-taro/.ssh/uranairanking_key"
	exit 1
else
	echo
    echo "-- key file for scp connection --"
	echo "$1"
	echo
	
	IDENTITY_FILE="$1"
	
	if [ ! -f "$IDENTITY_FILE" ]; then
		echo "[ERROR] key file not found. ($IDENTITY_FILE)"
		exit 1
	fi
fi



# remove trailing slashes
targetdir=$(echo $PWD | sed 's/\/*$//g')
ignorefile="deploy.sh"

# get list of all changed files
# changes=$(git status --porcelain 2>&1)
# ブランチ vs origin/ブランチ の差分を取得
currentbranch=$(git branch | sed -n -e 's/^\* \(.*\)/\1/p')
currentbarnchorigin="origin/$currentbranch"

# changes=$(git diff --name-status "$currentbranch"  "$currentbarnchorigin" 2>&1)
# originが先じゃないと新規ファイルが「D」扱いになったため↓に変更
changes=$(git diff --name-status "$currentbarnchorigin"  "$currentbranch" 2>&1)

# exit if git status returned an error code
if [ $? -ne 0 ]
then
    echo
    echo "  $changes"
    echo
    exit 1
fi

# file length counters
maxfilelength=0
filelength=0

# extract added and modified files
files_modified=$(echo "$changes" | awk '{if($1~"A|M") print $2}')

# このファイル自身は無視する
files_modified=$(printf '%s\n' "${files_modified//$ignorefile/}")

if [ "$files_modified" == "" ]
then
    echo
    echo "Nothing to sync"
    echo
    exit 0
fi
echo "-- modified files --"
for file in $files_modified;
do
    echo "$file"
done
echo

# selected server
echo "-- What server do you deploy? --"
ans1="uranairank00001@telnet.uranairanking.jp:/home/uranairank/uranairank00001/develop"
ans2="uranairank00001@telnet.uranairanking.jp:/home/uranairank/uranairank00001"

select ANS in "$ans1" "$ans2"
do
  if [ -z "$ANS" ]; then
    continue
  else
    break
  fi
done
echo

serverno=$REPLY
servername=$ANS

# dry run
for file in $files_modified;
do
	if [ -z "$IDENTITY_FILE" ]
	then
		cmd="scp -Cpr $file $servername/$file"
	else
		cmd="scp -i "$IDENTITY_FILE" -Cpr $file $servername/$file"
	fi
	echo "$cmd"	
done

echo 
echo -n 'Do you want to continue (yes/no)?: '
read answer

# exit it "yes" or "y" not received
if [ "$answer" != "yes" ] && [ "$answer" != "y" ]
then
    echo
    exit 1
fi

echo

# copy files
for file in $files_modified;
do
	if [ -z "$IDENTITY_FILE" ]
	then
		cmd="scp -Cpr $file $servername/$file"
	else
		cmd="scp -i "$IDENTITY_FILE" -Cpr $file $servername/$file"
	fi
    echo "$cmd"
    output=$($cmd 2>&1)

    if [ $? -eq 0 ]
    then
        echo "ok"
    else
        echo "failed ($output)"
    fi
    echo
done

echo