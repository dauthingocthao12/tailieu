# .bashrc
export LANG=ja_JP.UTF-8

plugin_test() {
    if [ $# != 1 ]; then
        echo plugin_test [SiteId]
    else
        php -dxdebug.mode=debug -dxdebug.client_host=host.docker.internal -dxdebug.client_port=9003 \
            -dxdebug.start_with_request=yes ./uranai_lib/bat/index_topic_test.php \
            --test --now --patternMake test --site $1
    fi
}

# admin画面の[起動]と同等のことをする
# 参考:uranai_lib/libadmin/site.php L:511付近 function batch_run
plugin_run() {
    if [ $# != 1 ]; then
        echo plugin_run [SiteId]
    else
	php ./bin/check-server.php
        read -p "プラグイン $1番を本実行します。よろしければなにかキーを押してください。(ctrl-cでキャンセル)"

        php -dxdebug.mode=debug -dxdebug.client_host=host.docker.internal -dxdebug.client_port=9003 \
            -dxdebug.start_with_request=yes ./uranai_lib/bat/index_topic_test.php \
            --site $1 --now
    fi
}
