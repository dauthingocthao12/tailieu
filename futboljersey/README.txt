ローカル設定に関して
---------------------

リポジトリーには、3ブランチがあります。
- master は本番です
- mysql はMySQLにする作業
- responsive はレスポンシブ対応


PHPのバージョン
----------------

php 7.2 （7.4も可能） 


アパッチの設定
--------------

どのPHPのバージョンでも、APACHEの設定は同じにしてもいいと思います。

vhostsは xampp/apache/conf/extra/httpd-vhosts.conf
になります。
WINDOWSのため、ポート80は使えない場合は、ポート82（別番号でもいい）に以下のサンプルになります。
（自分の環境に合わせてください）

>>>
NameVirtualHost *:82

<VirtualHost *:82>
	DocumentRoot "f:/Sites/neighbors/www"
	ServerName futboljersey.local
	
	<Directory />
		Options FollowSymLinks +Includes +Indexes
		AllowOverride All
		Allow from all
		SSIErrorMsg "<!-- Error (because of cgi?) -->"
	</Directory>
</VirtualHost>
<<<


別環境について
--------------

Gitに無視されているファイル：
- cone.inc
- www/.htaccess

環境によって、別ファイルを使う可能性があります。
例:
www/.htaccess
そのファイルは、Gitに無視されます。
ですので、クローンした後に、www/.htaccess_simon　を参考にして、
www/.htaccess を作ってください。

本番のファイルは、cone.inc_prod、www/.htaccess_prodになります。
本番にアップしたい時に、そのまま本番用のファイルをアップして、リネームしてください。

複数環境があったら（検証など）、同じです。

新しい環境ファイルがあったら、.gitignoreに追加を忘れないようにご注意ください。

www/master（管理画面）
---------------------

.htaccessもありますが、Gitに無視されています。
本番の .htaccess_honban は本番にご利用ください。

