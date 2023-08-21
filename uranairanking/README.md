# uranairanking

## web
http://localhost:20000/

## mysql
.docker/app/server.php
```php
if (_IS_LOCAL_DB) {
    // Local DB
} else {
    // Development DB
}
```

## dump database

- 占いランキングの本番サーバーへのsshパスワードを問われます。
- `log`, `topic_log` の収集情報はコピーされません。

```powershell
docker exec -it uranai_db /opt/scripts/table-dump.sh
```

## phpmyadmin
http://localhost:20001/
```
Host: localhost
User: uranairank00002
Password: local
DB Name: uranairank00002
Port: 3306
```

zend_extension="xdebug.so"
xdebug.mode=debug
xdebug.discover_client_host=false
xdebug.start_with_request=trigger
xdebug.remote_port=9003
xdebug.idekey="VSCODE"
xdebug.log="/var/www/xdebug.log"
xdebug.client_host=host.docker.internal


## plugin
```
plugin_test [siteId]
```
see .bashrc

## bash
```
docker container exec -it uranai_web bash
```

## deploy

1. `Git Bash`を起動
2. `deploy.sh`を実行。

```
./deploy.sh
```

※パスワード不要でアップしたいときは、引数1に秘密鍵ファイルを指定する。
共有サーバーから入手可。  
`\\Azetserver\社内システム共有資料\06_星座占いランキング\01_ログイン情報各種\ssh鍵\azet-common`

```
./deploy.sh /c/Users/YOUR-NAME/.ssh/uranairanking_key
```

3. プロンプトに従ってファイルをデプロイする。


## メールサーバー起動（半手動)

1. powershellで下記を実行し、dockerにrootでログイン

```
docker exec -u 0 -it <コンテナ名> bash

# 例
docker exec -u 0 -it uranai_web bash
```

2. dockerのコンソール内で下記を実行

```
/etc/init.d/postfix start
```

例
```
root@azet:/home/uranairank/uranairank00001# /etc/init.d/postfix start
```

3.送信テスト

dockerのコンソール内で下記を実行 -> bool(true)になって、宛先にメールが飛んでいれば送信テストok。

```
php -r "mb_language('ja'); mb_internal_encoding('Shift_JIS');  var_dump(mb_send_mail('kimura@azet.jp', 'this is test', 'hello', \"From: uranairanking.jp <sender@uranairanking.jp>\nReply-to:sender@uranairanking.jp\", \"-fsender@uranairanking.jp\"));"
```